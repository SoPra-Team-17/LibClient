/**
 * @file   Network.cpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Definition of the network class (Interface to Server).
 */

#include "Network.hpp"
#include <network/messages/MessageTypeEnum.hpp>
#include <network/MessageContainer.hpp>
#include <network/messages/Hello.hpp>
#include <network/messages/ItemChoice.hpp>
#include <network/messages/EquipmentChoice.hpp>
#include <network/messages/GameOperation.hpp>
#include <network/messages/GameLeave.hpp>
#include <network/messages/GamePause.hpp>
#include <network/messages/RequestGamePause.hpp>
#include <network/messages/RequestMetaInformation.hpp>
#include <network/messages/RequestReplay.hpp>
#include <network/messages/Reconnect.hpp>
#include <network/messages/HelloReply.hpp>
#include <network/messages/GameStarted.hpp>
#include <network/messages/RequestItemChoice.hpp>
#include <network/messages/RequestEquipmentChoice.hpp>
#include <network/messages/GameStatus.hpp>
#include <network/messages/RequestGameOperation.hpp>
#include <network/messages/StatisticsMessage.hpp>
#include <network/messages/GameLeft.hpp>
#include <network/messages/Error.hpp>
#include <network/messages/Strike.hpp>
#include <network/messages/MetaInformation.hpp>
#include <util/UUID.hpp>
#include <utility>

namespace libclient {
    Network::Network(libclient::Callback *c, std::shared_ptr<Model> m) : callback(c), model(std::move(m)) {}

    void Network::onReceiveMessage(const std::string &message) {
        auto json = nlohmann::json::parse(message);
        auto mc = json.get<spy::network::MessageContainer>();

        model->clientState.debugMessage = mc.getDebugMessage();

        if (model->clientState.id.has_value() && model->clientState.id.value() != mc.getClientId()) {
            // received message that was not for this client
            callback->wrongDestination();
            return;
        }

        switch (mc.getType()) {
            case spy::network::messages::MessageTypeEnum::HELLO_REPLY: {
                model->clientState.id = mc.getClientId();
                auto m = json.get<spy::network::messages::HelloReply>();
                model->clientState.sessionId = m.getSessionId();
                model->gameState.level = m.getLevel();
                model->gameState.settings = m.getSettings();
                model->gameState.characterSettings = m.getCharacterSettings();

                for (const auto &info: model->gameState.characterSettings) {
                    model->aiState.unknownFaction[info.getCharacterId()];
                    model->aiState.properties[info.getCharacterId()] = info.getFeatures();
                }

                // not nice but have to iterate over enum values
                for (int gadgetType = 1; gadgetType < 22; gadgetType++) {
                    model->aiState.unknownGadgets[std::make_shared<spy::gadget::Gadget>(
                            spy::gadget::GadgetEnum(gadgetType))];
                }

                state = NetworkState::WELCOMED;
                callback->onHelloReply();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_STARTED: {
                auto m = json.get<spy::network::messages::GameStarted>();
                if (model->clientState.sessionId != m.getSessionId()) {
                    // received message that was not for this session
                    callback->wrongDestination();
                    return;
                }
                model->clientState.playerOneId = m.getPlayerOneId();
                model->clientState.playerTwoId = m.getPlayerTwoId();
                model->clientState.playerOneName = m.getPlayerOneName();
                model->clientState.playerTwoName = m.getPlayerTwoName();

                state = NetworkState::IN_GAME;
                callback->onGameStarted();
                break;
            }
            case spy::network::messages::MessageTypeEnum::REQUEST_ITEM_CHOICE: {
                auto m = json.get<spy::network::messages::RequestItemChoice>();
                model->gameState.offeredCharacters = m.getOfferedCharacterIds();
                model->gameState.offeredGadgets = m.getOfferedGadgets();

                state = NetworkState::IN_ITEMCHOICE;
                callback->onRequestItemChoice();
                break;
            }
            case spy::network::messages::MessageTypeEnum::REQUEST_EQUIPMENT_CHOICE: {
                auto m = json.get<spy::network::messages::RequestEquipmentChoice>();
                model->gameState.chosenCharacter = m.getChosenCharacterIds();
                model->gameState.chosenGadget = m.getChosenGadgets();

                for (const auto c: model->gameState.chosenCharacter) {
                    model->aiState.addFaction(c, model->aiState.myFaction);
                }

                state = NetworkState::IN_EQUIPMENTCHOICE;
                callback->onRequestEquipmentChoice();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_STATUS: {
                auto m = json.get<spy::network::messages::GameStatus>();
                model->gameState.lastActiveCharacter = m.getActiveCharacterId();
                model->gameState.operations.reserve(model->gameState.operations.size() + m.getOperations().size());
                for (const auto &op: m.getOperations()) {
                    model->gameState.operations.push_back(op->clone());
                }
                model->gameState.handleLastClientOperation(m.getState());
                model->gameState.isGameOver = m.getIsGameOver();

                // update AIState for each operation
                if (model->clientState.role != spy::network::RoleEnum::SPECTATOR) {
                    auto myFaction = model->clientState.amIPlayer1() ? spy::character::FactionEnum::PLAYER1
                                                                     : spy::character::FactionEnum::PLAYER2;
                    for (const auto &op: m.getOperations()) {
                        model->aiState.processOperation(op, model->gameState.state, model->gameState.settings, myFaction);
                        model->aiState.applySureInformation(model->gameState.state, myFaction);
                    }
                }

                // set current state and merge AIState into
                model->gameState.state = m.getState();

                // check for gadgets that are on the floor
                const auto &mo = model;
                auto map = model->gameState.state.getMap();
                for (auto y = 0U; y < map.getMap().size(); y++) {
                    for (auto x = 0U; x < map.getMap().at(y).size(); x++) {
                        auto field = (map.getField(x, y));
                        auto gad = field.getGadget();
                        if (gad.has_value() && gad.value()->getType() != spy::gadget::GadgetEnum::COCKTAIL) {
                            mo->aiState.addGadget(gad.value()->getType(), std::nullopt);
                        }
                    }
                }

                if (model->clientState.role != spy::network::RoleEnum::SPECTATOR) {
                    auto myFaction = model->clientState.amIPlayer1() ? spy::character::FactionEnum::PLAYER1
                                                                     : spy::character::FactionEnum::PLAYER2;
                    model->aiState.applySureInformation(model->gameState.state, myFaction);
                }

                state = m.getIsGameOver() ? NetworkState::GAME_OVER : NetworkState::IN_GAME;
                callback->onGameStatus();
                break;
            }
            case spy::network::messages::MessageTypeEnum::REQUEST_GAME_OPERATION: {
                auto m = json.get<spy::network::messages::RequestGameOperation>();
                model->clientState.activeCharacter = m.getCharacterId();

                state = NetworkState::IN_GAME_ACTIVE;
                callback->onRequestGameOperation();
                break;
            }
            case spy::network::messages::MessageTypeEnum::STATISTICS: {
                auto m = json.get<spy::network::messages::StatisticsMessage>();
                model->gameState.winner = m.getWinner();
                model->gameState.winningReason = m.getReason();
                model->gameState.statistics = m.getStatistics();
                model->gameState.hasReplay = m.getHasReplay();

                state = NetworkState::GAME_OVER;
                callback->onStatistics();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_LEFT: {
                auto m = json.get<spy::network::messages::GameLeft>();
                model->clientState.leftUserId = m.getLeftUserId();

                state = NetworkState::GAME_OVER;
                callback->onGameLeft();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_PAUSE: {
                auto m = json.get<spy::network::messages::GamePause>();
                model->clientState.gamePaused = m.isGamePause();
                model->clientState.serverEnforced = m.isServerEnforced();

                if (m.isGamePause()) {
                    stateBeforePause = state;
                    state = NetworkState::PAUSE;
                } else {
                    state = stateBeforePause;
                }
                callback->onGamePause();
                break;
            }
            case spy::network::messages::MessageTypeEnum::META_INFORMATION: {
                auto m = json.get<spy::network::messages::MetaInformation>();
                model->clientState.information = m.getInformation();

                callback->onMetaInformation();
                break;
            }
            case spy::network::messages::MessageTypeEnum::STRIKE: {
                auto m = json.get<spy::network::messages::Strike>();
                model->clientState.strikeNr = m.getStrikeNr();
                model->clientState.strikeMax = m.getStrikeMax();
                model->clientState.strikeReason = m.getReason();

                callback->onStrike();
                break;
            }
            case spy::network::messages::MessageTypeEnum::ERROR: {
                auto m = json.get<spy::network::messages::Error>();
                model->clientState.errorReason = m.getReason();

                callback->onError();
                break;
            }
            case spy::network::messages::MessageTypeEnum::REPLAY: {
                auto m = json.get<spy::network::messages::Replay>();
                if (model->clientState.sessionId != m.getSessionId()) {
                    // received message that was not for this session
                    callback->wrongDestination();
                    return;
                }
                model->replay = m;

                state = NetworkState::GAME_OVER;
                callback->onReplay();
                break;
            }
            default:
                // do nothing
                callback->wrongDestination();
                break;
        }
    }

    bool Network::reconnectPlayerAfterCrash(const std::string &servername,
                                            int port,
                                            const std::string &clientName,
                                            const spy::util::UUID &clientId,
                                            const spy::util::UUID &sessionId,
                                            const spy::util::UUID &playerOneId,
                                            const spy::util::UUID &playerTwoId,
                                            const std::string &playerOneName,
                                            const std::string &playerTwoName) {
        // save params
        this->serverName = servername;
        this->serverPort = port;
        model->clientState.role = spy::network::RoleEnum::PLAYER;
        model->clientState.name = clientName;
        model->clientState.id = clientId;
        model->clientState.sessionId = sessionId;
        model->clientState.playerOneId = playerOneId;
        model->clientState.playerTwoId = playerTwoId;
        model->clientState.playerOneName = playerOneName;
        model->clientState.playerTwoName = playerTwoName;

        // start reconnect procedure
        state = NetworkState::RECONNECT;
        return sendReconnect();
    }

    bool Network::connect(const std::string &servername, int port) {
        if (state != NetworkState::NOT_CONNECTED && state != NetworkState::CONNECTED &&
            state != NetworkState::RECONNECT && state != NetworkState::GAME_OVER) {
            return false;
        }

        if (state != NetworkState::NOT_CONNECTED && state != NetworkState::GAME_OVER) {
            if (this->serverName != servername || this->serverPort != port) {
                // connect to another server -> disconnect to current server
                disconnect();
            } else if (model->clientState.isConnected) {
                // connect to same server and still connected
                return true;
            }
        }

        this->serverName = servername;
        this->serverPort = port;

        try {
            this->webSocketClient.emplace(servername, "/", port, "");
            webSocketClient->receiveListener.subscribe(
                    std::bind(&Network::onReceiveMessage, this, std::placeholders::_1));
            webSocketClient->closeListener.subscribe(std::bind(&Network::onClose, this));

            state = (state == NetworkState::RECONNECT) ? NetworkState::RECONNECT : NetworkState::CONNECTED;
            model->clientState.isConnected = true;
            return true;
        } catch (std::runtime_error &e) {
            // could not connect
            return false;
        }
    }

    void Network::onClose() {
        if (model->clientState.id.has_value() && model->clientState.role != spy::network::RoleEnum::SPECTATOR) {
            // Client is allowed to reconnect
            state = NetworkState::RECONNECT;
            if (webSocketClient.has_value()) {
                webSocketClient.reset();
            }
            model->clientState.isConnected = false;
        } else {
            disconnect();
        }
        callback->connectionLost();
    }

    void Network::disconnect() {
        if (webSocketClient.has_value()) {
            webSocketClient.reset();
        }
        state = NetworkState::NOT_CONNECTED;
        model = std::make_shared<Model>();
    }

    bool Network::sendHello(const std::string &name, spy::network::RoleEnum role) {
        auto message = spy::network::messages::Hello(spy::util::UUID(), name, role);
        if (!message.validate() || state != NetworkState::CONNECTED) {
            return false;
        }
        model->clientState.role = role;
        model->clientState.name = name;
        state = NetworkState::SENT_HELLO;
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendItemChoice(std::variant<spy::util::UUID, spy::gadget::GadgetEnum> choice) {
        if (state != NetworkState::IN_ITEMCHOICE) {
            return false;
        }
        auto message = spy::network::messages::ItemChoice(model->clientState.id.value(), std::move(choice));
        if (!message.validate(model->clientState.role, model->gameState.offeredCharacters,
                              model->gameState.offeredGadgets)) {
            return false;
        }
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendEquipmentChoice(const std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> &equipment) {
        if (state != NetworkState::IN_EQUIPMENTCHOICE) {
            return false;
        }
        auto message = spy::network::messages::EquipmentChoice(model->clientState.id.value(), equipment);
        if (!message.validate(model->clientState.role, model->gameState.chosenCharacter,
                              model->gameState.chosenGadget)) {
            return false;
        }

        model->gameState.equipmentMap = equipment;
        for (auto &it : equipment) {
            for (auto gadgetType: it.second) {
                model->aiState.addGadget(gadgetType, it.first);
            }
        }

        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendGameOperation(const std::shared_ptr<spy::gameplay::BaseOperation> &operation,
                                    const spy::MatchConfig &config) {
        if (state != NetworkState::IN_GAME_ACTIVE) {
            return false;
        }
        auto message = spy::network::messages::GameOperation(model->clientState.id.value(), operation);
        if (!message.validate(model->clientState.role, model->gameState.state, model->clientState.activeCharacter,
                              config)) {
            return false;
        }
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendGameLeave() {
        if (state == NetworkState::NOT_CONNECTED || state == NetworkState::CONNECTED ||
            state == NetworkState::SENT_HELLO || state == NetworkState::RECONNECT) {
            return false;
        }
        auto message = spy::network::messages::GameLeave(model->clientState.id.value());
        if (!message.validate(model->clientState.role)) {
            return false;
        }
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendRequestGamePause(bool gamePause) {
        if (state != NetworkState::IN_GAME && state != NetworkState::IN_GAME_ACTIVE && state != NetworkState::PAUSE) {
            return false;
        }
        auto message = spy::network::messages::RequestGamePause(model->clientState.id.value(), gamePause);
        if (!message.validate(model->clientState.role, model->clientState.gamePaused,
                              model->clientState.serverEnforced)) {
            return false;
        }
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendRequestMetaInformation(std::vector<spy::network::messages::MetaInformationKey> keys) {
        if (state == NetworkState::NOT_CONNECTED || state == NetworkState::CONNECTED ||
            state == NetworkState::SENT_HELLO || state == NetworkState::RECONNECT) {
            return false;
        }
        auto message = spy::network::messages::RequestMetaInformation(model->clientState.id.value(), std::move(keys));
        if (!message.validate(model->clientState.role)) {
            return false;
        }
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendRequestReplayMessage() {
        if (state != NetworkState::GAME_OVER) {
            return false;
        }
        auto message = spy::network::messages::RequestReplay(model->clientState.id.value());
        if (!message.validate(model->clientState.role, model->gameState.hasReplay)) {
            return false;
        }
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendReconnect() {
        if (state != NetworkState::RECONNECT) {
            return false;
        }
        auto message = spy::network::messages::Reconnect(model->clientState.id.value(), model->clientState.sessionId);
        if (!message.validate(model->clientState.role)) {
            return false;
        }
        if (!connect(this->serverName, this->serverPort)) {
            return false;
        }
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    Network::NetworkState Network::getState() const {
        return state;
    }

}
