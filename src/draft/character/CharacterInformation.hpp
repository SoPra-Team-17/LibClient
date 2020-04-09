/**
 * @file   CharacterInformation.hpp
 * @author Carolin
 * @date   02.04.2020 (creation)
 * @brief  Declaration of the character information class.
 */

#ifndef LIBCOMMON_CHARACTER_INFORMATION_HPP
#define LIBCOMMON_CHARACTER_INFORMATION_HPP

#include <util/UUID.hpp>
#include <string>
#include <list>
#include <datatypes/character/Character.hpp>
#include <datatypes/character/CharacterDescription.hpp>

namespace spy::character {
    /**
     * @brief Representation of the information of characters being used in the network protocol.
     */
    class CharacterInformation {
        public:
            CharacterInformation(const util::UUID &characterId, const CharacterDescription &character);

            [[nodiscard]] const util::UUID &getCharacterId() const;

            [[nodiscard]] const CharacterDescription &getCharacter() const;


        private:
            const spy::util::UUID characterId;
            const CharacterDescription character;
    };

}

#endif //LIBCOMMON_CHARACTER_INFORMATION_HPP
