#include "Solenoid.h"

namespace Solenoid{

    TIM_HandleTypeDef timer_handle = {};

    std::array<std::uint8_t, 2> channel_pulse = {};
    std::uint16_t ch0_pulse = 0;
    std::uint16_t ch1_pulse = 0;

    void configure_timings(std::uint8_t channel, uint16_t off_time_ms, uint16_t on_time_ms){

        if(channel > 1)
            return;

        // Update autoreload register
        TIM1->ARR = 2*(off_time_ms + on_time_ms);

        channel_pulse.at(channel) = 2*on_time_ms;
    }

    void initialize(void){

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_TIM1_CLK_ENABLE();

        TIM_ClockConfigTypeDef sClockSourceConfig = {};
        TIM_MasterConfigTypeDef sMasterConfig = {};
        TIM_OC_InitTypeDef sConfigOC = {};

        timer_handle.Instance = TIM1;
        timer_handle.Init.Prescaler = 36000;
        timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
        timer_handle.Init.Period = 500; // 2000 - 1 second
        timer_handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
        timer_handle.Init.RepetitionCounter = 0;
        timer_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
        HAL_TIM_Base_Init(&timer_handle);

        sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
        HAL_TIM_ConfigClockSource(&timer_handle, &sClockSourceConfig);
        
        HAL_TIM_PWM_Init(&timer_handle);
        
        sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
        sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
        sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
        HAL_TIMEx_MasterConfigSynchronization(&timer_handle, &sMasterConfig);

        sConfigOC.OCMode = TIM_OCMODE_PWM1;
        sConfigOC.Pulse = 0;
        sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
        sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
        sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
        sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
        sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
        HAL_TIM_PWM_ConfigChannel(&timer_handle, &sConfigOC, TIM_CHANNEL_1);
        HAL_TIM_PWM_ConfigChannel(&timer_handle, &sConfigOC, TIM_CHANNEL_2);

        GPIO_InitTypeDef gpio_init_struct = {};
        gpio_init_struct.Pin = solenoid0_pwm_pin|solenoid1_pwm_pin;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
        gpio_init_struct.Alternate = GPIO_AF1_TIM1;
        HAL_GPIO_Init(solenoid_port, &gpio_init_struct);

        HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_1);
        HAL_TIM_PWM_Start(&timer_handle, TIM_CHANNEL_2);

    }

    void off(std::uint8_t channel){
        
        if(channel > 1)
            return;

        if(channel == 0)
            TIM1->CCR1 = 0;

        if(channel == 1)
            TIM1->CCR2 = 0;
    }
    void off(void){

        off(0);
        off(1);
    }

    void on(std::uint8_t channel){

        if(channel > 1)
            return;    

        if(channel == 0)
            TIM1->CCR1 = channel_pulse.at(channel);

        if(channel == 1)
            TIM1->CCR2 = channel_pulse.at(channel);
        

    }
}