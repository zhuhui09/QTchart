#include  "systick.h"
#include "bsp_usart.h"
#include "ADC.h"
#include "led.h"
void TIM3_Int_Init(u16 arr,u16 psc){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	
	TIM_TimeBaseInitStruct.TIM_Period=arr;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	TIM_ITConfig(TIM3, TIM_IT_Update ,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  TIM_Cmd(TIM3,ENABLE);
	

	
}
void TIM3_IRQHandler(void){
	
	
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!=RESET){
		
		Usart_sendhalfword(USART1,ADC_ConvertedValue);	
		
		if(LED1==1)
			LED1=0;
		else
			LED1=1;
		}

		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
	
		
	
}


