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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO 1                   /* �������ȼ� */
#define START_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            StartTask_Handler;  /* ������ */
void start_task(void *pvParameters);        /* ������ */

/* TASK1 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK1_PRIO      2                   /* �������ȼ� */
#define TASK1_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task1Task_Handler;  /* ������ */
void task1(void *pvParameters);             /* ������ */

/* TASK2 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK2_PRIO      2                   /* �������ȼ� */
#define TASK2_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task2Task_Handler;  /* ������ */
void task2(void *pvParameters);             /* ������ */

/* TASK3 ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TASK3_PRIO      2                   /* �������ȼ� */
#define TASK3_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Task3Task_Handler;  /* ������ */
void task3(void *pvParameters);             /* ������ */

/* TEST_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define TEST_TASK_PRIO      2                   /* �������ȼ� */
#define TEST_TASK_STK_SIZE  128                 /* �����ջ��С */
TaskHandle_t            Test_Task_Handler;      /* ������ */
void test_task(void *pvParameters);                 /* ������ */
/* ����ָʾ״̬ */
enum{
    WORKING = 0,
    UP = 1,
    DOWN = 2,
    WORKERROR = 3
};

/* ���ȫ�ֱ��� */
uint8_t work_flag = 0;

/* �洢ADC������ض��� */
QueueHandle_t AD0_queue;            // �����ѹ���о��
QueueHandle_t AD1_queue;            // ����������о��
QueueHandle_t AD2_queue;            // �����ѹ���о��

/******************************************************************************************************/

/* FreeRTOS������ں��� */
void freertos_demo(void)
{
    /* �������Դ洢ADC��ֵ��ض��� */
    AD0_queue = xQueueCreate( 1, sizeof( uint16_t ) );
    if( AD0_queue != NULL )                                        // �����Ƿ�ɹ�
    {
        printf("����0�����ɹ�����\r\n");
    }
    else
    {
        printf("����0����ʧ�ܣ���\r\n");
    }

    AD1_queue = xQueueCreate( 1, sizeof( uint16_t ) );
    if( AD1_queue != NULL )                                        // �����Ƿ�ɹ�
    {
        printf("����1�����ɹ�����\r\n");
    }
    else
    {
        printf("����1����ʧ�ܣ���\r\n");
    }

    AD2_queue = xQueueCreate( 1, sizeof( uint16_t ) );
    if( AD2_queue != NULL )                                        // �����Ƿ�ɹ�
    {
        printf("����2�����ɹ�����\r\n");
    }
    else
    {
        printf("����2����ʧ�ܣ���\r\n");
    }


    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ������������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
    vTaskStartScheduler();
}


/* ��ʼ�������������������� */
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           /* �����ٽ��� */
    /* ��ʼ�� PWM ��ʱ�� */

    /* ��������1 */
    xTaskCreate((TaskFunction_t )task1,
                (const char*    )"task1",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    /* ��������2 */
    xTaskCreate((TaskFunction_t )task2,
                (const char*    )"task2",
                (uint16_t       )TASK2_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK2_PRIO,
                (TaskHandle_t*  )&Task2Task_Handler);

    /* ��������3 */
    xTaskCreate((TaskFunction_t )task3,
                (const char*    )"task3",
                (uint16_t       )TASK3_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK3_PRIO,
                (TaskHandle_t*  )&Task3Task_Handler);

    /* ������������ */
    xTaskCreate((TaskFunction_t )test_task,
                (const char*    )"test_task",
                (uint16_t       )TEST_TASK_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TEST_TASK_PRIO,
                (TaskHandle_t*  )&Test_Task_Handler);
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
}


/* ����1��led��ʾ����״̬�����¼Ӱ������̵��������¼����������������·��������ָʾ�ƣ����Ƴ��� */
void task1(void *pvParameters)
{
    while (1)
    {
        switch ( work_flag )
        {
            case WORKING:
                {
                    LED2(LED_ON);// ����������ʱ�����Ƽ�������
                }
            break;

            case UP:
                {
                    LED0_TOGGLE();// ������Ϊ��ʱ���̵Ʒ�ת
                }
            break;
            case DOWN:
                {
                    LED1_TOGGLE();// ������Ϊ��ʱ����Ʒ�ת
                }
            break;

            case WORKERROR:
                {
                    LED2_TOGGLE();// ������״̬�쳣ʱ���������
                }
            break;

            default:
                break;
        }
        vTaskDelay(1000);
    }
}


/* ����2��OLED��ʾ�����ѹ�������ѹ��������� */
void task2(void *pvParameters)
{
    while (1)
    {
        // ���� AD ԭʼ���ݲ�����ת��Ϊ��ѹֵ

        vTaskDelay(1000);
    }
}

/* ����3������ ADC ���� */
enum{
    AD_VIN  = 0,        // �����ѹ
    AD_VOUT = 1,        // �������
    AD_IIN  = 2         // �����ѹ
};
#define ADC_DMA_BUF_SIZE        10 * 3      /* ADC DMA�ɼ� BUF��С, Ӧ����ADCͨ������������ */
uint16_t g_adc_dma_buf[ADC_DMA_BUF_SIZE];   /* ADC DMA BUF */
extern uint8_t g_adc_dma_sta;               /* DMA����״̬��־, 0,δ���; 1, ����� */
void task3(void *pvParameters)
{
    uint16_t i,j;
    uint16_t adcx;
    uint32_t sum;
    BaseType_t err = 0;                              // ����ָʾ
    adc_nch_dma_init((uint32_t)&g_adc_dma_buf);      /* ��ʼ��ADC DMA�ɼ� */
    adc_dma_enable(ADC_DMA_BUF_SIZE);                /* ����ADC DMA�ɼ� */
    while (1)
    {
        if (g_adc_dma_sta == 1)
        {
            /* ѭ����ʾͨ��0~ͨ��2�Ľ�� */
            for(j = 0; j < 3; j++)  /* ����3��ͨ�� */
            {
                sum = 0; /* ���� */
                for (i = 0; i < ADC_DMA_BUF_SIZE / 3; i++)  /* ÿ��ͨ���ɼ���10������,����10���ۼ� */
                {
                    sum += g_adc_dma_buf[(3 * i) + j];      /* ��ͬͨ����ת�������ۼ� */
                }
                adcx = sum / (ADC_DMA_BUF_SIZE / 3);        /* ȡƽ��ֵ */

                /* �� AD ����洢���� */
                switch( j )
                {
                    case AD_VIN:
                    {
                        err = xQueueSend( AD0_queue, &adcx, portMAX_DELAY );
                        if( err != pdTRUE )
                        {
                            printf("����0���\r\n");
                        }
                    }
                    break;
                    case AD_VOUT:
                    {
                        err = xQueueSend( AD1_queue, &adcx, portMAX_DELAY );
                        if( err != pdTRUE )
                        {
                            printf("����1���\r\n");
                        }
                    }
                    break;
                    case AD_IIN:
                    {
                        err = xQueueSend( AD2_queue, &adcx, portMAX_DELAY );
                        if( err != pdTRUE )
                        {
                            printf("����2���\r\n");
                        }
                    }
                    break;
                }
            }
            g_adc_dma_sta = 0;                      /* ���DMA�ɼ����״̬��־ */
            adc_dma_enable(ADC_DMA_BUF_SIZE);       /* ������һ��ADC DMA�ɼ� */
        }
    }
}

/* �������� */
void test_task(void *pvParameters)
{
    /* version1������led�Ĳ��� */
    // while (1)
    // {
    //     LED0(LED_ON);
    //     LED1(LED_ON);
    //     LED2(LED_ON);

    //     vTaskDelay(1000);
    // }

    /* ���ڰ����Ĳ��� */
    // ���ⲿ�ж��У�ֻҪ��������⵽���ƾͻ��в�ͬ����˸״̬

}
