/**
 * @file   Model.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the model class (contains all Info needed by Clients).
 */

#ifndef LIBCLIENT_MODEL_HPP
#define LIBCLIENT_MODEL_HPP

#include <model/AIState.hpp>
#include <model/ClientState.hpp>
#include <model/GameState.hpp>
#include <network/messages/Replay.hpp>

namespace libclient {
    class Model {
        public:
            model::AIState aiState;
            model::ClientState clientState;
            model::GameState gameState;
            std::optional<spy::network::messages::Replay> replay; // set by REPLAY message
    };
}

#endif //LIBCLIENT_MODEL_HPP