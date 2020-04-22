#include <utility>

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

namespace libclient {
    Network::Network(std::shared_ptr<Callback> c, std::shared_ptr<Model> m) : callback(std::move(c)),
                                                                              model(std::move(m)) {}

    void Network::onReceiveMessage(std::string message) {
        auto json = nlohmann::json::parse(message);
        auto mc = json.get<spy::network::MessageContainer>();

        if (model->clientState.id.has_value() && model->clientState.id.value() != mc.getPlayerId()) {
            // received message that was not for this client
            return;
        }

        switch (mc.getType()) {
            case spy::network::messages::MessageTypeEnum::HELLO_REPLY: {
                model->clientState.id = mc.getPlayerId();
                auto m = json.get<spy::network::messages::HelloReply>();
                model->clientState.sessionId = m.getSessionId();
                model->gameState.level = m.getLevel();
                model->gameState.settings = m.getSettings();
                model->gameState.characterSettings = m.getCharacterSettings();

                callback->onHelloReply();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_STARTED: {
                auto m = json.get<spy::network::messages::GameStarted>();
                if (model->clientState.sessionId != m.getSessionId()) {
                    // received message that was not for this session
                    return;
                }
                model->clientState.playerOneId = m.getPlayerOneId();
                model->clientState.playerTwoId = m.getPlayerTwoId();
                model->clientState.playerOneName = m.getPlayerOneName();
                model->clientState.playerTwoName = m.getPlayerTwoName();

                callback->onGameStarted();
                break;
            }
            case spy::network::messages::MessageTypeEnum::REQUEST_ITEM_CHOICE: {
                auto m = json.get<spy::network::messages::RequestItemChoice>();
                model->gameState.offeredCharacters = m.getOfferedCharacterIds();
                model->gameState.offeredGadgets = m.getOfferedGadgets();

                callback->onRequestItemChoice();
                break;
            }
            case spy::network::messages::MessageTypeEnum::REQUEST_EQUIPMENT_CHOICE: {
                auto m = json.get<spy::network::messages::RequestEquipmentChoice>();
                model->gameState.chosenCharacter = m.getChosenCharacterIds();
                model->gameState.chosenGadget = m.getChosenGadgets();

                callback->onRequestEquipmentChoice();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_STATUS: {
                auto m = json.get<spy::network::messages::GameStatus>();
                model->gameState.lastActiveCharacter = m.getActiveCharacterId();
                model->gameState.operations = m.getOperations();
                model->gameState.state = m.getState();
                model->gameState.isGameOver = m.getIsGameOver();

                callback->onGameStatus();
                break;
            }
            case spy::network::messages::MessageTypeEnum::REQUEST_GAME_OPERATION: {
                auto m = json.get<spy::network::messages::RequestGameOperation>();
                model->clientState.activeCharacter = m.getCharacterId();

                callback->onRequestGameOperation();
                break;
            }
            case spy::network::messages::MessageTypeEnum::STATISTICS: {
                auto m = json.get<spy::network::messages::StatisticsMessage>();
                model->gameState.winner = m.getWinner();
                model->gameState.winningReason = m.getReason();
                model->gameState.statistics = m.getStatistics();
                model->gameState.hasReplay = m.getHasReplay();

                callback->onStatistics();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_LEFT: {
                auto m = json.get<spy::network::messages::GameLeft>();
                model->clientState.leftUserId = m.getLeftUserId();

                callback->onGameLeft();
                break;
            }
            case spy::network::messages::MessageTypeEnum::GAME_PAUSE: {
                auto m = json.get<spy::network::messages::GamePause>();
                model->clientState.gamePaused = m.isGamePause();
                model->clientState.serverEnforced = m.isServerEnforced();

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
                //TODO implement with validation check (sessionId, ..)
                /*
                util::UUID sessionId;
                std::string gameStart;
                std::string gameEnd;
                util::UUID playerOneId;
                util::UUID playerTwoId;
                std::string playerOneName;
                std::string playerTwoName;
                unsigned int rounds;
                spy::scenario::Scenario level;
                spy::MatchConfig settings;
                spy::character::CharacterDescription characterSettings;
                std::vector<MessageContainer> messages;
                 */

                callback->onReplay();
                break;
            }
            default:
                // do nothing
                break;
        }
    }

    void Network::connect(const std::string &servername, int port) {
        websocket::network::WebSocketClient client(servername, "/", port, "");

        this->webSocketClient.emplace(servername, "/", port, "");
        webSocketClient->receiveListener.subscribe(std::bind(&Network::onReceiveMessage, this, std::placeholders::_1));
        webSocketClient->closeListener.subscribe(std::bind(&Network::onClose, this));

        model->clientState.isConnected = true;
    }

    void Network::onClose() {
        callback->connectionLost();
    }

    void Network::disconnect() {
        // TODO validation check (rules of game, ...)
        if (webSocketClient.has_value()) {
            webSocketClient.reset();
        }
        //TODO implement (reset model, ...)
    }

    bool Network::sendHello(std::string name, spy::network::RoleEnum role) {
        if (role == spy::network::RoleEnum::INVALID) {
            return false;
        }
        //TODO validation check (rules of game, ...)
        auto message = spy::network::messages::Hello(model->clientState.id, std::move(name), role);
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendItemChoice(std::variant<spy::util::UUID, spy::gadget::GadgetEnum> choice) {
        if (model->clientState.role == spy::network::RoleEnum::INVALID ||
            model->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, valid UUID, valid GadgetEnum, ...)
        auto message = spy::network::messages::ItemChoice(model->clientState.id, std::move(choice));
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendEquipmentChoice(std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> equipment) {
        if (model->clientState.role == spy::network::RoleEnum::INVALID ||
            model->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, valid UUID, valid set of GadgetEnums, ...)
        auto message = spy::network::messages::EquipmentChoice(model->clientState.id, std::move(equipment));
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendGameOperation(spy::gameplay::Operation operation) {
        if (model->clientState.role == spy::network::RoleEnum::INVALID ||
            model->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, valid operation values, ...)
        auto message = spy::network::messages::GameOperation(model->clientState.id, std::move(operation));
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendGameLeave() {
        if (model->clientState.role == spy::network::RoleEnum::INVALID) {
            return false;
        }
        //TODO validation check (rules of game, ...)
        auto message = spy::network::messages::GameLeave(model->clientState.id);
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendRequestGamePause(bool gamePause) {
        if (model->clientState.role == spy::network::RoleEnum::INVALID ||
            model->clientState.role == spy::network::RoleEnum::SPECTATOR ||
            model->clientState.role == spy::network::RoleEnum::AI) {
            return false;
        }
        //TODO validation check (rules of game, valid bool for gamePause, ...)
        auto message = spy::network::messages::RequestGamePause(model->clientState.id, gamePause);
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendRequestMeatInformation(std::vector<std::string> keys) {
        if (model->clientState.role == spy::network::RoleEnum::INVALID) {
            return false;
        }
        //TODO validation check (rules of game, valid key values, ...)
        auto message = spy::network::messages::RequestMetaInformation(model->clientState.id, std::move(keys));
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendRequestReplayMessage() {
        if (model->clientState.role == spy::network::RoleEnum::INVALID ||
            model->clientState.role == spy::network::RoleEnum::AI) {
            return false;
        }
        //TODO validation check (rules of game, replay available, ...)
        auto message = spy::network::messages::RequestReplay(model->clientState.id);
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }

    bool Network::sendReconnect() {
        // TODO check how reconnect is handled by SopraNetwork
        if (model->clientState.role == spy::network::RoleEnum::INVALID ||
            model->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, check connection, ...)
        auto message = spy::network::messages::Reconnect(model->clientState.id, model->clientState.sessionId);
        nlohmann::json j = message;
        webSocketClient->send(j.dump());
        return true;
    }
}