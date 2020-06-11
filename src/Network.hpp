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
#include <datatypes/gameplay/BaseOperation.hpp>
#include <Client/WebSocketClient.hpp>

namespace libclient {

    /**
     * implements Network interface for Clients
     * method naming pattern: sendMessage(...) with Message being the type of Message to send
     */
    class Network {
        public:
            enum NetworkState {
                NOT_CONNECTED,
                CONNECTED,
                RECONNECT,
                SENT_HELLO,
                WELCOMED,
                IN_ITEMCHOICE,
                IN_EQUIPMENTCHOICE,
                IN_GAME,
                IN_GAME_ACTIVE,
                PAUSE,
                GAME_OVER
            };

            Network(libclient::Callback *c, std::shared_ptr<libclient::Model> m);

            [[nodiscard]] NetworkState getState() const;

            /**
             * reconnect after Client in role PLAYER crashed (!! model is reset -> clients job to get info if needed !!)
             * @param servername name or ip of server
             * @param port port of server
             * @param clientName name of client / player
             * @param clientId id client had before crash
             * @param sessionId id of session client was in before crash
             * @return true if reconnect message could be sent
             */
            bool reconnectPlayerAfterCrash(const std::string &servername, int port, std::string clientName,
                                           spy::util::UUID clientId, spy::util::UUID sessionId);

            /**
             * connect to specified server (possible in following states: NOT_CONNECTED, CONNECTED, RECONNECT, GAME_OVER)
             * @param servername name or ip of server
             * @param port port of server
             * @return true if connect was successful (depends on NetworkState, ...)
             */
            bool connect(const std::string &servername, int port);

            /**
             * disconnect from server, model is reset (can be called any time, e.g. to force connect method to work)
             */
            void disconnect();

            bool sendHello(const std::string &name, spy::network::RoleEnum role);

            bool sendItemChoice(std::variant<spy::util::UUID, spy::gadget::GadgetEnum> choice);

            bool sendEquipmentChoice(const std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> &equipment);

            bool sendGameOperation(const std::shared_ptr<spy::gameplay::BaseOperation> &operation,
                                   const spy::MatchConfig &config);

            bool sendGameLeave();

            bool sendRequestGamePause(bool gamePause);

            bool sendRequestMetaInformation(std::vector<spy::network::messages::MetaInformationKey> keys);

            bool sendRequestReplayMessage();

            bool sendReconnect();

        private:
            Callback *callback;
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
            void onReceiveMessage(const std::string &message);

            /**
             * function to handle connection lost
             */
            void onClose();

            /**
             * sets fields in AIState initially
             */
            void onFirstGameStatus();
    };
}

#endif //LIBCLIENT_NETWORK_HPP
