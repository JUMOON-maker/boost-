#ifndef __EXTI_H
#define __EXTI_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* 引脚 和 中断编号 & 中断服务函数 定义 */

#define KEY0_INT_GPIO_PORT              GPIOA
#define KEY0_INT_GPIO_PIN               GPIO_PIN_3
#define KEY0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */
#define KEY0_INT_IRQn                   EXTI3_IRQn
#define KEY0_INT_IRQHandler             EXTI3_IRQHandler

#define KEY1_INT_GPIO_PORT              GPIOA
#define KEY1_INT_GPIO_PIN               GPIO_PIN_4
#define KEY1_INT_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */
#define KEY1_INT_IRQn                   EXTI4_IRQn
#define KEY1_INT_IRQHandler             EXTI4_IRQHandler

/******************************************************************************************/
/* 工作指示状态 */
extern enum{
    WORKING = 0,
    UP = 1,
    DOWN = 2,
    WORKERROR = 3
};

void exti_init(void);  /* 外部中断初始化 */

#endif

























