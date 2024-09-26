#include "./BSP/PWM/atim.h"
#include "./BSP/LED/led.h"

/*互补输出带死区控制*/
static TIM_HandleTypeDef g_timx_cplm_pwm_handle;                              /* 定时器x句柄 */
static TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};             /* 死区时间设置 */

/* PWM输入状态(g_timxchy_cap_sta)
 * 0,没有成功捕获.
 * 1,已经成功捕获了
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM输入状态 */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM输入分频系数 */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM的高电平脉宽 */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM的周期宽度 */


/*******************************互补输出带死区控制程序**************************************/

/**
 * @brief       高级定时器TIMX 互补输出 初始化函数（使用PWM模式1）
 * @note
 *              配置高级定时器TIMX 互补输出, 一路OCy 一路OCyN, 并且可以设置死区时间
 *
 *              高级定时器的时钟来自APB2, 而PCLK2 = 72Mhz, 我们设置PPRE2不分频, 因此
 *              高级定时器时钟 = 72Mhz
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */

void atim_timx_cplm_pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef sConfigOC = {0};
    GPIO_InitTypeDef gpio_init_struct = {0};

    ATIM_TIMX_CPLM_CLK_ENABLE();            /* TIM1 时钟使能 */
    ATIM_TIMX_CPLM_CHY_GPIO_CLK_ENABLE();   /* 通道X对应IO口时钟使能 */
    ATIM_TIMX_CPLM_CHYN_GPIO_CLK_ENABLE();  /* 通道X互补通道对应IO口时钟使能 */

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHY_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH ;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHY_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = ATIM_TIMX_CPLM_CHYN_GPIO_PIN;
    HAL_GPIO_Init(ATIM_TIMX_CPLM_CHYN_GPIO_PORT, &gpio_init_struct);

    g_timx_cplm_pwm_handle.Instance = ATIM_TIMX_CPLM;
    g_timx_cplm_pwm_handle.Init.Prescaler = psc;
    g_timx_cplm_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_timx_cplm_pwm_handle.Init.Period = arr;
    g_timx_cplm_pwm_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    g_timx_cplm_pwm_handle.Init.RepetitionCounter = 0;
    g_timx_cplm_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
    HAL_TIM_PWM_Init(&g_timx_cplm_pwm_handle) ;

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;           /* OCy 低电平有效 */
    sConfigOC.OCNPolarity = TIM_OCNPOLARITY_LOW;         /* OCyN 低电平有效 */
    HAL_TIM_PWM_ConfigChannel(&g_timx_cplm_pwm_handle, &sConfigOC, ATIM_TIMX_CPLM_CHY);     /*配置后默认清CCER的互补输出位*/

    /* 设置PWM死区参数 */
    sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;              /* 运行模式的关闭输出状态 */
    sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;             /* 空闲模式的关闭输出状态 */
    sBreakDeadTimeConfig.BreakState = TIM_BREAK_ENABLE;                   /* 使能刹车输入 */
    sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;                   /* 关闭寄存器锁定 */
    sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;          /* 断路输入极性为高 */
    sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_ENABLE;    /* 自动恢复输出使能控制 */
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &sBreakDeadTimeConfig);  /* 初始化定时器的断路和死区时间 */

    HAL_TIM_PWM_Start(&g_timx_cplm_pwm_handle, ATIM_TIMX_CPLM_CHY);     /* OCy 输出使能 */
    HAL_TIMEx_PWMN_Start(&g_timx_cplm_pwm_handle,ATIM_TIMX_CPLM_CHY);   /* OCyN 输出使能 */

}

/**
 * @brief       定时器TIMX 设置输出比较值 & 死区时间
 * @param       ccr: 输出比较值
 * @param       dtg: 死区时间
 *   @arg       dtg[7:5]=0xx时, 死区时间 = dtg[7:0] * tDTS
 *   @arg       dtg[7:5]=10x时, 死区时间 = (64 + dtg[6:0]) * 2  * tDTS
 *   @arg       dtg[7:5]=110时, 死区时间 = (32 + dtg[5:0]) * 8  * tDTS
 *   @arg       dtg[7:5]=111时, 死区时间 = (32 + dtg[5:0]) * 16 * tDTS
 *   @note      tDTS = 1 / (Ft /  CKD[1:0]) = 1 / 18M = 55.56ns
 * @retval      无
 */
void atim_timx_cplm_pwm_set(uint16_t ccr, uint8_t dtg)
{
    sBreakDeadTimeConfig.DeadTime = dtg;
    HAL_TIMEx_ConfigBreakDeadTime(&g_timx_cplm_pwm_handle, &sBreakDeadTimeConfig);      /*重设死区时间*/
    __HAL_TIM_MOE_ENABLE(&g_timx_cplm_pwm_handle);  /* MOE=1,使能主输出 */
    ATIM_TIMX_CPLM_CHY_CCRY = ccr;          /* 设置比较寄存器 */
}



