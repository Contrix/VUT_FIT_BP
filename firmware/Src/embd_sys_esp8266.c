/**
 * Water level monitoring
 * ESP8266 module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_esp8266.c
 */

#include "../Inc/embd_sys_esp8266.h"


uint8_t ESP_init(char *msg, char *ssid, char *password) {

    ESP_restart();
    if (ESP_test() ||
        ESP_set_feedback(false_e) ||
        //ESP_set_autoconnect(false_e) ||
        ESP_set_mode(ESP_CWMODE_AP) ||
        ESP_set_multiple_connections(false_e)) {

        sprintf(msg, "ESP init failed.\r\n");
        return EMBD_FAILURE;
    }

    if (ESP_connect_to_wifi(ssid, password)) {
        sprintf(msg, "Connect to wifi failed.\r\n");
        return EMBD_FAILURE;
    }

    if (ESP_get_IP(esp_buffer)) {
        sprintf(msg, "Can not get IP.\r\n");
        return EMBD_FAILURE;
    }

    return EMBD_SUCCESS;
}


void ESP_restart() {

    sprintf(esp_cmd_buffer, "AT+RST");
    ESP_send_command();

    uint16_t limit = 0;
    do {
        UART_read_line(UART_ESP8266, esp_buffer, ESP_BUFFER_SIZE, ESP_TIMEOUT);
    } while (strcmp(esp_buffer, "ready\r\n") != 0 && limit++ < ESP_LINES_LIMIT);
}


void ESP_send_command() {

    uint16_t len = (uint16_t) strlen(esp_cmd_buffer);

    esp_cmd_buffer[len++] = '\r';
    esp_cmd_buffer[len++] = '\n';

    UART_flush_Rx(UART_ESP8266);
    UART_send_message(UART_ESP8266, esp_cmd_buffer, len);
    bzero(esp_cmd_buffer, ESP_CMD_BUFFER_SIZE);
}


void ESP_send_data(char* data, uint16_t size) {

    UART_send_message(UART_ESP8266, data, size);
}


uint8_t ESP_check_response() {

    uint16_t limit = 0;
    do {
        UART_read_line(UART_ESP8266, esp_buffer, ESP_BUFFER_SIZE, 500);

        if (strcmp(esp_buffer, "FAIL\r\n") == 0 ||
                strcmp(esp_buffer, "ERROR\r\n") == 0) {

            return EMBD_FAILURE;
        }

        if (strcmp(esp_buffer, "SEND OK\r\n") == 0 ||
                strcmp(esp_buffer, "CLOSED\r\n") == 0) {

            return EMBD_SUCCESS;
        }
    } while (strcmp(esp_buffer, "OK\r\n") != 0  && limit++ < ESP_LINES_LIMIT);

    return EMBD_SUCCESS;
}


uint8_t ESP_test() {

    sprintf(esp_cmd_buffer, "AT");
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_sleep(uint32_t time) {

    sprintf(esp_cmd_buffer, "AT+GSLP=%"PRIu32, time);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_ping(char* ip_addres) {

    sprintf(esp_cmd_buffer, "AT+PING=\"%s\"", ip_addres);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_set_mode(uint8_t mode) {

    if (mode < 1 || mode > 3) {
        return EMBD_FAILURE;
    }

    sprintf(esp_cmd_buffer, "AT+CWMODE=%u", mode);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_set_feedback(uint8_t mode) {

    if (mode > 1) {
        return EMBD_FAILURE;
    }

    sprintf(esp_cmd_buffer, "ATE%u", mode);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_set_autoconnect(uint8_t mode) {

    if (mode > 1) {
        return EMBD_FAILURE;
    }

    sprintf(esp_cmd_buffer, "AT+CWAUTOCONN=%u", mode);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_set_multiple_connections(uint8_t mode) {

    if (mode > 1) {
        return EMBD_FAILURE;
    }

    sprintf(esp_cmd_buffer, "AT+CIPMUX=%u", mode);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_connect_to_wifi(char *ssid, char *password) {

    sprintf(esp_cmd_buffer, "AT+CWJAP=\"%s\",\"%s\"", ssid, password);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_get_IP(char *msg) {

    char ip[32];
    sprintf(esp_cmd_buffer, "AT+CIFSR");
    ESP_send_command();

    //UART_read_line(UART_ESP8266, esp_buffer, ESP_BUFFER_SIZE, ESP_TIMEOUT);
    //UART_read_line(UART_ESP8266, esp_buffer, ESP_BUFFER_SIZE, ESP_TIMEOUT);
    //UART_read_line(UART_ESP8266, esp_buffer, ESP_BUFFER_SIZE, ESP_TIMEOUT);
    sscanf(esp_buffer, "+CIFSR:STAIP,\"%s\"", ip);          // STAIP

    if (strcmp(ip, "0.0.0.0") == 0) {
        sprintf(msg, "Unable to get IP address.\r\n");
        return EMBD_FAILURE;
    }

    UART_read_line(UART_ESP8266, esp_buffer, ESP_BUFFER_SIZE, ESP_TIMEOUT);

    return ESP_check_response();
}


uint8_t ESP_open_TCP_connection(char *ip, uint16_t port) {

    sprintf(esp_cmd_buffer, "AT+CIPSTART=\"TCP\",\"%s\",%u", ip, port);
    ESP_send_command();

    return ESP_check_response();
}


uint8_t ESP_send_TCP_data(char* data, uint16_t len) {

    sprintf(esp_cmd_buffer, "AT+CIPSEND=%u", len);
    ESP_send_command();

    if (ESP_check_response() == EMBD_SUCCESS) {
        ESP_send_data(data, len);
    } else {
        return EMBD_FAILURE;
    }

    return ESP_check_response();
}


uint8_t ESP_get_response(Vector_t *response, char *msg) {

    uint16_t msg_len;
    uint16_t limit = 0;
    char number[STR_TO_UINT16_BUFFER_SIZE];

    Vector_t *buffer = init_vector();

    skip_blank_lines(UART_ESP8266);

    while (UART_compare_char(UART_ESP8266, '+', ESP_TIMEOUT) == true_e  && limit++ < ESP_LINES_LIMIT) {
        UART_read_message_until_char(UART_ESP8266, buffer, ':');

        if (get_char(buffer) == '+' &&
                get_char(buffer) == 'I' &&
                get_char(buffer) == 'P' &&
                get_char(buffer) == 'D' &&
                get_char(buffer) == ',') {

            msg_len = 0;
            bzero(number, STR_TO_UINT16_BUFFER_SIZE);

            for (uint8_t i = 0; i < STR_TO_UINT16_BUFFER_SIZE && buffer->message_len; ++i) {
                number[i] = get_char(buffer);

                if (i > 5) {
                    sprintf(msg, "Wrong response format - number length\r\n");
                    deinit_vector_(buffer);
                    return EMBD_FAILURE;
                }
            }
            if (!str_to_uint16(number, &msg_len)) {
                sprintf(msg, "Wrong response format - number\r\n");
                deinit_vector_(buffer);
                return EMBD_FAILURE;
            }

            UART_read_message(UART_ESP8266, response, msg_len, 5000);
        } else {
            sprintf(msg, "Wrong response format - start character\r\n");
            deinit_vector_(buffer);
            return EMBD_FAILURE;
        }
        skip_blank_lines(UART_ESP8266);
    }

    deinit_vector_(buffer);
    return ESP_check_response();
}

uint8_t ESP_TCP_GET(char *ip, uint16_t port, char* data, uint16_t len, char* msg, Vector_t *vector) {

    uint8_t open = false_e;
    for (uint8_t i = 0; i < 5; ++i) {
        if (ESP_open_TCP_connection(ip, port) == EMBD_SUCCESS) {
            open = true_e;
            break;
        }
    }

    if (!open) {
        sprintf(msg, "Unable to connect to server.\r\n");
        return EMBD_FAILURE;
    }


    if (ESP_send_TCP_data(data, len) == EMBD_FAILURE) {
        sprintf(msg, "Data transfer failed.\r\n");
        return EMBD_FAILURE;
    }

    if (ESP_get_response(vector, msg) == EMBD_FAILURE) {
        return EMBD_FAILURE;
    }

    return EMBD_SUCCESS;
}
