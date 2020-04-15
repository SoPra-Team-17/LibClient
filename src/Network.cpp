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

namespace libclient {
    Network::Network(std::shared_ptr<Callback> c, std::shared_ptr<Model> m) {
        mCallback = std::move(c);
        mModel = std::move(m);
    }

    void Network::onReceiveMessage(std::string message) {
        auto json = nlohmann::json::parse(message);
        auto mc = json.get<spy::network::MessageContainer>();

        switch (mc.getType()) {
            case spy::network::messages::MessageTypeEnum::HELLO_REPLY:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onHelloReply();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_STARTED:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onGameStarted();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_ITEM_CHOICE:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onRequestItemChoice();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_EQUIPMENT_CHOICE:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onRequestEquipmentChoice();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_STATUS:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onGameStatus();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_GAME_OPERATION:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onRequestGameOperation();
                break;
            case spy::network::messages::MessageTypeEnum::STATISTICS:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onStatistics();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_LEFT:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onGameLeft();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_PAUSE:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onGamePause();
                break;
            case spy::network::messages::MessageTypeEnum::META_INFORMATION:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onMetaInformation();
                break;
            case spy::network::messages::MessageTypeEnum::STRIKE:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onStrike();
                break;
            case spy::network::messages::MessageTypeEnum::ERROR:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onError();
                break;
            case spy::network::messages::MessageTypeEnum::REPLAY:
                //TODO implement with validation check (playerId, sessionId, ..)
                mCallback->onReplay();
                break;
            default:
                // do nothing
                break;
        }
    }

    void Network::connect(const std::string &servername, int port) {
        websocket::network::WebSocketClient client(servername, "/", port, "");

        this->mWebSocketClient.emplace(servername, "/", port, "");
        mWebSocketClient->receiveListener.subscribe(std::bind(&Network::onReceiveMessage, this, std::placeholders::_1));
        mWebSocketClient->closeListener.subscribe(std::bind(&Network::disconnect, this));
    }

    void Network::disconnect() {
        // TODO validation check (rules of game, ...)
        if (mWebSocketClient.has_value()) {
            mWebSocketClient.reset();
        }
        //TODO implement (reset model, ...)
    }

    bool Network::sentHello(std::string name, spy::network::RoleEnum role) {
        if (role == spy::network::RoleEnum::INVALID) {
            return false;
        }
        //TODO validation check (rules of game, ...)
        auto message = spy::network::messages::Hello(mModel->clientState.id, std::move(name), role);
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentItemChoice(std::variant<spy::util::UUID, spy::gadget::GadgetEnum> choice) {
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID ||
            mModel->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, valid UUID, valid GadgetEnum, ...)
        auto message = spy::network::messages::ItemChoice(mModel->clientState.id, std::move(choice));
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentEquipmentChoice(std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> equipment) {
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID ||
            mModel->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, valid UUID, valid set of GadgetEnums, ...)
        auto message = spy::network::messages::EquipmentChoice(mModel->clientState.id, std::move(equipment));
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentGameOperation(spy::gameplay::Operation operation) {
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID ||
            mModel->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, valid operation values, ...)
        auto message = spy::network::messages::GameOperation(mModel->clientState.id, std::move(operation));
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentGameLeave() {
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID) {
            return false;
        }
        //TODO validation check (rules of game, ...)
        auto message = spy::network::messages::GameLeave(mModel->clientState.id);
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentRequestGamePause(bool gamePause) {
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID ||
            mModel->clientState.role == spy::network::RoleEnum::SPECTATOR ||
            mModel->clientState.role == spy::network::RoleEnum::AI) {
            return false;
        }
        //TODO validation check (rules of game, valid bool for gamePause, ...)
        auto message = spy::network::messages::RequestGamePause(mModel->clientState.id, gamePause);
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentRequestMeatInformation(std::vector<std::string> keys) {
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID) {
            return false;
        }
        //TODO validation check (rules of game, valid key values, ...)
        auto message = spy::network::messages::RequestMetaInformation(mModel->clientState.id, std::move(keys));
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentRequestReplayMessage() {
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID ||
            mModel->clientState.role == spy::network::RoleEnum::AI) {
            return false;
        }
        //TODO validation check (rules of game, replay available, ...)
        auto message = spy::network::messages::RequestReplay(mModel->clientState.id);
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }

    bool Network::sentReconnect() {
        // TODO check how reconnect is handled by SopraNetwork
        if (mModel->clientState.role == spy::network::RoleEnum::INVALID ||
            mModel->clientState.role == spy::network::RoleEnum::SPECTATOR) {
            return false;
        }
        //TODO validation check (rules of game, check connection, ...)
        auto message = spy::network::messages::Reconnect(mModel->clientState.id, mModel->clientState.sessionId);
        nlohmann::json j = message;
        mWebSocketClient->send(j.dump());
        return true;
    }
}