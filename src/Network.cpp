/**
 * @file   Network.cpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Definition of the network class (Interface to Server).
 */

#include "Network.hpp"
#include <network/messages/MessageTypeEnum.hpp>
#include <network/MessageContainer.hpp>

namespace libclient {
    Network::Network(std::shared_ptr<Callback> c, std::shared_ptr<Model> m) {
        mCallback = std::move(c);
        mModel = std::move(m);
    }

    void Network::onReceiveMessage(std::string message) {
        auto json = nlohmann::json::parse(message);
        auto mc = json.get<spy::network::MessageContainer>();
        //TODO validation check (playerId, SessionId, ...)

        switch (mc.getType()) {
            case spy::network::messages::MessageTypeEnum::HELLO_REPLY:
                //TODO implement
                mCallback->onHelloReply();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_STARTED:
                //TODO implement
                mCallback->onGameStarted();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_ITEM_CHOICE:
                //TODO implement
                mCallback->onRequestItemChoice();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_EQUIPMENT_CHOICE:
                //TODO implement
                mCallback->onRequestEquipmentChoice();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_STATUS:
                //TODO implement
                mCallback->onGameStatus();
                break;
            case spy::network::messages::MessageTypeEnum::REQUEST_GAME_OPERATION:
                //TODO implement
                mCallback->onRequestGameOperation();
                break;
            case spy::network::messages::MessageTypeEnum::STATISTICS:
                //TODO implement
                mCallback->onStatistics();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_LEFT:
                //TODO implement
                mCallback->onGameLeft();
                break;
            case spy::network::messages::MessageTypeEnum::GAME_PAUSE:
                //TODO implement
                mCallback->onGamePause();
                break;
            case spy::network::messages::MessageTypeEnum::META_INFORMATION:
                //TODO implement
                mCallback->onMetaInformation();
                break;
            case spy::network::messages::MessageTypeEnum::STRIKE:
                //TODO implement
                mCallback->onStrike();
                break;
            case spy::network::messages::MessageTypeEnum::ERROR:
                //TODO implement
                mCallback->onError();
                break;
            case spy::network::messages::MessageTypeEnum::REPLAY:
                //TODO implement
                mCallback->onReplay();
                break;
            default:
                // do nothing
                break;
        }
    }

    void Network::connect(const std::string& servername, int port) {
        websocket::network::WebSocketClient client(servername, "/", port, "");

        this->mWebSocketClient.emplace(servername, "/", port, "");
        mWebSocketClient->receiveListener.subscribe(std::bind(&Network::onReceiveMessage, this, std::placeholders::_1));
        mWebSocketClient->closeListener.subscribe(std::bind(&Network::disconnect, this));
    }

    void Network::disconnect() {
        if (mWebSocketClient.has_value()) {
            mWebSocketClient.reset();
        }
        //TODO implement
    }

    bool Network::sentHello() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentItemChoice() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentEquipmentChoice() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentOperation() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentGameLeave() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentRequestGamePause() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentRequestMeatInformation() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentRequestReplayMessage() {
        //TODO implement with validation check (role, ...)
        return true;
    }

    bool Network::sentReconnect() {
        // TODO implement with validation check (role, ...) and check how to handle reconnect in SopraNetwork
        return true;
    }
}