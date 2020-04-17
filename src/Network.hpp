/**
 * @file   Network.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the network class (Interface to Serve).
 */

#include <SopraNetwork/WebSocketClient.hpp>

#ifndef LIBCLIENT_NETWORK_HPP
#define LIBCLIENT_NETWORK_HPP

#include <memory>
#include <variant>
#include <Callback.hpp>
#include <model/Model.hpp>
#include <network/RoleEnum.hpp>
#include <util/UUID.hpp>
#include <datatypes/gadgets/GadgetEnum.hpp>
#include <datatypes/gameplay/Operation.hpp>

namespace libclient {

    class Network {
        private:
            std::shared_ptr<Callback> callback;
            std::shared_ptr<Model> model;
            std::optional<websocket::network::WebSocketClient> webSocketClient;

            /**
             * function to handle received messages
             * @param message std::string received message from server
             */
            void onReceiveMessage(std::string message);

        public:
            Network(std::shared_ptr<libclient::Callback> c, std::shared_ptr<libclient::Model> m);

            void connect(const std::string& servername, int port);

            void disconnect();

            bool sendHello(std::string name, spy::network::RoleEnum role);

            bool sendItemChoice(std::variant<spy::util::UUID, spy::gadget::GadgetEnum> choice);

            bool sendEquipmentChoice(std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> equipment);

            bool sendGameOperation(spy::gameplay::Operation operation);

            bool sendGameLeave();

            bool sendRequestGamePause(bool gamePause);

            bool sendRequestMeatInformation(std::vector<std::string> keys);

            bool sendRequestReplayMessage();

            bool sendReconnect();
    };
}

#endif //LIBCLIENT_NETWORK_HPP
