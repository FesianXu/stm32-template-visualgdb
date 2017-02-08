#include "usart.h"
 

/*******************************************************************
下面是串口打印函数printfx()
*******************************************************************/
void printf1(char *fmt, ...) 
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
 
void printf2(char *fmt, ...)
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
 
void printf3(char *fmt, ...)
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

