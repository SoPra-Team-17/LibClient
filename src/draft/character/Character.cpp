/**
 * @file   Character.cpp
 * @author Carolin
 * @date   02.04.2020 (creation)
 * @brief  Definition of the character class.
 */

#include "Character.hpp"

namespace spy::character {

    Character::Character(const util::UUID &characterId, const std::string &name) : characterId(characterId),
                                                                                   name(name) {}

    const util::UUID &Character::getCharacterId() const {
        return characterId;
    }

    const std::string &Character::getName() const {
        return name;
    }

    const util::Point &Character::getCoordinates() const {
        return coordinates;
    }

    void Character::setCoordinates(const util::Point &coordinate) {
        Character::coordinates = coordinate;
    }

    unsigned int Character::getMovePoints() const {
        return movePoints;
    }

    void Character::setMovePoints(unsigned int mp) {
        Character::movePoints = mp;
    }

    unsigned int Character::getActionPoints() const {
        return actionPoints;
    }

    void Character::setActionPoints(unsigned int ap) {
        Character::actionPoints = ap;
    }

    unsigned int Character::getHealthPoints() const {
        return healthPoints;
    }

    void Character::setHealthPoints(unsigned int hp) {
        Character::healthPoints = hp;
    }

    unsigned int Character::getIntelligencePoints() const {
        return intelligencePoints;
    }

    void Character::setIntelligencePoints(unsigned int ip) {
        Character::intelligencePoints = ip;
    }

    unsigned int Character::getChips() const {
        return chips;
    }

    void Character::setChips(unsigned int chip) {
        Character::chips = chip;
    }

    const std::list<PropertyEnum> &Character::getProperties() const {
        return properties;
    }

    void Character::setProperties(const std::list<PropertyEnum> &propertyList) {
        Character::properties = propertyList;
    }

    const std::list<gadget::Gadget> &Character::getGadgets() const {
        return gadgets;
    }

    void Character::setGadgets(const std::list<gadget::Gadget> &gadgetList) {
        Character::gadgets = gadgetList;
    }

}  // namespace spy::character
