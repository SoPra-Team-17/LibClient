/**
 * @file   Model.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the model class (contains all Info needed by Clients).
 */

#ifndef SAMPLELIBRARY_LIBRARY_H
#define SAMPLELIBRARY_LIBRARY_H

#include <model/ClientState.hpp>
#include <model/GameState.hpp>
#include <network/messages/Replay.hpp>
#include <LibClient.hpp>

namespace libclient {
    class Model {
        public:
            Model() = default;

            libclient::model::ClientState clientState;
            libclient::model::GameState gameState;
            spy::network::messages::Replay replay;
    };
}

#endif //SAMPLELIBRARY_LIBRARY_H