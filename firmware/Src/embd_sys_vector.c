/**
 * Water level monitoring
 * Vector module
 * @Author HanakJiri, xhanak33
 * @file embd_sys_vector.c
 */

#include "../Inc/embd_sys_vector.h"


Vector_t * init_vector() {
    Vector_t *vector = malloc(sizeof(Vector_t));

    if (vector == NULL) {
        return NULL;
    }

    Vector_unit_t *unit = malloc(sizeof(Vector_unit_t));

    if (unit == NULL) {
        free(vector);
        return NULL;
    }

    unit->next = NULL;
    unit->len = 0;

    vector->head = unit;
    vector->tail = unit;
    vector->index = 0;
    vector->message_len = 0;

    return vector;
}


void deinit_vector_(Vector_t *vector) {
    Vector_unit_t *tmp;

    for (Vector_unit_t *unit = vector->head; unit != NULL;) {
        tmp = unit;
        unit = unit->next;
        free(tmp);
    }

    free(vector);
}


uint8_t put_char(Vector_t *vector, char c) {
    Vector_unit_t *unit;

    if (vector->message_len >= VECTOR_MAX_LEN) {
        return EMBD_FAILURE;
    }

    if (vector->tail->len == VECTOR_UNIT_LEN) {

        /// last unit is full
        unit = malloc(sizeof(Vector_unit_t));

        if (unit == NULL) {
            return EMBD_FAILURE;
        }

        unit->next = NULL;
        unit->len = 0;
        vector->tail->next = unit;
        vector->tail = unit;

    } else {
        unit = vector->tail;
    }

    unit->data[unit->len++] = c;
    vector->message_len++;

    return EMBD_SUCCESS;
}


uint16_t put_string(Vector_t *vector, char *string, uint16_t len) {

    for (uint16_t i = 0; i < len; ++i) {
        if (put_char(vector, string[i]) == EMBD_FAILURE) {
            return i;
        }
    }

    return EMBD_SUCCESS;
}


void copy_string(Vector_t *src, Vector_t *dst, uint16_t len) {

    for (uint16_t i = 0; i < len; ++i) {
        put_char(dst, get_char(src));
    }
}


char get_char(Vector_t *vector) {

    char c = vector->head->data[vector->index++];
    vector->message_len--;

    if (vector->index == VECTOR_UNIT_LEN) {
        vector->index = 0;
        if (vector->head == vector->tail) {
            vector->head->len = 0;
        } else {
            Vector_unit_t *unit = vector->head;
            vector->head = unit->next;
            free(unit);
        }
    }

    return c;
}

char *get_char_ptr(Vector_t *vector) {

    char *c_ptr = &vector->head->data[vector->index++];
    vector->message_len--;

    if (vector->index == VECTOR_UNIT_LEN) {
        vector->index = 0;
        if (vector->head == vector->tail) {
            vector->head->len = 0;
        } else {
            Vector_unit_t *unit = vector->head;
            vector->head = unit->next;
            free(unit);
        }
    }

    return c_ptr;
}


uint16_t get_message_len(Vector_t *vector) {

    return vector->message_len;
}


uint8_t get_message_until_char(Vector_t *vector, Vector_t *buffer, char stop_char) {

    char tmp_char;

    while (vector->message_len) {
        tmp_char = get_char(vector);
        put_char(buffer, tmp_char);

        if (tmp_char == stop_char) {
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}


uint8_t show_first_char(Vector_t *vector) {

    if (vector->message_len) {
        return (uint8_t)vector->head->data[vector->index];
    }

    return 0;
}


uint8_t show_last_char(Vector_t *vector) {

    if (vector->message_len) {
        return (uint8_t)vector->tail->data[vector->tail->len - 1];
    }

    return 0;
}


void vector_flush(Vector_t *vector) {

    Vector_unit_t *tmp;

    for (Vector_unit_t *unit = vector->head->next; unit != NULL;) {
        tmp = unit;
        unit = unit->next;
        free(tmp);
    }

    vector->tail = vector->head;
    vector->head->next = NULL;
    vector->head->len = 0;
    vector->index = 0;
    vector->message_len = 0;
}