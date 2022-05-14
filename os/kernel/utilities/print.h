#ifndef __PRINT_H__
#define __PRINT_H__

#ifdef __cplusplus
extern "C" {
#endif


int console_uart_init(unsigned int baud_rate);

int getch(unsigned char *ch);

int putch(unsigned char *ch);


#ifdef __cplusplus
}
#endif


#endif

