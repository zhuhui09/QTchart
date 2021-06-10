#ifndef _ADC_H_
#define _ADC_H_
#include "stm32f10x.h"
 extern u16 ADC_ConvertedValue;
void ADC1_Init(void);
#define ADC_IRQHandler ADC1_2_IRQHandler
#endif

