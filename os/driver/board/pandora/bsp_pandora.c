#include "bsp_pandora.h"
#include "osal_driver.h"
#include "print.h"


/* leds description */
static struct led_description led_description_list[] = {
	{
		.label = "red",
		.id = 0,
		.default_lev = 1,
		.gpio_obj = GPIOE, 
		.gpio_num = GPIO_PIN_7,
		.vaild = 1,
	},
	{
		.label = "green",
		.id = 1,
		.default_lev = 1,
		.gpio_obj = GPIOE,
		.gpio_num = GPIO_PIN_8,
		.vaild = 1,
	},
	{
		.label = "blue",
		.id = 2,
		.default_lev = 1,
		.gpio_obj = GPIOE,
		.gpio_num = GPIO_PIN_9,
		.vaild = 1,
	},
};

/* usart1 description */
static struct usart_description usart1_description = {
	.id = 1,
		
	.clkPin.obj = GPIOA, 
	.clkPin.pin = GPIO_PIN_8, 
	.clkPin.alternate = GPIO_AF7_USART1,
	.clkPin.vaild = 0,
	
	.txPin.obj = GPIOA,
	.txPin.pin = GPIO_PIN_9,
	.txPin.alternate = GPIO_AF7_USART1,
	.txPin.vaild = 1,
	
	.rxPin.obj = GPIOA,
	.rxPin.pin = GPIO_PIN_10,
	.rxPin.alternate = GPIO_AF7_USART1,
	.rxPin.vaild = 1,
	
	.vaild = 1,
};

/* usart2 descripation */
static struct usart_description usart2_description = {
	.id = 2,
	.clkPin = {0},
	.txPin = {0},
	.rxPin = {0},
	.vaild = 0,
};

/* usart3 descripation */
static struct usart_description usart3_description = {
	.id = 3,
	.clkPin = {0},
	.txPin = {0},
	.rxPin = {0},
	.vaild = 0,
};

static void bsp_pandora_driver_register(void)
{
	/* leds */
	osal_driver_set_description(e_driver_leds,
		(void *)led_description_list,
		sizeof(led_description_list)/sizeof(struct led_description));
	osal_driver_new(e_driver_leds);

	/* usart1 */
	osal_driver_set_description(e_driver_usart1,
		(void *)&usart1_description,
		usart1_description.id);
	osal_driver_new(e_driver_usart1);

	/* usart2 */
	osal_driver_set_description(e_driver_usart2,
		(void *)&usart2_description,
		usart2_description.id);
	osal_driver_new(e_driver_usart2);

	/* usart3 */
	osal_driver_set_description(e_driver_usart3,
		(void *)&usart3_description,
		usart3_description.id);
	osal_driver_new(e_driver_usart3);	
}

void bsp_pandora_board_init(void)
{
	/* register driver */
	bsp_pandora_driver_register();

	/* init console */
	console_uart_init(115200);
}
