/**
 * @file   CharacterInformation.cpp
 * @author Carolin
 * @date   02.04.2020 (creation)
 * @brief  Definition of the character information class.
 */

#include "CharacterInformation.hpp"

namespace spy::character {

    CharacterInformation::CharacterInformation(const util::UUID &characterId, const CharacterDescription &character) :
        characterId(characterId), character(character) {}

    const util::UUID &CharacterInformation::getCharacterId() const {
        return characterId;
    }

    const CharacterDescription &CharacterInformation::getCharacter() const {
        return character;
    }

}   // namespace spy::character
