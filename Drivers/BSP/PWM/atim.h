#ifndef __ATIM_H
#define __ATIM_H

#include "./SYSTEM/sys/sys.h"

/* TIMX 互补输出模式 定义
 * 这里设置互补输出相关硬件配置, CHY即正常输出, CHYN即互补输出
 * 修改CCRx可以修改占空比.
 * 默认是针对TIM1
 * 注意: 通过修改这些宏定义,可以支持TIM1/TIM8定时器, 任意一个IO口输出互补PWM(前提是必须有互补输出功能)
 */

/* 输出通道引脚 */
#define ATIM_TIMX_CPLM_CHY_GPIO_PORT            GPIOA
#define ATIM_TIMX_CPLM_CHY_GPIO_PIN             GPIO_PIN_8
#define ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE()    do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

/* 互补输出通道引脚 */
#define ATIM_TIMX_CPLM_CHYN_GPIO_PORT           GPIOB
#define ATIM_TIMX_CPLM_CHYN_GPIO_PIN            GPIO_PIN_13
#define ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

/* 刹车输入引脚 */
#define ATIM_TIMX_CPLM_BKIN_GPIO_PORT           GPIOB
#define ATIM_TIMX_CPLM_BKIN_GPIO_PIN            GPIO_PIN_12
#define ATIM_TIMX_CPLM_BKIN_GPIO_CLK_ENABLE()   do{ __HAL_RCC_GPIOE_CLK_ENABLE(); }while(0)   /* PB口时钟使能 */

/* 互补输出使用的定时器 */
#define ATIM_TIMX_CPLM                          TIM1
#define ATIM_TIMX_CPLM_CHY                      TIM_CHANNEL_1
#define ATIM_TIMX_CPLM_CHY_CCRY                 ATIM_TIMX_CPLM->CCR1
#define ATIM_TIMX_CPLM_CLK_ENABLE()             do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)    /* TIM1 时钟使能 */


/******************************************************************************************/

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc);   /* 高级定时器 互补输出 初始化函数 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg);     /* 高级定时器 互补输出 设置输出比较值 & 死区时间 */

#endif

















