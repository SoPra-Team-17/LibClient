//
// Created by jonas on 03.04.20.
//

#ifndef LIBCOMMON_GADGETENUM_HPP
#define LIBCOMMON_GADGETENUM_HPP

#include <nlohmann/json.hpp>

namespace spy::gadget {
    enum class GadgetEnum {
        INVALID,
        HAIRDRYER,
        MOLEDIE,
        TECHNICOLOUR_PRISM,
        BOWLER_BLADE,
        MAGNETIC_WATCH,
        POISON_PILLS,
        LASER_COMPACT,
        ROCKET_PEN,
        GAS_GLOSS,
        MOTHBALL_POUCH,
        FOG_TIN,
        GRAPPLE,
        WIRETAP_WITH_EARPLUGS,
        DIAMOND_COLLAR,
        JETPACK,
        CHICKEN_FEED,
        NUGGET,
        MIRROR_OF_WILDERNESS,
        POCKET_LITTER,
        COCKTAIL
    };

    NLOHMANN_JSON_SERIALIZE_ENUM(GadgetEnum, {
        { GadgetEnum::INVALID, nullptr },
        { GadgetEnum::HAIRDRYER, "HAIRDRYER" },
        { GadgetEnum::MOLEDIE, "MOLEDIE" },
        { GadgetEnum::TECHNICOLOUR_PRISM, "TECHNICOLOUR_PRISM" },
        { GadgetEnum::BOWLER_BLADE, "BOWLER_BLADE" },
        { GadgetEnum::MAGNETIC_WATCH, "MAGNETIC_WATCH" },
        { GadgetEnum::POISON_PILLS, "POISON_PILLS" },
        { GadgetEnum::LASER_COMPACT, "LASER_COMPACT" },
        { GadgetEnum::ROCKET_PEN, "ROCKET_PEN" },
        { GadgetEnum::GAS_GLOSS, "GAS_GLOSS" },
        { GadgetEnum::MOTHBALL_POUCH, "MOTHBALL_POUCH" },
        { GadgetEnum::FOG_TIN, "FOG_TIN" },
        { GadgetEnum::GRAPPLE, "GRAPPLE" },
        { GadgetEnum::WIRETAP_WITH_EARPLUGS, "WIRETAP_WITH_EARPLUGS" },
        { GadgetEnum::DIAMOND_COLLAR, "DIAMOND_COLLAR" },
        { GadgetEnum::JETPACK, "JETPACK" },
        { GadgetEnum::CHICKEN_FEED, "CHICKEN_FEED" },
        { GadgetEnum::NUGGET, "NUGGET" },
        { GadgetEnum::MIRROR_OF_WILDERNESS, "MIRROR_OF_WILDERNESS" },
        { GadgetEnum::POCKET_LITTER, "POCKET_LITTER" },
        { GadgetEnum::COCKTAIL, "COCKTAIL" },
    })
}

#endif //LIBCOMMON_GADGETENUM_HPP
