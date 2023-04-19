/**
 * @file pwm_driver.c
 * @author Yifan Xiao (1993996310@qq.com)
 * @brief PWM driver
 * @version 0.1
 * @date 2023-04-16
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "ch32v30x_conf.h"
#include "debug.h"

#define PWM_MAX_DUTY 10000

// Dead Time (us)
#define DEAD_TIME

#define FREQ_TO_DIV(freq) ((uint16_t)((SystemCoreClock / freq) >> 16))
#define FREQ_TO_PERIOD(freq) ((SystemCoreClock / freq) / (FREQ_TO_DIV(freq) + 1))

// DRV1 -> TIM1
static void _drv1_pwm_init(int freq)
{

    // TIM1 CH A8 A9 A10 CHN B13 B14 B15
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure = {0};


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_TIM1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // And we need GPIO PB12 to be used as driver enable pin.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // Calculate the prescaler and period, ensure can reach the target frequency.

    TIM_TimeBaseStructure.TIM_Period = FREQ_TO_PERIOD(freq);
    TIM_TimeBaseStructure.TIM_Prescaler = FREQ_TO_DIV(freq);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // We use Center-aligned mode 1, Because current detection needs to be done in the middle of the PWM cycle.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);

    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 0xFF;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_CtrlPWMOutputs(TIM1, ENABLE);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Disable);

    TIM_ARRPreloadConfig(TIM1, ENABLE);
    TIM_Cmd(TIM1, ENABLE);
}

// DRV2 -> TIM10
static void _drv2_pwm_init(int freq)
{

    // TIM10 CH1 PB3 CH2 PB4 CH3 PB5 NCH1 PA5 NCH2 PA6 NCH3 PA7
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
    TIM_OCInitTypeDef TIM_OCInitStructure = {0};
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_TIM10 | RCC_APB2Periph_AFIO, ENABLE);
    // Umm... Remap gpio
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM10, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // And we need GPIO PD2 to be used as driver enable pin.
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_Period = FREQ_TO_PERIOD(freq);
    TIM_TimeBaseStructure.TIM_Prescaler = FREQ_TO_DIV(freq);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    // We use Center-aligned mode 1, Because current detection needs to be done in the middle of the PWM cycle.
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned1;
    TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;

    TIM_OC1Init(TIM10, &TIM_OCInitStructure);
    TIM_OC2Init(TIM10, &TIM_OCInitStructure);
    TIM_OC3Init(TIM10, &TIM_OCInitStructure);

    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
    TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime = 72;
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM10, &TIM_BDTRInitStructure);

    TIM_CtrlPWMOutputs(TIM10, ENABLE);

    TIM_OC1PreloadConfig(TIM10, TIM_OCPreload_Disable);
    TIM_OC2PreloadConfig(TIM10, TIM_OCPreload_Disable);
    TIM_OC3PreloadConfig(TIM10, TIM_OCPreload_Disable);

    TIM_ARRPreloadConfig(TIM10, ENABLE);
    TIM_Cmd(TIM10, ENABLE);
}


/**
 * @brief Set PWM duty
 *
 * @param channel
 * @param duty
 */
static void _drv1_set_pwm_duty(uint8_t channel, uint16_t duty)
{
    switch (channel)
    {
    case 1:
        TIM1->CH1CVR = TIM1->ATRLR * duty / PWM_MAX_DUTY;
        break;
    case 2:
        TIM1->CH2CVR = TIM1->ATRLR * duty / PWM_MAX_DUTY;
        break;
    case 3:
        TIM1->CH3CVR = TIM1->ATRLR * duty / PWM_MAX_DUTY;
        break;
    default:
        break;
    }
}

/**
 * @brief Set PWM duty
 *
 * @param channel
 * @param duty
 */
static void _drv2_set_pwm_duty(uint8_t channel, uint16_t duty)
{
    switch (channel)
    {
    case 1:
        TIM10->CH1CVR = TIM10->ATRLR * duty / PWM_MAX_DUTY;
        break;
    case 2:
        TIM10->CH2CVR = TIM10->ATRLR * duty / PWM_MAX_DUTY;
        break;
    case 3:
        TIM10->CH3CVR = TIM10->ATRLR * duty / PWM_MAX_DUTY;
        break;
    default:
        break;
    }
}

static void _drv1_set_enable(uint8_t enable)
{
    if (enable)
    {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);
    }
    else
    {
        GPIO_ResetBits(GPIOB, GPIO_Pin_12);
    }
}

static void _drv2_set_enable(uint8_t enable)
{
    if (enable)
    {
        GPIO_SetBits(GPIOD, GPIO_Pin_2);
    }
    else
    {
        GPIO_ResetBits(GPIOD, GPIO_Pin_2);
    }
}

void pwm_driver_init(void)
{
    _drv2_pwm_init(20000);
    _drv2_set_enable(0);

    // PWM Test
    _drv2_set_pwm_duty(1, 5000);
    _drv2_set_pwm_duty(2, 5000);
    _drv2_set_pwm_duty(3, 5000);

    while (1)
        ;
}
