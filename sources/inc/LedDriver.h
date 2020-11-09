#ifndef LedDriver_H
#define LedDriver_H

#include "stm32l4xx_hal.h"

#include <array>
#include <cstdint>

namespace LedDriver {

    void initialize(void);
    void off(void);
    void off(std::uint8_t channel);
    void on(void);
    void on(std::uint8_t channel);

    void set_current(std::uint8_t channel, std::uint16_t current_ma);

    const auto timer_instance = TIM2;
    const auto timer_period = 72000;

}

#endif