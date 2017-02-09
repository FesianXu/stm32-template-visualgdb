#include "cpp_interface.hpp"
/////////////////////////////////////////////////////////////////////////////////
#ifdef _EXIST_USART_PORT1_
USART usart1(1,115200) ;

extern "C" void USART1_IRQHandler(void)
{
	unsigned char tmp_data = 0 ;
	static bool isStopBefore = false ;
	if(usart1.receiveMode == 1)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE) ; // ����жϱ�־λ
		if(usart1.isBufCanBeRead == false)
		{
			tmp_data = (unsigned char)(USART1->DR & (uint16_t)0x01ff) ; // get data
			usart1.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
			if(tmp_data == '\r' && isStopBefore == false)
				isStopBefore = true ;
			if(isStopBefore == true && tmp_data == '\n')
			{
				isStopBefore = false ;
				usart1.isBufCanBeRead = true ;
			}
		}
	} // \r\n �Ͼ�
	else if(usart1.receiveMode == 2)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE) ; // ����жϱ�־λ
		tmp_data = (unsigned char)(USART1->DR & (uint16_t)0x01ff) ; // get data
		usart1.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
	} // ��������
}

#endif

#ifdef _EXIST_USART_PORT2_
USART usart2(2,115200) ;

extern "C" void USART2_IRQHandler(void)
{
	unsigned char tmp_data = 0 ;
	static bool isStopBefore = false ;
	if(usart2.receiveMode == 1)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE) ; // ����жϱ�־λ
		if(usart2.isBufCanBeRead == false)
		{
			tmp_data = (unsigned char)(USART2->DR & (uint16_t)0x01ff) ; // get data
			usart2.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
			if(tmp_data == '\r' && isStopBefore == false)
				isStopBefore = true ;
			if(isStopBefore == true && tmp_data == '\n')
			{
				isStopBefore = false ;
				usart2.isBufCanBeRead = true ;
			}
		}
	} // \r\n �Ͼ�
	else if(usart2.receiveMode == 2)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE) ; // ����жϱ�־λ
		tmp_data = (unsigned char)(USART2->DR & (uint16_t)0x01ff) ; // get data
		usart2.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
	} // ��������
}



#endif

#ifdef _EXIST_USART_PORT3_
USART usart3(3,115200) ;

extern "C" void USART3_IRQHandler(void)
{
	static unsigned char tmp_data = 0 ;
	static bool isStopBefore = false ;
	if(usart3.receiveMode == 1)
	{
		if(usart3.isBufCanBeRead() == false)
		{
			USART_ClearITPendingBit(USART3, USART_IT_RXNE) ; // ����жϱ�־λ
			tmp_data = (unsigned char)(USART3->DR & (uint16_t)0x01ff) ; // get data
			usart3.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
			if(tmp_data == '\r' && isStopBefore == false)
				isStopBefore = true ;
			if(isStopBefore == true && tmp_data == '\n')
			{
				isStopBefore = false ;
				usart3.isBufCanBeRead = true ;
			}
		}
	} // \r\n �Ͼ�
	else if(usart3.receiveMode == 2)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE) ; // ����жϱ�־λ
		tmp_data = (unsigned char)(USART3->DR & (uint16_t)0x01ff) ; // get data
		usart3.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
	} // ��������
}

#endif

/////////////////////////////////////////////////////////////////////////////////

/*******************************************************************
�����Ǵ��ڳ�ʼ������USARTx_Init(u32 bound)
*******************************************************************/
static void USART1_Init(u32 bound){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
	USART_DeInit(USART1);  //��λ����1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA9

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA10

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART1, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ��� 

}

static void USART2_Init(u32 bound){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

}

static void USART3_Init(u32 bound){
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ��PB10

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);  //��ʼ��PB11

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(USART3, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�����ж�
	USART_Cmd(USART3, ENABLE);                    //ʹ�ܴ��� 

}

/*******************************************************************
�����Ǵ��ڴ�ӡ����printfx()
*******************************************************************/
static void printf1(char *fmt, ...) 
{ 
	char buffer[BUF_USART1+1] = {0} ;
	int i = 0; 
	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer,BUF_USART1,fmt,arg_ptr); 
	USART_ClearFlag(USART1,USART_FLAG_TC);
	while (buffer[i] !=0 && (i <BUF_USART1)) 
	{
		USART1->DR = (buffer[i++] & (uint16_t)0x01FF); // send data to usart port 1
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 

static void printf2(char *fmt, ...)
{ 
	char buffer[BUF_USART2+1] = {0} ;
	int i = 0; 
	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer,BUF_USART2,fmt,arg_ptr); 
	USART_ClearFlag(USART2,USART_FLAG_TC);
	while (buffer[i] !=0 && (i <BUF_USART2)) 
	{
		USART2->DR = (buffer[i++] & (uint16_t)0x01FF); // send data to usart port 2
		while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 

static void printf3(char *fmt, ...)
{ 
	char buffer[BUF_USART3+1] = {0} ;
	int i = 0; 
	va_list arg_ptr; 
	va_start(arg_ptr, fmt);   
	vsnprintf(buffer,BUF_USART3,fmt,arg_ptr); 
	USART_ClearFlag(USART3,USART_FLAG_TC);
	while (buffer[i] !=0 && (i <BUF_USART3)) 
	{
		USART3->DR = (buffer[i++] & (uint16_t)0x01FF); // send data to usart port 3
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);  
	} 
	va_end(arg_ptr); 
} 

/////////////////////////////////////////////////////////////////////////////////

USART::USART(const int &port, const int &baud)
{
	this->port = port ;
	this->baud = baud ;
	this->receiveMode = 1 ; // Ĭ����\r\n�Ͼ�
	isBufCanBeRead = false ;
	switch(port)
	{
	case 1: USART1_Init(baud) ; break ;
	case 2: USART2_Init(baud) ; break ;
	case 3: USART3_Init(baud) ; break ;
	default:
		break ;
	} // initiate the usart port from port1 to port3 
	buf = new queue_buf(10) ;
}

USART::~USART(void)
{
	delete buf ;
	buf = NULL ;
}

void USART::print(char *fmt, ...)
{
	va_list ap ;
	va_start(ap, fmt) ;
	char tmp[2000] = {0} ;
	vsnprintf(tmp, 2000-1, fmt, ap) ;
	switch(port)
	{
	case 1: printf1(tmp) ; break ;
	case 2: printf2(tmp) ; break ;
	case 3: printf3(tmp) ; break ;
	default:
		break ;
	}
	va_end(ap) ;
}

void USART::printBuf(bool isClear)
{
	if(buf == NULL)
		return ;
	if(isBufCanBeRead == true || receiveMode == 2)
	{
		char *tmp_str = buf->turn2String() ;
		print(tmp_str) ;
		if(tmp_str != NULL)
			delete tmp_str ;
		if(isClear == true)
			buf->freshBuf() ;
		isBufCanBeRead = false ;
	}
}

void USART::cleanBuf(void)
{
	buf->freshBuf() ;
	isBufCanBeRead = false ;
}

queue_buf *USART::getBufPtr(void)
{
	return buf ;
}



/////////////////////////////////////////////////////////////////////////////////