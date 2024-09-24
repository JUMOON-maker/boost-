#include "./BSP/LED/led.h"


/* 初始化led引脚 */
void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    LED0_GPIO_CLK_ENABLE();                                 /* LED0时钟使能 */
    LED1_GPIO_CLK_ENABLE();                                 /* LED1时钟使能 */
    LED2_GPIO_CLK_ENABLE();                                 /* LED2时钟使能 */

    gpio_init_struct.Pin = LED0_GPIO_PIN;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */

    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* 初始化LED0引脚 */

    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1引脚 */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* 初始化LED1引脚 */

    gpio_init_struct.Pin = LED2_GPIO_PIN;                   /* LED2引脚 */
    HAL_GPIO_Init(LED2_GPIO_PORT, &gpio_init_struct);       /* 初始化LED2引脚 */

    LED0(LED_OFF);                                                /* 关闭 LED0 */
    LED1(LED_OFF);                                                /* 关闭 LED1 */
    LED2(LED_ON);                                                 /* 打开 LED2(因为正常工作情况下蓝灯一直亮) */
}
