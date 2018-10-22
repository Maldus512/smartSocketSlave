#ifndef __STATE_H__
#define __STATE_H__

struct StatoMacchina {
    char f_relayOn;
    char f_transmitSensorReadings;
};



extern struct StatoMacchina stato;


#endif