/**
 * Water level monitoring
 * UART module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_uart.h
 */

#ifndef WATER_MONITORING_SYSTEM_UART_H
#define WATER_MONITORING_SYSTEM_UART_H

#include "../Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030xc.h"

#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_dma.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_gpio.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_uart.h"

#include <stdio.h>
#include <string.h>
#include <stdint-gcc.h>

#include "../Inc/embd_sys_inc.h"
#include "../Inc/embd_sys_vector.h"
#include "../Inc/embd_sys_gpio.h"

#define UART_OPTIONS_LEN (uint8_t)3
#define UART_COUNT (uint8_t)5

#define UART1_ID (uint8_t)0x01
#define UART2_ID (uint8_t)0x02
#define UART3_ID (uint8_t)0x04
#define UART4_ID (uint8_t)0x08
#define UART5_ID (uint8_t)0x0f

#define DBG_BEL (uint8_t)0x07
#define DBG_ACK (uint8_t)0x06

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;

typedef struct UART_t {
    uint8_t UART_id;
    UART_HandleTypeDef *huart;
    char Rx_char;
    uint8_t options[UART_OPTIONS_LEN];
    Vector_t *Rx_vector;
    Vector_t *Tx_vector;
    uint8_t transmitting;
} UART_t;

typedef enum { UART1_e, UART2_e, UART3_e, UART4_e, UART5_e } UART_index;
typedef enum { EMBD_INIT_e, EMBD_CONFIGURATION_e, EMBD_RUN_e } System_status;

UART_t uarts[6];

uint8_t uart_Rx_vector;
uint8_t uart_Tx_vector;

extern uint8_t embd_system_status;
extern void set_configuration(Vector_t *conf, uint8_t force);
extern void print_configuration();

/**
 * Initialization of UART interfaces
 */
void UART_init();

/**
 * Sends message throw UART
 * @param uart_id UART id
 * @param message Text to be send
 * @param len Length of text
 */
void UART_send_message(uint8_t uart_id, char *message, uint16_t len);

/**
 * Reads text from UART to vector
 * @param uart_id UART id
 * @param message Destination vector
 * @param len Length of text to be read
 * @param timeout Timeout for read, 0 nor none
 * @return 0 for success
 */
uint8_t UART_read_message(uint8_t uart_id, Vector_t *message, uint16_t len, uint16_t timeout);

/**
 * Reads text from UART to vector after stop char
 * @param uart_id UART id
 * @param buffer Destination vector
 * @param stop_char Stop char
 * @return 0 for success
 */
uint8_t UART_read_message_until_char(uint8_t uart_id, Vector_t *buffer, char stop_char);

/**
 * Reads one line from UART
 * @param uart_id UART id
 * @param message Destination pointer
 * @param max_len Maximal length of text to be read
 * @return 0 for success
 */
uint8_t UART_read_line(uint8_t uart_id, char *message, uint16_t max_len, uint16_t timeout);

/**
 * Compares first char in Rx vector with char
 * @param uart_id UART id
 * @param c Char to comapare
 * @return 1 for same
 */
uint8_t UART_compare_char(uint8_t uart_id, char c, uint16_t timeout);

/**
 * Flush Rx vector
 * @param uart_id UART id
 */
void UART_flush_Rx(uint8_t uart_id);

/**
 * Skips blank lines
 * @param uart_id UART id
 */
void skip_blank_lines(uint8_t uart_id);

#endif //WATER_MONITORING_SYSTEM_UART_H
