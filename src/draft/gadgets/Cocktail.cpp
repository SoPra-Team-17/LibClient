//
// Created by jonas on 02.04.20.
//

#include "Cocktail.hpp"

namespace spy::gadget {
    bool Cocktail::isPoisoned() const {
        return poisoned;
    }

    void Cocktail::setIsPoisoned(bool isPoisoned) {
        poisoned = isPoisoned;
    }
}
