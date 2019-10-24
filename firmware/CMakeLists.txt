cmake_minimum_required(VERSION 3.12)
project(water_monitoring)

set(CMAKE_C_STANDARD 99)

file(
        GLOB_RECURSE SRC
        LIST_DIRECTORIES false
        RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
        "*.c" "*.h"
)
add_executable(main ${SRC} Inc/embd_sys_uart.h Src/embd_sys_uart.c Inc/embd_sys_inc.h Inc/embd_sys_vector.h Src/embd_sys_vector.c Inc/embd_sys_esp8266.h Src/embd_sys_esp8266.c Inc/embd_sys_utils.h Src/embd_sys_utils.c Inc/embd_sys_lidar.h Src/embd_sys_lidar.c Inc/embd_sys_jsn-sr04t2.h Src/embd_sys_jsn-sr04t2.c Inc/embd_sys_gpio.h Src/embd_sys_gpio.c Inc/embd_sys_configuration.h Src/embd_sys_configuration.c Inc/embd_sys_mcp9808.h Src/embd_sys_mcp9808.c Inc/embd_sys_i2c.h Src/embd_sys_i2c.c)

set(SOURCE_FILES Src/main.c Inc/main.h Inc/embd_sys_uart.h Src/embd_sys_uart.c Inc/embd_sys_inc.h Inc/embd_sys_vector.h Inc/embd_sys_esp8266.h Inc/embd_sys_utils.h Src/embd_sys_utils.c Inc/embd_sys_lidar.h Src/embd_sys_lidar.c Inc/embd_sys_jsn-sr04t2.h Src/embd_sys_jsn-sr04t2.c Inc/embd_sys_gpio.h Src/embd_sys_gpio.c Inc/embd_sys_configuration.h Src/embd_sys_configuration.c Inc/embd_sys_mcp9808.h Src/embd_sys_mcp9808.c Inc/embd_sys_i2c.h Src/embd_sys_i2c.c)
add_executable(test ${SOURCE_FILES})