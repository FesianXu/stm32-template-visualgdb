#ifndef _CPP_INTERFACES_HPP_
#define _CPP_INTERFACES_HPP_

extern "C"
{
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include <stdarg.h>
#include <stdio.h>
#include "../sys/sys.h"
}
#include "../../software/cpp_buf/cpp_buf.hpp"

#define BUF_USART1 2000
#define BUF_USART2 2000
#define BUF_USART3 2000

#define _EXIST_USART_PORT1_ // new usart1

static void USART1_Init(u32 bound) ;
static void USART2_Init(u32 bound) ;
static void USART3_Init(u32 bound) ;
static void printf1(char *fmt, ...);
static void printf2(char *fmt, ...);
static void printf3(char *fmt, ...);

class USART
{
private:
	queue_buf *buf ;
	unsigned char port ;
	unsigned int baud ;
	explicit USART(const USART &cpy) ; // ������������
public:
	USART(const int &port, const int &baud) ;
	~USART(void) ;

	void print(char *fmt, ...);
	void printBuf(bool isClear = false) ;
	void cleanBuf(void) ; // ��������

	queue_buf *getBufPtr(void) ;

public:
	bool isBufCanBeRead ;
	unsigned char receiveMode ; // 1Ϊ\r\n�Ͼ䣬 2Ϊ��������
};
// #VisualGDB: AutoSourceFiles		#<--- remove this line to disable auto-updating of SOURCEFILES and EXTERNAL_LIBS

#endif
