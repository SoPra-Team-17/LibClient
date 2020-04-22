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
            spy::util::UUID playerOneId; //set by GameStarted message
            spy::util::UUID playerTwoId; //set by GameStarted message
            std::string playerOneName; //set by GameStarted message
            std::string playerTwoName; //set by GameStarted message
            spy::util::UUID activeCharacter; //set by RequestGameOperation message
            std::optional<spy::util::UUID> leftUserId; //set by GameLeft message
            std::optional<spy::network::ErrorTypeEnum> errorReason; //set by Error message
            bool gamePaused = false; //set by GamePause message
            bool serverEnforced = false; //set by GamePause message
            unsigned int strikeNr = 0; //set by Strike message
            unsigned int strikeMax; //set by Strike message
            std::string strikeReason; //set by Strike message
    };
}

#endif //LIBCLIENT_CLIENTSTATE_HPP
