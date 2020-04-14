/**
 * @file   Network.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the network class (Interface to Serve).
 */

#include <SopraNetwork/WebSocketClient.hpp>

#ifndef SAMPLELIBRARY_NETWORK_HPP
#define SAMPLELIBRARY_NETWORK_HPP

#include <memory>
#include "Callback.hpp"
#include "Model.hpp"

namespace libclient::network {

    class Network {
        private:
            std::shared_ptr<model::Model> mModel;
            std::shared_ptr<callback::Callback> mCallback;
            std::optional<websocket::network::WebSocketClient> mWebSocketClient;

            /**
             * function to handle received messages
             * @param message std::string received message from server
             */
            void onReceiveMessage(std::string message);

        public:
            Network(std::shared_ptr<callback::Callback> c, std::shared_ptr<model::Model> m);

            void connect(std::string servername, int port);

            void disconnect();

            bool sentHello();

            bool sentItemChoice();

            bool sentEquipmentChoice();

            bool sentOperation();

            bool sentGameLeave();

            bool sentRequestGamePause();

            bool sentRequestMeatInformation();

            bool sentRequestReplayMessage();

            // TODO check how to handled reconnect
            void sentReconnect();
    };
}

#endif //SAMPLELIBRARY_NETWORK_HPP
