//! HardwareDriver declaration file.
/**
 * @file      HardwareDriver.h
 * @brief     This file contains:
 *              - Peripheral initializations.
 *              - Command handlers.
 * @author    Stanislav Sotnikov (stanislav.sotnikov145@gmail.com)
 */
#ifndef HardwareDriver_h
#define HardwareDriver_h

#include "stm32l4xx_hal.h"

#include "HeartBeat.h"
#include "SuctionControl.h"
#include "Solenoid.h"
#include "LedDriver.h"
#include "Tmc5130.h"

#include <algorithm>
#include <string>
#include <vector>

namespace HardwareDriver {

    void error_handler(void);

    //! method get_status
    /**
    * @return status from execution of the previous command..
    */
    const std::string& get_status(void);

    void gpio_init(void);
    void initialize(void);

    void led(const std::vector<std::string> &args);
    void solenoid(const std::vector<std::string> &args);
    void suction_power(const std::vector<std::string> &args);
    void stepper(const std::vector<std::string> &args);
    void sys_clock_config(void);

    const std::string response_ok = "ok\n\r";
    const std::string invalid_argument = "invalid argument\n\r";
    const std::string arguments_out_of_range = "arguments out of range\n\r";
    const std::string error_reset = "Runtime error! Reset required!\n\r";

}
#endif