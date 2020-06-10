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

        auto handleFaction = [](std::set<spy::util::UUID> &list, FactionEnum faction, spy::character::Character &c) {
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

        // inverted roulette
        if (posOfInvertedRoulette.has_value()) {
            s.getMap().getField(posOfInvertedRoulette.value()).isInverted() = true;
        }
    }

    bool AIState::addFaction(const spy::util::UUID &id, std::set<spy::util::UUID> &factionList) {
        auto charId = unknownFaction.find(id);
        if (charId == unknownFaction.end()) {
            return false;
        }
        factionList.insert(charId->first);
        unknownFaction.erase(charId);
        return true;
    }

    bool
    AIState::addGadget(spy::gadget::GadgetEnum gadgetType, const std::optional<spy::util::UUID> &id) {
        auto gadget = std::make_shared<spy::gadget::Gadget>(gadgetType);

        if (id.has_value()) {
            // add Gadget to characterGadgets list
            return addGadgetToCharacter(gadget, id.value());
        }

        // add Gadget to floorGadgets list
        return addGadgetToFloor(gadget);
    }

    bool
    AIState::addGadgetToCharacter(const std::shared_ptr<spy::gadget::Gadget> &gadgetType, const spy::util::UUID &id) {
        auto unknown = unknownGadgets.find(gadgetType);

        if (unknown == unknownGadgets.end()) {
            auto floor = std::find(floorGadgets.begin(), floorGadgets.end(), gadgetType);
            if (floor == floorGadgets.end()) {
                return false;
            }
            // from floorGadgets list to characterGadgets list
            characterGadgets[*floor] = id;
            floorGadgets.erase(floor);
            return true;
        }
        // from unknownGadgets list to characterGadgets list
        characterGadgets[unknown->first] = id;
        unknownGadgets.erase(unknown);
        return true;
    }

    bool AIState::addGadgetToFloor(const std::shared_ptr<spy::gadget::Gadget> &gadgetType) {
        auto unknown = unknownGadgets.find(gadgetType);

        if (unknown == unknownGadgets.end()) {
            auto character = characterGadgets.find(gadgetType);
            if (character == characterGadgets.end()) {
                return false;
            }
            // from characterGadgets list to floorGadgets list
            floorGadgets.insert(character->first);
            characterGadgets.erase(character);
            return true;
        }
        // from unknownGadgets list to floorGadgets list
        floorGadgets.insert(unknown->first);
        unknownGadgets.erase(unknown);
        return true;
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

                auto sourceChar = s.getCharacters().findByUUID(op->getCharacterId());
                if (targetChar != s.getCharacters().end()) { // spy on person
                    // spy on me -> executor is enemy
                    bool isTargetCharMyFaction =
                            std::find(myFaction.begin(), myFaction.end(), targetChar->getCharacterId()) !=
                            myFaction.end();
                    bool isSourceCharMyFaction =
                            std::find(myFaction.begin(), myFaction.end(), sourceChar->getCharacterId()) !=
                            myFaction.end();
                    if (isTargetCharMyFaction && !isSourceCharMyFaction) {
                        addFaction(targetChar->getCharacterId(), enemyFaction);
                    }

                    // spy successful -> target is npc
                    if (op->isSuccessful()) {
                        addFaction(targetChar->getCharacterId(), npcFaction);
                    }

                    // TODO prob: spy not successful -> target is enemy with prob

                } else { // spy on safe
                    // TODO prob: spy on safe -> executor has diamond collar with prob
                }

                break;
            }
            case spy::gameplay::OperationEnum::PROPERTY_ACTION: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::PropertyAction>(operation);

                if (op->getUsedProperty() == spy::character::PropertyEnum::OBSERVATION) {
                    // TODO prob: faction of target (but take pocket littler into account and success prob) with prob
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
        auto gadget = std::make_shared<spy::gadget::Gadget>(action->getGadget());
        auto targetChar = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(s.getCharacters(),
                                                                                     action->getTarget());
        auto sourceChar = s.getCharacters().findByUUID(action->getCharacterId());
        auto targetField = s.getMap().getField(action->getTarget());

        // executing character has gadget
        addGadgetToCharacter(gadget, action->getCharacterId());

        switch (action->getGadget()) {
            case spy::gadget::GadgetEnum::HAIRDRYER:
                // remove property clammy clothes from target character
                properties.at(targetChar->getCharacterId()).erase(spy::character::PropertyEnum::CLAMMY_CLOTHES);
                break;
            case spy::gadget::GadgetEnum::MOLEDIE:
                // TODO (prob): calc who now has moledie may with prob
                break;
            case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM:
                // invert roulette table
                posOfInvertedRoulette = action->getTarget();

                // after usage: disappear
                characterGadgets.erase(gadget);
                break;
            case spy::gadget::GadgetEnum::BOWLER_BLADE:
                // TODO prob: if not successful target has MAGENTIC_WATCH (prob of success, honey, babysitter) with prob
                break;
            case spy::gadget::GadgetEnum::POISON_PILLS:
                // cocktail at target is poisoned
                if (targetChar != s.getCharacters().end()) { // character holds cocktail
                    poisonedCocktails.push_back(targetChar->getCharacterId());
                } else { // cocktail is on bar table
                    poisonedCocktails.push_back(action->getTarget());
                }

                // after usage: modify usagesLeft
                modifyUsagesLeft(characterGadgets.find(gadget)->first);

                break;
            case spy::gadget::GadgetEnum::LASER_COMPACT:
                // if done on poisoned cocktail -> remove from poisonedCocktails list
                if (action->isSuccessful()) {
                    if (targetChar != s.getCharacters().end()) { // character holds cocktail
                        poisonedCocktails.erase(std::remove(poisonedCocktails.begin(), poisonedCocktails.end(),
                                                            std::variant<spy::util::UUID, spy::util::Point>(
                                                                    targetChar->getCharacterId())));
                    } else { // cocktail is on bar table
                        poisonedCocktails.erase(
                                std::remove(poisonedCocktails.begin(), poisonedCocktails.end(),
                                            std::variant<spy::util::UUID, spy::util::Point>(action->getTarget())));
                    }
                }
                break;
            case spy::gadget::GadgetEnum::ROCKET_PEN:
                // after usage: disappear
                characterGadgets.erase(gadget);
                break;
            case spy::gadget::GadgetEnum::GAS_GLOSS:
                // after usage: disappear
                characterGadgets.erase(gadget);
                break;
            case spy::gadget::GadgetEnum::MOTHBALL_POUCH:
                // after usage: modify usagesLeft
                modifyUsagesLeft(characterGadgets.find(gadget)->first);
                break;
            case spy::gadget::GadgetEnum::FOG_TIN:
                // after usage: disappear
                characterGadgets.erase(gadget);
                break;
            case spy::gadget::GadgetEnum::GRAPPLE:
                // add gadget hit by grapple to executing character
                if (action->isSuccessful()) {
                    addGadgetToCharacter(
                            std::make_shared<spy::gadget::Gadget>(targetField.getGadget().value()->getType()),
                            action->getCharacterId());
                }
                break;
            case spy::gadget::GadgetEnum::WIRETAP_WITH_EARPLUGS:
                // after usage: disappear
                characterGadgets.erase(gadget);
                break;
            case spy::gadget::GadgetEnum::JETPACK:
                // after usage: disappear
                characterGadgets.erase(gadget);
                break;
            case spy::gadget::GadgetEnum::CHICKEN_FEED:
                // not working -> target is np, working -> target is enemy
                if (action->isSuccessful()) {
                    addFaction(targetChar->getCharacterId(), enemyFaction);
                } else if (getFaction(sourceChar->getCharacterId()) != getFaction(targetChar->getCharacterId())) {
                    addFaction(targetChar->getCharacterId(), npcFaction);
                }

                // after usage: disappear
                characterGadgets.erase(gadget);

                break;
            case spy::gadget::GadgetEnum::NUGGET:
                // not working -> target is enemy, working -> target was npc and now joins my faction
                // after usage: not working -> move to target character, working -> disappear
                if (action->isSuccessful()) {
                    addFaction(targetChar->getCharacterId(), npcFaction);
                    npcFaction.erase(targetChar->getCharacterId());
                    myFaction.insert(targetChar->getCharacterId());

                    characterGadgets.erase(gadget);
                } else {
                    if (getFaction(sourceChar->getCharacterId()) != getFaction(targetChar->getCharacterId())) {
                        addFaction(targetChar->getCharacterId(), enemyFaction);
                    }
                    characterGadgets[gadget] = targetChar->getCharacterId();
                }
                break;
            case spy::gadget::GadgetEnum::MIRROR_OF_WILDERNESS:
                // after usage: not same faction and working -> disappear
                if (getFaction(sourceChar->getCharacterId()) != getFaction(targetChar->getCharacterId()) &&
                    action->isSuccessful()) {
                    characterGadgets.erase(gadget);
                }
                break;
            case spy::gadget::GadgetEnum::COCKTAIL: {
                bool pour = targetChar != s.getCharacters().end();
                bool drink = sourceChar->getCoordinates().value() == action->getTarget();

                // poured or drunk -> remove from poisonedCocktails list
                if (pour || drink) {
                    poisonedCocktails.erase(
                            std::remove(poisonedCocktails.begin(), poisonedCocktails.end(),
                                        std::variant<spy::util::UUID, spy::util::Point>(action->getCharacterId())));
                }

                // successfully poured -> add property clammy clothes to target
                if (pour && action->isSuccessful()) {
                    properties.at(targetChar->getCharacterId()).insert(spy::character::PropertyEnum::CLAMMY_CLOTHES);
                }

                // taken from bar table and poisoned -> update poisonedCocktails
                if (!pour && !drink) {
                    auto cocktail = std::remove(poisonedCocktails.begin(), poisonedCocktails.end(),
                                                std::variant<spy::util::UUID, spy::util::Point>(action->getTarget()));
                    if (cocktail != poisonedCocktails.end()) {
                        // cocktail from bar table is poisoned
                        poisonedCocktails.erase(cocktail);
                        poisonedCocktails.push_back(action->getCharacterId());
                    }
                }

                break;
            }
            default:
                // no additional info can be gained
                break;
        }
    }

    spy::character::FactionEnum AIState::getFaction(const spy::util::UUID &id) {
        auto isInList = [&id](std::set<spy::util::UUID> &list) {
            auto it = std::find(list.begin(), list.end(), id);
            return it != list.end();
        };

        if (isInList(npcFaction)) {
            return spy::character::FactionEnum::NEUTRAL;
        }
        if (isInList(myFaction)) {
            return spy::character::FactionEnum::PLAYER1;
        }
        if (isInList(enemyFaction)) {
            return spy::character::FactionEnum::PLAYER2;
        }
        return spy::character::FactionEnum::INVALID;

    }

    void AIState::modifyUsagesLeft(const std::shared_ptr<spy::gadget::Gadget> &gad) {
        gad->setUsagesLeft(gad->getUsagesLeft().value() - 1);
        if (gad->getUsagesLeft() == 0) {
            characterGadgets.erase(gad);
        }
    }

}
