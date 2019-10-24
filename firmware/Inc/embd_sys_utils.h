/**
 * Water level monitoring
 * Utils module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_utils.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_UTILS_H
#define WATER_MONITORING_EMBD_SYS_UTILS_H

#include <stdint-gcc.h>
#include "../Inc/embd_sys_inc.h"

#define STR_TO_UINT16_BUFFER_SIZE 8

/**
 * Converts string to integer
 * @param str String to be converted
 * @param number Pointer to number
 * @return 0 ofr success
 */
uint8_t str_to_uint16(char *str, uint16_t *number);

#endif //WATER_MONITORING_EMBD_SYS_UTILS_H
