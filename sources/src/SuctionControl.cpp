//! SuctionControl implementation file.
/**
 * @file      SuctionControl.cpp
 * @author    Stanislav Sotnikov (stanislav.sotnikov145@gmail.com)
 *
 */

#include "SuctionControl.h"
#include "stm32l4xx_hal_tim.h"

namespace SuctionControl{

    bool is_initialized = false;
    bool is_enabled = false;
    float current = 0;
    TIM_HandleTypeDef suction_timer_handle = {};
    ADC_HandleTypeDef adc_handle = {};

    void adc_interrupt_handler(void){

        current = HAL_ADC_GetValue(&adc_handle)*3.3/4095;

        HAL_ADC_IRQHandler(&adc_handle);
    }
    void initialize(void){

        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_TIM16_CLK_ENABLE();
        __HAL_RCC_ADC_CLK_ENABLE();
        
        // const_cast is evil, avoid using it.
        HAL_GPIO_WritePin(suction_port, suction_enable_pin | suction_pwm_pin, GPIO_PIN_RESET);

        // Initialize gpio
        GPIO_InitTypeDef gpio_init_struct = {};

        // adc pin
        gpio_init_struct = {};
        gpio_init_struct.Pin = suction_adc_pin;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG_ADC_CONTROL;
        gpio_init_struct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(suction_adc_port, &gpio_init_struct);

        // Enable pin
        gpio_init_struct = {};
        gpio_init_struct.Pin = suction_enable_pin;
        gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;

        HAL_GPIO_Init(suction_port, &gpio_init_struct);

        // pwm pin
        gpio_init_struct = {};
        gpio_init_struct.Pin = suction_pwm_pin;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
        gpio_init_struct.Alternate = GPIO_AF14_TIM16;

        HAL_GPIO_Init(suction_port, &gpio_init_struct);

        // Initialize timer.
        suction_timer_handle.Instance  = suction_timer;
        suction_timer_handle.Init.Prescaler = 1,    
        suction_timer_handle.Init.CounterMode = TIM_COUNTERMODE_UP,
        suction_timer_handle.Init.Period = 36000,
        
        HAL_TIM_Base_Init(&suction_timer_handle);
        HAL_TIM_PWM_Init(&suction_timer_handle);

        TIM_OC_InitTypeDef tim_oc_config = {};

        /* Common configuration for all channels */
        tim_oc_config.OCMode = TIM_OCMODE_PWM1;
        tim_oc_config.Pulse = 0;

        HAL_TIM_PWM_ConfigChannel(&suction_timer_handle, &tim_oc_config, suction_timer_channel);  
        HAL_TIM_PWM_Start(&suction_timer_handle, suction_timer_channel);

        ADC_ChannelConfTypeDef adc_ch_config = {};

        /** Common config
         */
        adc_handle.Instance = suction_adc_instance;
        adc_handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV256;
        adc_handle.Init.Resolution = ADC_RESOLUTION_12B;
        adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        adc_handle.Init.ScanConvMode = ADC_SCAN_DISABLE;
        adc_handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        adc_handle.Init.LowPowerAutoWait = DISABLE;
        adc_handle.Init.ContinuousConvMode = ENABLE;
        adc_handle.Init.NbrOfConversion = 1;
        adc_handle.Init.DiscontinuousConvMode = DISABLE;
        adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        adc_handle.Init.DMAContinuousRequests = DISABLE;
        adc_handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
        adc_handle.Init.OversamplingMode = DISABLE;

        HAL_ADC_Init(&adc_handle);
        /** Configure Regular Channel
         */
        adc_ch_config.Channel = suction_adc_channel;
        adc_ch_config.Rank = ADC_REGULAR_RANK_1;
        adc_ch_config.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
        adc_ch_config.SingleDiff = ADC_SINGLE_ENDED;
        adc_ch_config.OffsetNumber = ADC_OFFSET_NONE;
        adc_ch_config.Offset = 0;

        HAL_ADC_ConfigChannel(&adc_handle, &adc_ch_config);
         //Set up adc interrupt.
        HAL_NVIC_SetPriority(ADC1_2_IRQn, 2, 0);
        HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
        
        HAL_ADC_Start_IT(&adc_handle);

        is_initialized = true;

    }
    void set_power(std::uint8_t power){
        if(0 <= power and power <=100 and is_initialized and is_enabled){
            // Manipulate the duty cycle.
            suction_timer->CCR1 = static_cast<std::uint16_t>(suction_timer_handle.Init.Period*power/100.0);

            if(power)                     
                HAL_TIM_PWM_Start(&suction_timer_handle, suction_timer_channel);
            else
                HAL_TIM_PWM_Stop(&suction_timer_handle, suction_timer_channel);
        }
    }
    void enable_power(bool state){
        
        is_enabled = state;

        if(is_initialized){
            if(state)
                HAL_GPIO_WritePin(suction_port, suction_enable_pin, GPIO_PIN_SET);

            else
            {   
                set_power(0);
                HAL_GPIO_WritePin(suction_port, suction_enable_pin, GPIO_PIN_RESET);
            }
                
        }  
    }
}
