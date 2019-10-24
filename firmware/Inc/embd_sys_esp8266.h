/**
 * Water level monitoring
 * ESP8266 module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_esp8266.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_ESP8266_H
#define WATER_MONITORING_EMBD_SYS_ESP8266_H

#include "../Inc/embd_sys_uart.h"
#include "../Inc/embd_sys_vector.h"
#include "../Inc/embd_sys_inc.h"
#include "../Inc/embd_sys_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint-gcc.h>

#define ESP_CMD_BUFFER_SIZE 128
#define ESP_BUFFER_SIZE 512
#define ESP_TIMEOUT 3000
#define ESP_LINES_LIMIT 16
#define ESP_CWMODE_AP 1

char esp_cmd_buffer[ESP_CMD_BUFFER_SIZE];
char esp_buffer[ESP_BUFFER_SIZE];

/**
 * Initialization the ESP module
 * @param msg error message
 * @param ssid SSID of target AP
 * @param password Password of target AP
 * @return 0 for success
 */
uint8_t ESP_init(char *msg, char *ssid, char *password);

/**
 * Restarts the ESP module
 */
void ESP_restart();


/**
 * Sends the command
 */
void ESP_send_command();

/**
 * Sends only data to ESP module
 * @param data Data to send
 * @param size Size of data
 */
void ESP_send_data(char* data, uint16_t size);

/**
 * Check response on last action
 * @return 0 for success
 */
uint8_t ESP_check_response();

/**
 * Tests AT commands
 * @return 0 for success
 */
uint8_t ESP_test();

/**
 * Enters deep sleep mode
 * @param time Time in ms
 * @return 0 for success
 */
uint8_t ESP_sleep(uint32_t time);

/**
 * Ping server
 * @param ip_addres IP adderss of the server
 * @return 0 for success
 */
uint8_t ESP_ping(char* ip_addres);

/**
 * Set the mode of the ESP module - Station/AP/Station+AP
 * @param mode Mode (1 Station mode, 2 SoftAP mode, 3: SoftAP+Station mode)
 * @return 0 for success
 */
uint8_t ESP_set_mode(uint8_t mode);

/**
 * Enable command echo
 * @param mode 1 for turn on echo
 * @return 0 for success
 */
uint8_t ESP_set_feedback(uint8_t mode);

/**
 * Set auto connect on power-up
 * @param mode 1 for autoconnect
 * @return 0 for success
 */
uint8_t ESP_set_autoconnect(uint8_t mode);

/**
 * Enable multiple connections
 * @param mode 1 multiple connections
 * @return 0 for success
 */
uint8_t ESP_set_multiple_connections(uint8_t mode);

/**
 * Connects to AP
 * @param ssid SSID of target AP
 * @param password Password of target AP
 * @return 0 for success
 */
uint8_t ESP_connect_to_wifi(char *ssid, char *password);

/**
 * Return Local IP address
 * @param ip Pointer to local IP address
 * @return 0 for success
 */
uint8_t ESP_get_IP(char *ip);

/**
 * Open TCP connection
 * @param ip IP address of target server
 * @param port Port of target server
 * @return 0 for success
 */
uint8_t ESP_open_TCP_connection(char *ip, uint16_t port);

/**
 * Sends data throw TCP to target
 * @param data Data to be send
 * @param len Data length
 * @return 0 for success
 */
uint8_t ESP_send_TCP_data(char* data, uint16_t len);

/**
 * Parse server response
 * @param response Vector for response
 * @param msg Error message
 * @return 0 for success
 */
uint8_t ESP_get_response(Vector_t *response, char *msg);

/**
 * Sends data to target
 * @param ip IP address of target
 * @param port Port of target
 * @param data Data to be send
 * @param len Data length
 * @param msg Error message
 * @param vector Vector for response
 * @return 0 for success
 */
uint8_t ESP_TCP_GET(char *ip, uint16_t port, char* data, uint16_t len, char* msg, Vector_t *vector);

#endif //WATER_MONITORING_EMBD_SYS_ESP8266_H
