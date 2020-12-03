#include "Tmc5130.h"
#include "UsbComm.h"
// 40 mm per roatation 
namespace TMC5130{

    SPI_HandleTypeDef spi_handle = {};

    void disable_drive(void){

        write(TMC5130_CHOPCONF, 0x28000);
    }
    void enable_drive(void){

        write(TMC5130_XTARGET, 0x0);
        write(TMC5130_XACTUAL, 0x0); // Home
        write(TMC5130_CHOPCONF, chop_conf_on);
        write(TMC5130_IHOLD_IRUN, 0x00050F05); // IHOLD_IRUN: IHOLD=5, IRUN=16 (max. current), IHOLDDELAY=6
    }
    void find_home(void){
        
        // Check if the endstop is triggered
        if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == RESET)
        {
            // Negative Velocity 
            write(TMC5130_RAMPMODE, 0x2);

            // Move
            write(TMC5130_VMAX, 10000); // VMAX = 200 000
            write(TMC5130_VACTUAL, 1);
            
            // Wait for endstop
            while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_4) == RESET){}

            // Stop
            write(TMC5130_VACTUAL, 0x0);
            HAL_Delay(1000);
        }
        // Set Home and back to positioning mode
        write(TMC5130_RAMPMODE, 0x0); // RAMPMODE = 0 (Target position move)
        write(TMC5130_XACTUAL, 0x0); // Home
        write(TMC5130_XTARGET, 0x0);

        // Set velocity back to superfast
        write(TMC5130_VMAX, 200000); // VMAX = 200 000

    }

    void initialize(void){

        __HAL_RCC_SPI1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        
        spi_handle.Instance = spi_instance;
        spi_handle.Init.Mode = SPI_MODE_MASTER;
        spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
        spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;
        spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
        spi_handle.Init.NSS = SPI_NSS_HARD_OUTPUT;
        spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
        spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
        spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
        spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
        spi_handle.Init.CRCPolynomial = 7;
        spi_handle.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
        spi_handle.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
        HAL_SPI_Init(&spi_handle);

        GPIO_InitTypeDef gpio_init_struct = {};
        gpio_init_struct.Pin = GPIO_PIN_6;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        // SPI
        gpio_init_struct = {};
        gpio_init_struct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_5;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        gpio_init_struct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &gpio_init_struct);

        // Endstop Input
        gpio_init_struct = {};
        gpio_init_struct.Pin = GPIO_PIN_4;
        gpio_init_struct.Mode = GPIO_MODE_INPUT;
        gpio_init_struct.Pull = GPIO_PULLDOWN;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);

        // Load Configuration.
        std::int32_t debug_data = 0;

        read(TMC5130_DRVSTATUS, debug_data);
        read(TMC5130_GSTAT, debug_data);
        
        enable_drive();

        write(TMC5130_TPOWERDOWN, 0xA); // TPOWERDOWN=10: Delay before power down in stand still

        write(TMC5130_GCONF, 0x10);// Inverse_Direction=1; EN_PWM_MODE=1 enables stealthChop (with default PWM_CONF)
        write(TMC5130_COOLCONF, 0x7E0200);//write(TMC5130_COOLCONF, 0x7E0200); // Current threshold
        write(TMC5130_TCOOLTHRS, 5000); // Threshold speed for stallguard
        write(TMC5130_SWMODE, 0x000); // Disable stallguard
        
        //write(TMC5130_TPWMTHRS, 0x1F4); // TPWM_THRS=500 yields a switching velocity about 35000 = ca. 30RPM
        //write(TMC5130_PWMCONF, 0x401C8);// PWM_CONF: AUTO=1, 2/1024 Fclk, Switch amplitude limit=200, Grad=1
        
        write(TMC5130_XTARGET, 0x0);
        write(TMC5130_XACTUAL, 0x0); // Home
        write(TMC5130_VACTUAL, 0x0);
        write(TMC5130_A1, 1000); // A1 = 250 First acceleration
        write(TMC5130_V1, 10000); // V1 = 50 000 Acceleration threshold velocity V1
        write(TMC5130_AMAX, 500); // AMAX = 500 Acceleration above V1
        write(TMC5130_VMAX, 200000); // VMAX = 200 000
        write(TMC5130_DMAX, 700); // DMAX = 700 Deceleration above V1
        write(TMC5130_D1, 1400); // D1 = 1400 Deceleration below V1
        write(TMC5130_VSTOP, 20); // VSTOP = 10 Stop velocity (Near to zero)
        write(TMC5130_RAMPMODE, 0x2); // RAMPMODE = 0 (Target position move)
        

        find_home();
    }

    void move_absolute(float position_mm){
        
        std::int32_t xtarget = static_cast<std::int32_t>(position_mm * stepps_per_mm);

        write(TMC5130_XTARGET, xtarget);
    }
    bool position_reached(void){

        std::int32_t debug_data;
        std::uint8_t status = read(TMC5130_DRVSTATUS, debug_data);
        HAL_Delay(50);
         // Dummy read
        return status & 0x80; // Check status flags
    }

    std::uint8_t read(std::uint8_t address, std::int32_t &data){

        std::array<std::uint8_t, datagram_size> rx = {};
        std::array<std::uint8_t, datagram_size> tx = {address, 0, 0, 0, 0};
        

        HAL_SPI_TransmitReceive(&spi_handle, tx.data(), rx.data(), datagram_size, spi_timeout);
        __HAL_SPI_DISABLE(&spi_handle);
        data = (rx[1] << 24) + (rx[2] << 16) + (rx[3] << 8) + rx[4];

        return rx[0];
    }

    std::uint8_t write(std::uint8_t address, std::int32_t data){

        std::array<std::uint8_t, datagram_size> rx = {};
        std::array<std::uint8_t, datagram_size> tx = {address | 0x80, (0xFF000000 & data) >> 24, (0xFF0000 & data) >> 16, (0xFF00 & data) >> 8, 0xFF & data};

        HAL_SPI_TransmitReceive(&spi_handle, tx.data(), rx.data(), datagram_size, spi_timeout);

        __HAL_SPI_DISABLE(&spi_handle); // Raise Chip Select.

        return rx[0];
    }

    std::uint8_t write_read(std::uint8_t address, std::int32_t tx_data, std::int32_t &rx_data){

        std::array<std::uint8_t, datagram_size> rx = {};
        std::array<std::uint8_t, datagram_size> tx = {address | 0x80, 
                                                      (0xFF000000 & tx_data) >> 24, 
                                                      (0xFF0000 & tx_data) >> 16, 
                                                      (0xFF00 & tx_data) >> 8, 
                                                      0xFF & tx_data};

        HAL_SPI_TransmitReceive(&spi_handle, tx.data(), rx.data(), datagram_size, spi_timeout);

        __HAL_SPI_DISABLE(&spi_handle); // Raise Chip Select.

        rx_data = (rx[1] << 24) + (rx[2] << 16) + (rx[3] << 8) + rx[4];

        return rx[0];
    }

}