#include "main_app.h"
#include "delay.h"
#include <string.h>
#include <stdio.h>
#include "print.h"



void main_app(void)
{
	struct leds_driverObject *led_instance;
	//unsigned int i;
	unsigned char ch;

	led_instance = (struct leds_driverObject *)osal_driver_get_object(e_driver_leds);
	led_instance->init(led_instance, NULL);

	//i = 0;
	while(1){
		getch(&ch);
		if(ch == '\r')
		{
			printf("%c", ch);
			printf("\nsys>");
		}
		else
			printf("%c", ch);
	
		led_instance->toggle(led_instance, 1);
	}
}
