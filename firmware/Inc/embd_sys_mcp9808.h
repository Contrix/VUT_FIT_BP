/**
 * Water level monitoring
 * MCP9808 module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_mcp9808.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_MCP9808_H
#define WATER_MONITORING_EMBD_SYS_MCP9808_H

#include "../Inc/embd_sys_inc.h"
#include "../Inc/embd_sys_i2c.h"


#define MCP9808_A0 (uint8_t)0
#define MCP9808_A1 (uint8_t)0
#define MCP9808_A2 (uint8_t)0

#define MCP9808_DEVICE_ADDRESS (uint8_t)(0x18 | MCP9808_A0 | MCP9808_A1 | MCP9808_A2)<<1
#define MCP9808_FLAG_CLEAR (uint8_t)0x1f
#define MCP9808_FLAG_UNDER_ZERO (uint8_t)0x10
#define MCP9808_CLEAR_SIGN (uint8_t)0x0f
#define MCP9808_TEMPERATURE (uint8_t)0x05

#define MCP9808_MSB (uint8_t)0
#define MCP9808_LSB (uint8_t)1
#define MCP9808_DELAY (uint8_t)100
#define MCP9808_SCALE (uint8_t)10

#define SONIC_SPEED_20 343.477
#define SONIC_AIR_ONSTANT 402.402

uint8_t reverse;

/**
 * Initializes MCP9808
 * @param i2c I2C interface
 * @return 0 for success
 */
uint8_t MCP9808_init(I2C_HandleTypeDef *i2c);


/**
 * Reads the temperature from MCP9808
 * @param i2c I2C interface
 * @return temperature in Celsius multiplied by ten
 */
uint16_t MCP9808_get_temperature(I2C_HandleTypeDef *i2c);

#endif //WATER_MONITORING_EMBD_SYS_MCP9808_H
