/**
 * @file    LibClient.hpp
 * @author  Marco
 * @date    14.04.2020 (creation)
 * @brief   Definition of the client interface
 */
#ifndef LIBCLIENT_LIBCLIENT_HPP
#define LIBCLIENT_LIBCLIENT_HPP

#include <model/Model.hpp>
#include <Network.hpp>
#include <Callback.hpp>
#include <util/UUID.hpp>

namespace libclient {
    class LibClient {
        private:
            std::shared_ptr<libclient::Model> model;
        public:
            explicit LibClient(Callback *callback);

            libclient::Network network;


            [[nodiscard]] const std::optional<spy::util::UUID> &getId() const;

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

            [[nodiscard]] const std::map<spy::network::messages::MetaInformationKey, spy::network::messages::MetaInformation::Info> &
            getInformation() const;

            [[nodiscard]] const std::optional<std::string> &getDebugMessage() const;

            [[nodiscard]] const spy::scenario::Scenario &getLevel() const;

            [[nodiscard]] const spy::MatchConfig &getSettings() const;

            [[nodiscard]] const std::vector<spy::character::CharacterInformation> &getCharacterSettings() const;

            [[nodiscard]] const std::vector<spy::util::UUID> &getOfferedCharacters() const;

            [[nodiscard]] const std::vector<spy::gadget::GadgetEnum> &getOfferedGadgets() const;

            [[nodiscard]] const std::vector<spy::util::UUID> &getChosenCharacters() const;

            [[nodiscard]] const std::vector<spy::gadget::GadgetEnum> &getChosenGadgets() const;

            [[nodiscard]] const std::map<spy::util::UUID, std::set<spy::gadget::GadgetEnum>> &getEquipmentMap() const;

            [[nodiscard]] const std::vector<std::shared_ptr<const spy::gameplay::BaseOperation>> &getOperations() const;

            [[nodiscard]] const spy::gameplay::State &getState() const;

            [[nodiscard]] bool isGameOver() const;

            [[nodiscard]] const std::optional<spy::util::UUID> &getWinner() const;

            [[nodiscard]] const std::optional<spy::statistics::Statistics> &getStatistics() const;

            [[nodiscard]] const std::optional<spy::statistics::VictoryEnum> &getWinningReason() const;

            [[nodiscard]] bool hasReplay() const;

            [[nodiscard]] const spy::util::UUID &getLastActiveCharacter() const;

            /**
             * set name of client
             * @param name name of client to be sent to server
             * @return true if setting worked (depends on network state)
             */
            bool setName(const std::string &name);

            [[nodiscard]] std::optional<std::pair<bool, spy::util::UUID>> isEnemy() const;

            [[nodiscard]] bool lastOpSuccessful() const;

            /**
             * set role of client
             * @param role role of client to be sent to server
             * @return true if setting worked (depends on network state)
             */
            bool setRole(const spy::network::RoleEnum &role);

            /**
             * checks if client is player1
             * @return nullopt for spectator or if state is not valid
             */
            [[nodiscard]] std::optional<bool> amIPlayer1();

            /**
             * @brief sets Faction of given character
             * @param id id of the client to set faction
             * @param facrion faction to be set to character
             * @return true if setting was successful (depends on network state and client role)
             */
            bool setFaction(spy::util::UUID id, spy::character::FactionEnum faction);

            [[nodiscard]] auto
            getUnknownFactionList() -> const std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, std::vector<double>>>> &;

            [[nodiscard]] auto
            getUnknownGadgetsList() -> const std::map<std::shared_ptr<spy::gadget::Gadget>, std::vector<std::pair<spy::util::UUID, std::vector<double>>>, util::cmpGadgetPtr> &;

            /**
            * find out how certain it is that given character has given faction
            * @param id id of the character to be searched for
            * @param faction faction of the character to be checked
            * @return nullopt if no info available, 0 and 1 indicate bool result, double between 0 and 1 indicates certainty
            */
            [[nodiscard]] std::optional<double>
            hasCharacterFaction(const spy::util::UUID &id, spy::character::FactionEnum faction);

            /**
             * find out how certain it is that given character has given gadget
             * @param id id of the character to be searched for
             * @param type type of the gadget to be checked
             * @return nullopt if no info available, 0 and 1 indicate bool result, double between 0 and 1 indicates certainty
             */
            [[nodiscard]] std::optional<double>
            hasCharacterGadget(const spy::util::UUID &id, spy::gadget::GadgetEnum type);

            /**
             * calculate unique index for safe
             * @param s current state
             * @param p position of the safe as Point
             * @return index for the safe at position p
             */
            unsigned int safePosToIndex(const spy::gameplay::State &s, const spy::util::Point &p);

            [[nodiscard]] const std::set<unsigned int> &getOpenedSafes();

            [[nodiscard]] const std::map<unsigned int, int> &getTriedSafes();

            [[nodiscard]] const std::set<spy::util::UUID> &getCombinationsFromNpcs();

            [[nodiscard]] const std::set<spy::util::UUID> &getMyFactionList();

            [[nodiscard]] const std::set<spy::util::UUID> &getEnemyFactionList();

            [[nodiscard]] const std::set<spy::util::UUID> &getNpcFactionList();

            /**
             * @brief       creates information string about an operation
             * @notes       client wants to display information about a operation, but can't pointer cast
             * @param op    operation, from which info is extracted
             * @return      std::string containing info
             */
            [[nodiscard]] std::string
            operationToString(const std::shared_ptr<const spy::gameplay::BaseOperation> op) const;

            /**
             * sets configs and initialises unknownLists in AIState
             * @param charInfo character config
             * @param matchInfo match config
             */
            void setConfigs(const std::vector<spy::character::CharacterInformation> &charInfo,
                            const spy::MatchConfig &matchInfo);

            [[nodiscard]] std::optional<spy::network::messages::Replay> getReplay() const;
    };

}


#endif //LIBCLIENT_LIBCLIENT_HPP
