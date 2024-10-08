#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"

DMA_HandleTypeDef g_dma_nch_adc_handle = {0};                               /* 定义要搬运ADC多通道数据的DMA句柄 */
ADC_HandleTypeDef g_adc_nch_dma_handle = {0};                               /* 定义ADC（多通道DMA读取）句柄 */
uint8_t g_adc_dma_sta = 0;                                                  // ADC传输完成标志


void adc_nch_dma_init(uint32_t mar)
{
    GPIO_InitTypeDef gpio_init_struct;
    RCC_PeriphCLKInitTypeDef adc_clk_init = {0};
    ADC_ChannelConfTypeDef adc_ch_conf = {0};

    ADC_ADCX_CHY_CLK_ENABLE();                                                /* 使能ADCx时钟 */
    __HAL_RCC_GPIOA_CLK_ENABLE();                                             /* 开启GPIOA时钟 */

    if ((uint32_t)ADC_ADCX_DMACx > (uint32_t)DMA1_Channel7)                   /* 大于DMA1_Channel7, 则为DMA2的通道了 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                                          /* DMA2时钟使能 */
    }
    else
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                                          /* DMA1时钟使能 */
    }

    /* 设置ADC时钟 */
    adc_clk_init.PeriphClockSelection = RCC_PERIPHCLK_ADC;                    /* ADC外设时钟 */
    adc_clk_init.AdcClockSelection = RCC_ADCPCLK2_DIV6;                       /* 分频因子6时钟为72M/6=12MHz */
    HAL_RCCEx_PeriphCLKConfig(&adc_clk_init);                                 /* 设置ADC时钟 */

    /*
        设置ADC1通道0~5对应的IO口模拟输入
        AD采集引脚模式设置,模拟输入
        PA0对应 ADC1_IN0
        PA1对应 ADC1_IN1
        PA2对应 ADC1_IN2
    */
    gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;                  /* GPIOA0~2 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;                                 /* 模拟 */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    /* 初始化DMA */
    g_dma_nch_adc_handle.Instance = ADC_ADCX_DMACx;                           /* 设置DMA通道 */
    g_dma_nch_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;               /* 从外设到存储器模式 */
    g_dma_nch_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                   /* 外设非增量模式 */
    g_dma_nch_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                       /* 存储器增量模式 */
    g_dma_nch_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;  /* 外设数据长度:16位 */
    g_dma_nch_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;     /* 存储器数据长度:16位 */
    g_dma_nch_adc_handle.Init.Mode = DMA_NORMAL;                              /* 外设流控模式 */
    g_dma_nch_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                 /* 中等优先级 */
    HAL_DMA_Init(&g_dma_nch_adc_handle);

    __HAL_LINKDMA(&g_adc_nch_dma_handle, DMA_Handle, g_dma_nch_adc_handle);   /* 将DMA与adc联系起来 */

    /* 初始化ADC */
    g_adc_nch_dma_handle.Instance = ADC_ADCX;                                 /* 选择哪个ADC */
    g_adc_nch_dma_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                /* 数据对齐方式：右对齐 */
    g_adc_nch_dma_handle.Init.ScanConvMode = ADC_SCAN_ENABLE;                 /* 使能扫描模式 */
    g_adc_nch_dma_handle.Init.ContinuousConvMode = ENABLE;                    /* 使能连续转换 */
    g_adc_nch_dma_handle.Init.NbrOfConversion = 3;                            /* 赋值范围是1~16，本实验用到3个规则通道序列 */
    g_adc_nch_dma_handle.Init.DiscontinuousConvMode = DISABLE;                /* 禁止规则通道组间断模式 */
    g_adc_nch_dma_handle.Init.NbrOfDiscConversion = 0;                        /* 配置间断模式的规则通道个数，禁止规则通道组间断模式后，此参数忽略 */
    g_adc_nch_dma_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;          /* 软件触发 */
    HAL_ADC_Init(&g_adc_nch_dma_handle);                                      /* 初始化 */

    HAL_ADCEx_Calibration_Start(&g_adc_nch_dma_handle);                       /* 校准ADC */

    /* 配置ADC通道 */
    adc_ch_conf.Channel = ADC_CHANNEL_0;                                      /* 配置使用的ADC通道 */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_1;                                    /* 采样序列里的第1个 */
    adc_ch_conf.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;                    /* 采样时间，设置最大采样周期:239.5个ADC周期 */
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);               /* 通道配置 */

    adc_ch_conf.Channel = ADC_CHANNEL_1;                                      /* 配置使用的ADC通道 */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_2;                                    /* 采样序列里的第2个 */
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);               /* 配置ADC通道 */

    adc_ch_conf.Channel = ADC_CHANNEL_2;                                      /* 配置使用的ADC通道 */
    adc_ch_conf.Rank = ADC_REGULAR_RANK_3;                                    /* 采样序列里的第3个 */
    HAL_ADC_ConfigChannel(&g_adc_nch_dma_handle, &adc_ch_conf);               /* 配置ADC通道 */


    /* 配置DMA数据流请求中断优先级 */
    HAL_NVIC_SetPriority(ADC_ADCX_DMACx_IRQn, 3, 3);
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMACx_IRQn);

    // ADC1->DR：ADC转换的结果均存储在这里
    HAL_DMA_Start_IT(&g_dma_nch_adc_handle, (uint32_t)&ADC1->DR, mar, 0);     /* 启动DMA，并开启中断 */
    HAL_ADC_Start_DMA(&g_adc_nch_dma_handle, &mar, 0);                        /* 开启ADC，通过DMA传输结果 */
}

/**
 * @brief       使能一次ADC DMA传输
 * @note        该函数用寄存器来操作，防止用HAL库操作对其他参数有修改,也为了兼容性
 * @param       cndtr: DMA传输的次数
 * @retval      无
 */
void adc_dma_enable(uint16_t cndtr)
{
    ADC_ADCX->CR2 &= ~(1 << 0);                 /* 先关闭ADC */

    ADC_ADCX_DMACx->CCR &= ~(1 << 0);           /* 关闭DMA传输 */
    while (ADC_ADCX_DMACx->CCR & (1 << 0));     /* 确保DMA可以被设置 */
    ADC_ADCX_DMACx->CNDTR = cndtr;              /* DMA传输数据量 */
    ADC_ADCX_DMACx->CCR |= 1 << 0;              /* 开启DMA传输 */

    ADC_ADCX->CR2 |= 1 << 0;                    /* 重新启动ADC */
    ADC_ADCX->CR2 |= 1 << 22;                   /* 启动规则转换通道 */
}

/**
 * @brief       ADC DMA采集中断服务函数
 * @param       无
 * @retval      无
 */
void ADC_ADCX_DMACx_IRQHandler(void)
{
    if (ADC_ADCX_DMACx_IS_TC())
    {
        g_adc_dma_sta = 1;                      /* 标记DMA传输完成 */
        ADC_ADCX_DMACx_CLR_TC();                /* 清除DMA1 数据流7 传输完成中断 */
    }
}

// AD 值转化为电压实验
// AD_data: AD 原始数据
// REF: 外界参考电压，因为实际上单片机的输入最大参考电压为3.3，需要进行换算显示实际电压值
float AD_Trans( uint16_t AD_data, float REF )
{
    float temp;                                  // 返回值
    temp = (float)AD_data * (3.3 / 4096);        /* 获取计算后的带小数的实际电压值，比如3.1111 */
    temp = temp * REF / 3.3;                     // 还要再进行一次转化
}







