//
// Created by jonas on 02.04.20.
//

#include "WiretapWithEarplugs.hpp"

namespace spy::gadget {

    bool WiretapWithEarplugs::isWorking() const {
        return working;
    }

    void WiretapWithEarplugs::setWorking(bool isWorking) {
        WiretapWithEarplugs::working = isWorking;
    }

    auto WiretapWithEarplugs::getActiveOn() const -> const std::optional<spy::util::UUID> & {
        return activeOn;
    }

    void WiretapWithEarplugs::setActiveOn(const std::optional<spy::util::UUID> &active) {
        WiretapWithEarplugs::activeOn = active;
    }

}  // namespace spy::gadget
