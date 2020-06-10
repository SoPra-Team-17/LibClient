//
// Created by Carolin on 06.06.2020.
//

#ifndef LIBCLIENT_AISTATE_HPP
#define LIBCLIENT_AISTATE_HPP


#include <util/UUID.hpp>
#include <vector>
#include <datatypes/character/FactionEnum.hpp>
#include <datatypes/gameplay/State.hpp>
#include <variant>
#include <datatypes/gameplay/GadgetAction.hpp>
#include <util/OrderUtils.hpp>
#include <datatypes/matchconfig/MatchConfig.hpp>

namespace libclient::model {
    class AIState {
            // TODO (optional): count hp of characters to check for exfiltration (might find out who has AntiPlagueMask and take into account) -> more variables needed
            // TODO (optional): track who uses actions against which persons (probs for faction groups) -> more variables needed
            // TODO (optional): character got rid of moledie -> is npc (but could also be enemy trying to tarn as npc) with prob -> more variables needed

        public:
            // double is percentage to show how sure one is
            std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, double>>> unknownFaction; // initially set by HelloReply message
            std::set<spy::util::UUID> myFaction; // set by RequestEquipmentChoice message
            std::set<spy::util::UUID> enemyFaction; // set during game
            std::set<spy::util::UUID> npcFaction; // set during game

            // double is percentage to show how sure one is
            std::map<std::shared_ptr<spy::gadget::Gadget>, std::vector<std::pair<spy::util::UUID, double>>, util::cmpGadgetPtr> unknownGadgets; // initially set by HelloReply message
            std::map<std::shared_ptr<spy::gadget::Gadget>, spy::util::UUID, util::cmpGadgetPtr> characterGadgets; // initially set by sendEquipmentChoice method
            std::set<std::shared_ptr<spy::gadget::Gadget>, util::cmpGadgetPtr> floorGadgets; // set by GameStatus message (but not if removed from floor)
            std::vector<std::variant<spy::util::UUID, spy::util::Point>> poisonedCocktails; // initially no cocktails are poisoned

            std::map<spy::util::UUID, std::set<spy::character::PropertyEnum>> properties; // initially set by Hello Reply message

            std::optional<spy::util::Point> posOfInvertedRoulette;

            spy::util::UUID lastCharTurn;
            bool gotRidOfMoleDie = false;

            std::set<int> openedSafes;
            // from safe to numberOfSafeCombinations when tried to open
            std::map<int, int> triedSafes;
            std::set<int> safeComibnations;
            std::set<int> openedSafesTotal;

            /**
             * applies all lists without "unknown" in name to current state
             * @param s current state
             * @param me FactionEnum of my faction
             */
            void
            applySureInformation(spy::gameplay::State &s, spy::character::FactionEnum me); // done by GameStatus message

            /**
             * moves character id from unknownFaction list to specified faction list
             * @param id id of character to be set
             * @param factionList list that character should be moved to
             * @return true if method was successful
             */
            bool addFaction(const spy::util::UUID &id, std::set<spy::util::UUID> &factionList);

            /**
             * moves gadget to characterGadgets or floorGadgets if not already in list
             * @param gadgetType type of gadget to be set
             * @param id id of character to move to, if not given gadget is moved to floor
             * @return true if method was successful
             */
            bool addGadget(spy::gadget::GadgetEnum gadgetType, const std::optional<spy::util::UUID> &id);

            /**
            * processes single operation into state lists/maps/...
            * @param operation operation to be processed
            * @param s state without operation applied
            * @param config match config
            */
            void processOperation(std::shared_ptr<const spy::gameplay::BaseOperation> operation,
                                  const spy::gameplay::State &s, const spy::MatchConfig &config,
                                  spy::character::FactionEnum me); // done by GameStatus message

        private:
            /**
             * moves gadget to characterGadgets list
             * @param gadgetType gadget representing type to be added to character
             * @param id id of character to move to
             * @return true if method was successful
             */
            bool
            addGadgetToCharacter(const std::shared_ptr<spy::gadget::Gadget> &gadgetType, const spy::util::UUID &id);

            /**
             * moves gadget to floorGadgets list
             * @param gadgetType gadget representing type to be added to floor
             * @return true if method was successful
             */
            bool addGadgetToFloor(const std::shared_ptr<spy::gadget::Gadget> &gadgetType);

            /**
             * processes single gadget action into state lists/maps/...
             * @param action gadget action to be processed
            * @param s state without gadget action applied
            * @param config match config
             */
            void processGadgetAction(std::shared_ptr<const spy::gameplay::GadgetAction> action,
                                     const spy::gameplay::State &s, const spy::MatchConfig &config);

            /**
             * get faction of character according to AIState
             * @param id character id of the character faction wants to be known
             * @return faction, INVALID if unknown, NEURAL if npc, PLAYER1 if my, PLAYER2 if enemy
             */
            spy::character::FactionEnum getFaction(const spy::util::UUID &id);

            void modifyUsagesLeft(const std::shared_ptr<spy::gadget::Gadget> &gad);

            std::optional<double> hasCharacterGadget(const spy::util::UUID &id, spy::gadget::GadgetEnum type);

            bool hasCharacterProperty(const spy::util::UUID &id, spy::character::PropertyEnum prop);

            void push_back_toUnknownGadgets(std::shared_ptr<spy::gadget::Gadget> key, const spy::util::UUID &val, double certainty);

            void push_back_toUnknownFaction(const spy::util::UUID &key, spy::character::FactionEnum val, double certainty);
    };
}


#endif //LIBCLIENT_AISTATE_HPP
