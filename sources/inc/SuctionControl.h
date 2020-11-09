#ifndef SuctionControl_h
#define SuctionControl_h

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_tim.h"
#include "stm32l4xx_hal_adc.h"

#include <cstdint>

namespace SuctionControl{

    void adc_interrupt_handler(void);
    void enable_power(bool state);
    void initialize(void);
    void set_power(std::uint8_t power);
    void tick(void);

    const auto suction_adc_channel = ADC_CHANNEL_10;
    const auto suction_adc_instance = ADC2;
    const auto suction_adc_interval_ms = 100;
    const auto suction_adc_port = GPIOA;
    const auto suction_adc_pin = GPIO_PIN_5;
    
    const auto suction_enable_pin GPIO_PIN_2;
    const auto suction_port GPIOB;
    const auto suction_pwm_pin GPIO_PIN_8; // Timer16 Ch 1
    const auto suction_timer TIM16;
    const auto suction_timer_channel = TIM_CHANNEL_1;
}

#endif