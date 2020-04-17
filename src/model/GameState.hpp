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
#include <datatypes/gameplay/Operation.hpp>
#include <datatypes/gameplay/State.hpp>
#include <datatypes/statistics/Statistics.hpp>
#include <datatypes/statistics/VictoryEnum.hpp>

namespace libclient::model {
    class GameState {
        public:
            spy::scenario::Scenario level;
            spy::MatchConfig settings;
            std::vector<spy::character::CharacterInformation> characterSettings;
            std::vector<spy::util::UUID> offeredCharacters;
            std::vector<spy::gadget::GadgetEnum> offeredGadgets;
            std::vector<spy::util::UUID> chosenCharacter;
            std::vector<spy::gadget::GadgetEnum> chosenGadget;
            std::map<spy::util::UUID, spy::gadget::GadgetEnum> equipmentMap;
            std::vector<spy::gameplay::Operation> operations;
            spy::gameplay::State state;
            bool isGameOver = false;
            std::optional<spy::util::UUID> winner;
            std::optional<spy::statistics::Statistics> statistics;
            std::optional<spy::statistics::VictoryEnum> winningReason;
            bool hasReplay = false;
    };

}

#endif //LIBCLIENT_GAMESTATE_HPP
