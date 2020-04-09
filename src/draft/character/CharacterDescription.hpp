/**
 * @file   CharacterDescription.hpp
 * @author Carolin
 * @date   02.04.2020 (creation)
 * @brief  Declaration of the character description class.
 */

#ifndef LIBCOMMON_CHARACTER_DESCRIPTION_HPP
#define LIBCOMMON_CHARACTER_DESCRIPTION_HPP

#include <string>
#include <list>
#include <datatypes/character/Character.hpp>

namespace spy::character {
    enum class GenderEnum {
        MALE,
        FEMALE,
        DIVERSE
    };

    /**
     * @brief Representation of the character information stored in the character configuration file.
     */
    class CharacterDescription {
        public:

            CharacterDescription(const std::string &name,
                                 const std::string &description,
                                 GenderEnum gender,
                                 const std::list<PropertyEnum> &features);

            [[nodiscard]] const std::string &getName() const;

            [[nodiscard]] const std::string &getDescription() const;

            [[nodiscard]] GenderEnum getGender() const;

            [[nodiscard]] const std::list<PropertyEnum> &getFeatures() const;

        private:
            const std::string name;
            const std::string description;
            GenderEnum gender;
            const std::list<spy::character::PropertyEnum> features;
    };
}

#endif //LIBCOMMON_CHARACTER_DESCRIPTION_HPP
