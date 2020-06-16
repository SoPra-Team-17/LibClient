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
     * method naming pattern: onMessage() with Message being the type of Message received
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

            /**
             * called when client looses connection to server
             */
            virtual void connectionLost() = 0;

            /**
             * called when client receives message of unknown type or that was not meant for its player or session id
             * message params are not stored (besides debugMessage string) --> client only should log this for server debugging
             */
            virtual void wrongDestination() = 0;
    };
}

#endif //LIBCLIENT_CALLBACK_HPP
