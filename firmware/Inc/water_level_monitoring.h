/**
 * Water level monitoring
 * Main module
 * @Author HanakJiri, xhanak33
 * @file water_level_monitoring.h
 */

#ifndef WATER_WATER_LEVEL_MONITORING_H
#define WATER_WATER_LEVEL_MONITORING_H

#include "../Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030xc.h"

#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dma.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_i2c.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_gpio.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_uart.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_tim.h"

#include <stdio.h>
#include <string.h>
#include <stdint-gcc.h>
#include <tgmath.h>

#include "../Inc/embd_sys_inc.h"
#include "../Inc/embd_sys_uart.h"
#include "../Inc/embd_sys_i2c.h"
#include "../Inc/embd_sys_esp8266.h"
#include "../Inc/embd_sys_lidar.h"
#include "../Inc/embd_sys_jsn-sr04t2.h"
#include "../Inc/embd_sys_gpio.h"
#include "../Inc/embd_sys_configuration.h"
#include "../Inc/embd_sys_mcp9808.h"

#define LIDAR_ID (uint8_t)0
#define JSN_ID (uint8_t)1
#define MCP_ID (uint8_t)2

#define EMBD_MAX_MEASURING (uint8_t)32
#define EMBD_SENSORS_COUNT (uint8_t)3

#define EMBD_JSN_OFFSET (uint8_t)40


Vector_t *response;
char buffer[256];
uint64_t main_counter;


uint16_t lidar_level;
uint16_t ultrasonic_level;
uint16_t temperature;
uint8_t floats_level;
uint8_t SSR_active;

uint8_t embd_system_status;
union EMBD_configuration *embd_configuration;

/**
 * System initialization
 */
void monitoring_init();

/**
 * Main system loop
 */
void monitoring_loop();

/**
 * Check active sensors for water level
 */
void check_level();

/**
 * Control SSR by water level
 */
void SSR_control();
#endif //WATER_WATER_LEVEL_MONITORING_H
