#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./SYSTEM/USART/usart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/KEY/exti.h"
#include "./BSP/PWM/atim.h"

/**
 * @brief       KEY0 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY0_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_INT_GPIO_PIN);         /* 调用中断处理公用函数 清除KEY1所在中断线 的中断标志位 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY0_INT_GPIO_PIN);         /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}

/**
 * @brief       KEY1 外部中断服务程序
 * @param       无
 * @retval      无
 */
void KEY1_INT_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY1_INT_GPIO_PIN);        /* 调用中断处理公用函数 清除KEY1所在中断线 的中断标志位，中断下半部在HAL_GPIO_EXTI_Callback执行 */
    __HAL_GPIO_EXTI_CLEAR_IT(KEY1_INT_GPIO_PIN);        /* HAL库默认先清中断再处理回调，退出时再清一次中断，避免按键抖动误触发 */
}


/**
 * @brief       中断服务程序中需要做的事情
                在HAL库中所有的外部中断服务函数都会调用此函数
 * @param       GPIO_Pin:中断引脚号
 * @retval      无
 */
extern uint8_t work_flag;           // 工作状态相关全局变量

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // 记录占空比初始值，以便于使用按键控制
    static uint16_t oring_pwm = 300;
    delay_ms(20);      /* 消抖 */
    switch(GPIO_Pin)
    {
        case KEY0_INT_GPIO_PIN:         // 如果是按键一按下
            {
                if (KEY0 == KEY_PRES)
                {
                    if( oring_pwm != 1000)
                    {
                        atim_timx_cplm_pwm_set(oring_pwm ++, 100);               /* 占空比:7:3, 死区时间 100*tDTS */
                    }
                    else
                    {
                        printf("数值已到最大值！！！");
                    }

                    work_flag = UP;
                }
            }
            break;

        case KEY1_INT_GPIO_PIN:         // 如果是按键二按下
            {
                if (KEY1 == KEY_PRES)
                {
                    if( oring_pwm != 0)
                    {
                        atim_timx_cplm_pwm_set(oring_pwm --, 100);               /* 占空比:7:3, 死区时间 100*tDTS */
                    }
                    else
                    {
                        printf("数值已到最小值！！！");
                    }
                    work_flag = DOWN;
                }
            }
            break;
        default:
            break;
    }
}

/**
 * @brief       外部中断初始化程序
 * @param       无
 * @retval      无
 */
void exti_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    KEY0_GPIO_CLK_ENABLE();                                  /* KEY0时钟使能 */
    KEY1_GPIO_CLK_ENABLE();                                  /* KEY1时钟使能 */

    gpio_init_struct.Pin = KEY0_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* 上升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY0_INT_GPIO_PORT, &gpio_init_struct);    /* KEY0配置为上升沿触发中断 */

    gpio_init_struct.Pin = KEY1_INT_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_IT_RISING;             /* 上升沿触发 */
    gpio_init_struct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(KEY1_INT_GPIO_PORT, &gpio_init_struct);    /* KEY1配置为上升沿触发中断 */

    HAL_NVIC_SetPriority(KEY0_INT_IRQn, 3, 0);               /* 抢占3，子优先级0 */
    HAL_NVIC_EnableIRQ(KEY0_INT_IRQn);                       /* 使能中断线3 */

    HAL_NVIC_SetPriority(KEY1_INT_IRQn, 3, 0);               /* 抢占3，子优先级0 */
    HAL_NVIC_EnableIRQ(KEY1_INT_IRQn);                       /* 使能中断线4 */

}












