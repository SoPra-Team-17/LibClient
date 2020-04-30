/**
 * @file   Network.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the network class (Interface to Serve).
 */



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
#include <Client/WebSocketClient.hpp>

namespace libclient {

    class Network {
        public:
            enum NetworkState {
                NOT_CONNECTED,
                CONNECTED,
                SENT_HELLO,
                WELCOMED,
                IN_ITEMCHOICE,
                IN_EQUIPMENTCHOICE,
                IN_GAME,
                IN_GAME_ACTIVE,
                PAUSE,
                GAME_OVER
            };

            Network(std::shared_ptr<libclient::Callback> c, std::shared_ptr<libclient::Model> m);

            [[nodiscard]] NetworkState getState() const;

            bool connect(const std::string &servername, int port);

            void disconnect();

            bool sendHello(const std::string &name, spy::network::RoleEnum role);

            bool sendItemChoice(std::variant<spy::util::UUID, spy::gadget::GadgetEnum> choice);

            bool sendEquipmentChoice(std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> equipment);

            bool sendGameOperation(spy::gameplay::Operation operation);

            bool sendGameLeave();

            bool sendRequestGamePause(bool gamePause);

            bool sendRequestMetaInformation(std::vector<spy::network::messages::MetaInformationKey> keys);

            bool sendRequestReplayMessage();

            bool sendReconnect();

        private:
            std::shared_ptr<Callback> callback;
            std::shared_ptr<Model> model;
            std::optional<websocket::network::WebSocketClient> webSocketClient;
            NetworkState state = NetworkState::NOT_CONNECTED;
            NetworkState stateBeforePause;

            std::string serverName;
            int serverPort;

            /**
             * function to handle received messages
             * @param message std::string received message from server
             */
            void onReceiveMessage(const std::string& message);

            /**
             * function to handle connection lost
             */
            void onClose();
    };
}

#endif //LIBCLIENT_NETWORK_HPP
