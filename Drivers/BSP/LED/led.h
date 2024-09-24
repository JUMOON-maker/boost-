#ifndef _LED_H
#define _LED_H
#include "./SYSTEM/sys/sys.h"

/*
PA5 -> �̵�
PA6 -> ���
PA7 -> ����
*/

#define LED0_GPIO_PORT                  GPIOA
#define LED0_GPIO_PIN                   GPIO_PIN_5
#define LED0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA��ʱ��ʹ�� */

#define LED1_GPIO_PORT                  GPIOA
#define LED1_GPIO_PIN                   GPIO_PIN_6
#define LED1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA��ʱ��ʹ�� */

#define LED2_GPIO_PORT                  GPIOA
#define LED2_GPIO_PIN                   GPIO_PIN_7
#define LED2_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)             /* PA��ʱ��ʹ�� */

/******************************************************************************************/
/* Ϊ�˷������� LED �򿪻���Ϩ�����ﶨ�������������� */
#define LED_ON  1
#define LED_OFF 0

/******************************************************************************************/
/* LED�˿ڶ��� */
#define LED0(x)   do{ x ? \
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : \
                    HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* LED0��ת */

#define LED1(x)   do{ x ? \
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : \
                    HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* LED1��ת */

#define LED2(x)   do{ x ? \
                    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : \
                    HAL_GPIO_WritePin(LED2_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)      /* LED1��ת */


/* LEDȡ������ */
#define LED0_TOGGLE()   do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); }while(0)        /* ��תLED0 */
#define LED1_TOGGLE()   do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)        /* ��תLED1 */
#define LED2_TOGGLE()   do{ HAL_GPIO_TogglePin(LED2_GPIO_PORT, LED2_GPIO_PIN); }while(0)        /* ��תLED2 */

/******************************************************************************************/
/* �ⲿ�ӿں���*/
void led_init(void);                                                                            /* ��ʼ�� */

#endif
