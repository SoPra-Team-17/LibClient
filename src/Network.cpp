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

namespace libclient {
    Network::Network(std::shared_ptr<Callback> c, std::shared_ptr<Model> m) {
        callback = c;
        model = m;
    }

    void Network::onReceiveMessage(std::string message) {
        auto json = nlohmann::json::parse(message);
        auto mc = json.get<spy::network::MessageContainer>();

        switch (mc.getType()) {
            case spy::network::messages::MessageTypeEnum::HELLO_REPLY:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onHelloReply();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_STARTED:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onGameStarted();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_ITEM_CHOICE:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onRequestItemChoice();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_EQUIPMENT_CHOICE:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onRequestEquipmentChoice();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_STATUS:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onGameStatus();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_GAME_OPERATION:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onRequestGameOperation();
                break;
            case spy::network::messages::MessageTypeEnum::STATISTICS:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onStatistics();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_LEFT:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onGameLeft();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_PAUSE:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onGamePause();
                break;
            case spy::network::messages::MessageTypeEnum::META_INFORMATION:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onMetaInformation();
                break;
            case spy::network::messages::MessageTypeEnum::STRIKE:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onStrike();
                break;
            case spy::network::messages::MessageTypeEnum::ERROR:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onError();
                break;
            case spy::network::messages::MessageTypeEnum::REPLAY:
                //TODO implement with validation check (playerId, sessionId, ..)
                callback->onReplay();
                break;
            default:
                // do nothing
                break;
        }
    }

    void Network::connect(const std::string &servername, int port) {
        websocket::network::WebSocketClient client(servername, "/", port, "");

        this->webSocketClient.emplace(servername, "/", port, "");
        webSocketClient->receiveListener.subscribe(std::bind(&Network::onReceiveMessage, this, std::placeholders::_1));
        webSocketClient->closeListener.subscribe(std::bind(&Network::disconnect, this));
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