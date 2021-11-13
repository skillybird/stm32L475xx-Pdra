#ifndef __OSAL_USART_H__
#define __OSAL_USART_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "osal_driver_inner.h"

typedef enum {
	e_usart1 = 1,
	e_usart2,
	e_usart3,

	e_usart_max,
}e_usart;

 struct usart_pinDescription{
	 GPIO_TypeDef *obj;
	 unsigned int pin;
	 unsigned int alternate;

	 unsigned char vaild;
 };
 
 struct usart_description{
	 unsigned int id;
 
	 /* sharepin description */
	 struct usart_pinDescription clkPin;
	 struct usart_pinDescription txPin;
	 struct usart_pinDescription rxPin;

	 unsigned char vaild;
 };
 
 struct usart_driverObject{
	 /* user config */
	 struct usart_description *descriptions;
	 void *config_param;
 
	 /* driver config */
	 void *priv;
	 
	 /* driver io and config functions */
	 int (*init)(void *obj, void *config_param);
	 int (*deinit)(void *obj);
	 int (*transmit)(void *obj, void *pbuf, unsigned short len);
	 int (*receive)(void *obj, void *pbuf, unsigned short len);
 };

#ifdef __cplusplus
 }
#endif

#endif
