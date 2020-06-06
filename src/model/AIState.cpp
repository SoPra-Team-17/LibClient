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
        }

        // TODO gadgets
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

}
