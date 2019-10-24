/**
 * Water level monitoring
 * JSN SR04t2 module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_jsn-sr04t2.h
 */

#include "../Inc/embd_sys_jsn-sr04t2.h"


uint16_t JSN_get_distance() {

    uint16_t distance = 0;
    uint8_t check_sum = 0;
    uint8_t c;
    Vector_t *vector = init_vector();

    UART_flush_Rx(UART_JSN);
    UART_send_message(UART_JSN, JSN_QUESTION, 1);

    if (UART_read_message(UART_JSN, vector, 4, 200) == EMBD_FAILURE) {
        deinit_vector_(vector);
        return 0;
    }

    c = (uint8_t)get_char(vector);
    if (c != JSN_HEADER) {
        deinit_vector_(vector);
        return 0;
    }

    check_sum += c;

    c = (uint8_t)get_char(vector);
    check_sum += c;
    distance += c * 256;

    c = (uint8_t)get_char(vector);
    check_sum += c;
    distance += c;

    c = (uint8_t)get_char(vector);

    deinit_vector_(vector);

    if (distance < JSN_SHORTEST || distance > JSN_LONGEST) {
        return 0;
    }

    if (c == check_sum) {
        return distance;
    }
    return 0;
}