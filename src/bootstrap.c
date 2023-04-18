/********************************** (C) COPYRIGHT *******************************
* File Name          : main.c
* Author             : WCH
* Version            : V1.0.0
* Date               : 2021/06/06
* Description        : Main program body.
*********************************************************************************
* Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

/*
 *@Note
 USART Print debugging routine:
 USART1_Tx(PA9).
 This example demonstrates using USART1(PA9) as a print debug port output.

*/

#include "debug.h"
#include "driver\pwm_driver.h"


/* Global typedef */

/* Global define */

/* Global Variable */

void LED1_BLINK_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure={0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//SystemInit();
 
	Delay_Init();
	USART_Printf_Init(115200);	
	Delay_Ms(50);
	printf("SystemClk:%d\r\n",SystemCoreClock);
	printf("This is printf example\r\n");

	//LED1_BLINK_INIT();
    //GPIO_ResetBits(GPIOB,GPIO_Pin_0);


    pwm_driver_init();
    while(1)
    {
        GPIO_SetBits(GPIOB,GPIO_Pin_0);
        Delay_Ms(500);
        GPIO_ResetBits(GPIOB,GPIO_Pin_0);
        Delay_Ms(500);
    }

}