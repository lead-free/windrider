# List header locations.
target_include_directories(${PROJECT_NAME}.elf PUBLIC
                            CMSIS_5/CMSIS/Core/Include
                            cmsis_device_l4/Include
                            stm32l4xx_hal_driver/Inc
                            stm32_mw_usb_device/Core/Inc
                            stm32_mw_usb_device/Class/CDC/Inc)

target_sources(${PROJECT_NAME}.elf PUBLIC 
                ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_device_l4/Source/Templates/system_stm32l4xx.c
                ${CMAKE_CURRENT_SOURCE_DIR}/cmsis_device_l4/Source/Templates/gcc/startup_stm32l412xx.s)

# Collect HAL Driver source files.
#file(GLOB SOURCES stm32l4xx_hal_driver/Src/stm32l4xx_hal_*)
target_sources(${PROJECT_NAME}.elf PUBLIC 
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_cortex.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_gpio.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pcd.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pcd_ex.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pwr.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_pwr_ex.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_rcc.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_hal_rcc_ex.c

                ${CMAKE_CURRENT_SOURCE_DIR}/stm32l4xx_hal_driver/Src/stm32l4xx_ll_usb.c)

# Collect USB CDC Driver.
target_sources(${PROJECT_NAME}.elf PUBLIC 
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32_mw_usb_device/Core/Src/usbd_ioreq.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32_mw_usb_device/Core/Src/usbd_ctlreq.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32_mw_usb_device/Core/Src/usbd_core.c
                ${CMAKE_CURRENT_SOURCE_DIR}/stm32_mw_usb_device/Class/CDC/Src/usbd_cdc.c)