#include "osal_leds.h"
#include <stdlib.h>
#include <string.h>

//RGB接口定义
#define LED_Output(obj, pin, lev)		(lev?HAL_GPIO_WritePin(obj,pin,GPIO_PIN_SET):HAL_GPIO_WritePin(obj,pin,GPIO_PIN_RESET))
#define LED_TogglePin(obj, pin)			HAL_GPIO_TogglePin(obj,pin)

struct leds_priv{
	struct leds_driverObject obj;
	
	unsigned char f_init;
};

static struct leds_priv leds = {
	.obj = {0},
	.f_init = 0,
};


static int osal_leds_init(void *obj, void *user_data)
{
	struct leds_driverObject *leds = (struct leds_driverObject *)obj;
	struct leds_priv *priv;
    GPIO_InitTypeDef GPIO_InitStruct;
	unsigned int i;

	priv = (struct leds_priv *)leds->priv;
	/* check already init finish */
	if(priv->f_init)
		return 0;

	/* init leds manage */
	priv->f_init = 1;

	/* enable gpio clk */
	// main.c enable all gpio instance clock, SystemClock_All_GpioClock_Enable

	/* set gpio as leds function */
	for(i = 0; i < leds->description_count; i++)
	{
		if(!leds->descriptions[i].vaild)
			continue;
		
		GPIO_InitStruct.Pin = leds->descriptions[i].gpio_num;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(leds->descriptions[i].gpio_obj, &GPIO_InitStruct);

		LED_Output(leds->descriptions[i].gpio_obj, leds->descriptions[i].gpio_num, leds->descriptions[i].default_lev);
	}

	return 0;
}


static int osal_leds_deinit(void *obj)
{
	struct leds_driverObject *leds = (struct leds_driverObject *)obj;
	struct leds_priv *priv;
	unsigned int i;

	priv = (struct leds_priv *)leds->priv;
	if(!priv->f_init)
		return 0;
	
	priv->f_init = 0;

	/* release gpio from leds function */
	for(i = 0; i < leds->description_count; i++){
		if(!leds->descriptions[i].vaild)
			continue;

		HAL_GPIO_DeInit(leds->descriptions[i].gpio_obj, leds->descriptions[i].gpio_num);
	}

	return 0;
}

static int osal_leds_toggle(void *obj, unsigned int id)
{
	struct leds_driverObject *leds = (struct leds_driverObject *)obj;
	struct leds_priv *priv;
	unsigned int i;

	priv = (struct leds_priv *)leds->priv;
	if(!priv->f_init)
		return -1;

	for(i = 0; i < leds->description_count; i++){
		if(leds->descriptions[i].id == id){
			if(!leds->descriptions[i].vaild)
				continue;
			LED_TogglePin(leds->descriptions[i].gpio_obj, leds->descriptions[i].gpio_num);
			break;
		}
	}

	if(i == leds->description_count)
		return -1;

	return 0;
}

static int osal_leds_set_output(void *obj, unsigned int id, unsigned int lev)
{
	struct leds_driverObject *leds = (struct leds_driverObject *)obj;
	struct leds_priv *priv;
	unsigned int i;

	priv = (struct leds_priv *)leds->priv;
	if(!priv->f_init)
		return -1;

	for(i = 0; i < leds->description_count; i++){
		if(leds->descriptions[i].id == id){
			if(!leds->descriptions[i].vaild)
				continue;
			LED_Output(leds->descriptions[i].gpio_obj, leds->descriptions[i].gpio_num, lev);
			break;
		}
	}

	if(i == leds->description_count)
		return -1;

	return 0;
}

void *osal_leds_new(void *obj, unsigned int param)
{
	struct osal_driver_obj *config = (struct osal_driver_obj *)(obj);
	struct leds_driverObject *drv = &leds.obj;

	/* init leds object */
	drv->descriptions = (struct led_description *)config->description;
	drv->description_count = param;
	drv->priv = (void *)&leds;
	drv->init = osal_leds_init;
	drv->deinit = osal_leds_deinit;
	drv->set = osal_leds_set_output;
	drv->toggle = osal_leds_toggle;

	/* init driver object */
	config->object = (void *)drv;

	return drv;
}

