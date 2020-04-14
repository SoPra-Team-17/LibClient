#include <utility>

#include <utility>

/**
 * @file   Network.hcp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Definition of the network class (Interface to Server).
 */

#include "Network.hpp"
#include <network/messages/MessageTypeEnum.hpp>
#include <network/MessageContainer.hpp>

namespace libclient::network {
    Network::Network(std::shared_ptr<callback::Callback> c, std::shared_ptr<model::Model> m) {
        mCallback = std::move(c);
        mModel = std::move(m);
    }

    void Network::onReceiveMessage(std::string message) {
        message.crend(); //TODO delete this line
        //TODO switch MessageType and call callback->onReceiveMessage
    }

    void Network::connect(std::string servername, int port) {
        websocket::network::WebSocketClient client(std::move(servername), "/", port, "");

        this->mWebSocketClient.emplace(servername, "/", port, "");
        mWebSocketClient->receiveListener.subscribe(std::bind(&Network::onReceiveMessage, this, std::placeholders::_1));
        mWebSocketClient->closeListener.subscribe(std::bind(&Network::disconnect, this));
    }

    void Network::disconnect() {
        if (mWebSocketClient.has_value()) {
            mWebSocketClient.reset();
        }
        //TODO update Model
    }

    //TODO implement sent methods with validation check (role, ...)
}