#include "osal_driver.h"

extern void *osal_leds_new(void *obj, unsigned int param);
extern void *osal_usart_new(void *obj, unsigned int param);

static struct osal_driver_obj s_osal_driver_support_list[e_driver_max] =
{
	/* leds */
	{
		.name = "leds",
		.param = 0,
		.description = NULL,
		.object = NULL,
		.new_object = osal_leds_new,
	},
	/* usart1 */
	{
		.name = "usart1",
		.param = 0,
		.description = NULL,
		.object = NULL,
		.new_object = osal_usart_new,
	},
	/* usart2 */
	{
		.name = "usart2",
		.param = 0,
		.description = NULL,
		.object = NULL,
		.new_object = osal_usart_new,
	},
	/* usart3 */
	{
		.name = "usart3",
		.param = 0,
		.description = NULL,
		.object = NULL,
		.new_object = osal_usart_new,
	},
};


void *osal_driver_get_object(e_osal_driver id)
{
	if(id >= e_driver_max)
		return NULL;

	return s_osal_driver_support_list[id].object;
}

int osal_driver_set_description(e_osal_driver id, void *description, unsigned int param)
{
	struct osal_driver_obj *instance;

	if(id >= e_driver_max)
		return -1;

	instance = (struct osal_driver_obj *)&s_osal_driver_support_list[id];
	instance->description = description;
	instance->param = param;

	return 0;
}

void *osal_driver_new(e_osal_driver id)
{
	struct osal_driver_obj *instance;

	if(id >= e_driver_max)
		return NULL;

	instance = (struct osal_driver_obj *)&s_osal_driver_support_list[id];
	if(instance->description == NULL)
		return NULL;

	if(instance->new_object){
		return instance->new_object((void *)instance, instance->param);
	}

	return NULL;
}
