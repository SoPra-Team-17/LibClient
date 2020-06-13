//
// Created by Carolin on 10.06.2020.
//

#ifndef LIBCLIENT_ORDERUTILS_HPP
#define LIBCLIENT_ORDERUTILS_HPP

namespace libclient::util {
    struct cmpGadgetPtr {
        bool operator()(const std::shared_ptr<spy::gadget::Gadget> &a, const std::shared_ptr<spy::gadget::Gadget>& b) const {
            return a->getType() < b->getType();
        }
    };
}

#endif //LIBCLIENT_ORDERUTILS_HPP
