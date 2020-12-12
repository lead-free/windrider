#include "HardwareDriver.h"


namespace HardwareDriver{

    // We'll use this string to save the status of the command execution.
    std::string status = {};

    void error_handler(void){
        
        // Toggle very slowly to showcase the error.
        HeartBeat::set_toggle_interval(5000);

        while(true){
            HAL_Delay(1000);
        }
    }

    //! method get_status
    /**
    * @return status from execution of the previous command..
    */
    const std::string& get_status(void){
        return status;
    }

    void gpio_init(void){

        GPIO_InitTypeDef GPIO_InitStruct = {};

        /* GPIO Ports Clock Enable */
        __HAL_RCC_GPIOH_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();
        
    }
    void initialize(void){

        sys_clock_config();
        gpio_init();
        HeartBeat::initialize();
        SuctionControl::initialize();
        Solenoid::initialize();
        LedDriver::initialize();
        TMC5130::initialize();
    }

    void led(const std::vector<std::string> &args) {

        // Sainity checks.
        if(args.size() > 2){
 
            status = invalid_argument;
            return;
        }

        if(args.front() == "off"){

            LedDriver::off();
            status = response_ok;
            return;
        }
        if(args.front()== "on"){
            
            LedDriver::on();

            status = response_ok;
            return;
        }
        
        //! Check if all of the characters in the args are digits
        /**
         * Ideally std::stoul() would throw an exception,
         * though we are using the nano version of C++ stl,
         * which does not support exceptions, a thrown exception will
         * end up in the terminate() handler 
         */ 
        const auto first_arg_is_digit = std::all_of(args.front().begin(), 
                                                    args.front().end(), 
                                                    static_cast<int(*)(int)>(std::isdigit));
        if(not first_arg_is_digit){
            
            status = invalid_argument;
            return;
        }

        // Convert argument strings to integers.
        const auto channel = std::stoul(args.front());

        if(not (0 <= channel and  channel <= 4)){

            status = arguments_out_of_range;
            return;
        }

        if(args.at(1) == "off"){

            LedDriver::off(channel);

            status = response_ok;
            return;
        }
        if(args.at(1) == "on"){
            
            LedDriver::on(channel);

            status = response_ok;
            return;
        }

        const auto second_arg_is_digit = std::all_of(args.at(1).begin(), 
                                              args.at(1).end(), 
                                              static_cast<int(*)(int)>(std::isdigit));

        if(not second_arg_is_digit){
            
            status = invalid_argument;
            return;
        }

        const auto current_ma = std::stoul(args.at(1));

        if(not (0 <= current_ma <= 1500)){

            status = arguments_out_of_range;
            return;
        }

        LedDriver::set_current(channel, current_ma);

        status = response_ok;
    }
    void solenoid(const std::vector<std::string> &args) {

        // Sainity checks.
        if(args.size() == 1 or args.size() > 3){
 
            status = invalid_argument;
            return;
        }

        auto first_arg_is_digit = std::all_of(args.front().begin(), 
                                        args.front().end(), 
                                        static_cast<int(*)(int)>(std::isdigit));

        if(not first_arg_is_digit) {

            status = invalid_argument;
            return;
        }
 
        const auto channel = std::stoul(args.front());

        if(channel > 1){
            
            status = arguments_out_of_range;
            return;
        }

        if(args.at(1) == "off"){
            
            Solenoid::off(channel);
            status = response_ok;
            return;
        }

        if(args.at(1) == "on"){

            Solenoid::on(channel);
            status = response_ok;
            return;
        }

        if(args.size() == 2){
            
            status = invalid_argument;
            return;
        }

        auto second_arg_is_digit = std::all_of(args.at(1).begin(), 
                                              args.at(1).end(), 
                                              static_cast<int(*)(int)>(std::isdigit));

        auto third_arg_is_digit = std::all_of(args.at(2).begin(), 
                                              args.at(2).end(), 
                                              static_cast<int(*)(int)>(std::isdigit));

        if(not second_arg_is_digit or not third_arg_is_digit){
                
            status = invalid_argument;
            return;
        }

        auto off_time_ms = std::stoul(args.at(1));
        auto on_time_ms = std::stoul(args.at(2));

        Solenoid::configure_timings(channel, off_time_ms, on_time_ms);

        status = response_ok;
    }

    void suction_power(const std::vector<std::string> &args){
        
        if(args.size() != 1){

            status = invalid_argument;
            return;
        }

        // on/off    
        if(args.front() == "on"){

            SuctionControl::enable_power(true);
            status = response_ok;
            return;
        }
        if(args.front() == "off"){
            
            SuctionControl::enable_power(false);
            status = response_ok;
            return;
        }
        
        auto first_arg_is_digit = std::all_of(args.front().begin(), 
                                        args.front().end(), 
                                        static_cast<int(*)(int)>(std::isdigit));
        
        if(not first_arg_is_digit) {

            status = invalid_argument;
            return;
        }

        auto power = std::stoul(args.front());
        
        if(not (0 <= power <= 100)){

            status = arguments_out_of_range;
            return;
        }

        SuctionControl::set_power(power);
        
        status = response_ok;

    }
    void stepper(const std::vector<std::string> &args){

        // Sainity checks.
        if(args.size() != 1){
 
            status = invalid_argument;
            return;
        }
        if(args.front() == "off"){
            TMC5130::disable_drive();

            status = response_ok;
            return;

        }
        if(args.front() == "on"){

            TMC5130::enable_drive();

            status = response_ok;
            return;
        }
        if(args.front() == "home"){

            TMC5130::find_home();

            status = response_ok;
            return;
        }

        const auto first_arg_is_digit = std::all_of(args.front().begin(), 
                                        args.front().end(), 
                                        static_cast<int(*)(int)>(std::isdigit));
        if(not first_arg_is_digit) {

            status = invalid_argument;
            return;
        }

        const auto pose = std::stoul(args.front());

        if(not (0 <= pose <= 1500)){

            status = arguments_out_of_range;
            return;
        }

        TMC5130::move_absolute(pose);

        HAL_Delay(1000);
        while(not TMC5130::position_reached()){}
        
        status = response_ok;
    }
    void sys_clock_config(void){

        RCC_OscInitTypeDef RCC_OscInitStruct = {0};
        RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
        RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

        /** Configure the main internal regulator output voltage
         */
        if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
            error_handler();
        /** Initializes the RCC Oscillators according to the specified parameters
         * in the RCC_OscInitTypeDef structure.
         */
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLM = 2;
        RCC_OscInitStruct.PLL.PLLN = 36;
        RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV6;
        RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV4;
        if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
            error_handler();
        /** Initializes the CPU, AHB and APB buses clocks
         */
        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

        if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
            error_handler();

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB|RCC_PERIPHCLK_ADC;
        PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
            error_handler();

    }

}