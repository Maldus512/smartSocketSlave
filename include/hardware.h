/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* TODO Define system operating frequency */
#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#define _XTAL_FREQ  32000000UL
#include <pic.h>
#include <xc.h>


#define RELAY_TRIS      TRISCbits.TRISC2
#define RELAY           LATCbits.LATC2



#endif