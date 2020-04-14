/**
 * @file   Callback.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the callback class (Interface to Clients).
 */

#ifndef SAMPLELIBRARY_CALLBACK_HPP
#define SAMPLELIBRARY_CALLBACK_HPP

#include <string>
#include <iostream>
#include "Model.hpp"
//#include "Network.hpp"

/**
 * Client has to implement class that has CallbackClass as parent
 */
class Callback {
public:

    Callback() = default;
    virtual ~Callback() = default;

    // TODO define onReceiveMessage as virtual methods

};


#endif //SAMPLELIBRARY_CALLBACK_HPP
