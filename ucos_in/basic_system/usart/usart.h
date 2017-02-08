#ifndef _USART_H_
#define _USART_H_

#include "../sys/sys.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include <stdio.h>
#include "stdarg.h"


#define BUF_USART1 2000
#define BUF_USART2 2000
#define BUF_USART3 2000

#define Rx1Length 2000
#define Rx2Length 100
#define Rx3Length 100

void printf1(char *fmt, ...);
void printf2(char *fmt, ...);
void printf3(char *fmt, ...);


#endif

