#ifndef __OSAL_DRIVER_INNER_H__
#define __OSAL_DRIVER_INNER_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32l4xx_hal.h"

typedef enum {
	e_driver_leds = 0,
	e_driver_usart1,
	e_driver_usart2,
	e_driver_usart3,

	e_driver_max,
}e_osal_driver;

struct osal_driver_obj{
	unsigned char name[32];
	unsigned int param;
	void *description;
	void *object;
	
	void *(*new_object)(void *obj, unsigned int param);
};


#ifdef __cplusplus
 }
#endif

#endif

