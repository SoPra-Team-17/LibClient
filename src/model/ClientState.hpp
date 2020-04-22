/**
 * @file    ClientState.hpp
 * @author  Marco
 * @date    14.04.2020 (creation)
 * @brief   Definition of ClientState
 */

#ifndef LIBCLIENT_CLIENTSTATE_HPP
#define LIBCLIENT_CLIENTSTATE_HPP

#include <string>
#include <network/RoleEnum.hpp>
#include <network/ErrorTypeEnum.hpp>
#include <util/UUID.hpp>

namespace libclient::model {
    class ClientState {
        public:
            spy::util::UUID id;
            std::string name;
            bool isConnected = false;
            spy::network::RoleEnum role;
            spy::util::UUID sessionId;
            spy::util::UUID playerOneId;
            spy::util::UUID playerTwoId;
            std::string playerOneName;
            std::string playerTwoName;
            spy::util::UUID activeCharacter;
            std::optional<spy::util::UUID> leftUserId;
            std::optional<spy::network::ErrorTypeEnum> errorReason;
            bool gamePaused = false;
            bool serverEnforced = false;
            unsigned int strikeNr = 0;
            unsigned int strikeMax;
            std::string strikeReason;
    };
}

#endif //LIBCLIENT_CLIENTSTATE_HPP