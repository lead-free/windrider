target_include_directories(${PROJECT_NAME} PUBLIC inc/)

file(GLOB SOURCES src/*.*)
target_sources(${PROJECT_NAME} PUBLIC ${SOURCES})

set(LINKER_SCRIPT STM32L412KBTx_FLASH.ld)
