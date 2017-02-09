#include "basic_system/cpp_interface/cpp_interface.hpp"
extern "C"
{
#include "basic_system/delay/delay.h"
}
extern USART usart1 ;
int main()
{
	char *tmp = NULL ;
	delay_init() ;	
	while(1)
	{
		usart1.print("get\r\n");
		delay_ms(10) ;
	}
	//usart1.receiveMode = 1 ;
	//usart1.print("ready\r\n") ;
	//while(1){
	//	delay_ms(10) ;
	//	if(usart1.isBufCanBeRead == true){
	//		tmp = usart1.getBufPtr()->turn2String() ;
	//		usart1.print(tmp) ;
	//		delete tmp ;
	//		usart1.isBufCanBeRead = false ;
	//		usart1.cleanBuf();
	//	}
	//}

}
