/**
 * Water level monitoring
 * GPIO module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_gpio.c
 */

#include "../Inc/embd_sys_gpio.h"


uint8_t get_matrix() {

    uint8_t matrix = 0;
    HAL_GPIO_WritePin(MATRIX_PORT, MATRIX_PIN_X1, GPIO_PIN_SET);
    HAL_GPIO_WritePin(MATRIX_PORT, MATRIX_PIN_X2, GPIO_PIN_RESET);
    HAL_Delay(5);

    if (HAL_GPIO_ReadPin(MATRIX_PORT, MATRIX_PIN_Y1) == GPIO_PIN_SET) {
        matrix |= MATRIX_CONTACT_1;
    }

    if (HAL_GPIO_ReadPin(MATRIX_PORT, MATRIX_PIN_Y2) == GPIO_PIN_SET) {
        matrix |= MATRIX_CONTACT_2;
    }

    HAL_GPIO_WritePin(MATRIX_PORT, MATRIX_PIN_X1, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(MATRIX_PORT, MATRIX_PIN_X2, GPIO_PIN_SET);
    HAL_Delay(5);

    if (HAL_GPIO_ReadPin(MATRIX_PORT, MATRIX_PIN_Y1) == GPIO_PIN_SET) {
        matrix |= MATRIX_CONTACT_3;
    }

    if (HAL_GPIO_ReadPin(MATRIX_PORT, MATRIX_PIN_Y2) == GPIO_PIN_SET) {
        matrix |= MATRIX_CONTACT_4;
    }

    HAL_GPIO_WritePin(MATRIX_PORT, MATRIX_PIN_X2, GPIO_PIN_RESET);

    return matrix;
}


void LED1_set() {
    HAL_GPIO_WritePin(LED_PORT, LED_1_PIN, GPIO_PIN_RESET);
}


void LED1_reset() {
    HAL_GPIO_WritePin(LED_PORT, LED_1_PIN, GPIO_PIN_SET);
}


void LED1_toggle() {
    HAL_GPIO_TogglePin(LED_PORT, LED_1_PIN);
}


void LED2_set() {
    HAL_GPIO_WritePin(LED_PORT, LED_2_PIN, GPIO_PIN_RESET);
}


void LED2_reset() {
    HAL_GPIO_WritePin(LED_PORT, LED_2_PIN, GPIO_PIN_SET);
}


void LED2_toggle() {
    HAL_GPIO_TogglePin(LED_PORT, LED_2_PIN);
}


void SSR_set() {
    HAL_GPIO_WritePin(SSR_PORT, SSR_PIN, GPIO_PIN_SET);
}


void SSR_reset() {
    HAL_GPIO_WritePin(SSR_PORT, SSR_PIN, GPIO_PIN_RESET);
}
