/**
 * @file   Callback.hpp
 * @author Carolin
 * @date   14.04.2020 (creation)
 * @brief  Declaration of the callback class (Interface to Clients).
 */

#ifndef LIBCLIENT_CALLBACK_HPP
#define LIBCLIENT_CALLBACK_HPP

namespace libclient {

    /**
     * Client has to implement class that has Callback as parent.
     * This class has to implement all pure virtual methods declared in Callback.
     */
    class Callback {
        public:
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

            virtual void connectionLost() = 0;
    };
}

#endif //LIBCLIENT_CALLBACK_HPP
