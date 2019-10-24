/**
 * Water level monitoring
 * GPIO module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_gpio.h
 */

#ifndef WATER_MONITORING_EMBD_SYS_GPIO_H
#define WATER_MONITORING_EMBD_SYS_GPIO_H

#include "../Drivers/CMSIS/Device/ST/STM32F0xx/Include/stm32f030xc.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal.h"
#include "../Drivers/STM32F0xx_HAL_Driver/Inc/stm32f0xx_hal_gpio.h"
#include <stdint-gcc.h>

#define SSR_PIN GPIO_PIN_6
#define SSR_PORT GPIOA

#define LED_1_PIN GPIO_PIN_0
#define LED_2_PIN GPIO_PIN_1
#define LED_PORT GPIOB

#define MATRIX_PIN_X1 GPIO_PIN_5
#define MATRIX_PIN_X2 GPIO_PIN_7
#define MATRIX_PIN_Y1 GPIO_PIN_6
#define MATRIX_PIN_Y2 GPIO_PIN_8
#define MATRIX_PORT GPIOB

#define MATRIX_CONTACT_1 0x01
#define MATRIX_CONTACT_2 0x02
#define MATRIX_CONTACT_3 0x04
#define MATRIX_CONTACT_4 0x08

/**
 * Performs float measurement
 * @return sum of IDs of floats sensors
 */
uint8_t get_matrix();

/**
 * Turn on LED1
 */
void LED1_set();

/**
 * Turn off LED1
 */
void LED1_reset();

/**
 * Toggle LED1
 */
void LED1_toggle();

/**
 * Turn on LED2
 */
void LED2_set();

/**
 * Turn off LED2
 */
void LED2_reset();

/**
 * Toggle LED2
 */
void LED2_toggle();

/**
 * Turn on SSR
 */
void SSR_set();

/**
 * Turn off SSR
 */
void SSR_reset();

#endif //WATER_MONITORING_EMBD_SYS_GPIO_H
