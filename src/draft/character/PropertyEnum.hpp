/**
 * @file   PropertyEnum.hpp
 * @author Carolin
 * @date   02.04.2020 (creation)
 * @brief  Declaration of the character properties.
 */

#ifndef LIBCOMMON_PROPERTY_ENUM_HPP
#define LIBCOMMON_PROPERTY_ENUM_HPP


namespace spy::character {
    /**
     * Represents the different possible properties of characters.
     */
    enum class PropertyEnum {
        NIMBLENESS,
        SLUGGISHNESS,
        SPRYNESS,
        AGILITY,
        LUCKY_DEVIL,
        JINX,
        CLAMMY_CLOTHES,
        CONSTANT_CLAMMY_CLOTHES,
        ROBUST_STOMACH,
        TOUGHNESS,
        BABYSITTER,
        HONEY_TRAP,
        BANG_AND_BURN,
        FLAPS_AND_SEALS,
        TRADECRAFT,
        OBSERVATION
    };


    //TODO: Wofuer ist das gut ? -> never used + misleading, because there are also NPC agents
    enum class NPCEnum {
        CAT,
        JANITOR
    };
}


#endif //LIBCOMMON_PROPERTY_ENUM_HPP
