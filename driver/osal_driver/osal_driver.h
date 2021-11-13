#ifndef __OSAL_DRIVER_H__
#define __OSAL_DRIVER_H__

#ifdef __cplusplus
extern "c"{
#endif

#include "osal_leds.h"
#include "osal_usart.h"
#include "osal_driver_inner.h"



void *osal_driver_get_object(e_osal_driver id);
	
int osal_driver_set_description(e_osal_driver id, void *description, unsigned int param);
	
void *osal_driver_new(e_osal_driver id);



#ifdef __cplusplus
}
#endif
#endif
