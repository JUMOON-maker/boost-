#include "freertos_demo.h"
#include "./SYSTEM/DELAY/delay.h"
#include "./SYSTEM/USART/usart.h"
#include "./BSP/LED/led.h"

/*FreeRTOS*********************************************************************************************/
#include "FreeRTOS.h"
#include "task.h"

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

/* ����ָʾ״̬ */
enum{
    WORKING = 0,
    UP = 1,
    DOWN = 2,
    WORKERROR = 3
};

/* ���ȫ�ֱ��� */
uint8_t work_flag = 0;

/******************************************************************************************************/

/* FreeRTOS������ں��� */
void freertos_demo(void)
{

    xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ������������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */
    vTaskStartScheduler();
}


/* ��ʼ���������������������Լ���һЩ������ʼ�� */
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

        vTaskDelay(1000);
    }
}