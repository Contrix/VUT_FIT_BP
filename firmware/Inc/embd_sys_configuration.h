/**
 * Water level monitoring
 * Configuration module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_configuration.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_CONFIGURATION_H
#define WATER_MONITORING_EMBD_SYS_CONFIGURATION_H

#include "../Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f0xx.h"
#include "../Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030xc.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_flash.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_flash_ex.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy/stm32_hal_legacy.h"

#include "../Inc/embd_sys_vector.h"
#include "../Inc/embd_sys_uart.h"

#include <strings.h>
#include <stdlib.h>
#include <stdint-gcc.h>

#define CONFIG_VALUE_LEN 130
#define CONFIG_ID_LEN 24
#define CONNFIG_COUNT 3
#define COMMNET '#'
#define EQ '='

#define CONFIG_CHANGE_ID "CHANGE_ID"
#define CONFIG_REFRESH "REFRESH"
#define CONFIG_SERVER "SERVER"
#define CONFIG_LOCATION "LOCATION"
#define CONFIG_PORT "PORT"
#define CONFIG_MINIMUM "MINIMUM"
#define CONFIG_MAXIMUM "MAXIMUM"
#define CONFIG_HEIGHT "HEIGHT"
#define CONFIG_PUMP_ACTIVE "PUMP_ACTIVE"
#define CONFIG_SSID "SSID"
#define CONFIG_PASSWORD "PASSWORD"
#define CONFIG_CONTROL_SENSORS "CONTROL_SENSORS"
#define CONFIG_AVERAGE "AVERAGE_COUNT"

#define CONFIG_BUFFER_SIZE 160
#define UINT64_MAX_LEN 21

#define CONFIG_LIDAR_ID 1
#define CONFIG_JSN_ID 2
#define CONFIG_FLOATS_ID 4
#define CONFIG_MCP_ID 8

#define CONFIG_FLASH_WORD_LEN 160
#define FLASH_PAGE_ADDRESS 0x803F000U // page 127

typedef enum { STOP_e, FILL_e, DRAIN_e } SSR_state;

typedef struct Configuration_t {
    uint32_t change_id;
    uint32_t refresh;
    uint32_t level_min;
    uint32_t level_max;
    uint32_t height;
    uint8_t pump_active;
    uint8_t control_sensors;
    uint8_t average;
    uint8_t server_address[CONFIG_VALUE_LEN];
    uint16_t server_port;
    uint8_t ssid[CONFIG_VALUE_LEN];
    uint8_t password[CONFIG_VALUE_LEN];
    uint8_t location[CONFIG_VALUE_LEN];
} Configuration_t;

union EMBD_configuration {
    Configuration_t config;
    uint32_t array[CONFIG_FLASH_WORD_LEN];
};

char config_buffer[CONFIG_BUFFER_SIZE];

extern union EMBD_configuration *embd_configuration;
extern void FLASH_PageErase(uint32_t PageAddress);

/**
 *  Initialization of configuration
 */
void init_configuration(union EMBD_configuration *configuration);

/**
 * Reads setting string
 * @param msg Query string
 * @param string Source string
 * @param max Maximal length
 */
void read_string(char *msg, uint8_t *string, uint8_t max);

/**
 * Reads setting number
 * @param msg Query string
 * @param min Minimal value
 * @param max Maximal value
 * @return The number
 */
uint32_t read_number(char *msg, uint32_t min, uint32_t max);

/**
 * Reads word from flash memory
 * @param address
 * @return
 */
uint32_t flash_read(uint32_t address);

/**
 * Reads configuration from flash memory
 * @param configuration Configuration unit
 */
void load_configuration(union EMBD_configuration *configuration);

/**
 * Stores configuration in to flash memory
 */
void store_configuration(union EMBD_configuration *configuration);

/**
 * Sets configuration from string in Vector
 * @param conf Source vector
 * @param force 1 for save by force
 */
void set_configuration(Vector_t *conf, uint8_t force);

/**
 * Sets one setting in configuration
 * @param configuration Destination configuration
 * @param word Key word of setting
 * @param value String to by set
 * @param number Number to by set
 * @return 0 for success
 */
uint8_t set_value(union EMBD_configuration *configuration, char *word, char *value, uint64_t number);

/**
 * Prints all settings
 */
void print_configuration();

/**
 * Skips line in Vector
 * @param conf Source vector
 */
void skip_line(Vector_t *conf);

/**
 * Compare two strings in case insensitivity mode
 * @param id First string
 * @param value Second string
 * @return 1 for same strings
 */
uint8_t string_compare(char* id, char* value);
#endif //WATER_MONITORING_EMBD_SYS_CONFIGURATION_H
