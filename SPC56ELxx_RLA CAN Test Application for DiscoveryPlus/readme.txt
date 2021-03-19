This Test Application use the CAN loopback to send and receive CAN messages

Every 250ms 3 message are sent
 -  Extended ID 0x89012345 
 -  Extended ID 0x70
 -  Standard ID 0x11
 
The first message is read using can_lld_receive on mailbox 1 (LED D7 toggles
each time a message is received).
The last two message are received by the FIFO and managed in the callback
function (LED D5 and LED D6 toggles each time the correct ID is received).

An error callback is also defined. Interrupt sources managed are: Bus Off, Error, Tx Warning and Rx Warning. 