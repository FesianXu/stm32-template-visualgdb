#include "basic_system/cpp_interface/cpp_interface.hpp"
extern "C"
{
#include "basic_system/delay/delay.h"
}
extern USART usart1 ;
int main()
{
	char *tmp = NULL ;
	delay_init(72) ;	
	usart1.setReceiveMode(1) ;
	printf1("ready\r\n") ;
	while(1){
		delay_ms(10) ;
		if(usart1.isBufCanBeRead() == true){
			tmp = usart1.getBufPtr()->turn2String() ;
			printf1(tmp) ;
			delete tmp ;
			usart1.setBufReadStatus(false) ;
			usart1.cleanBuf();
		}
	}

}
