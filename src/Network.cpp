/**
 * @file   Network.hcp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Definition of the network class (Interface to Server).
 */

#include "Network.hpp"

Network::Network(Callback *c, Model *m)  {
    callback = c;
    model = m;
}

void Network::onReceiveMessage(std::string message) {
    //TODO switch MessageType and call callback->onReceiveMessage
}