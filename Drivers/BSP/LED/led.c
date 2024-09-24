#include "./BSP/LED/led.h"


/* ��ʼ��led���� */
void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    LED0_GPIO_CLK_ENABLE();                                 /* LED0ʱ��ʹ�� */
    LED1_GPIO_CLK_ENABLE();                                 /* LED1ʱ��ʹ�� */
    LED2_GPIO_CLK_ENABLE();                                 /* LED2ʱ��ʹ�� */

    gpio_init_struct.Pin = LED0_GPIO_PIN;                   /* LED0���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */

    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* ���� */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED0���� */

    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1���� */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED1���� */

    gpio_init_struct.Pin = LED2_GPIO_PIN;                   /* LED2���� */
    HAL_GPIO_Init(LED2_GPIO_PORT, &gpio_init_struct);       /* ��ʼ��LED2���� */

    LED0(LED_OFF);                                                /* �ر� LED0 */
    LED1(LED_OFF);                                                /* �ر� LED1 */
    LED2(LED_ON);                                                 /* �� LED2(��Ϊ�����������������һֱ��) */
}
