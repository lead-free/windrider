#include "LedDriver.h"

namespace LedDriver{

    std::array<std::uint32_t, 4> pulse = {};

    void initialize(void){
        __HAL_RCC_TIM2_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        TIM_MasterConfigTypeDef sMasterConfig = {0};
        TIM_OC_InitTypeDef sConfigOC = {0};

        TIM_HandleTypeDef timer_handle = {};
        timer_handle.Instance = TIM2;
        timer_handle.Init.Prescaler = 0;
        timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
        timer_handle.Init.Period = timer_period;
        timer_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        HAL_TIM_PWM_Init(&timer_handle);

        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization(&timer_handle, &sMasterConfig);

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        HAL_TIM_PWM_ConfigChannel(&timer_handle, &sConfigOC, TIM_CHANNEL_1);  
        HAL_TIM_PWM_ConfigChannel(&timer_handle, &sConfigOC, TIM_CHANNEL_2);
        HAL_TIM_PWM_ConfigChannel(&timer_handle, &sConfigOC, TIM_CHANNEL_3);
        HAL_TIM_PWM_ConfigChannel(&timer_handle, &sConfigOC, TIM_CHANNEL_4);

        GPIO_InitTypeDef gpio_init_struct = {};
        gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
        gpio_init_struct.Alternate = GPIO_AF1_TIM2;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_2);
        HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_3);
        HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_4);

    }
    void off(void){
        timer_instance->CCR1 = 0;
        timer_instance->CCR2 = 0;
        timer_instance->CCR3 = 0;
        timer_instance->CCR4 = 0;
        
    }
    void off(std::uint8_t channel){
        if(channel == 0)
            timer_instance->CCR1 = 0;

        else if(channel == 1)
            timer_instance->CCR2 = 0;

        else if(channel == 3)
            timer_instance->CCR3 = 0;

        else if(channel == 4)
            timer_instance->CCR4 = 0;
    }
    void on(void){

        timer_instance->CCR1 = pulse[0];
        timer_instance->CCR2 = pulse[1];
        timer_instance->CCR3 = pulse[2];
        timer_instance->CCR4 = pulse[3];
    }

    void on(std::uint8_t channel){
        if(channel == 0)
            timer_instance->CCR1 = pulse[0];

        else if(channel == 1)
            timer_instance->CCR2 = pulse[1];

        else if(channel == 3)
            timer_instance->CCR3 = pulse[2];

        else if(channel == 4)
            timer_instance->CCR4 = pulse[3];
    }
    void set_current(std::uint8_t channel, std::uint16_t current_ma){

        if(current_ma > 1500 or channel > 3)
            return;

        pulse.at(channel) = current_ma*timer_period/1500.0;
    }
}