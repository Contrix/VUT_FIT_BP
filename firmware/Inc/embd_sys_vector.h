/**
 * Water level monitoring
 * Vector module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_vector.h
 */

#ifndef WATER_MONITORING_EMBD_VECTOR_H
#define WATER_MONITORING_EMBD_VECTOR_H

#include <stdint-gcc.h>
#include <stdlib.h>
#include "../Inc/embd_sys_inc.h"

#define VECTOR_UNIT_LEN 128
#define VECTOR_MAX_LEN 2048

typedef struct Vector_unit_t{
    struct Vector_unit_t *next;
    uint8_t len;
    char data[VECTOR_UNIT_LEN];
} Vector_unit_t;

typedef struct Vector_t {
    struct Vector_unit_t *head;
    struct Vector_unit_t *tail;
    uint8_t index;
    uint16_t message_len;
} Vector_t;

/**
 * Initialize the vector
 * @return
 */
Vector_t * init_vector();

/**
 * De-initialize the vector
 * @param vector
 */
void deinit_vector_(Vector_t *vector);

/**
 * Puts char to vector
 * @param vector Vector
 * @param c char to put
 * @return 0 for success
 */
uint8_t put_char(Vector_t *vector, char c);

/**
 * Inserts string to Vector
 * @param vector Destination vector
 * @param string String to insert
 * @param len String length
 * @return 0 for success
 */
uint16_t put_string(Vector_t *vector, char *string, uint16_t len);

/**
 * Moves string from source Vector to destination vector
 * @param src Source Vector
 * @param dst Destination Vector
 * @param len Data length
 */
void copy_string(Vector_t *src, Vector_t *dst, uint16_t len);

/**
 *
 * @param vector struct of Vector
 * @return Char
 */
char get_char(Vector_t *vector);

/**
 * Returns pointer to first char and deletes it
 * @param vector Source Vector
 * @return Pointer to char
 */
char *get_char_ptr(Vector_t *vector);

/**
 * Returns count of chars in Vector
 * @param vector Source Vector
 * @return Count of chars
 */
uint16_t get_message_len(Vector_t *vector);

/**
 * Moves the text to the specified character
 * @param vector Source Vector
 * @param buffer Destination Vector
 * @return 0 for success
 */
uint8_t get_message_until_char(Vector_t *vector, Vector_t *buffer, char stop_char);

/**
 * Shows first char in vector
 * @param vector Source Vector
 * @return First char in vector
 */
uint8_t show_first_char(Vector_t *vector);

/**
 * Shows last char in Vector
 * @param vector Source Vector
 * @return Last char in Vector
 */
uint8_t show_last_char(Vector_t *vector);

/**
 * Flushes the Vector
 * @param vector Source Vector
 */
void vector_flush(Vector_t *vector);

#endif //WATER_MONITORING_EMBD_VECTOR_H
