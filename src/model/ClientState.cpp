//
// Created by Carolin on 07.06.2020.
//

#include "ClientState.hpp"

namespace libclient::model {

    bool ClientState::amIPlayer1() {
        return id.value() == playerOneId;
    }

}
