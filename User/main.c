/* -*- coding: GB2312 -*-
***
Author    				:    wangyanxin
Email Address         	:    401714937@qq.com
Filename             	:    main.c
Data                 	:    2024-9-24
Description           	:    boost 稳压电源.
***

here is the main structure:

start_task: 初始化相关外设：pwm(2), ADC(3), IO external interrupt(2 for keys), LED(2), IIC(for OLED)
task1:      led显示工作状态：按下加按键，绿灯亮；按下减按键，红灯亮；电路正常工作指示灯，蓝灯常亮。
task2:      OLED显示输入电压，输出电压，输出电流

interrupt1: ADC1(输入电压检测)
interrupt2: ADC2(输出电压检测)
interrupt3: ADC3(输出电流检测)
以上中断的AD数据通过消息队列传递
interrupt4: 按键的外部中断

timer: 进行 PID 闭环
***

模块:
    1. LED: led相关
    2. ADC: 初始化三个ADC引脚
    3. OLED: 屏幕显示
    4. KEY: 按键相关，外部中断
    5. TIMER: 定时器
    6. PWM: 互补pwm输出
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

按键:
PA3 -> key1
PA4 -> key2

PWM(TIM2):
PB3  -> PWM1
PA15 -> PWM2

OLED:
PB6 -> IIC_SCL
PB7 -> IIC_SDA

LED:
PA5 -> 绿灯
PA6 -> 红灯
PA7 -> 蓝灯
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
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(RCC_PLL_MUL9); /* 设置时钟, 72Mhz */
    delay_init(72);                     /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */

    oled_init();                        /* OLED初始化 */
    led_init();                         /* 初始化LED */
    key_init();                         /* 初始化按键 */
    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
    exti_init();                        /* 外部中断初始化 */
    freertos_demo();                    /* 运行FreeRTOS例程 */
}
