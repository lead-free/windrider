target_include_directories(${PROJECT_NAME}.elf PUBLIC inc/)

file(GLOB SOURCES src/*.*)
target_sources(${PROJECT_NAME}.elf PUBLIC ${SOURCES})

set(LINKER_SCRIPT STM32L412KBTx_FLASH.ld)
