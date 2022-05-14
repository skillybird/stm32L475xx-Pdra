#include "osal_driver.h"
#include "stdio.h"
#include "print.h"


#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
/**
 * @brief	����_sys_exit()�Ա���ʹ�ð�����ģʽ
 *
 * @param	void
 *
 * @return  void
 */
void _sys_exit(int x)
{
    x = x;
}
/**
 * @brief	�ض���fputc����
 *
 * @param	ch		����ַ���
 * @param	f		�ļ�ָ��
 *
 * @return  void
 */
int fputc(int ch, FILE *f)
{
    while((USART1->ISR & 0X40) == 0); //ѭ������,ֱ���������

    USART1->TDR = (unsigned char) ch;
    return ch;
}
#endif

/* console config */
#define CONSOLE_DRIVER		e_driver_usart1

struct console_priv{
	struct usart_driverObject *usart_instance;
	
	unsigned char f_init;
};
static struct console_priv s_console_priv = {0};


static int console_uart_getch(unsigned char *ch)
{
	if(!ch)
		return 0;

	if(!s_console_priv.f_init)
		return 0;
	
	s_console_priv.usart_instance->receive(s_console_priv.usart_instance, ch, 1);

	return 1;
}

static int console_uart_putch(unsigned char *ch)
{
	if(!ch)
		return 0;

	if(!s_console_priv.f_init)
		return 0;
	
	s_console_priv.usart_instance->transmit(s_console_priv.usart_instance, ch, 1);

	return 1;
}

int console_uart_init(unsigned int baud_rate)
{
	s_console_priv.usart_instance = (struct usart_driverObject *)osal_driver_get_object(e_driver_usart1);
	s_console_priv.usart_instance->init(s_console_priv.usart_instance, &baud_rate);

	printf("\r\n");
	printf("Console init successfully!!!\r\n");
	printf("Console driver: uart-%d \r\n", s_console_priv.usart_instance->descriptions->id);
	printf("Console baud rate: %d \r\n", baud_rate);

	s_console_priv.f_init = 1;

	return 0;
}

int getch(unsigned char *ch)
{
	if(!ch)
		return 0;
	
	return console_uart_getch(ch);
}

int putch(unsigned char *ch)
{
	if(!ch)
		return 0;
	
	return console_uart_putch(ch);
}

