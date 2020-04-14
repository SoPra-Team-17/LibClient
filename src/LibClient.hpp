//
// Created by marco on 14.04.20.
//

#ifndef LIBCLIENT_LIBCLIENT_HPP
#define LIBCLIENT_LIBCLIENT_HPP

#include <model/Model.hpp>
#include <Network.hpp>

namespace libclient {
    class LibClient {
        public:
            LibClient() = default;


            [[nodiscard]] const std::string &getName() const;
            [[nodiscard]] bool isConnected() const;
            [[nodiscard]] const spy::network::RoleEnum &getRole() const;
            [[nodiscard]] const spy::util::UUID &getSessionId() const;
            [[nodiscard]] const spy::util::UUID &getPlayerOneId() const;
            [[nodiscard]] const spy::util::UUID &getPlayerTwoId() const;
            [[nodiscard]] const std::string &getPlayerOneName() const;
            [[nodiscard]] const std::string &getPlayerTwoName() const;
            [[nodiscard]] const spy::util::UUID &getActiveCharacter() const;
            [[nodiscard]] const std::optional<spy::util::UUID> &getLeftUserId() const;
            [[nodiscard]] const std::optional<spy::network::ErrorTypeEnum> &getErrorReason() const;
            [[nodiscard]] bool isGamePaused() const;
            [[nodiscard]] bool isServerEnforced() const;
            [[nodiscard]] const unsigned int &getStrikeNr() const;
            [[nodiscard]] const unsigned int &getStrikeMax() const;
            [[nodiscard]] const std::string &getStrikeReason() const;

            [[nodiscard]] const spy::scenario::Scenario &getLevel() const;
            [[nodiscard]] const spy::MatchConfig &getSettings() const;
            [[nodiscard]] const std::vector<spy::character::CharacterInformation> &getCharacterSettings() const;
            [[nodiscard]] const std::vector<spy::util::UUID> &getOfferedCharacters() const;
            [[nodiscard]] const std::vector<spy::gadget::GadgetEnum> &getOfferedGadgets() const;
            [[nodiscard]] const std::vector<spy::util::UUID> &getChosenCharacters() const;
            [[nodiscard]] const std::vector<spy::gadget::GadgetEnum> &getChosenGadgets() const;
            [[nodiscard]] const std::map<spy::util::UUID, spy::gadget::GadgetEnum> &getEquipmentMap() const;
            [[nodiscard]] const std::vector<spy::gameplay::Operation> &getOperations() const;
            [[nodiscard]] const spy::gameplay::State &getState() const;
            [[nodiscard]] bool isGameOver() const;
            [[nodiscard]] const std::optional<spy::util::UUID> &getWinner() const;
            [[nodiscard]] const std::optional<spy::statistics::Statistics> &getStatistics() const;
            [[nodiscard]] const std::optional<spy::statistics::VictoryEnum> &getWinningReasoin() const;
            [[nodiscard]] bool getHasReplay() const;

        private:
            libclient::model::Model model;

    };

}


#endif //LIBCLIENT_LIBCLIENT_HPP
