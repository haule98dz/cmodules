#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H

#include <stdint.h>
#include <stdbool.h>

/******************************************************************************
 * Definitions
 *****************************************************************************/

/**
 * Config to 1 if data pushed to linked list is dynamically allocated
 */
#define LINKED_LIST_DATA_IS_DYNAMIC  1

typedef enum {
    LINKED_LIST_SUCCESS = 0,
    LINKED_LIST_INVALID_PARM,
    LINKED_LIST_INVALID_POSITION,
    LINKED_LIST_MEM_ALLOCATE_FAILED,
} linked_list_resp_t;

typedef struct {
    void* head;
    uint32_t number_of_elements;
} linked_list_handle_t;

typedef (*linked_list_iterate_cb)(uint32_t position, void* data);

/******************************************************************************
 * API
 *****************************************************************************/

linked_list_resp_t linked_list_init(linked_list_handle_t* handle);

linked_list_resp_t linked_list_insert(linked_list_handle_t* handle, void* element, uint32_t position);

linked_list_resp_t linked_list_remove(linked_list_handle_t* handle, uint32_t position, bool free_data);

linked_list_resp_t linked_list_get(linked_list_handle_t* handle, void** output_data, uint32_t position);

linked_list_resp_t linked_list_reset(linked_list_handle_t* handle, bool free_data);

linked_list_resp_t linked_list_for_each(linked_list_handle_t* handle, linked_list_iterate_cb cb);

#endif /* _LINKED_LIST_H */
