# LibClient
Library für Netzwerk und Model der Clients

## "State Machine" des Network
NOT_CONNECT
* Start-Zustand oder Verbindung wurde getrennt und reconnect ist nicht möglich  
nicht verbunden, d.h `webSocketClient` existiert nicht
* in diesem state darf nur connect() aufgerufen werden  
`name` und `role` des Client dürfen verändert werden
* REQUEST_META_INFORMATION Nachrichten dürfen NICHT gesendet werden

CONNECTED
* verbunden, d.h `webSocketClient` existiert
* in diesem state darf nur die HELLO Nachricht gesendet werden  
`name` und `role` des Client dürfen verändert werden
* REQUEST_META_INFORMATION Nachrichten dürfen NICHT gesendet werden

SENT_HELLO
* HELLO Nachricht wurde über sendHello() verschickt
* REQUEST_META_INFORMATION Nachrichten dürfen NICHT gesendet werden

WELCOMED
* HELLO_REPLY Nachricht wurde empfangen

IN_ITEMCHOICE
* REQUEST_ITEM_CHOICE Nachricht wurde empfangen
* in diesem state dürfen ITEM_CHOICE Nachrichten gesendet werden

IN_EQUIPMENTCHOICE
* REQUEST_EQUIPMENT_CHOICE Nachricht wurde empfangen
* in diesem state dürfen EQUIPMENT_CHOICE Nachrichten gesendet werden
                
IN_GAME
* GAME_STARTED Nachricht wurde empfangen
GAME_STATUS Nachricht mit gameOver==false wurde empfangen
* in diesem state dürfen REQUEST_GAME_PAUSE Nachrichten gesendet werden

IN_GAME_ACTIVE
* REQUEST_GAME_OPERATION Nachricht wurde empfangen  
* in diesem state dürfen GAME_OPERATION und REQUEST_GAME_PAUSE Nachrichten gesendet werden

GAME_OVER
* STATISTICS Nachricht wurde empfangen  
GAME_STATUS Nachricht mit gameOver==true wurde empfangen  
GAME_LEFT Nachricht wurde empfangen  
REPLAY Nachricht wurde empfangen
* in diesem state dürfen REQUEST_REPLAY Nachrichten gesendet werden

PAUSE
* GAME_PAUSE Nachricht mit gamePause==true wurde empfangen
* in diesem state dürfen REQUEST_GAME_PAUSE Nachrichten gesendet werden (aufheben der Pause)

RECONNECT
* Verbindung wurde getrennt und reconnect ist möglich
* in diesem state dürfen RECONNECT Nachrichten gesendet werden

stateBeforePause
* wird zum state wenn GAME_PAUSE Nachricht mit gamePause==false empfangen wurde

## For Developers
LibClient is implemented in C++. You can use e.g. CLION as development environment.  
The following libraries where used. The respective licences  can be found in this repositories Licence file:
* LibCommon: version see submodule
* WebsocketCpp: version see submodule 