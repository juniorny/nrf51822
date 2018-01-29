#include "iap.h"
#include "nrf_nvmc.h"
#include "simple_uart.h"

iapfun jump2app; 
uint8_t iapbuf[1024]; 

//appxaddr:Ӧ�ó������ʼ��ַ
//appbuf:Ӧ�ó���CODE.
//appsize:Ӧ�ó����С(�ֽ�).
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
	uint16_t t;
	uint16_t i = 0;
	uint8_t temp;
	uint32_t fwaddr = appxaddr;//��ǰд��ĵ�ַ
	uint8_t *dfu = appbuf;
	for(t = 0; t < appsize; t++)
	{						    
		temp = *dfu; 
		dfu++;
		iapbuf[i++] = temp;	    
		if(i == 1024)
		{
			i = 0;
			nrf_nvmc_page_erase(fwaddr);
			nrf_nvmc_write_bytes(fwaddr, iapbuf, 1024);			
			fwaddr += 1024;			//ƫ��2048  16=2*8.����Ҫ����2.
		}
	}
	if(i)
	{
		nrf_nvmc_page_erase(fwaddr);
		nrf_nvmc_write_bytes(fwaddr, iapbuf, i);//������һЩ�����ֽ�д��ȥ. 
	}		
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(uint32_t appxaddr)
{
//	if(((*(uint32_t*)appxaddr) & 0x2FFE0000) == 0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
//	{ 
		jump2app = (iapfun)*(uint32_t*)(appxaddr + 4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
//		MSR_MSP(*(uint32_t*)appxaddr);								//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();																		//��ת��APP.
//	}
}		 
