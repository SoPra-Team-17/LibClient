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
            std::optional<spy::util::UUID> id; //set by HelloReply message
            std::string name;
            bool isConnected = false; //set to true by connect method
            spy::network::RoleEnum role;
            spy::util::UUID sessionId; //set by HelloReply message
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
