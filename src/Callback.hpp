/**
 * @file   Callback.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the callback class (Interface to Clients).
 */

#ifndef SAMPLELIBRARY_CALLBACK_HPP
#define SAMPLELIBRARY_CALLBACK_HPP

namespace libclient::callback {
/**
 * Client has to implement class that has CallbackClass as parent
 */
    class Callback {
        public:

            Callback() = default;

            virtual ~Callback() = default;

            virtual void onHelloReply() = 0;

            virtual void onGameStarted() = 0;

            virtual void onRequestItemChoice() = 0;

            virtual void onRequestEquipmentChoice() = 0;

            virtual void onGameStatus() = 0;

            virtual void onRequestGameOperation() = 0;

            virtual void onStatistics() = 0;

            virtual void onGameLeft() = 0;

            virtual void onGamePause() = 0;

            virtual void onMetaInformation() = 0;

            virtual void onStrike() = 0;

            virtual void onError() = 0;

            virtual void onReplay() = 0;
    };
}

#endif //SAMPLELIBRARY_CALLBACK_HPP
