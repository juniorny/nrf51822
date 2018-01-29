#line 1 "..\\user\\iap.c"
#line 1 "..\\user\\iap.h"



#line 1 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"
 
 





 









     
#line 27 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"
     











#line 46 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"





 

     

     
typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
typedef   signed       __int64 int64_t;

     
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
typedef unsigned       __int64 uint64_t;

     

     
     
typedef   signed          char int_least8_t;
typedef   signed short     int int_least16_t;
typedef   signed           int int_least32_t;
typedef   signed       __int64 int_least64_t;

     
typedef unsigned          char uint_least8_t;
typedef unsigned short     int uint_least16_t;
typedef unsigned           int uint_least32_t;
typedef unsigned       __int64 uint_least64_t;

     

     
typedef   signed           int int_fast8_t;
typedef   signed           int int_fast16_t;
typedef   signed           int int_fast32_t;
typedef   signed       __int64 int_fast64_t;

     
typedef unsigned           int uint_fast8_t;
typedef unsigned           int uint_fast16_t;
typedef unsigned           int uint_fast32_t;
typedef unsigned       __int64 uint_fast64_t;

     




typedef   signed           int intptr_t;
typedef unsigned           int uintptr_t;


     
typedef   signed     long long intmax_t;
typedef unsigned     long long uintmax_t;




     

     





     





     





     

     





     





     





     

     





     





     





     

     






     






     






     

     


     


     


     

     
#line 216 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"

     



     






     
    
 



#line 241 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"

     







     










     











#line 305 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdint.h"






 
#line 5 "..\\user\\iap.h"

typedef  void (*iapfun)(void);				

																		   
void iap_load_app(uint32_t appxaddr);			
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t applen);	


#line 2 "..\\user\\iap.c"
#line 1 "..\\..\\..\\..\\Include\\nrf_nvmc.h"











 




 




#line 23 "..\\..\\..\\..\\Include\\nrf_nvmc.h"












 







 
void nrf_nvmc_page_erase(uint32_t address);










 
void nrf_nvmc_write_byte(uint32_t address , uint8_t value);






 
void nrf_nvmc_write_word(uint32_t address, uint32_t value);








 
void nrf_nvmc_write_bytes(uint32_t  address, const uint8_t * src, uint32_t num_bytes);








 
void nrf_nvmc_write_words(uint32_t address, const uint32_t * src, uint32_t num_words);



 


#line 3 "..\\user\\iap.c"
#line 1 "..\\..\\..\\..\\Include\\simple_uart.h"
 









 




 

#line 1 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdbool.h"
 






 





#line 25 "D:\\Keil_v5\\ARM\\ARMCC\\Bin\\..\\include\\stdbool.h"



#line 19 "..\\..\\..\\..\\Include\\simple_uart.h"
#line 20 "..\\..\\..\\..\\Include\\simple_uart.h"









 




 
uint8_t simple_uart_get(void);






 
_Bool simple_uart_get_with_timeout(int32_t timeout_ms, uint8_t *rx_data);




 
void simple_uart_put(uint8_t cr);





 
void simple_uart_putstring(const uint8_t *str);







 
void simple_uart_config(uint8_t rts_pin_number, uint8_t txd_pin_number, uint8_t cts_pin_number, uint8_t rxd_pin_number, _Bool hwfc);



 

 
#line 4 "..\\user\\iap.c"

iapfun jump2app; 
uint8_t iapbuf[1024]; 




void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
	uint16_t t;
	uint16_t i = 0;
	uint8_t temp;
	uint32_t fwaddr = appxaddr;
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
			fwaddr += 1024;			
		}
	}
	if(i)
	{
		nrf_nvmc_page_erase(fwaddr);
		nrf_nvmc_write_bytes(fwaddr, iapbuf, i);
	}		
}



void iap_load_app(uint32_t appxaddr)
{


		jump2app = (iapfun)*(uint32_t*)(appxaddr + 4);		

		jump2app();																		

}		 
