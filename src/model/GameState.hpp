/**
 * @file    GameState.hpp
 * @author  Marco
 * @date    14.04.2020 (creation)
 * @brief   Definition of GameState
 */

#ifndef LIBCLIENT_GAMESTATE_HPP
#define LIBCLIENT_GAMESTATE_HPP

#include <datatypes/scenario/Scenario.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>
#include <datatypes/character/CharacterInformation.hpp>
#include <datatypes/gameplay/BaseOperation.hpp>
#include <datatypes/gameplay/State.hpp>
#include <datatypes/statistics/Statistics.hpp>
#include <datatypes/statistics/VictoryEnum.hpp>

namespace libclient::model {
    class GameState {
        public:
            spy::scenario::Scenario level; //set by HelloReply message
            spy::MatchConfig settings; //set by HelloReply message
            std::vector<spy::character::CharacterInformation> characterSettings; //set by HelloReply message
            std::vector<spy::util::UUID> offeredCharacters; //set by RequestItemChoice message
            std::vector<spy::gadget::GadgetEnum> offeredGadgets; //set by RequestItemChoice message
            std::vector<spy::util::UUID> chosenCharacter; //set by RequestEquipmentChoice message
            std::vector<spy::gadget::GadgetEnum> chosenGadget; //set by RequestEquipmentChoice message
            std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> equipmentMap; // set by sendEquipmentChoice method
            std::vector<std::shared_ptr<const spy::gameplay::BaseOperation>> operations; //set by GameStatus message
            spy::gameplay::State state; //set by GameStatus message
            bool isGameOver = false; //set by GameStatus message
            std::optional<spy::util::UUID> winner; //set by Statistics message
            std::optional<spy::statistics::Statistics> statistics; //set by Statistics message
            std::optional<spy::statistics::VictoryEnum> winningReason; //set by Statistics message
            bool hasReplay = false; //set by Statistics message
            spy::util::UUID lastActiveCharacter; //set by GameStatusMessage
            bool lastOpSuccessful = false; // set by GameStatusMessage
            std::optional<std::pair<bool, spy::util::UUID>> isEnemy;    // set by GameStatusMessage

            /**
             * @brief   Helper function for client, to check if the last operation by the client was successfull
             * @note    To determine success a dynamic ptr cast is needed, which I wasn't able to achieve through cppyy
             * @param s up to date state without AIState information
             * @author  Marco Deuscher (Carolin changed param list)
             */
            void handleLastClientOperation(const spy::gameplay::State &s);
    };

}

#endif //LIBCLIENT_GAMESTATE_HPP
