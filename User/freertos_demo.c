#include "freertos_demo.h"
#include "./SYSTEM/DELAY/delay.h"
#include "./SYSTEM/USART/usart.h"
#include "./BSP/LED/led.h"

/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO 1                   /* 任务优先级 */
#define START_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            StartTask_Handler;  /* 任务句柄 */
void start_task(void *pvParameters);        /* 任务函数 */

/* TASK1 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK1_PRIO      2                   /* 任务优先级 */
#define TASK1_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task1Task_Handler;  /* 任务句柄 */
void task1(void *pvParameters);             /* 任务函数 */

/* TASK2 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK2_PRIO      2                   /* 任务优先级 */
#define TASK2_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task2Task_Handler;  /* 任务句柄 */
void task2(void *pvParameters);             /* 任务函数 */

/* 工作指示状态 */
enum{
    WORKING = 0,
    UP = 1,
    DOWN = 2,
    WORKERROR = 3
};

/* 相关全局变量 */
uint8_t work_flag = 0;

/******************************************************************************************************/

/* FreeRTOS例程入口函数 */
void freertos_demo(void)
{

    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}


/* 开始任务：用来创建其他任务，以及做一些基本初始化 */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* 进入临界区 */
    /* 初始化 PWM 定时器 */

    /* 创建任务1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* 创建任务2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);

    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
    taskEXIT_CRITICAL();            /* 退出临界区 */
}


/* 任务1：led显示工作状态：按下加按键，绿灯亮；按下减按键，红灯亮；电路正常工作指示灯，蓝灯常亮 */
void task1(void *pvParameters)
{
    while (1)
    {
        switch ( work_flag )
        {
            case WORKING:
                {
                    LED2(LED_ON);// 当工作正常时，蓝灯继续亮着
                }
            break;

            case UP:
                {
                    LED0_TOGGLE();// 当按键为加时，绿灯翻转
                }
            break;
            case DOWN:
                {
                    LED1_TOGGLE();// 当按键为减时，红灯翻转
                }
            break;

            case WORKERROR:
                {
                    LED2_TOGGLE();// 当工作状态异常时，蓝灯灭掉
                }
            break;

            default:
                break;
        }
        vTaskDelay(1000);
    }
}


/* 任务2：OLED显示输入电压，输出电压，输出电流 */
void task2(void *pvParameters)
{
    while (1)
    {

        vTaskDelay(1000);
    }
}