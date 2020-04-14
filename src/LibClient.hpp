//
// Created by marco on 14.04.20.
//

#ifndef LIBCLIENT_LIBCLIENT_HPP
#define LIBCLIENT_LIBCLIENT_HPP

#include <model/Model.hpp>
#include <Network.hpp>

namespace libclient {
    class LibClient {
        public:
            LibClient() = default;


        private:
            libclient::model::Model model;

    };

}


#endif //LIBCLIENT_LIBCLIENT_HPP
