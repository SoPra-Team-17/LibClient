//
// Created by Carolin on 06.06.2020.
//

#include "AIState.hpp"
#include <datatypes/gadgets/Cocktail.hpp>
#include <datatypes/gameplay/PropertyAction.hpp>
#include <datatypes/gameplay/SpyAction.hpp>
#include <util/GameLogicUtils.hpp>

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


        if (id.has_value()) {
            // add Gadget to characterGadgets list
            return addGadgetToCharacter(gadget, id);
        }

        // add Gadget to floorGadgets list
        return addGadgetToFloor(gadget);
    }

    bool AIState::addGadgetToCharacter(const std::shared_ptr<spy::gadget::Gadget> &gadget,
                                       const std::optional<spy::util::UUID> &id) {
        auto unknown = unknownGadgets.find(gadget);

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

    bool AIState::addGadgetToFloor(const std::shared_ptr<spy::gadget::Gadget> &gadget) {
        auto unknown = unknownGadgets.find(gadget);

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

    void AIState::processOperationList(
            const std::vector<std::shared_ptr<const spy::gameplay::BaseOperation> > &operationList,
            const spy::gameplay::State &s) {
        for (const auto &op: operationList) {
            processOperation(op, s);
        }
    }

    void AIState::processOperation(std::shared_ptr<const spy::gameplay::BaseOperation> operation,
                                   const spy::gameplay::State &s) {
        switch (operation->getType()) {
            case spy::gameplay::OperationEnum::GADGET_ACTION: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::GadgetAction>(operation);
                processGadgetAction(op, s);
                break;
            }
            case spy::gameplay::OperationEnum::SPY_ACTION: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::SpyAction>(operation);

                auto targetChar = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(s.getCharacters(),
                                                                                             op->getTarget());
                if (targetChar != s.getCharacters().end()) { // spy on person
                    // spy on me -> executor is enemy
                    bool isTargetCharMyFaction =
                            std::find(myFaction.begin(), myFaction.end(), targetChar) != myFaction.end();
                    if (isTargetCharMyFaction) {
                        addFaction(targetChar->getCharacterId(), enemyFaction);
                    }

                    // spy successful -> target is npc
                    if (op->isSuccessful()) {
                        addFaction(targetChar->getCharacterId(), npcFaction);
                    }

                    // TODO: spy not successful -> target is enemy with prob

                } else { // spy on safe
                    // TODO: spy on safe -> executor has diamond collar with prob
                }

                break;
            }
            case spy::gameplay::OperationEnum::PROPERTY_ACTION: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::PropertyAction>(operation);

                if (op->getUsedProperty() == spy::character::PropertyEnum::OBSERVATION) {
                    // TODO: faction of target (but take pocket littler into account and success prob) with prob
                }

                break;
            }
            case spy::gameplay::OperationEnum::MOVEMENT: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::Movement>(operation);

                // collect gadget if gadget is on target field
                auto gadget = s.getMap().getField(op->getTarget()).getGadget();
                if (gadget.has_value()) {
                    addGadgetToCharacter(gadget.value(), op->getCharacterId());
                }

                break;
            }
            default:
                // no additional info can be gained
                break;
        }
    }

    void AIState::processGadgetAction(std::shared_ptr<const spy::gameplay::GadgetAction> action,
                                      const spy::gameplay::State &s) {
        switch (action->getGadget()) {
            // TODO: source char has gadget
            case spy::gadget::GadgetEnum::HAIRDRYER:
                // TODO: remove property clammy clothes of target
                break;
            case spy::gadget::GadgetEnum::MOLEDIE:
                // TODO: npc gets rid of this in turn (but could also be enemy trying to tarn as npc)
                // TODO: calc who now has moledie may with prob
                break;
            case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM:
                // TODO: inverts target (--> variable needed)
                // TODO: after usage erase
                break;
            case spy::gadget::GadgetEnum::BOWLER_BLADE:
                // TODO: if not successful target has MAGENTIC_WATCH (prob of success, honey, babysitter) with prob
                break;
            case spy::gadget::GadgetEnum::POISON_PILLS:
                // TODO: target cocktail poisoned
                // TODO: after usage mod usagesLeft or erase
                break;
            case spy::gadget::GadgetEnum::LASER_COMPACT:
                // TODO: if done on poisoned cocktail -> remove from list
                break;
            case spy::gadget::GadgetEnum::ROCKET_PEN:
                // TODO: after usage erase
                break;
            case spy::gadget::GadgetEnum::GAS_GLOSS:
                // TODO: after usage erase
                break;
            case spy::gadget::GadgetEnum::MOTHBALL_POUCH:
                // TODO: after usage mod usagesLeft or erase
                break;
            case spy::gadget::GadgetEnum::FOG_TIN:
                // TODO: after usage erase
                break;
            case spy::gadget::GadgetEnum::GRAPPLE:
                // TODO: add grappled gadget to character
                break;
            case spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS:
                // TODO: after usage erase
                break;
            case spy::gadget::GadgetEnum::JETPACK:
                // TODO: after usage erase
                break;
            case spy::gadget::GadgetEnum::CHICKEN_FEED:
                // TODO: not working -> target is npc, working -> target is enemy
                // TODO: after usage erase
                break;
            case spy::gadget::GadgetEnum::NUGGET:
                // TODO: not working -> target is enemy, working -> target was npc and now joins my faction
                // TODO: after usage move to enemy (not working) or erase (working)
                // TODO: if working: add npc to chosen characters for Client
                break;
            case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
                // TODO: after working usage erase
                break;
            case spy::gadget::GadgetEnum::COCKTAIL:
                // TODO: add property clammy clothes
                // TODO: if poisoned cocktail is drunk -> remove from list
                break;
            default:
                // no additional info can be gained
                break;
        }
    }

}
