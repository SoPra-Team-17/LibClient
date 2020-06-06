//
// Created by Carolin on 06.06.2020.
//

#ifndef LIBCLIENT_AISTATE_HPP
#define LIBCLIENT_AISTATE_HPP


#include <util/UUID.hpp>
#include <vector>
#include <datatypes/character/FactionEnum.hpp>
#include <datatypes/gameplay/State.hpp>

namespace libclient::model {
    class AIState {
        public:
            std::map<spy::util::UUID, std::vector<std::pair<spy::character::FactionEnum, float>>> unknownFaction; // set by HelloReply message
            std::vector<spy::util::UUID> myFaction; // set by RequestEquipmentChoice message
            std::vector<spy::util::UUID> enemyFaction; // TODO
            std::vector<spy::util::UUID> npcFaction; // TODO

            // TODO gadgets
            //unknownGadgets; // TODO GadgetObject to vector of pair(UUID, float)
            //floorGadget; // TODO GadgetObject to Point
            //myGadgets; // TODO GadgetObject to UUID
            //otherGadget; // TODO GadgetObject to UUID
            // TODO helper to get GadgetObject by GadgetType -> no real map ! -> struct ?

            // TODO getter for AI and Client

            void applySureInformation(spy::gameplay::State &s, spy::character::FactionEnum me); // done by GameStatus message

            bool addFaction(spy::util::UUID &id, std::vector<spy::util::UUID> &factionList);
    };
}


#endif //LIBCLIENT_AISTATE_HPP
