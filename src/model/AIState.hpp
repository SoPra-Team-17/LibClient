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

namespace libclient::model {
    class AIState {
        public:
            // TODO set unknownFaction, enemyFaction and npcFaction during game
            // float is percentage to show how sure one is
            std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, float>>> unknownFaction; // initially set by HelloReply message
            std::vector<spy::util::UUID> myFaction; // set by RequestEquipmentChoice message
            std::vector<spy::util::UUID> enemyFaction; // set during game
            std::vector<spy::util::UUID> npcFaction; // set during game

            // TODO set poisonedCocktails, unknownGadgets, floorGadgets and characterGadgets during game
            // float is percentage to show how sure one is
            std::unordered_map<std::shared_ptr<spy::gadget::Gadget>, std::vector<std::pair<spy::util::UUID, float>>> unknownGadgets; // initially set by HelloReply message
            std::unordered_map<std::shared_ptr<spy::gadget::Gadget>, spy::util::UUID> characterGadgets; // initially set by sendEquipmentChoice method
            std::vector<std::shared_ptr<spy::gadget::Gadget>> floorGadgets; // initially set by first GameStatus message
            std::vector<std::variant<spy::util::UUID, spy::util::Point>> poisonedCocktails; // initially no cocktails are poisoned

            // TODO set / remove property ClammyClothes during game
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
             * moves gadget from unkownGadgets list to characterGadgets or floorGadgets
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
            void processOperation(std::shared_ptr<const spy::gameplay::BaseOperation> operation);
    };
}


#endif //LIBCLIENT_AISTATE_HPP
