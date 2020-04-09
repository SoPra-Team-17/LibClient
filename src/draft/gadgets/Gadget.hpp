//
// Created by jonas on 02.04.20.
//

#ifndef LIBCOMMON_GADGET_HPP
#define LIBCOMMON_GADGET_HPP

#include <nlohmann/json.hpp>
#include <datatypes/gadgets/GadgetEnum.hpp>

namespace spy::gadget {

    class Gadget {
    public:
        Gadget() : type{GadgetEnum::INVALID}, usagesLeft{0} {};

        explicit Gadget(GadgetEnum type) : type{type}, usagesLeft{0} {};

        [[nodiscard]] GadgetEnum getType() const;

        [[nodiscard]] int getUsagesLeft() const;

        void setUsagesLeft(int newUsages);

        friend void to_json(nlohmann::json &j, const Gadget &g);

        friend void from_json(const nlohmann::json &j, Gadget &g);

    private:
        GadgetEnum type;
        int usagesLeft;
    };
}

#endif //LIBCOMMON_GADGET_HPP
