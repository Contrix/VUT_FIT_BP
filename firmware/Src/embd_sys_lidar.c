/**
 * Water level monitoring
 * LIDAR module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_lidar.c
 */

#include <stdlib.h>
#include "../Inc/embd_sys_lidar.h"


uint16_t LIDAR_get_distance() {

    Vector_t *vector = init_vector();
    uint8_t header = false_e;
    uint8_t sum = 0;
    uint8_t index = 0;
    uint16_t distance = 0;
    uint8_t c;
    uint8_t window[10];

    UART_flush_Rx(UART_LIDAR);
    if (UART_read_message(UART_LIDAR, vector, LIDAR_WINDOW_LEN * 4, 200) == EMBD_FAILURE) {
        deinit_vector_(vector);
        return 0;
    }

    while(vector->message_len) {
        c = (uint8_t)get_char(vector);
        if (c == LIDAR_HEADER && !header) {
            if (index == 1) {
                header = true_e;
            } else {
                index = 0;
                window[index++] = c;
            }
        }

        if (header) {
            window[index++] = c;
        }

        if (index == LIDAR_WINDOW_LEN && header) {
            header = false_e;
            sum = 0;
            for (uint8_t j = 0; j < LIDAR_WINDOW_LEN; ++j) {
                if (j < LIDAR_WINDOW_LEN - 1) {
                    sum += window[j];
                }
            }
            if (sum != window[8]) {
                continue;
            }

            distance += window[2];
            distance += window[3] * 256;

            if (distance > LIDAR_LONGEST || distance < LIDAR_SHORTEST) {
                continue;
            }

            deinit_vector_(vector);
            return distance;
        }
    }

    deinit_vector_(vector);
    return 0;
}
