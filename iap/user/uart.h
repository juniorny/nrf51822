#ifndef _UART_H_
#define _UART_H_

#include <stdint.h>
#include "simple_uart.h"
#include "nrf.h"

#define USART_REC_LEN  			10 * 1024 				//�����������ֽ��� 10K
extern uint8_t  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 	
extern uint16_t USART_RX_CNT;									//���յ��ֽ���

extern void uart_int_config(void);

#endif
