/**
 * Water level monitoring
 * LIDAR module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_lidar.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_LIDAR_H
#define WATER_MONITORING_EMBD_SYS_LIDAR_H

#include "../Inc/embd_sys_inc.h"
#include "../Inc/embd_sys_vector.h"
#include "../Inc/embd_sys_uart.h"

#include <stdint-gcc.h>

#define LIDAR_HEADER 0x59
#define LIDAR_WINDOW_LEN 9

#define LIDAR_LONGEST 12000
#define LIDAR_SHORTEST 300

/**
 * Return distance from sensor
 * @return distance in mm
 */
uint16_t LIDAR_get_distance();

#endif //WATER_MONITORING_EMBD_SYS_LIDAR_H
