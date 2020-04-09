//
// Created by jonas on 02.04.20.
//

#ifndef LIBCOMMON_WIRETAPWITHEARPLUGS_HPP
#define LIBCOMMON_WIRETAPWITHEARPLUGS_HPP

#include <datatypes/gadgets/Gadget.hpp>
#include <util/UUID.hpp>
#include <optional>

namespace spy::gadget {
    class WiretapWithEarplugs : public Gadget {
    public:
        WiretapWithEarplugs() : Gadget{GadgetEnum::WIRETAP_WITH_EARPLUGS}, working{true}, activeOn{std::nullopt} {};

        [[nodiscard]] bool isWorking() const;

        void setWorking(bool isWorking);

        [[nodiscard]] auto getActiveOn() const -> const std::optional<spy::util::UUID> &;

        void setActiveOn(const std::optional<spy::util::UUID> &active);

    private:
        bool working;
        std::optional<spy::util::UUID> activeOn;
    };
}

#endif //LIBCOMMON_WIRETAPWITHEARPLUGS_HPP
