/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"
#include "HardwareDriver.h"
#include "CommandParser.h"
#include "UsbComm.h"

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  HardwareDriver::initialize();

  MX_USB_DEVICE_Init();


  while (1)
  {
    // If there are commands in the usb queue -- execute them.  
    if(UsbComm::usb_queue.get_queue_size()){

      const auto response = CommandParser::execute(UsbComm::usb_queue.get_next_cmd());

        // Respond to the command
      UsbComm::usb_send(response);

        // Remove pending command from the queue
      UsbComm::usb_queue.free_pending_cmd();
    }
  }
}
// The following functions are needed by stm usb files.
void Error_Handler(void){

  HardwareDriver::error_handler();
}
void SystemClock_Config(void){
  HardwareDriver::sys_clock_config();
}
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
