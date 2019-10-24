/**
 * Water level monitoring
 * Configuration module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_configuration.h
 */


#include "../Inc/embd_sys_configuration.h"

__attribute__((section(".flash_data"))) volatile uint32_t flash_data;


void init_configuration(union EMBD_configuration *configuration) {

    LED2_set();

    for (uint16_t i = 0; i < CONFIG_FLASH_WORD_LEN * 4; i++) {
        configuration->array[i] = 0;
    }

    configuration->config.change_id = (uint32_t)1;

    sprintf(config_buffer, "\r\n\r\nSystem initialization\r\n");
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.refresh = read_number("REFRESH=", 1, UINT32_MAX);
    sprintf(config_buffer, "REFRESH=%"PRIu32"\r\n", configuration->config.refresh);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    read_string("SERVER=", configuration->config.server_address, 128);
    sprintf(config_buffer, "SERVER=%s\r\n", configuration->config.server_address);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.server_port = (uint16_t)read_number("PORT=", 1, UINT16_MAX);
    sprintf(config_buffer, "PORT=%u\r\n", configuration->config.server_port);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    read_string("SSID=", configuration->config.ssid, 128);
    sprintf(config_buffer, "SSID=%s\r\n", configuration->config.ssid);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    read_string("PASSWORD=", configuration->config.password, 64);
    sprintf(config_buffer, "PASSWORD=%s\r\n", configuration->config.password);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.height = read_number("HEIGHT=", 0, UINT32_MAX);
    sprintf(config_buffer, "HEIGHT=%"PRIu32"\r\n", configuration->config.height);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.level_max = read_number("MAXIMUM=", 0, configuration->config.height);
    sprintf(config_buffer, "MAXIMUM=%"PRIu32"\r\n", configuration->config.level_max);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.level_min = read_number("MINIMUM=", 0, configuration->config.level_max);
    sprintf(config_buffer, "MINIMUM=%"PRIu32"\r\n", configuration->config.level_min);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.pump_active = (uint8_t)read_number("PUMP_ACTIVE=", 0, 2);
    sprintf(config_buffer, "PUMP_ACTIVE=%u\r\n", configuration->config.pump_active & 0xFF);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.average = (uint8_t)read_number("AVERAGE_COUNT=", 1, 30);
    sprintf(config_buffer, "AVERAGE_COUNT=%u\r\n", configuration->config.average & 0xFF);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    configuration->config.control_sensors = (uint8_t)read_number("CONTROL_SENSORS=", 0, 15);
    sprintf(config_buffer, "CONTROL_SENSORS=%u\r\nInitialization complete.\r\n\r\n",
            configuration->config.control_sensors & 0xFF);
    UART_send_message(UART_DBG, config_buffer, (uint16_t)strlen(config_buffer));

    store_configuration(configuration);
    LED2_reset();
}


void read_string(char *msg, uint8_t *string, uint8_t max) {

    do {
        UART_send_message(UART_DBG, msg, (uint16_t) strlen(msg));
        if (UART_read_line(UART_DBG, config_buffer, CONFIG_BUFFER_SIZE, 0) == EMBD_FAILURE) {
            UART_send_message(UART_DBG, "\r\n", 2);
            continue;
        }

        if (config_buffer[0] != '\"') {
            UART_send_message(UART_DBG, "\r\nString must be in double quotes.\r\n", 36);
            continue;
        }

        for (uint8_t i = 0; i < max; ++i) {
            if (config_buffer[i + 1] == '\"' && i > 0) {
                UART_send_message(UART_DBG, "\r\n", 2);
                return;
            }

            string[i] = (uint8_t)config_buffer[i + 1];
        }
        UART_send_message(UART_DBG, "\r\nInvalid string\r\n", 18);
    } while (true_e);
}


uint32_t read_number(char *msg, uint32_t min, uint32_t max) {

    uint64_t number;
    uint8_t correct;

    do {
        correct = true_e;
        number = 0;
        UART_send_message(UART_DBG, msg, (uint16_t) strlen(msg));
        if (UART_read_line(UART_DBG, config_buffer, CONFIG_BUFFER_SIZE, 0) == EMBD_FAILURE) {
            UART_send_message(UART_DBG, "\r\n", 2);
            continue;
        }

        for (uint8_t i = 0; i < strlen(config_buffer) && i < UINT64_MAX_LEN; ++i) {
            if (config_buffer[i] == '\r' || config_buffer[i] == '\n') {
                break;
            }
            if (config_buffer[i] < '0' || config_buffer[i] > '9') {
                correct = false_e;
                break;
            }
            number *= 10;
            number += config_buffer[i] - '0';
        }
        UART_send_message(UART_DBG, "\r\n", 2);

        if (!correct) {
            UART_send_message(UART_DBG, "Not a number.\r\n", 15);
        } else if (number < min || number > max) {
            UART_send_message(UART_DBG, "Too small / too big.\r\n", 22);
        }

    } while (number < min || number > max || !correct);

    return (uint32_t)number;
}


uint32_t flash_read(uint32_t address) {
    return *((uint32_t *) address);
}


void load_configuration(union EMBD_configuration *configuration) {

    for (uint16_t i = 0; i < CONFIG_FLASH_WORD_LEN; i++) {
        configuration->array[i] = flash_read(FLASH_PAGE_ADDRESS + i * 4);
    }
}


void store_configuration(union EMBD_configuration *configuration) {

    uint32_t error;
    FLASH_EraseInitTypeDef erase;

    erase.TypeErase   = FLASH_TYPEERASE_PAGES;
    erase.PageAddress = FLASH_PAGE_ADDRESS;
    erase.NbPages     = 1;

    if (HAL_FLASH_Unlock() != HAL_OK) {
        UART_send_message(UART_DBG, "Flash error 1.\r\n", 16);
        return;
    }

    if (HAL_FLASHEx_Erase(&erase, &error) != HAL_OK) {
        UART_send_message(UART_DBG, "Flash error 2.\r\n", 16);
        return;
    }

    for (uint16_t i = 0; i < CONFIG_FLASH_WORD_LEN; i++) {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_PAGE_ADDRESS + i * 4, configuration->array[i]) != HAL_OK) {
            UART_send_message(UART_DBG, "Flash error 3.\r\n", 16);
            return;
        }
    }

    if (HAL_FLASH_Lock() != HAL_OK) {
        UART_send_message(UART_DBG, "Flash error 4.\r\n", 16);
    }
}

void set_configuration(Vector_t *conf, uint8_t force) {

    union EMBD_configuration *configuration_new = malloc(sizeof(union EMBD_configuration));

    char word[CONFIG_ID_LEN] = {0, };
    char value[CONFIG_VALUE_LEN] = {0, };
    uint64_t number = 0;
    char c;
    uint32_t change_id;

    load_configuration(configuration_new);
    change_id = configuration_new->config.change_id;


    while (conf->message_len) {
        c = get_char(conf);

        /// skip blank chars
        while (conf->message_len && (c == '\n' || c == '\r' || c == ' ' || c == '\t')) {
            c = get_char(conf);
        }

        /// skip comments
        if (c == COMMNET) {
            skip_line(conf);
            continue;
        }

        /// load key word
        bzero(word, CONFIG_ID_LEN);
        for (uint8_t i = 0; i < CONFIG_ID_LEN; ++i) {
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_') {
                word[i] = c;
                c = get_char(conf);
            } else {
                break;
            }
        }

        while (conf->message_len && (c == ' ' || c == '\t')) {
            c = get_char(conf);
        }

        /// eq
        if (c == EQ) {
            c = get_char(conf);
        } else {
            skip_line(conf);
            continue;
        }

        while (conf->message_len && (c == ' ' || c == '\t')) {
            c = get_char(conf);
        }

        /// Data
        bzero(value, CONFIG_VALUE_LEN);
        if (c == '\"') {
            for (uint8_t i = 0; i < CONFIG_VALUE_LEN && conf->message_len; ++i) {
                c = get_char(conf);

                if (c == '\"') {
                    break;
                }
                if (c != '\t' && c < ' ') {
                    break;
                }
                value[i] = c;
            }
            if (c != '\"') {
                skip_line(conf);
                continue;
            }
        } else if (c >= '0' && c <= '9') {
            value[0] = c;
            for (uint8_t i = 1; i < CONFIG_VALUE_LEN && conf->message_len; ++i) {
                c = get_char(conf);
                if (c < '0' || c > '9') {
                    break;
                }
                value[i] = c;
            }

            number = 0;
            for (uint8_t i = 0; i < strlen(value) && i < UINT64_MAX_LEN; ++i) {
                number *= 10;
                number += value[i] - '0';
            }
        } else {
            if (c != '\n') {
                skip_line(conf);
            }
            continue;
        }

        /// compare key word and set value
        set_value(configuration_new, word, value, number);

        if (c != '\n' && conf->message_len) {
            skip_line(conf);
        }
    }

    if (force || configuration_new->config.change_id != change_id) {
        store_configuration(configuration_new);
        UART_send_message(UART_DBG, "\r\nSettings is saving.\r\n", 23);
    }

    free(configuration_new);
}


uint8_t set_value(union EMBD_configuration *configuration, char *word, char *value, uint64_t number) {

    if (string_compare(word, CONFIG_CHANGE_ID)) {
        if (number <= UINT32_MAX) {
            configuration->config.change_id = (uint32_t)number;
            sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_CHANGE_ID, configuration->config.change_id);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_REFRESH)) {
        if (number <= UINT32_MAX && number > 0) {
            configuration->config.refresh = (uint32_t)number;
            sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_REFRESH, configuration->config.refresh);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_SERVER)) {
        if (strlen(value) <= 128) {
            strcpy((char *) configuration->config.server_address, value);
            sprintf(msg_buffer, "%s=%s\r\n", CONFIG_SERVER, configuration->config.server_address);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_PORT)) {
        if (number <= UINT16_MAX && number > 0) {
            configuration->config.server_port = (uint16_t) number;
            sprintf(msg_buffer, "%s=%u\r\n", CONFIG_PORT, configuration->config.server_port);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }
    } else if (string_compare(word, CONFIG_MINIMUM)) {
        if (number < configuration->config.level_max) {
            configuration->config.level_min = (uint32_t)number;
            sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_MINIMUM, configuration->config.level_min);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_MAXIMUM)) {
        if ((uint32_t)number < configuration->config.height) {
            configuration->config.level_max = (uint32_t)number;
            sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_MAXIMUM, configuration->config.level_max);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_HEIGHT)) {
        if (number <= UINT32_MAX) {
            configuration->config.height= (uint32_t)number;
            sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_HEIGHT, configuration->config.height);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_PUMP_ACTIVE)) {
        if (number <= 2) {
            configuration->config.pump_active = (uint8_t)number;
            sprintf(msg_buffer, "%s=%u\r\n", CONFIG_PUMP_ACTIVE, configuration->config.pump_active & 0xFF);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_CONTROL_SENSORS)) {
        if (number <= UINT8_MAX) {
            configuration->config.control_sensors = (uint8_t)number;
            sprintf(msg_buffer, "%s=%u\r\n", CONFIG_CONTROL_SENSORS, configuration->config.control_sensors & 0xFF);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_AVERAGE)) {
        if (number <= 30 && number > 0) {
            configuration->config.average = (uint8_t)number;
            sprintf(msg_buffer, "%s=%u\r\n", CONFIG_AVERAGE, configuration->config.average & 0xFF);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_SSID)) {
        if (strlen(value) <= 128) {
            strcpy((char *) configuration->config.ssid, value);
            sprintf(msg_buffer, "%s=%s\r\n", CONFIG_SSID, configuration->config.ssid);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }

    } else if (string_compare(word, CONFIG_PASSWORD)) {
        if (strlen(value) <= 64) {
            strcpy((char *) configuration->config.password, value);
            sprintf(msg_buffer, "%s=%s\r\n", CONFIG_PASSWORD, configuration->config.password);
            UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
        }
    } else {
        return EMBD_FAILURE;
    }

    return EMBD_SUCCESS;
}


void print_configuration() {

    sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_CHANGE_ID, embd_configuration->config.change_id);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_REFRESH, embd_configuration->config.refresh);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%s\r\n", CONFIG_SERVER, embd_configuration->config.server_address);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%u\r\n", CONFIG_PORT, embd_configuration->config.server_port);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_MINIMUM, embd_configuration->config.level_min);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_MAXIMUM, embd_configuration->config.level_max);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%"PRIu32"\r\n", CONFIG_HEIGHT, embd_configuration->config.height);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%u\r\n", CONFIG_PUMP_ACTIVE, embd_configuration->config.pump_active & 0xFF);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%u\r\n", CONFIG_CONTROL_SENSORS, embd_configuration->config.control_sensors & 0xFF);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%u\r\n", CONFIG_AVERAGE, embd_configuration->config.average & 0xFF);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%s\r\n", CONFIG_SSID, embd_configuration->config.ssid);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));

    sprintf(msg_buffer, "%s=%s\r\n", CONFIG_PASSWORD, embd_configuration->config.password);
    UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));
}


void skip_line(Vector_t *conf) {

    while (conf->message_len && get_char(conf) != '\n') { }
}


uint8_t string_compare(char* id, char* value) {
    for (uint8_t i = 0; i < CONFIG_ID_LEN; ++i) {
        if (id[i] == '\0') {
            if (value[i] != '\0') {
                break;
            }
            return true_e;;
        }
        //x != A | a
        if (id[i] != value[i] && id[i] != (value[i]) - 32) {
            break;
        }
    }

    return false_e;
}