/**
 * Water level monitoring
 * Main module
 * @Author HanakJiri, xhanak33
 * @file water_level_monitoring.h
 */


#include "../Inc/water_level_monitoring.h"


void monitoring_init() {

    LED1_set();
    UART_init();
    UART_send_message(UART_DBG, "System started.\r\n", 17);

    /// MCP9808
    if (MCP9808_init(&hi2c2) == EMBD_FAILURE) {
        sprintf(msg_buffer, "MCP9808 init failed.\r\n");
        UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));
    }

    response = init_vector();

    embd_configuration = malloc(sizeof(union EMBD_configuration));
    if (embd_configuration == NULL) {
        sprintf(msg_buffer, "Malloc failed\r\n");
        UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));
        return;
    }

    HAL_Delay(3000);
    load_configuration(embd_configuration);

    /// first run
    if (embd_configuration->config.change_id == UINT32_MAX || embd_configuration->config.change_id == 0) {
        embd_system_status = EMBD_INIT_e;
        init_configuration(embd_configuration);
    }

    embd_system_status = EMBD_RUN_e;
    main_counter = 0;
    SSR_reset();

    /// ESP
    if (ESP_init(msg_buffer,
            (char *)embd_configuration->config.ssid, (char *)embd_configuration->config.password) == EMBD_FAILURE) {
        UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));
    }
    UART_send_message(UART_DBG, "Loop started.\r\n", 15);
}

void monitoring_loop() {
    HAL_Delay(1000);
    LED1_toggle();

    if (main_counter % 30 == 0) {
        sprintf(msg_buffer, "id1=%u\tid2=%u\tid4=%u\tid8=%u.%u\r\n", lidar_level,
                ultrasonic_level, floats_level, temperature / 10, temperature % 10);
        UART_send_message(UART_DBG, msg_buffer, (uint16_t)strlen(msg_buffer));
        LED1_set();
        check_level();
        SSR_control();
    }

    if (++main_counter >= 60 * embd_configuration->config.refresh) {
        main_counter = 0;

        vector_flush(response);

        if (ESP_connect_to_wifi((char *)embd_configuration->config.ssid,
                (char *)embd_configuration->config.password) == EMBD_SUCCESS) {
            sprintf(buffer, "GET /water_level_monitoring?id1=%u&id2=%u&id4=%u&id8=%u.%u HTTP/1.0\r\n\r\n",
                    lidar_level, ultrasonic_level, floats_level, temperature / 10, temperature % 10);
            if (ESP_TCP_GET((char *) embd_configuration->config.server_address, embd_configuration->config.server_port,
                            buffer, (uint16_t) strlen(buffer), msg_buffer, response) == EMBD_FAILURE) {
                UART_send_message(UART_DBG, msg_buffer, (uint16_t) strlen(msg_buffer));
            } else {
                set_configuration(response, false_e);
            }
        } else {
            UART_send_message(UART_DBG, "Connection to wifi failed.\r\n", 28);
        }
    }

    if (embd_system_status != EMBD_RUN_e) {
        return;
    }
}


void check_level() {

    uint16_t lidar_measurement[EMBD_MAX_MEASURING] = {0, };
    uint16_t jsn_measurement[EMBD_MAX_MEASURING] = {0, };
    uint16_t mcp_measurement[EMBD_MAX_MEASURING] = {0, };
    uint8_t floats_measurement = 0;

    uint32_t level[EMBD_SENSORS_COUNT] = {0, };
    uint8_t counter[EMBD_SENSORS_COUNT] = {0, };

    uint8_t count;

    if (embd_configuration->config.average > 1 && embd_configuration->config.average <= 30) {
        count = embd_configuration->config.average;
    } else {
        count = 5;
    }

    /// get values
    for (uint8_t i = 0; i < count; ++i) {
        lidar_measurement[i] = LIDAR_get_distance();
        jsn_measurement[i] = JSN_get_distance();
        mcp_measurement[i] = MCP9808_get_temperature(&hi2c2);
        floats_measurement |= get_matrix();
        HAL_Delay(1000);
    }

    /// check values
    for (uint8_t i = 0; i < count; ++i) {
        if (lidar_measurement[i]) {
            level[LIDAR_ID] += lidar_measurement[i];
            counter[LIDAR_ID]++;
        }

        if (jsn_measurement[i]) {
            level[JSN_ID] += jsn_measurement[i];
            counter[JSN_ID]++;
        }

        if (mcp_measurement[i]) {
            level[MCP_ID] += mcp_measurement[i];
            counter[MCP_ID]++;
        }
    }

    /// make average
    lidar_level = counter[LIDAR_ID] ? (uint16_t)(level[LIDAR_ID] / counter[LIDAR_ID]) : (uint16_t)0;
    ultrasonic_level = counter[JSN_ID] ? (uint16_t)(level[JSN_ID] / counter[JSN_ID]) : (uint16_t)0;
    ultrasonic_level -= EMBD_JSN_OFFSET;
    temperature = counter[MCP_ID] ? (uint16_t)(level[MCP_ID] / counter[MCP_ID]) : (uint16_t)0;
    floats_level = floats_measurement;

    /// ultrasonic sensor calibration
    if (embd_configuration->config.control_sensors & CONFIG_MCP_ID) {
        double speed = sqrt((SONIC_AIR_ONSTANT * (273.15 + temperature / 10.0)));
        double dist = speed * (ultrasonic_level / SONIC_SPEED_20);
        ultrasonic_level = (uint16_t)dist;
    }
}


void SSR_control() {

    if (embd_configuration->config.pump_active == STOP_e) {
        SSR_reset();
        return;
    }

    uint8_t control = 0;
    uint8_t min_level = 0;
    uint8_t max_level = 0;

    /// check levels
    if ((embd_configuration->config.control_sensors & CONFIG_LIDAR_ID) && lidar_level) {
        control++;
        if (lidar_level < embd_configuration->config.level_min) {
            min_level++;
        }
        if (lidar_level > embd_configuration->config.level_max) {
            max_level++;
        }
    }

    if ((embd_configuration->config.control_sensors & CONFIG_JSN_ID) && ultrasonic_level) {
        control++;
        if (ultrasonic_level < embd_configuration->config.level_min) {
            min_level++;
        }
        if (ultrasonic_level > embd_configuration->config.level_max) {
            max_level++;
        }
    }

    if (embd_configuration->config.control_sensors & CONFIG_FLOATS_ID) {
        control++;
        if (floats_level == 0) {
            min_level++;
        }
        if (floats_level & MATRIX_CONTACT_4) {
            max_level++;
        }
    }

    if ((control >= 3 && min_level >= 2) || (control >= 2 && min_level >= 1) || (control >= 1 && min_level)) {
        if (embd_configuration->config.pump_active == FILL_e) {
            SSR_set();
        } else if (embd_configuration->config.pump_active == DRAIN_e) {
            SSR_reset();
        }
    } else if ((control >= 3 && max_level >= 2) || (control >= 2 && max_level >= 1) || (control >= 1 && max_level)) {
        if (embd_configuration->config.pump_active == FILL_e) {
            SSR_reset();
        } else if (embd_configuration->config.pump_active == DRAIN_e) {
            SSR_set();
        }
    }
}
