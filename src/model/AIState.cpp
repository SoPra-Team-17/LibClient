//
// Created by Carolin on 06.06.2020.
//

#include "AIState.hpp"

namespace libclient::model {
    void AIState::applySureInformation(spy::gameplay::State &s, spy::character::FactionEnum me) {
        // factions
        using namespace spy::character;

        auto handleFaction = [](std::vector<spy::util::UUID> &list, FactionEnum faction, spy::character::Character &c) {
            auto it = std::find(list.begin(), list.end(), c.getCharacterId());
            if (it != list.end()) {
                c.setFaction(faction);
                return true;
            }
            return false;
        };

        for (auto &c: s.getCharacters()) {
            // faction
            if (handleFaction(npcFaction, FactionEnum::NEUTRAL, c)) {
                continue;
            }
            if (handleFaction(myFaction, me, c)) {
                continue;
            }
            if (handleFaction(enemyFaction, me == FactionEnum::PLAYER1 ? FactionEnum::PLAYER2 : FactionEnum::PLAYER1,
                              c)) {
                continue;
            }

            // properties
            c.setProperties(properties[c.getCharacterId()]);

            // TODO gadgets
        }
    }

    bool AIState::addFaction(spy::util::UUID &id, std::vector<spy::util::UUID> &factionList) {
        auto charId = std::find(factionList.begin(), factionList.end(), id);
        if (charId == factionList.end()) {
            return false;
        }
        factionList.push_back(*charId);
        unknownFaction.erase(*charId);
        return true;
    }

    bool AIState::addGadget(const std::shared_ptr<spy::gadget::Gadget> &gadget, const std::optional<spy::util::UUID> &id) {
        auto unknown = unknownGadgets.find(gadget);

        if (id.has_value()) {
            // add Gadget to characterGadgets list
            if (unknown == unknownGadgets.end()) {
                auto floor = std::find(floorGadgets.begin(), floorGadgets.end(), gadget);
                if (floor == floorGadgets.end()) {
                    return false;
                }
                // from floorGadgets list to characterGadgets list
                characterGadgets[gadget] = id.value();
                floorGadgets.erase(floor);
                return true;
            }
            // from unknownGadgets list to characterGadgets list
            characterGadgets[gadget] = id.value();
            unknownGadgets.erase(unknown);
            return true;
        }

        // add Gadget to floorGadgets list
        if (unknown == unknownGadgets.end()) {
            auto character = characterGadgets.find(gadget);
            if (character == characterGadgets.end()) {
                return false;
            }
            // from characterGadgets list to floorGadgets list
            floorGadgets.push_back(gadget);
            characterGadgets.erase(character);
            return true;
        }
        // from unknownGadgets list to floorGadgets list
        floorGadgets.push_back(gadget);
        unknownGadgets.erase(unknown);
        return true;
    }

}
