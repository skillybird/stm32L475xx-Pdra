#include "osal_usart.h"

struct usart_priv{
	struct usart_driverObject obj;
	UART_HandleTypeDef huart;
	IRQn_Type interrupt_id;
	
	unsigned char as_uart;
	unsigned char f_init;
};

static struct usart_priv s_usart_priv[e_usart_max] = {
	{0},
	{
		.huart.Instance = USART1,
		.interrupt_id = USART1_IRQn,
		.f_init = 0,
		.as_uart = 1,
	},
	{
		.huart.Instance = USART2,
		.interrupt_id = USART2_IRQn,
		.f_init = 0,
		.as_uart = 1,		
	},
	{
		.huart.Instance = USART3,
		.interrupt_id = USART3_IRQn,
		.f_init = 0,
		.as_uart = 1,
	},
};

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	struct usart_driverObject *obj;
	GPIO_InitTypeDef GPIO_Initure;	

	/* enable the usartx interface clock */
	if((unsigned int)huart->Instance == (unsigned int)USART1){
		obj = &s_usart_priv[1].obj;
		if(!obj->descriptions->vaild)
			return;
		__HAL_RCC_USART1_CLK_ENABLE();
	}
	else if((unsigned int)huart->Instance == (unsigned int)USART2){
		obj = &s_usart_priv[2].obj;
		if(!obj->descriptions->vaild)
			return;
		__HAL_RCC_USART2_CLK_ENABLE();
	}
	else if((unsigned int)huart->Instance == (unsigned int)USART3){
		obj = &s_usart_priv[3].obj;
		if(!obj->descriptions->vaild)
			return;
		__HAL_RCC_USART3_CLK_ENABLE();
	}
	else{
		return;
	}

	/*
	 * usart pins configuration:
	 * 1) enable the clock for the usart gpios
	 	      ** already enable all gpio clk, main.c
	 * 2) configure these usart pins as alternate function pull-up
	 */
	if(obj->descriptions->clkPin.vaild){
		GPIO_Initure.Pin = obj->descriptions->clkPin.pin;
		GPIO_Initure.Alternate = obj->descriptions->clkPin.alternate;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP; //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;
		GPIO_Initure.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(obj->descriptions->clkPin.obj, &GPIO_Initure);
	}
	if(obj->descriptions->txPin.vaild){
		GPIO_Initure.Pin = obj->descriptions->txPin.pin;
		GPIO_Initure.Alternate = obj->descriptions->txPin.alternate;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP; //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;
		GPIO_Initure.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(obj->descriptions->clkPin.obj, &GPIO_Initure);
	}
	if(obj->descriptions->rxPin.vaild){
		GPIO_Initure.Pin = obj->descriptions->rxPin.pin;
		GPIO_Initure.Alternate = obj->descriptions->rxPin.alternate;
		GPIO_Initure.Mode = GPIO_MODE_AF_PP; //复用推挽输出
		GPIO_Initure.Pull = GPIO_PULLUP;
		GPIO_Initure.Speed = GPIO_SPEED_FAST;
		HAL_GPIO_Init(obj->descriptions->clkPin.obj, &GPIO_Initure);
	}
		
	/* NVIC configuration:
 	 * 1) configure the usartx interrupt priority
 	 * 2）enable the NVIC usart IRQ handle
 	 */
	/* */	
}

static int osal_usart_init(void *obj, void *config_param)
{
	struct usart_driverObject *drv = (struct usart_driverObject *)obj;
	struct usart_priv *priv;
	unsigned int baud_rate = *(unsigned int *)config_param;

	if(!drv->descriptions->vaild)
		return -1;

	priv = (struct usart_priv *)drv->priv;
	if(priv->f_init)
		return -1;
	priv->f_init = 1;
	
	priv->huart.Init.BaudRate = baud_rate;
	priv->huart.Init.WordLength = UART_WORDLENGTH_8B;
	priv->huart.Init.StopBits = UART_STOPBITS_1;
	priv->huart.Init.Parity = UART_PARITY_NONE;
	priv->huart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	priv->huart.Init.Mode = UART_MODE_TX_RX;
 	HAL_UART_Init(&priv->huart);

	//__HAL_UART_ENABLE_IT(&priv->huart, UART_IT_RXNE);
	//HAL_NVIC_EnableIRQ(priv->interrupt_id);
	//HAL_NVIC_SetPriority(priv->interrupt_id, 3, 3);
	
	return 0;
}

static int osal_usart_deinit(void *obj)
{
	return -1;
}

static int osal_usart_assert(unsigned int status)
{
	if(status != HAL_OK)
		return -1;
	
	return 0;
}

static int osal_usart_transmit(void *obj, void *pbuf, unsigned short len)
{
	struct usart_driverObject *drv = (struct usart_driverObject *)obj;
	struct usart_priv *priv;
	HAL_StatusTypeDef status;

	priv = (struct usart_priv *)drv->priv;
	if(!priv->f_init)
		return -1;

	status = HAL_UART_Transmit(&priv->huart, (unsigned char *)pbuf, len, HAL_MAX_DELAY);
	
	return osal_usart_assert(status);
}

static int osal_usart_receive(void *obj, void *pbuf, unsigned short len)
{
	struct usart_driverObject *drv = (struct usart_driverObject *)obj;
	struct usart_priv *priv;
	HAL_StatusTypeDef status;

	priv = (struct usart_priv *)drv->priv;
	if(!priv->f_init)
		return -1;
	
	status = HAL_UART_Receive(&priv->huart, (unsigned char *)pbuf, len, HAL_MAX_DELAY);
	
	return osal_usart_assert(status);
}

void *osal_usart_new(void *obj, unsigned int param)
{
	struct osal_driver_obj *config = (struct osal_driver_obj *)obj;
	struct usart_driverObject *drv = &s_usart_priv[param].obj;
	struct usart_priv *priv = &s_usart_priv[param];

	/* init usartx object */
	drv->descriptions = (struct usart_description *)config->description;
	drv->config_param = NULL;
	drv->priv = (void *)priv;

	/* functions */
	drv->init = osal_usart_init;
	drv->deinit = osal_usart_deinit;
	drv->transmit = osal_usart_transmit;
	drv->receive = osal_usart_receive;

	/* init driver object */
	config->object = (void *)drv;

	return drv;
}
