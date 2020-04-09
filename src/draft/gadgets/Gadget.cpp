//
// Created by jonas on 02.04.20.
//

#include "Gadget.hpp"

namespace spy::gadget {

    GadgetEnum Gadget::getType() const {
        return type;
    }

    int Gadget::getUsagesLeft() const {
        return usagesLeft;
    }

    void Gadget::setUsagesLeft(int newUsages) {
        Gadget::usagesLeft = newUsages;
    }

    void to_json(nlohmann::json &j, const spy::gadget::Gadget &g) {
        j["GadgetEnum"] = g.type;
        j["usages"] = g.usagesLeft;
    }

    void from_json(const nlohmann::json &j, spy::gadget::Gadget &g) {
        j.at("GadgetEnum").get_to(g.type);
        j.at("usages").get_to(g.usagesLeft);
    }
}  // namespace spy::gadget
