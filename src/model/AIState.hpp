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
            std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, float>>> unknownFaction; // initially set by HelloReply message
            std::vector<spy::util::UUID> myFaction; // set by RequestEquipmentChoice message
            std::vector<spy::util::UUID> enemyFaction; // set during game
            std::vector<spy::util::UUID> npcFaction; // set during game

            // TODO set cocktails, unknownGadgets, floorGadgets and characterGadgets during game
            std::unordered_map<std::shared_ptr<spy::gadget::Gadget>, std::vector<std::pair<spy::util::UUID, float>>> unknownGadgets; // initially set by HelloReply message
            std::unordered_map<std::shared_ptr<spy::gadget::Gadget>, spy::util::UUID> characterGadgets; // initially set by sendEquipmentChoice method
            std::vector<std::shared_ptr<spy::gadget::Gadget>> floorGadgets; // initially set by first GameStatus message
            std::map<std::variant<spy::util::UUID, spy::util::Point>, bool> cocktails; // initially set by first GameStatus message

            // TODO set / remove property ClammyClothes during game
            std::map<spy::util::UUID, std::set<spy::character::PropertyEnum>> properties; // initially set by Hello Reply message

            void applySureInformation(spy::gameplay::State &s, spy::character::FactionEnum me); // done by GameStatus message

            bool addFaction(spy::util::UUID &id, std::vector<spy::util::UUID> &factionList);

            bool addGadget(const std::shared_ptr<spy::gadget::Gadget> &gadget, const std::optional<spy::util::UUID> &id);
    };
}


#endif //LIBCLIENT_AISTATE_HPP
