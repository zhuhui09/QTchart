#include "bsp_usart.h"
#include "delay.h"

void USART_Config(void)
	{
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                          	  //�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.10
   
  //USART1_RX	  GPIOA.10��3ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  //PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       //��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //��ʼ��GPIOA.10

   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate =9600;                                  //���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                //�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                     //һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;                        //����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;            //�շ�ģʽ
  USART_Init(USART1, &USART_InitStructure);                                  //��ʼ������
	USART_Cmd(USART1, ENABLE);                                                 //ʹ�ܴ���1 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                             //�������ڽ����ж�

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;                   //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		                     //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                       //IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                                           //����ָ���Ĳ�����ʼ��VIC�Ĵ���
    
		
}
	


	void Usart_SendBB(USART_TypeDef* pUSARTx, uint8_t Data)
	{
	  USART_SendData(pUSARTx,  Data);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==1);


	}

//���������ֽڵ�����
	void Usart_sendhalfword(USART_TypeDef* pUSARTx, u16 Data)
	{
//ADCֵΪ16λ�����Ϊ�߰�λ�ڰ�λ���з���
		uint8_t head1,head2,len,temp_h,temp_l,end,sum;
		u8 buf[5];
		head1=0xAA;
		len=0x02;
		temp_h=(Data&0xff00)>>8;
		temp_l=(Data&0xff);
		end=0xAB;
		buf[0]=head1;
		buf[1]=len;
		buf[2]=temp_h;
		buf[3]=temp_l;
		buf[4]=end;
		sum=Check_Sum(buf,5);
	
		USART_SendData(pUSARTx, head1);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);                
		USART_SendData(pUSARTx, len);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
		USART_SendData(pUSARTx,  temp_h);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
		USART_SendData(pUSARTx,  temp_l);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
		USART_SendData(pUSARTx,  end);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);
		USART_SendData(pUSARTx,  sum);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TXE)==RESET);


	}
//����8λ���ݵ�����
	void Usart_Sendarray(USART_TypeDef* pUSARTx, uint8_t *arr,uint8_t num)
	{
	uint8_t i;
		for(i=0;i<num;i++)
		{
		Usart_SendBB(pUSARTx, arr[i]);
		}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC==RESET));
		

	}
//�����ַ���
	void Usart_Sendstr(USART_TypeDef* pUSARTx, uint8_t *str)
		
	{
		
		uint8_t i=0;
		
	do
	{
		 Usart_SendBB( pUSARTx, *(str+i));
		i++;
		
	}
		while(*(str+i)!='\0');

   while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC==RESET));

	}
	
	
	//�ض���c�⺯��printf�����ڣ��ض���֮��Ϳ��Խ���ʹ��printf����
	int fputc(int ch,FILE*f)
	{
	  USART_SendData(USART1,(uint8_t)ch);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		return (ch);
		
		
	
	
	}

	uint8_t Check_Sum( u8* buf ,u16 len){  //ʹ����򵥵ĺ�У��
		u16 i=0;
		u8 sum=0;
		for(i=0;i<len;i++){
		
		sum+=buf[i];
		
		}
		
		return sum;
		
		
	}
