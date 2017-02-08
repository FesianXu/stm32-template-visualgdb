#include "cpp_interface.hpp"

void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}


/////////////////////////////////////////////////////////////////////////////////
#ifdef _EXIST_USART_PORT1_
USART usart1(1,115200) ;

extern "C" void USART1_IRQHandler(void)
{
	unsigned char tmp_data = 0 ;
	static bool isStopBefore = false ;
	if(usart1.getReceiveMode() == 1)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE) ; // 清楚中断标志位
		if(usart1.isBufCanBeRead() == false)
		{
			tmp_data = (unsigned char)(USART1->DR & (uint16_t)0x01ff) ; // get data
			usart1.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
			if(tmp_data == '\r' && isStopBefore == false)
				isStopBefore = true ;
			if(isStopBefore == true && tmp_data == '\n')
			{
				isStopBefore = false ;
				usart1.setBufReadStatus(true) ;
			}
		}
	} // \r\n 断句
	else if(usart1.getReceiveMode() == 2)
	{
		USART_ClearITPendingBit(USART1, USART_IT_RXNE) ; // 清楚中断标志位
		tmp_data = (unsigned char)(USART1->DR & (uint16_t)0x01ff) ; // get data
		usart1.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
	} // 连续接受
}

#endif

#ifdef _EXIST_USART_PORT2_
USART usart2(2,115200) ;

extern "C" void USART2_IRQHandler(void)
{
	unsigned char tmp_data = 0 ;
	static bool isStopBefore = false ;
	if(usart2.getReceiveMode() == 1)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE) ; // 清楚中断标志位
		if(usart2.isBufCanBeRead() == false)
		{
			tmp_data = (unsigned char)(USART2->DR & (uint16_t)0x01ff) ; // get data
			usart2.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
			if(tmp_data == '\r' && isStopBefore == false)
				isStopBefore = true ;
			if(isStopBefore == true && tmp_data == '\n')
			{
				isStopBefore = false ;
				usart2.setBufReadStatus(true) ;
			}
		}
	} // \r\n 断句
	else if(usart2.getReceiveMode() == 2)
	{
		USART_ClearITPendingBit(USART2, USART_IT_RXNE) ; // 清楚中断标志位
		tmp_data = (unsigned char)(USART2->DR & (uint16_t)0x01ff) ; // get data
		usart2.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
	} // 连续接受
}



#endif

#ifdef _EXIST_USART_PORT3_
USART usart3(3,115200) ;

extern "C" void USART3_IRQHandler(void)
{
	static unsigned char tmp_data = 0 ;
	static bool isStopBefore = false ;
	if(usart3.getReceiveMode() == 1)
	{
		if(usart3.isBufCanBeRead() == false)
		{
			USART_ClearITPendingBit(USART3, USART_IT_RXNE) ; // 清楚中断标志位
			tmp_data = (unsigned char)(USART3->DR & (uint16_t)0x01ff) ; // get data
			usart3.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
			if(tmp_data == '\r' && isStopBefore == false)
				isStopBefore = true ;
			if(isStopBefore == true && tmp_data == '\n')
			{
				isStopBefore = false ;
				usart3.setBufReadStatus(true) ;
			}
		}
	} // \r\n 断句
	else if(usart3.getReceiveMode() == 2)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE) ; // 清楚中断标志位
		tmp_data = (unsigned char)(USART3->DR & (uint16_t)0x01ff) ; // get data
		usart3.getBufPtr()->pushSingleDataIntoBuffer(tmp_data) ;
	} // 连续接受
}

#endif

USART::USART(const int &port, const int &baud)
{
	this->port = port ;
	this->baud = baud ;
	this->receiveMode = 1 ; // 默认是\r\n断句
	bufReadStatus = false ;
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
	if(bufReadStatus == true || receiveMode == 2)
	{
		char *tmp_str = buf->turn2String() ;
		print(tmp_str) ;
		if(tmp_str != NULL)
			delete tmp_str ;
		if(isClear == true)
			buf->freshBuf() ;
		bufReadStatus = false ;
	}
}

void USART::cleanBuf(void)
{
	buf->freshBuf() ;
	bufReadStatus = false ;
}

queue_buf *USART::getBufPtr(void)
{
	return buf ;
}

bool USART::isBufCanBeRead(void)
{
	return bufReadStatus ;
}

void USART::setBufReadStatus(bool status)
{
	bufReadStatus = status ;
}

int USART::getReceiveMode(void)
{
	return receiveMode ;
}

void USART::setReceiveMode(unsigned char mode)
{
	receiveMode = mode ;
}

/////////////////////////////////////////////////////////////////////////////////