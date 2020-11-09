#ifndef TMC5130_h
#define TMC5130_h

#include "TMC5130_Registers.h"

#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_spi.h"

#include <array>
#include <cstring>

namespace TMC5130{

    void disable_drive(void);
    void enable_drive(void);
    void find_home(void);
    void initialize(void);
    void move_absolute(float position_mm);
    bool position_reached(void);
    std::uint8_t read(std::uint8_t address, std::int32_t &data);
    std::uint8_t write(std::uint8_t address, std::int32_t data);
    std::uint8_t write_read(std::uint8_t address, std::int32_t tx_data, std::int32_t &rx_data);

    const auto datagram_size = 5;
    const auto spi_instance = SPI1;
    const auto spi_timeout = 1; //ms
    const auto stepps_per_mm = 1280;
    const auto chop_conf_on = 0x28002;
}

#endif