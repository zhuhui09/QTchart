#include "ADC.h"
static void ADC_GPIO_config(void)
{
	
	
GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	//A1ģ������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

}

static void ADCx_Mode_Config(void)
{
	
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1EN,ENABLE);//��ADC1ʱ��
	ADC_InitStructure.ADC_Mode= ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None ;
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel=1;
	ADC_Init(ADC1,&ADC_InitStructure);//����ǵ����ADC.C�ļ�
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);//8��Ƶ9��
	ADC_RegularChannelConfig(ADC1,ADC_Channel_1,1,ADC_SampleTime_55Cycles5);
	 ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);//ʹ��ADC
	ADC_StartCalibration(ADC1);//��ʼУ׼
	
	while(ADC_GetCalibrationStatus(ADC1));//�ȴ�У׼���
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	
	
}
static void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_InitStructure.NVIC_IRQChannel =ADC1_2_IRQn;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
}
void ADC1_Init(void)
{ 
	//u16  ADC_ConvertedValue;
  ADC_NVIC_Config();
	ADC_GPIO_config();
	ADCx_Mode_Config();
	
	

}


