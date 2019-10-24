/**
 * Water level monitoring
 * UART module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_uart.h
 */

#include "../Inc/embd_sys_uart.h"


void UART_init() {

    uarts[UART1_e].huart = &huart1;
    uarts[UART2_e].huart = &huart2;
    uarts[UART3_e].huart = &huart3;
    uarts[UART4_e].huart = &huart4;
    uarts[UART5_e].huart = &huart5;

    uarts[UART1_e].UART_id = UART1_ID;
    uarts[UART2_e].UART_id = UART2_ID;
    uarts[UART3_e].UART_id = UART3_ID;
    uarts[UART4_e].UART_id = UART4_ID;
    uarts[UART5_e].UART_id = UART5_ID;

    uart_Rx_vector = UART1_ID | UART2_ID | UART3_ID | UART4_ID | UART5_ID;
    uart_Tx_vector = UART1_ID | UART2_ID | UART3_ID | UART4_ID | UART5_ID;

    for (int i = 0; i < UART_COUNT; ++i) {
        if (uart_Tx_vector & uarts[i].UART_id) {
            uarts[i].Tx_vector = init_vector();
        }

        if (uart_Rx_vector & uarts[i].UART_id) {
            uarts[i].Rx_vector = init_vector();
            HAL_UART_Receive_IT(uarts[i].huart, (uint8_t *) &(uarts[i].Rx_char), 1);
        }
    }
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {

    for (uint8_t i = 0; i < UART_COUNT; ++i) {
        if (huart->Instance == uarts[i].huart->Instance) {
            if (get_message_len(uarts[i].Tx_vector)) {
                HAL_UART_Transmit_IT(uarts[i].huart, (uint8_t *)get_char_ptr(uarts[i].Tx_vector), 1);
            } else {
                uarts[i].transmitting = false_e;
            }
        }
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

    /// Lidar
    if (huart->Instance == USART1) {
        if (uarts[UART_LIDAR].Rx_vector->message_len < EMBD_FRAME_BUFFER_SIZE) {
            put_char(uarts[UART_LIDAR].Rx_vector, uarts[UART_LIDAR].Rx_char);
        }
        HAL_UART_Receive_IT(uarts[UART_LIDAR].huart, (uint8_t *)&(uarts[UART_LIDAR].Rx_char), 1);
    }

    // TODO
    /// ESP-01
    if (huart->Instance == USART2) {
        put_char(uarts[UART2_e].Rx_vector, uarts[UART2_e].Rx_char);
        HAL_UART_Receive_IT(&huart2, (uint8_t *) &(uarts[UART2_e].Rx_char), 1);
    }

    /// Debug
    if (huart->Instance == USART3) {
        if (embd_system_status == EMBD_CONFIGURATION_e) {
            if (uarts[UART3_e].Rx_char == DBG_ACK) {
                embd_system_status = EMBD_RUN_e;
                LED2_reset();
                UART_send_message(UART_DBG, "\r\nEnd of settings.\r\n", 20);
                set_configuration(uarts[UART3_e].Rx_vector, true_e);
            } else {
                put_char(uarts[UART3_e].Rx_vector, uarts[UART3_e].Rx_char);
            }
        } else if (embd_system_status == EMBD_RUN_e) {
            if (uarts[UART3_e].Rx_char == DBG_BEL) {
                embd_system_status = EMBD_CONFIGURATION_e;
                LED2_set();
                UART_send_message(UART_DBG, "\r\nWrite your settings.\r\n", 24);
                UART_flush_Rx(UART3_e);
            }

            if (uarts[UART3_e].Rx_char == DBG_ACK) {
                print_configuration();
            }
        } else if (embd_system_status == EMBD_INIT_e) {
            put_char(uarts[UART3_e].Rx_vector, uarts[UART3_e].Rx_char);
        }

        HAL_UART_Receive_IT(&huart3, (uint8_t *) &(uarts[UART3_e].Rx_char), 1);
    }

    /// ESP-12E
    if (huart->Instance == USART4) {
        UART_send_message(UART_DBG, &uarts[UART4_e].Rx_char, 1);
        HAL_UART_Receive_IT(&huart4, (uint8_t *) &(uarts[UART4_e].Rx_char), 1);
    }

    /// JSN
    if (huart->Instance == USART5) {
        put_char(uarts[UART_JSN].Rx_vector, uarts[UART_JSN].Rx_char);
        HAL_UART_Receive_IT(uarts[UART_JSN].huart, (uint8_t *) &(uarts[UART_JSN].Rx_char), 1);
    }
}


void UART_send_message(uint8_t uart_id, char *message, uint16_t len) {
    put_string(uarts[uart_id].Tx_vector, message, len);

    if (!uarts[uart_id].transmitting) {
        uarts[uart_id].transmitting = true_e;
        HAL_UART_Transmit_IT(uarts[uart_id].huart, (uint8_t *)get_char_ptr(uarts[uart_id].Tx_vector), 1);
    }
}


uint8_t UART_read_message(uint8_t uart_id, Vector_t *message, uint16_t len, uint16_t timeout) {

    uint16_t time = 0;
    while (get_message_len(uarts[uart_id].Rx_vector) < len) {

        if (timeout && time > timeout) {
            return EMBD_FAILURE;
        }

        time += 20;
        HAL_Delay(20);
    }
    copy_string(uarts[uart_id].Rx_vector, message, len);

    return EMBD_SUCCESS;
}


uint8_t UART_read_message_until_char(uint8_t uart_id, Vector_t *buffer, char stop_char) {
    return get_message_until_char(uarts[uart_id].Rx_vector, buffer, stop_char);
}


uint8_t UART_read_line(uint8_t uart_id, char *message, uint16_t max_len, uint16_t timeout) {

    bzero(message, max_len);
    uint16_t time;

    for (uint16_t i = 0; i < max_len; ++i) {
        time = 0;
        while (uarts[uart_id].Rx_vector->message_len == 0) {

            if (timeout && time > timeout) {
                return EMBD_FAILURE;
            }

            time += 200;
            HAL_Delay(200);
        }
        message[i] = get_char(uarts[uart_id].Rx_vector);
        if (message[i] == '\n') {
            return EMBD_SUCCESS;
        }
    }
    return EMBD_FAILURE;
}


uint8_t UART_compare_char(uint8_t uart_id, char c, uint16_t timeout) {

    uint16_t time = 0;

    while (uarts[uart_id].Rx_vector->message_len == 0) {
        if (timeout && time > timeout) {
            return false_e;
        }

        time += 200;
        HAL_Delay(200);
    }

    if (show_first_char(uarts[uart_id].Rx_vector) == c) {
        return true_e;
    }

    return false_e;
}


void UART_flush_Rx(uint8_t uart_id) {
    vector_flush(uarts[uart_id].Rx_vector);
}


void skip_blank_lines(uint8_t uart_id) {
    while (show_first_char(uarts[uart_id].Rx_vector) == '\r' || show_first_char(uarts[uart_id].Rx_vector) == '\n') {
        get_char(uarts[uart_id].Rx_vector);
    }
}