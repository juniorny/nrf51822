
#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "simple_uart.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "nrf_delay.h"
#include "uart.h"
#include "iap.h"

/** @brief Function for sending ' Exit!' string to UART.
Execution is blocked until UART peripheral detects all characters have been sent.
 */
static __INLINE void uart_quit()
{
  simple_uart_putstring((const uint8_t *)" \n\rExit!\n\r");
}

/** @brief Function for sending 'Start: ' string to UART.
Execution is blocked until UART peripheral detects all characters have been sent.
 */
static __INLINE void uart_start()
{
  simple_uart_putstring((const uint8_t *)"IAP bootloader start...\n");
	simple_uart_putstring((const uint8_t *)"KEY1：写入固件\n");
	simple_uart_putstring((const uint8_t *)"KEY2：执行固件\n");
	simple_uart_putstring((const uint8_t *)"等待接收固件...\n");
}


/**
 * @brief Function for application main entry. 
 * @return 0. int return type required by ANSI/ISO standard.
 */
int main(void)
{
  uint8_t t;
	uint16_t oldcount = 0;	//老的串口接收数据值
	uint16_t applenth = 0;	//接收到的app代码长度
	
	simple_uart_config(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, HWFC);
	uart_int_config();
	
	nrf_gpio_cfg_output(LED_0);
//	nrf_gpio_cfg_output(LED_1);
	nrf_gpio_range_cfg_input(BUTTON_START, BUTTON_STOP, BUTTON_PULL);

  uart_start();
  while(true)
  {	
		if(USART_RX_CNT)
		{
			if(oldcount == USART_RX_CNT)//新周期内,没有收到任何数据,认为本次数据接收完成.
			{
				applenth = USART_RX_CNT;
				oldcount = 0;
				USART_RX_CNT = 0;
				simple_uart_putstring("用户程序接收完成!\n");
			
			}
			else oldcount = USART_RX_CNT;			
		}
		
		if (nrf_gpio_pin_read(BUTTON_0) == 0)
		{
			nrf_delay_ms(10);	
			if (nrf_gpio_pin_read(BUTTON_0) == 0)
			{
				while (!nrf_gpio_pin_read(BUTTON_0));
				if (applenth)
				{
					simple_uart_putstring("开始更新固件...\n");
					if(((*(uint32_t*)(0X20000000 + 4)) & 0xFF000000) == 0x00000000)//判断是否为0X00XXXXXX.
					{	 
						iap_write_appbin(FLASH_APP1_ADDR, USART_RX_BUF, applenth);//更新FLASH代码   
						simple_uart_putstring("固件更新完成!\n");							
					}
					else 
					{ 
						simple_uart_putstring("非FLASH应用程序!\n");
					}				
				}
				else 
				{
					simple_uart_putstring("没有可以更新的固件!\n");
				}
			}
		}
		if (nrf_gpio_pin_read(BUTTON_1) == 0)
		{
			nrf_delay_ms(10);
			if (nrf_gpio_pin_read(BUTTON_1) == 0)			
			{
				while (!nrf_gpio_pin_read(BUTTON_1));
				if(((*(uint32_t*)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x00000000)//判断是否为0X08XXXXXX.
				{	 
					simple_uart_putstring("开始执行FLASH用户代码!\n");
					iap_load_app(FLASH_APP1_ADDR);		//执行FLASH APP代码
				}
				else 
				{
					simple_uart_putstring("非FLASH应用程序,无法执行!\n");	   
				}	
			}
		}
		
		t++;
		nrf_delay_ms(10);
		if (t == 20)
		{
			t = 0;
			nrf_gpio_pin_toggle(LED_0);
		}
  }
}

