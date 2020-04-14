/**
 * @file   Network.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the network class (Interface to Serve).
 */

#include <SopraNetwork/WebSocketClient.hpp>

#ifndef SAMPLELIBRARY_NETWORK_HPP
#define SAMPLELIBRARY_NETWORK_HPP

#include "Callback.hpp"
#include "model/Model.hpp"

class Network {
    private:
        Model *model;
        Callback *callback;

        /**
         * function to handle received messages
         * @param message std::string received message from server
         */
        void onReceiveMessage(std::string message);

    public:
        Network() = default;

        Network(Callback *c, Model *m);

        // TODO define connect method
        // TODO define all sentMessage methods
};


#endif //SAMPLELIBRARY_NETWORK_HPP
