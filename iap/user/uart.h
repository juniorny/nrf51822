#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include "simple_uart.h"
#include "nrf.h"

#define USART_REC_LEN  			10 * 1024 				//定义最大接收字节数 10K
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 	
extern uint16_t USART_RX_CNT;									//接收的字节数

extern void uart_int_config(void);

#endif
