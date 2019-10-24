/**
 * Water level monitoring
 * JSN SR04t2 module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_jsn-sr04t2.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_JSN_SR04T2_H
#define WATER_MONITORING_EMBD_SYS_JSN_SR04T2_H

#include "../Inc/embd_sys_uart.h"
#include <stdint-gcc.h>

#define JSN_QUESTION "\x55"
#define JSN_HEADER (uint8_t)0xff

#define JSN_LONGEST 6000
#define JSN_SHORTEST 200

/**
 * Return distance from sensor
 * @return distance in mm
 */
uint16_t JSN_get_distance();

#endif //WATER_MONITORING_EMBD_SYS_JSN_SR04T2_H
