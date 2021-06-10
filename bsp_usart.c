#include "bsp_usart.h"
#include "delay.h"

void USART_Config(void)
	{
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                                   //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                          	  //复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA.10
   
  //USART1_RX	  GPIOA.10初3始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;                                  //PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                       //浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);                                      //初始化GPIOA.10

   //USART 初始化设置

	USART_InitStructure.USART_BaudRate =9600;                                  //串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;                //字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;                     //一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;                        //无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;            //收发模式
  USART_Init(USART1, &USART_InitStructure);                                  //初始化串口
	USART_Cmd(USART1, ENABLE);                                                 //使能串口1 
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                             //开启串口接受中断

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;                   //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		                     //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			                       //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                                           //根据指定的参数初始化VIC寄存器
    
		
}
	


	void Usart_SendBB(USART_TypeDef* pUSARTx, uint8_t Data)
	{
	  USART_SendData(pUSARTx,  Data);
		while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==1);


	}

//发送两个字节的数据
	void Usart_sendhalfword(USART_TypeDef* pUSARTx, u16 Data)
	{
//ADC值为16位，拆分为高八位第八位进行发送
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
//发送8位数据的数组
	void Usart_Sendarray(USART_TypeDef* pUSARTx, uint8_t *arr,uint8_t num)
	{
	uint8_t i;
		for(i=0;i<num;i++)
		{
		Usart_SendBB(pUSARTx, arr[i]);
		}
	while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC==RESET));
		

	}
//发送字符串
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
	
	
	//重定向c库函数printf到串口，重定向之后就可以进行使用printf函数
	int fputc(int ch,FILE*f)
	{
	  USART_SendData(USART1,(uint8_t)ch);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
		return (ch);
		
		
	
	
	}

	uint8_t Check_Sum( u8* buf ,u16 len){  //使用最简单的和校验
		u16 i=0;
		u8 sum=0;
		for(i=0;i<len;i++){
		
		sum+=buf[i];
		
		}
		
		return sum;
		
		
	}
