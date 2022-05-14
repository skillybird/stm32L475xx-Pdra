#ifndef __OSAL_LEDS_H__
#define __OSAL_LEDS_H__

#ifdef __cplusplus
 extern "C" {
#endif

#include "osal_driver_inner.h"


struct led_description{
	unsigned char label[32];
	unsigned int id;
	unsigned int default_lev;

	GPIO_TypeDef *gpio_obj;
	unsigned int gpio_num;
	
	unsigned char vaild;
};

struct leds_driverObject{
	/* user config */
	struct led_description *descriptions;
	unsigned int description_count;

	/* driver config */
	void *user_data;
	void *priv;
	
	int (*init)(void *obj, void *user_data);
	int (*deinit)(void *obj);
	int (*set)(void *obj, unsigned int id, unsigned int lev);
	int (*toggle)(void *obj, unsigned int id);
};

#ifdef __cplusplus
 }
#endif

#endif
