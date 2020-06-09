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

namespace libclient::model {
    class AIState {
            // TODO: count hp of characters to check for exfiltration (might find out who has Anti Seuchen Schnabelmaske and take into account)
            // TODO: track who uses actions against which persons (probs for faction groups)

            // TODO: track which safes where opened with which key (remember opened safes and reset searched saves on new key) --> structure needed (on spy)

        public:
            // float is percentage to show how sure one is
            std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, float>>> unknownFaction; // initially set by HelloReply message
            std::vector<spy::util::UUID> myFaction; // set by RequestEquipmentChoice message
            std::vector<spy::util::UUID> enemyFaction; // set during game
            std::vector<spy::util::UUID> npcFaction; // set during game

            // float is percentage to show how sure one is
            std::unordered_map<std::shared_ptr<spy::gadget::Gadget>, std::vector<std::pair<spy::util::UUID, float>>> unknownGadgets; // initially set by HelloReply message
            std::unordered_map<std::shared_ptr<spy::gadget::Gadget>, spy::util::UUID> characterGadgets; // initially set by sendEquipmentChoice method
            std::vector<std::shared_ptr<spy::gadget::Gadget>> floorGadgets; // set by GameStatus message (but not if removed from floor)
            std::vector<std::variant<spy::util::UUID, spy::util::Point>> poisonedCocktails; // initially no cocktails are poisoned

            std::map<spy::util::UUID, std::set<spy::character::PropertyEnum>> properties; // initially set by Hello Reply message

            /**
             * applies all lists without "unknown" in name to current state
             * @param s current state
             * @param me FactionEnum of my faction
             */
            void applySureInformation(spy::gameplay::State &s, spy::character::FactionEnum me); // done by GameStatus message

            /**
             * moves character id from unknownFaction list to specified faction list
             * @param id id of character to be set
             * @param factionList list that character should be moved to
             * @return true if method was successful
             */
            bool addFaction(const spy::util::UUID &id, std::vector<spy::util::UUID> &factionList);

            /**
             * moves gadget to characterGadgets or floorGadgets if not already in list
             * @param gadget gadget to be set
             * @param id id of character to move to, if not given gadget is moved to floor
             * @return true if method was successful
             */
            bool addGadget(const std::shared_ptr<spy::gadget::Gadget> &gadget, const std::optional<spy::util::UUID> &id);

            /**
             * processes given operations into state lists/maps/...
             * @param operationList list of operations to be processed
             */
            void processOperationList(const std::vector<std::shared_ptr<const spy::gameplay::BaseOperation>> &operationList); // done by GameStatus message

        private:
            /**
             * moves gadget to characterGadgets list
             * @param gadget gadget to be added to character
             * @param id id of character to move to
             * @return true if method was successful
             */
            bool addGadgetToCharacter(const std::shared_ptr<spy::gadget::Gadget> &gadget, const std::optional<spy::util::UUID> &id);

            /**
             * moves gadget to floorGadgets list
             * @param gadget gadget to be added to floor
             * @return true if method was successful
             */
            bool addGadgetToFloor(const std::shared_ptr<spy::gadget::Gadget> &gadget);

            /**
             * processes single operation into state lists/maps/...
             * @param operation operation to be processed
             */
            void processOperation(std::shared_ptr<const spy::gameplay::BaseOperation> operation);

            /**
             * processes single gadget action into state lists/maps/...
             * @param action gaget action to be processed
             */
            void processGadgetAction(std::shared_ptr<const spy::gameplay::GadgetAction> action);
    };
}


#endif //LIBCLIENT_AISTATE_HPP
