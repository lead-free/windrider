//! Solenoid declaration file.
/**
 * @file      Solenoid.h
 * @brief     This solenoid abstraction class.
 * @author    Stanislav Sotnikov (stanislav.sotnikov145@gmail.com)
 */

#ifndef SOLENOID_H
#define SOLENOID_H

#include "stm32l4xx_hal.h"

#include <array>
#include <cstdint>
//! Solenoid class
/**
 * @brief This is a driver class for a solenoid. 
 *        WARNING: In the current implementation, solenoids use a software defined pwm
 *                 and share a Timer 3 with suction motor.
 */
namespace Solenoid{

    //! initialize method
    /**
     * @brief Initializes gpio hardware to drive solenoids.
     */
    void initialize(void);

    void configure_timings(std::uint8_t channel, uint16_t off_time_ms, uint16_t on_time_ms);

    //! method on
    /**
    * @brief Starts the impactor with given pulse lenth and period.
    * @param off_time_ms Delay between pulses in milliseconds.
    * @param on_time_ms Delay of the impact puse in milliseconds.
    */ 
    void on(std::uint8_t channel);

    //! method off
    /**
    * @brief Turns off the Impactor.
    */ 
    void off(std::uint8_t channel);
    void off(void);

    // Solenoid's port and pin.
    const auto solenoid_port = GPIOA;
    const auto solenoid0_pwm_pin = GPIO_PIN_8;
    const auto solenoid1_pwm_pin = GPIO_PIN_9;
};

#endif