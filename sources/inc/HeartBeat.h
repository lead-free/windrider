#ifndef HeartBeat_h
#define HeartBeat_h

#include <cstdint>
#include "stm32l4xx_hal.h"

namespace HeartBeat{
    
    void initialize(void);
    void tick(void);
    void toggle(void);
    void set_toggle_interval(void);
    void set_toggle_interval(const std::uint16_t interval_ms);

    const auto default_toggle_interval_ms = 1000;
    const auto led_port = GPIOB;
    const auto led0_pin = GPIO_PIN_12;
    const auto led1_pin = GPIO_PIN_13;

}

#endif