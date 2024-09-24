/* -*- coding: GB2312 -*-
***
Author    				:    wangyanxin
Email Address         	:    401714937@qq.com
Filename             	:    main.c
Data                 	:    2024-9-24
Description           	:    boost ��ѹ��Դ.
***

here is the main structure:

start_task: ��ʼ��������裺pwm(2), ADC(3), IO external interrupt(2 for keys), LED(2), IIC(for OLED)
task1:      led��ʾ����״̬�����¼Ӱ������̵��������¼����������������·��������ָʾ�ƣ����Ƴ�����
task2:      OLED��ʾ�����ѹ�������ѹ���������

interrupt1: ADC1(�����ѹ���)
interrupt2: ADC2(�����ѹ���)
interrupt3: ADC3(����������)
�����жϵ�AD����ͨ����Ϣ���д���
interrupt4: �������ⲿ�ж�

timer: ���� PID �ջ�
***

ģ��:
    1. LED: led���
    2. ADC: ��ʼ������ADC����
    3. OLED: ��Ļ��ʾ
    4. KEY: ������أ��ⲿ�ж�
    5. TIMER: ��ʱ��
    6. PWM: ����pwm���
***

Modification History    	:
Data            Author       Version         Change Description
=======================================================
24/9/24        wangyanxin    1.0              Original
***

PIN DESCRIPTION

ADC:
PA0 -> ADC1
PA1 -> ADC2
PA2 -> ADC3

����:
PA3 -> key1
PA4 -> key2

PWM(TIM2):
PB3  -> PWM1
PA15 -> PWM2

OLED:
PB6 -> IIC_SCL
PB7 -> IIC_SDA

LED:
PA5 -> �̵�
PA6 -> ���
PA7 -> ����
*/

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./MALLOC/malloc.h"
#include "freertos_demo.h"

int main(void)
{
    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* ����ʱ��, 72Mhz */
    delay_init(72);                     /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */

    oled_init();                        /* OLED��ʼ�� */
    led_init();                         /* ��ʼ��LED */
    key_init();                         /* ��ʼ������ */
    my_mem_init(SRAMIN);                /* ��ʼ���ڲ�SRAM�ڴ�� */
    exti_init();                        /* �ⲿ�жϳ�ʼ�� */
    freertos_demo();                    /* ����FreeRTOS���� */
}
