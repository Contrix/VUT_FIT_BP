/**
 * Water level monitoring
 * MCP9808 module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_mcp9808.c
 */

#include "../Inc/embd_sys_mcp9808.h"


uint8_t MCP9808_init(I2C_HandleTypeDef *i2c) {

    uint8_t ptr = MCP9808_TEMPERATURE;
    reverse = false_e;

    if (HAL_I2C_Master_Transmit(i2c, MCP9808_DEVICE_ADDRESS, &ptr, 1, MCP9808_DELAY) != HAL_OK) {
        return EMBD_FAILURE;
    }

    return EMBD_SUCCESS;
}


uint16_t MCP9808_get_temperature(I2C_HandleTypeDef *i2c) {

    uint8_t buffer[2] = {0, };
    uint16_t temperature;

    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(i2c, MCP9808_DEVICE_ADDRESS, buffer, 2, MCP9808_DELAY);

    if(status != HAL_OK) {
        reverse = true_e;
        return 0;
    }

    /// fix MCP9808 error
    if (reverse) {
        uint8_t tmp = buffer[1];
        buffer[1] = buffer[0];
        buffer[0] = tmp;
    }

    buffer[MCP9808_MSB] &= MCP9808_FLAG_CLEAR;

    /// Temperature under zero
    if ((buffer[MCP9808_MSB] & MCP9808_FLAG_UNDER_ZERO) == MCP9808_FLAG_UNDER_ZERO) {
        buffer[MCP9808_MSB] &= MCP9808_CLEAR_SIGN;

        temperature = (uint16_t)256 * MCP9808_SCALE;
        temperature -= buffer[MCP9808_MSB] * MCP9808_SCALE * (uint8_t)16;
        temperature -= buffer[MCP9808_LSB] * MCP9808_SCALE / (uint8_t)16;

        return temperature;
    }

    temperature = buffer[MCP9808_MSB] * MCP9808_SCALE * (uint8_t)16;
    temperature += buffer[MCP9808_LSB] * MCP9808_SCALE / (uint8_t)16;

    return temperature;
}
