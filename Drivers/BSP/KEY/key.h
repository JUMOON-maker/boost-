#ifndef __KEY_H
#define __KEY_H

#include "./SYSTEM/sys/sys.h"

/******************************************************************************************/
/* ���� ���� */

/* �Ӱ��� */
#define KEY0_GPIO_PORT                  GPIOA
#define KEY0_GPIO_PIN                   GPIO_PIN_3
#define KEY0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

/* ������ */
#define KEY1_GPIO_PORT                  GPIOA
#define KEY1_GPIO_PIN                   GPIO_PIN_4
#define KEY1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

/******************************************************************************************/

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)     /* ��ȡKEY0���� */
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)     /* ��ȡKEY1���� */

#define KEY0_PRES    1              /* KEY0���� */
#define KEY1_PRES    2              /* KEY1���� */

#define KEY_PRES     1              /* ��ȡ���ڰ�������ʱ�Ǹߵ�ƽ���ǵ͵�ƽ */
#define KEY_UP       0              /* ��Ҳ���ǰ���û�а���ʱ���Ŷ�����Ĭ��״̬ */
void key_init(void);                /* ������ʼ������ */
uint8_t key_scan(uint8_t mode);     /* ����ɨ�躯�� */

#endif


















