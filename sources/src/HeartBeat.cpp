#include "HeartBeat.h"


namespace HeartBeat{

    auto ticks = 0;

    auto toggle_interval_ms = 0;

    void initialize(void){

        GPIO_InitTypeDef GPIO_InitStruct = {};

        __HAL_RCC_GPIOB_CLK_ENABLE();

        /*Configure GPIO pin Output Level */
        HAL_GPIO_WritePin(led_port, led0_pin, GPIO_PIN_RESET);
        HAL_GPIO_WritePin(led_port, led1_pin, GPIO_PIN_SET);

        /*Configure GPIO pins : PB12 PB13 */
        GPIO_InitStruct.Pin = led0_pin|led1_pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(led_port, &GPIO_InitStruct);

        set_toggle_interval();
    }

    void tick(void){

        if(toggle_interval_ms == 0)
            return;

        --ticks;

        if(ticks < 1){

            ticks = toggle_interval_ms;
            toggle();
        }
    }
    void toggle(void){

        HAL_GPIO_TogglePin(led_port, led0_pin);
        HAL_GPIO_TogglePin(led_port, led1_pin);
    }
    void set_toggle_interval(void){

        toggle_interval_ms = default_toggle_interval_ms;
    }
    void set_toggle_interval(const std::uint16_t interval_ms){

        toggle_interval_ms = interval_ms;
    }

}


