#ifndef __ADC_H
#define __ADC_H

#include "./SYSTEM/sys/sys.h"


#define ADC_ADCX                            ADC1
#define ADC_ADCX_CHY_CLK_ENABLE()           do{ __HAL_RCC_ADC1_CLK_ENABLE(); }while(0)   /* ADC1 ʱ��ʹ�� */


/* ADC��ͨ��/��ͨ�� DMA�ɼ� DMA��ͨ�� ����
 * ע��: ADC1��DMAͨ��ֻ����: DMA1_Channel1, ���ֻҪ��ADC1, �����ǲ��ܸĶ���
 *       ADC2��֧��DMA�ɼ�
 *       ADC3��DMAͨ��ֻ����: DMA2_Channel5, ������ʹ�� ADC3 ����Ҫ�޸�
 */
#define ADC_ADCX_DMACx                      DMA1_Channel1
#define ADC_ADCX_DMACx_IRQn                 DMA1_Channel1_IRQn
#define ADC_ADCX_DMACx_IRQHandler           DMA1_Channel1_IRQHandler

#define ADC_ADCX_DMACx_IS_TC()              ( DMA1->ISR & (1 << 1) )    /* �ж� DMA1_Channel1 ������ɱ�־, ����һ���ٺ�����ʽ,
                                                                         * ���ܵ�����ʹ��, ֻ������if���������
                                                                         */
#define ADC_ADCX_DMACx_CLR_TC()             do{ DMA1->IFCR |= 1 << 1; }while(0) /* ��� DMA1_Channel1 ������ɱ�־ */

/******************************************************************************************/
void adc_dma_enable( uint16_t cndtr);               /* ʹ��һ��ADC DMA�ɼ����� */
void adc_nch_dma_init(uint32_t mar);                /* ADC��ͨ�� DMA�ɼ���ʼ�� */
float AD_Trans( uint16_t AD_data, float REF );      /* AD ��ֵ��ת�� */

#endif















