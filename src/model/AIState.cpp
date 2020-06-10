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

        // apply characterGadgets from state
        auto copyCharacterGadgets = characterGadgets;
        for (const auto &it : copyCharacterGadgets) {
            auto c = s.getCharacters().getByUUID(it.second);
            for (auto gad: c->getGadgets()) {
                addGadgetToCharacter(gad, c->getCharacterId());
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
            auto floor = floorGadgets.find(gadgetType);
            if (floor == floorGadgets.end()) {
                auto character = characterGadgets.find(gadgetType);
                if (character == characterGadgets.end()) {
                    return false;
                }
                // from characterGadgets list to characterGadgets list
                character->second = id;
                return true;
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
                auto floor = floorGadgets.find(gadgetType);
                return floor != floorGadgets.end(); // from floorGadgets list to floorGadgets list
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
                                   const spy::gameplay::State &s, const spy::MatchConfig &config,
                                   spy::character::FactionEnum me) {
        // check for getting rid of MOLEDIE
        auto opType = operation->getType();
        if (opType != spy::gameplay::OperationEnum::CAT_ACTION &&
            opType != spy::gameplay::OperationEnum::JANITOR_ACTION &&
            opType != spy::gameplay::OperationEnum::EXFILTRATION) {
            auto op = std::dynamic_pointer_cast<const spy::gameplay::CharacterOperation>(operation);
            if (op->getCharacterId() != lastCharTurn) {
                // next characters turn

                // character got not rid of MOLEDIE in turn -> character is enemy (take prob of having moledie into account)
                if (!gotRidOfMoleDie) {
                    auto probHasMoleDie = hasCharacterGadget(lastCharTurn, spy::gadget::GadgetEnum::MOLEDIE);
                    if (probHasMoleDie.has_value()) {
                        if (probHasMoleDie.value() == 1) {
                            addFaction(lastCharTurn, enemyFaction);
                        } else if (probHasMoleDie.value() != 0) {
                            push_back_toUnknownFaction(lastCharTurn, spy::character::FactionEnum::NEUTRAL,
                                                       probHasMoleDie.value());
                        }
                    }
                }

                lastCharTurn = op->getCharacterId();
                gotRidOfMoleDie = false;
            }
        }

        switch (operation->getType()) {
            case spy::gameplay::OperationEnum::GADGET_ACTION: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::GadgetAction>(operation);
                processGadgetAction(op, s, config);
                break;
            }
            case spy::gameplay::OperationEnum::SPY_ACTION: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::SpyAction>(operation);

                auto targetChar = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(s.getCharacters(),
                                                                                             op->getTarget());
                auto sourceChar = s.getCharacters().findByUUID(op->getCharacterId());
                bool isSourceCharMyFaction =
                        std::find(myFaction.begin(), myFaction.end(), sourceChar->getCharacterId()) !=
                        myFaction.end();

                if (targetChar != s.getCharacters().end()) { // spy on person
                    // spy on me -> executor is enemy
                    bool isTargetCharMyFaction =
                            std::find(myFaction.begin(), myFaction.end(), targetChar->getCharacterId()) !=
                            myFaction.end();
                    if (isTargetCharMyFaction && !isSourceCharMyFaction) {
                        addFaction(targetChar->getCharacterId(), enemyFaction);
                    }

                    // spy successful -> target is npc
                    if (op->isSuccessful() && isSourceCharMyFaction) {
                        addFaction(targetChar->getCharacterId(), npcFaction);
                    }

                    // spy not successful -> target is enemy with prob
                    if (!op->isSuccessful() && isSourceCharMyFaction && config.getSpySuccessChance() != 0) {
                        push_back_toUnknownFaction(targetChar->getCharacterId(),
                                                   me == spy::character::FactionEnum::PLAYER1
                                                   ? spy::character::FactionEnum::PLAYER2
                                                   : spy::character::FactionEnum::PLAYER1,
                                                   config.getSpySuccessChance());
                    }

                    // track safe combinations Client has
                    if (isSourceCharMyFaction && op->isSuccessful()) {
                        for (int comb: s.getMySafeCombinations()) {
                            safeComibnations.insert(comb);
                        }
                    }

                } else { // spy on safe
                    auto safeIndex = s.getMap().getField(op->getTarget()).getSafeIndex().value();

                    // track which safes are opened by Client
                    if (isSourceCharMyFaction) {
                        if (op->isSuccessful()) {
                            openedSafes.insert(safeIndex);
                            openedSafesTotal.insert(safeIndex);
                            triedSafes.erase(safeIndex);
                        } else {
                            triedSafes.insert(std::pair<int, int>(safeIndex, safeComibnations.size()));
                        }
                    }

                    // executor has diamond collar with prob
                    if (!isSourceCharMyFaction && op->isSuccessful() &&
                        std::find(openedSafesTotal.begin(), openedSafesTotal.end(), safeIndex) !=
                        openedSafesTotal.end()) { // safe was not opened before
                        openedSafesTotal.insert(safeIndex);

                        unsigned int maxSafeIndex = 1;
                        auto m = s.getMap();
                        for (auto y = 0U; y < m.getMap().size(); y++) {
                            for (auto x = 0U; x < m.getMap().at(y).size(); x++) {
                                const spy::scenario::Field f = m.getField(x, y);
                                if (f.getFieldState() == spy::scenario::FieldStateEnum::SAFE
                                    && f.getSafeIndex().value() > maxSafeIndex) {
                                    maxSafeIndex = f.getSafeIndex().value();
                                }
                            }
                        }

                        auto gad = std::make_shared<spy::gadget::Gadget>(spy::gadget::GadgetEnum::DIAMOND_COLLAR);
                        if (maxSafeIndex == 1) {
                            addGadgetToCharacter(gad, op->getCharacterId());
                        } else { // maxSafeIndex is always >= 1
                            double prob = 1 / maxSafeIndex;
                            push_back_toUnknownGadgets(gad, op->getCharacterId(), prob);
                        }
                    }
                }

                break;
            }
            case spy::gameplay::OperationEnum::PROPERTY_ACTION: {
                auto op = std::dynamic_pointer_cast<const spy::gameplay::PropertyAction>(operation);

                if (op->getUsedProperty() == spy::character::PropertyEnum::OBSERVATION) {

                    // isEnemy -> target character faction is enemy, not isEnemy -> target char is npc (take pocketlitter into account)
                    if (op->isSuccessful()) {
                        auto targetChar = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(
                                s.getCharacters(),
                                op->getTarget());
                        if (op->getIsEnemy()) {
                            addFaction(targetChar->getCharacterId(), enemyFaction);
                        } else {
                            auto probHasCharacterPocketLitter = hasCharacterGadget(op->getCharacterId(),
                                                                                   spy::gadget::GadgetEnum::POCKET_LITTER);
                            if (probHasCharacterPocketLitter.has_value()) {
                                if (probHasCharacterPocketLitter == 0) {
                                    addFaction(targetChar->getCharacterId(), npcFaction);
                                } else if (probHasCharacterPocketLitter.value() != 1) {
                                    push_back_toUnknownFaction(targetChar->getCharacterId(),
                                                               spy::character::FactionEnum::NEUTRAL,
                                                               1 - probHasCharacterPocketLitter.value());
                                }
                            }
                        }
                    }

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
                                      const spy::gameplay::State &s, const spy::MatchConfig &config) {
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
                // track getting rid of MOLEDIE
                gotRidOfMoleDie = true;

                // after usage: target is character -> target owns moledie (take honey trap into account)
                //              target is floor -> bowler blade is owned by closest character to target point
                if (targetChar != s.getCharacters().end()) { // target is character
                    if (!hasCharacterProperty(targetChar->getCharacterId(), spy::character::PropertyEnum::HONEY_TRAP)) {
                        addGadgetToCharacter(gadget, targetChar->getCharacterId());
                    } else {
                        characterGadgets.erase(gadget);
                        double prob = 1 - config.getHoneyTrapSuccessChance();
                        if (prob == 1) { // honeyTrapSuccessChange is 0
                            addGadgetToCharacter(gadget, targetChar->getCharacterId());
                        } else if (prob != 0) {
                            push_back_toUnknownGadgets(gadget, targetChar->getCharacterId(), prob);
                        }
                    }
                } else { // target is floor
                    auto closestPoints = spy::util::GameLogicUtils::getCharacterNearFields(s, action->getTarget());
                    if (closestPoints.size() == 1) {
                        // clear where moledie goes
                        auto closestPerson = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(
                                s.getCharacters(), closestPoints[0]);
                        addGadgetToCharacter(gadget, closestPerson->getCharacterId());
                    } else {
                        // unclear where moledie goes
                        double prob = 1 / closestPoints.size();
                        characterGadgets.erase(gadget);
                        for (auto p: closestPoints) {
                            auto person = spy::util::GameLogicUtils::findInCharacterSetByCoordinates(s.getCharacters(),
                                                                                                     p);
                            push_back_toUnknownGadgets(gadget, person->getCharacterId(), prob);
                        }
                    }
                }

                break;
            case spy::gadget::GadgetEnum::TECHNICOLOUR_PRISM:
                // invert roulette table
                posOfInvertedRoulette = action->getTarget();

                // after usage: disappear
                characterGadgets.erase(gadget);
                break;
            case spy::gadget::GadgetEnum::BOWLER_BLADE:
                // not working -> target has MAGENTIC_WATCH (take into account: prob of success) with prob
                if (!action->isSuccessful()) {
                    auto probHasMagenticWatch = hasCharacterGadget(targetChar->getCharacterId(), spy::gadget::GadgetEnum::MAGNETIC_WATCH);
                    if (probHasMagenticWatch.has_value()) {
                        if (probHasMagenticWatch.value() != 0 && probHasMagenticWatch.value() != 1) {
                            int numBabysitter = spy::util::GameLogicUtils::babysitterNumber(s, action);
                            double prob = 1 - (config.getBowlerBladeHitChance() * (std::pow(1 - config.getBabysitterSuccessChance(), numBabysitter)));
                            if (prob != 0 && prob != 1) {
                                push_back_toUnknownGadgets(gadget, targetChar->getCharacterId(), prob);
                            }
                        }
                    }
                }

                // after usage: bowler blade is on floor
                addGadgetToFloor(gadget);
                break;
            case spy::gadget::GadgetEnum::POISON_PILLS:
                // cocktail at target is poisoned
                if (targetChar != s.getCharacters().end()) { // character holds cocktail
                    poisonedCocktails.emplace_back(targetChar->getCharacterId());
                } else { // cocktail is on bar table
                    poisonedCocktails.emplace_back(action->getTarget());
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
                        poisonedCocktails.emplace_back(action->getCharacterId());
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

    std::optional<double> AIState::hasCharacterGadget(const spy::util::UUID &id, spy::gadget::GadgetEnum type) {
        auto gad = std::make_shared<spy::gadget::Gadget>(type);

        auto charGad = characterGadgets.find(gad);
        if (charGad != characterGadgets.end()) {
            return charGad->second == id ? 1 : 0;
        }

        auto floorGad = floorGadgets.find(gad);
        if (floorGad != floorGadgets.end()) {
            return 0;
        }

        auto unknownGad = unknownGadgets.find(gad);
        if (unknownGad != unknownGadgets.end() && !unknownGad->second.empty()) {
            auto b = std::find_if(unknownGad->second.begin(), unknownGad->second.end(), [&id](const std::pair<spy::util::UUID, double> &p) {
                return p.first == id;
            });
            if (b != unknownGad->second.end()) {
                return b->second;
            }
        }

        return std::nullopt;
    }

    bool AIState::hasCharacterProperty(const spy::util::UUID &id, spy::character::PropertyEnum prop) {
        auto p = std::find(properties[id].begin(), properties[id].end(), prop);
        return p != properties[id].end();
    }

    void
    AIState::push_back_toUnknownFaction(const spy::util::UUID &key, spy::character::FactionEnum val, double certainty) {
        auto keyInMap = unknownFaction.find(key);
        if (keyInMap != unknownFaction.end()) {
            // faction of character with id key is unknown
            auto valInMap = std::find_if(keyInMap->second.begin(), keyInMap->second.end(),
                                         [&val](const std::pair<spy::character::FactionEnum, double> &p) {
                                             return p.first == val;
                                         });
            if (valInMap != keyInMap->second.end()) {
                // certainty for value already in map
                valInMap->second = (valInMap->second + certainty) / 2;
            } else {
                unknownFaction[key].push_back(std::pair<spy::character::FactionEnum, double>(val, certainty));
            }
        }
    }

    void AIState::push_back_toUnknownGadgets(std::shared_ptr<spy::gadget::Gadget> key, const spy::util::UUID &val,
                                             double certainty) {
        auto keyInMap = unknownGadgets.find(key);
        if (keyInMap != unknownGadgets.end()) {
            // gadget location is unknown
            auto valInMap = std::find_if(keyInMap->second.begin(), keyInMap->second.end(),
                                         [&val](const std::pair<spy::util::UUID, double> &p) {
                                             return p.first == val;
                                         });
            if (valInMap != keyInMap->second.end()) {
                // certainty for value already in map
                valInMap->second = (valInMap->second + certainty) / 2;
            } else {
                unknownGadgets[key].push_back(std::pair<spy::util::UUID, double>(val, certainty));
            }
        }
    }

}
