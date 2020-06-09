//
// Created by Carolin on 06.06.2020.
//

#include "AIState.hpp"
#include <datatypes/gadgets/Cocktail.hpp>

namespace libclient::model {
    void AIState::applySureInformation(spy::gameplay::State &s, spy::character::FactionEnum me) {
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
            if (c.getFaction() != me) {
                c.setProperties(properties[c.getCharacterId()]);
            }
        }

        // gadgets
        for (const auto &it : characterGadgets) {
            auto c = s.getCharacters().getByUUID(it.second);
            if (c->getFaction() != me && !c->hasGadget(it.first->getType())) {
                c->addGadget(it.first);
            }
        }
        for (const auto &it: poisonedCocktails) {
            if (std::holds_alternative<spy::util::UUID>(it)) {
                // character has cocktail
                auto c = s.getCharacters().getByUUID(std::get<spy::util::UUID>(it));
                auto cocktail = c->getGadget(spy::gadget::GadgetEnum::COCKTAIL);
                if (cocktail.has_value()) {
                    std::dynamic_pointer_cast<spy::gadget::Cocktail>(cocktail.value())->setIsPoisoned(true);
                }
            } else {
                // cocktail is on playing field
                auto cocktail = s.getMap().getField(std::get<spy::util::Point>(it)).getGadget();
                if (cocktail.has_value()) {
                    std::dynamic_pointer_cast<spy::gadget::Cocktail>(cocktail.value())->setIsPoisoned(true);
                }
            }
        }
    }

    bool AIState::addFaction(const spy::util::UUID &id, std::vector<spy::util::UUID> &factionList) {
        auto charId = unknownFaction.find(id);
        if (charId == unknownFaction.end()) {
            return false;
        }
        factionList.push_back(charId->first);
        unknownFaction.erase(charId);
        return true;
    }

    bool
    AIState::addGadget(const std::shared_ptr<spy::gadget::Gadget> &gadget, const std::optional<spy::util::UUID> &id) {
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

    void AIState::processOperationList(const std::vector<std::shared_ptr<const spy::gameplay::BaseOperation>> &operationList) {
        for (auto &op: operationList) {
            processOperation(op);
        }
    }

    void AIState::processOperation(std::shared_ptr<const spy::gameplay::BaseOperation> operation) {
        switch (operation->getType()) {
            case spy::gameplay::OperationEnum::GADGET_ACTION:
                // TODO
                break;
            case spy::gameplay::OperationEnum::SPY_ACTION:
                // TODO
                break;
            case spy::gameplay::OperationEnum::PROPERTY_ACTION:
                // TODO
                break;
            case spy::gameplay::OperationEnum::MOVEMENT:
                // TODO
                break;
            default:
                // no additional info can be gained
                break;
        }
    }

}
