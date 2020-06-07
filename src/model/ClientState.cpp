//
// Created by Carolin on 07.06.2020.
//

#include "ClientState.hpp"

namespace libclient::model {

    bool ClientState::amIPlayer1(spy::network::RoleEnum r) {
        return id.value() == playerOneId;
    }

}
