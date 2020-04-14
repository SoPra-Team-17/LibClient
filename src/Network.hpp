/**
 * @file   Network.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the network class (Interface to Serve).
 */

#include <SopraNetwork/WebSocketClient.hpp>

#ifndef SAMPLELIBRARY_NETWORK_HPP
#define SAMPLELIBRARY_NETWORK_HPP

#include "CallbackClass.hpp"
#include "Model.hpp"

class Network {
    private:
        Model *model;
        CallbackClass *callback;

        /**
         * function to handle received messages
         * @param message std::string received message from server
         */
        void onReceiveMessage(std::string message);

    public:
        Network() = default;

        Network(CallbackClass *c, Model *m);

        // TODO define connect method
        // TODO define all sentMessage methods
};


#endif //SAMPLELIBRARY_NETWORK_HPP
