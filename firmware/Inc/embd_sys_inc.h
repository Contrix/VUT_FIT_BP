/**
 * Water level monitoring
 * Global include module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_inc.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_INC_H
#define WATER_MONITORING_EMBD_SYS_INC_H

#include <stdint-gcc.h>
#include <inttypes.h>

#define EMBD_FAILURE (uint8_t)1
#define EMBD_SUCCESS (uint8_t)0

#define EMBD_MSG_LEN (uint8_t)255
#define EMBD_FRAME_BUFFER_SIZE (uint8_t)64

#define UART_JSN (uint8_t)4
#define UART_LIDAR (uint8_t)0
#define UART_ESP8266 (uint8_t)3
#define UART_DBG (uint8_t)2


typedef enum { false_e, true_e } Bool;
char msg_buffer[EMBD_MSG_LEN];

#endif //WATER_MONITORING_EMBD_SYS_INC_H
