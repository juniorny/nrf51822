#include "uart.h"

uint8_t USART_RX_BUF[USART_REC_LEN]; //__attribute__ ((at(0x20000000)));//接收缓冲,最大USART_REC_LEN个字节,起始地址为0x20000000

uint16_t USART_RX_CNT = 0;			//接收的字节数	

void uart_int_config(void)
{
		NRF_UART0->INTENSET = (1 << 2);		// 使能 RXDRDY event
		NRF_UART0->EVENTS_RXDRDY = 0;
		NVIC_SetPriority(UART0_IRQn, 1);	
		NVIC_ClearPendingIRQ(UART0_IRQn);
		NVIC_EnableIRQ(UART0_IRQn);
}


void UART0_IRQHandler(void)
{
	uint8_t Res;
	if (NRF_UART0->EVENTS_RXDRDY != 0)
  {
		Res = simple_uart_get();
		if (USART_RX_CNT < USART_REC_LEN)
		{
			USART_RX_BUF[USART_RX_CNT] = Res;
			USART_RX_CNT++;			 									     
		} 
  }	
}
