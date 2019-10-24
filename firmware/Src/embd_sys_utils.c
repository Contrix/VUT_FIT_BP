/**
 * Water level monitoring
 * Utils module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_utils.c
 */

#include "../Inc/embd_sys_utils.h"

uint8_t str_to_uint16(char *str, uint16_t *number) {

    *number = 0;

    for (uint8_t i = 0; str[i]; ++i) {
        if (str[i] < '0' || str[i] > '9' || i > 5) {
            return EMBD_FAILURE;
        }
        *number *= 10;
        *number += str[i] - '0';
    }

    return EMBD_SUCCESS;
}