# This is required with modern version of CMake.
cmake_minimum_required(VERSION 3.10)

# Include yaccs main CMake file.
# This will automatically look and load the user-config file.
# This has to be done before calling the project() command!
include(cmake/yaccs/yaccs.cmake)

# Define the project.
project(windrider ASM C CXX)

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/sources/STM32L412KBTx_FLASH.ld)

add_executable(${PROJECT_NAME}.elf)

# Add stuff from the sources directory.
add_subdirectory(sources/)
add_subdirectory(libraries/)

add_definitions(-DSTM32L412xx -DHAL_TIM_MODULE_ENABLED -DHAL_ADC_MODULE_ENABLED -DRCC_CCIPR_ADCSEL -DHAL_SPI_MODULE_ENABLED)


set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}\nBuilding ${BIN_FILE}")

target_link_options(${PROJECT_NAME}.elf PUBLIC "-Wl,-gc-sections,--print-memory-usage,-T${LINKER_SCRIPT}")
