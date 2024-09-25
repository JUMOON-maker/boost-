#include "freertos_demo.h"
#include "./SYSTEM/DELAY/delay.h"
#include "./SYSTEM/USART/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/ADC/adc.h"
/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
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

/* TASK3 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TASK3_PRIO      2                   /* 任务优先级 */
#define TASK3_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Task3Task_Handler;  /* 任务句柄 */
void task3(void *pvParameters);             /* 任务函数 */

/* TEST_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define TEST_TASK_PRIO      2                   /* 任务优先级 */
#define TEST_TASK_STK_SIZE  128                 /* 任务堆栈大小 */
TaskHandle_t            Test_Task_Handler;      /* 任务句柄 */
void test_task(void *pvParameters);                 /* 任务函数 */
/* 工作指示状态 */
enum{
    WORKING = 0,
    UP = 1,
    DOWN = 2,
    WORKERROR = 3
};

/* 相关全局变量 */
uint8_t work_flag = 0;

/* 存储ADC数据相关队列 */
QueueHandle_t AD0_queue;            // 输出电压队列句柄
QueueHandle_t AD1_queue;            // 输出电流队列句柄
QueueHandle_t AD2_queue;            // 输入电压队列句柄

/******************************************************************************************************/

/* FreeRTOS例程入口函数 */
void freertos_demo(void)
{
    /* 创建用以存储ADC数值相关队列 */
    AD0_queue = xQueueCreate( 1, sizeof( uint16_t ) );
    if( AD0_queue != NULL )                                        // 创建是否成功
    {
        printf("队列0创建成功！！\r\n");
    }
    else
    {
        printf("队列0创建失败！！\r\n");
    }

    AD1_queue = xQueueCreate( 1, sizeof( uint16_t ) );
    if( AD1_queue != NULL )                                        // 创建是否成功
    {
        printf("队列1创建成功！！\r\n");
    }
    else
    {
        printf("队列1创建失败！！\r\n");
    }

    AD2_queue = xQueueCreate( 1, sizeof( uint16_t ) );
    if( AD2_queue != NULL )                                        // 创建是否成功
    {
        printf("队列2创建成功！！\r\n");
    }
    else
    {
        printf("队列2创建失败！！\r\n");
    }


    xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传入给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */
    vTaskStartScheduler();
}


/* 开始任务：用来创建其他任务 */
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

    /* 创建任务3 */
    xTaskCreate((TaskFunction_t )task3,
                (const char*    )"task3",
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (TaskHandle_t*  )&Task3Task_Handler);

    /* 创建测试任务 */
    xTaskCreate((TaskFunction_t )test_task,
                (const char*    )"test_task",
                (uint16_t       )TEST_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TEST_TASK_PRIO,
                (TaskHandle_t*  )&Test_Task_Handler);
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
        // 处理 AD 原始数据并将其转换为电压值

        vTaskDelay(1000);
    }
}

/* 任务3：处理 ADC 数据 */
enum{
    AD_VIN  = 0,        // 输出电压
    AD_VOUT = 1,        // 输出电流
    AD_IIN  = 2         // 输入电压
};
#define ADC_DMA_BUF_SIZE        10 * 3      /* ADC DMA采集 BUF大小, 应等于ADC通道数的整数倍 */
uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */
extern uint8_t g_adc_dma_sta;               /* DMA传输状态标志, 0,未完成; 1, 已完成 */
void task3(void *pvParameters)
{
    uint16_t i,j;
    uint16_t adcx;
    uint32_t sum;
    BaseType_t err = 0;                              // 错误指示
    adc_nch_dma_init((uint32_t)&g_adc_dma_buf);      /* 初始化ADC DMA采集 */
    adc_dma_enable(ADC_DMA_BUF_SIZE);                /* 启动ADC DMA采集 */
    while (1)
    {
        if (g_adc_dma_sta == 1)
        {
            /* 循环显示通道0~通道2的结果 */
            for(j = 0; j < 3; j++)  /* 遍历3个通道 */
            {
                sum = 0; /* 清零 */
                for (i = 0; i < ADC_DMA_BUF_SIZE / 3; i++)  /* 每个通道采集了10次数据,进行10次累加 */
                {
                    sum += g_adc_dma_buf[(3 * i) + j];      /* 相同通道的转换数据累加 */
                }
                adcx = sum / (ADC_DMA_BUF_SIZE / 3);        /* 取平均值 */

                /* 将 AD 结果存储起来 */
                switch( j )
                {
                    case AD_VIN:
                    {
                        err = xQueueSend( AD0_queue, &adcx, portMAX_DELAY );
                        if( err != pdTRUE )
                        {
                            printf("队列0入队\r\n");
                        }
                    }
                    break;
                    case AD_VOUT:
                    {
                        err = xQueueSend( AD1_queue, &adcx, portMAX_DELAY );
                        if( err != pdTRUE )
                        {
                            printf("队列1入队\r\n");
                        }
                    }
                    break;
                    case AD_IIN:
                    {
                        err = xQueueSend( AD2_queue, &adcx, portMAX_DELAY );
                        if( err != pdTRUE )
                        {
                            printf("队列2入队\r\n");
                        }
                    }
                    break;
                }
            }
            g_adc_dma_sta = 0;                      /* 清除DMA采集完成状态标志 */
            adc_dma_enable(ADC_DMA_BUF_SIZE);       /* 启动下一次ADC DMA采集 */
        }
    }
}

/* 测试任务 */
void test_task(void *pvParameters)
{
    /* version1：对于led的测试 */
    // while (1)
    // {
    //     LED0(LED_ON);
    //     LED1(LED_ON);
    //     LED2(LED_ON);

    //     vTaskDelay(1000);
    // }

    /* 对于按键的测试 */
    // 在外部中断中，只要按键被检测到，灯就会有不同的闪烁状态

}
