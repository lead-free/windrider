# This is required with modern version of CMake.
cmake_minimum_required(VERSION 3.10)

# Include yaccs main CMake file.
# This will automatically look and load the user-config file.
# This has to be done before calling the project() command!
include(cmake/yaccs/yaccs.cmake)

# Define the project.
project(windrider ASM C CXX)
add_executable(${PROJECT_NAME})

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/sources/STM32L412KBTx_FLASH.ld)
set_target_properties(${PROJECT_NAME} 
                        PROPERTIES 
                        SUFFIX ".elf"
                        LINK_OPTIONS "-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_NAME}.map,-T${LINKER_SCRIPT}")

target_compile_definitions(${PROJECT_NAME} PUBLIC -DSTM32L412xx -DHAL_TIM_MODULE_ENABLED -DHAL_ADC_MODULE_ENABLED -DRCC_CCIPR_ADCSEL -DHAL_SPI_MODULE_ENABLED)
 
# Include yaccs main CMake file.
# This will automatically look and load the user-config file.
# This has to be done before calling the project() command!
include(cmake/yaccs/yaccs.cmake)

# This is optional but useful to get a sense of what is happening.
# This command will print various information about the build configuration.
yaccs_system_info()

# This is also optional but it helps by neatly organizing the build tree.
yaccs_init_build_tree()
# Add stuff from the sources directory.
add_subdirectory(sources/)
add_subdirectory(libraries/)

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}\nBuilding ${BIN_FILE}")
