#ifndef __ADC_H__
#define __ADC_H__
#include <stdbool.h>
#include <stdint.h>

/* TODO User level functions prototypes (i.e. InitApp) go here */

void initADC(void);         /* I/O and Peripheral Initialization */

uint16_t readADC();
double currentRead(uint16_t calibration);

#endif