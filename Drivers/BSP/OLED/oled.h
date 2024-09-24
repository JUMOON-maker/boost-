#ifndef __OLED_H
#define __OLED_H

#include "stdlib.h" 
#include "./SYSTEM/sys/sys.h"


/* OLEDģʽ����
 * 0: 4�ߴ���ģʽ  ��ģ���BS1��BS2����GND��
 * 1: ����8080ģʽ ��ģ���BS1��BS2����VCC��
 */
#define OLED_MODE       1   /* Ĭ��ʹ��8080����ģʽ */

/******************************************************************************************/
/* OLED SPIģʽ���� ���� */
/* ע��:
 * 1, ����������� OLED 4��SPIģʽ����ʱ�� ���Ŷ���
 * 2, OLED_SPI_CS ��  �� OLED_SPI_RS �ڲ���ģʽ��ҲҪ�õ�
 */

/* RST��ֱ�Ӻ�MCU�ĸ�λ�Ź���, ���Բ��ö��� OLED_SPI_RST ���� */
//#define OLED_SPI_RST_PORT               GPIOG
//#define OLED_SPI_RST_PIN                GPIO_PIN_15
//#define OLED_SPI_RST_CLK_ENABLE()       do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /* PG��ʱ��ʹ�� */

#define OLED_SPI_CS_PORT                GPIOC
#define OLED_SPI_CS_PIN                 GPIO_PIN_9
#define OLED_SPI_CS_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

#define OLED_SPI_RS_PORT                GPIOC
#define OLED_SPI_RS_PIN                 GPIO_PIN_8
#define OLED_SPI_RS_CLK_ENABLE()        do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

#define OLED_SPI_SCLK_PORT              GPIOB
#define OLED_SPI_SCLK_PIN               GPIO_PIN_0
#define OLED_SPI_SCLK_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */

#define OLED_SPI_SDIN_PORT              GPIOB
#define OLED_SPI_SDIN_PIN               GPIO_PIN_1
#define OLED_SPI_SDIN_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PB��ʱ��ʹ�� */

/* OLED ����ģʽWR, RD���� ���� */
/* ע��:
 * 1, ����������� ���ڵ� WR �� RD �������Ŷ���. 8λ���ڵ�������, ��������̫��,�Ͳ�����������
 * 2, ����ģʽ��, ҲҪ�õ�: OLED_SPI_CS �� OLED_SPI_RS ����������
 */
#define OLED_WR_PORT                    GPIOC
#define OLED_WR_PIN                     GPIO_PIN_7
#define OLED_WR_CLK_ENABLE()            do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

#define OLED_RD_PORT                    GPIOC
#define OLED_RD_PIN                     GPIO_PIN_6
#define OLED_RD_CLK_ENABLE()            do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

/******************************************************************************************/

/* OLED SPIģʽ��ض˿ڿ��ƺ��� ���� 
 * ע��:OLED_RST/OLED_CS/OLED_RS,�������Ǻ�80����ģʽ���õ�,��80ģʽҲ����ʵ����3������!
 */
//#define OLED_RST(x)     do{ x ? \
//                                  HAL_GPIO_WritePin(OLED_SPI_RST_PORT, OLED_SPI_RST_PIN, GPIO_PIN_SET) : \
//                                  HAL_GPIO_WritePin(OLED_SPI_RST_PORT, OLED_SPI_RST_PIN, GPIO_PIN_RESET); \
//                        }while(0)       /* ����RST���� */

#define OLED_CS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����CS���� */
#define OLED_RS(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_RS_PORT, OLED_SPI_RS_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_RS_PORT, OLED_SPI_RS_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����RS���� */

#define OLED_SCLK(x)    do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_SCLK_PORT, OLED_SPI_SCLK_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_SCLK_PORT, OLED_SPI_SCLK_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����SCLK���� */
#define OLED_SDIN(x)    do{ x ? \
                                  HAL_GPIO_WritePin(OLED_SPI_SDIN_PORT, OLED_SPI_SDIN_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_SPI_SDIN_PORT, OLED_SPI_SDIN_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����SDIN���� */

/* OLED 80����ģʽWR,RD�˿ڿ��ƺ��� ���� */
#define OLED_WR(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_SET) :  \
                                  HAL_GPIO_WritePin(OLED_WR_PORT, OLED_WR_PIN, GPIO_PIN_RESET); \
                        } while (0)     /* ����WR���� */

#define OLED_RD(x)      do{ x ? \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_SET) : \
                                  HAL_GPIO_WritePin(OLED_RD_PORT, OLED_RD_PIN, GPIO_PIN_RESET); \
                        }while(0)       /* ����RD���� */

/* ����/���� ���� */
#define OLED_CMD        0       /* д���� */
#define OLED_DATA       1       /* д���� */

/******************************************************************************************/

static void oled_wr_byte(uint8_t data, uint8_t cmd);    /* дһ���ֽڵ�OLED */
static uint32_t oled_pow(uint8_t m, uint8_t n);         /* OLED��ƽ������ */


void oled_init(void);           /* OLED��ʼ�� */
void oled_clear(void);          /* OLED���� */
void oled_display_on(void);     /* ����OLED��ʾ */
void oled_display_off(void);    /* �ر�OLED��ʾ */
void oled_refresh_gram(void);   /* �����Դ浽OLED */
void oled_draw_point(uint8_t x, uint8_t y, uint8_t dot);    /* OLED���� */
void oled_fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);        /* OLED������� */
void oled_show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t size, uint8_t mode); /* OLED��ʾ�ַ� */
void oled_show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);  /* OLED��ʾ���� */
void oled_show_string(uint8_t x, uint8_t y, const char *p, uint8_t size);           /* OLED��ʾ�ַ��� */

#endif




