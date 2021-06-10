#include "led.h"
#include "delay.h"
#include "sys.h"
#include "bsp_usart.h"
#include "systick.h"
#include "ADC.h"
#include<stdio.h>
 int main(void)
 {
	USART_Config();
	ADC1_Init();
	 LED_Init();
	TIM3_Int_Init(79,7199);//8MS
  while(1);

 }
