#ifndef _STATIC_LINKED_LIST_H
#define _STATIC_LINKED_LIST_H

#include <stdbool.h>
#include <stdint.h>

/******************************************************************************
 * Definitions
 *****************************************************************************/

typedef enum {
    STATIC_LINKED_LIST_SUCCESS = 0,
    STATIC_LINKED_LIST_INVALID_PARM,
    STATIC_LINKED_LIST_INVALID_POSITION,
    STATIC_LINKED_LIST_OVERFLOW,
    STATIC_LINKED_LIST_UNDEFINED_RESP
} static_linked_list_resp_t;

typedef uint16_t static_linked_list_size_t;

typedef struct {
    void* static_data;
    static_linked_list_size_t* static_link_table;
    static_linked_list_size_t max_number_of_elements;
    uint32_t size_of_element;
} static_linked_list_init_t;

typedef struct {
    void* static_data;
    static_linked_list_size_t* static_link_table;
    static_linked_list_size_t max_number_of_elements;
    uint32_t size_of_element;
    static_linked_list_size_t current_number_of_elements;
    static_linked_list_size_t head;
} static_linked_list_handle_t;

typedef (*static_linked_list_iterate_cb)(static_linked_list_size_t position, void* data);

#define STATIC_LINKED_LIST_DEFINE(name, capacity, element_type) \
    static_linked_list_handle_t name;                           \
    element_type name##_static_data[capacity];                  \
    static_linked_list_size_t name##_static_link_table[capacity];

#define STATIC_LINKED_LIST_INIT(name, capacity, element_type)                               \
    static_linked_list_init_t name##_init = {.max_number_of_elements = capacity,             \
                                            .size_of_element = sizeof(element_type),        \
                                            .static_data = (void*)name##_static_data,       \
                                            .static_link_table = name##_static_link_table}; \
    static_linked_list_init(&name, &name##_init);

/******************************************************************************
 * API
 *****************************************************************************/

static_linked_list_resp_t static_linked_list_init(static_linked_list_handle_t* handle,
                                                  static_linked_list_init_t* init);

static_linked_list_resp_t static_linked_list_insert(static_linked_list_handle_t* handle,
                                                    void* new_element_data,
                                                    uint32_t position);

static_linked_list_resp_t static_linked_list_remove(static_linked_list_handle_t* handle,
                                                    uint32_t position);

static_linked_list_resp_t static_linked_list_get(static_linked_list_handle_t* handle,
                                                 void* output_data,
                                                 uint32_t position);

static_linked_list_resp_t static_linked_list_reset(static_linked_list_handle_t* handle);

static_linked_list_resp_t static_linked_list_for_each(static_linked_list_handle_t* handle,
                                                      static_linked_list_iterate_cb cb);

#endif /* _STATIC_LINKED_LIST_H */
