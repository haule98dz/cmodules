
#include "static_linked_list.h"
#include <string.h>

#ifndef NULL
#define NULL ((void*)0)
#endif

#define STATIC_LINKED_LIST_DEFAULT_LINK     ((static_linked_list_size_t)(-1))
#define STATIC_LINKED_LIST_END_OF_LIST      ((static_linked_list_size_t)(-2))
#define STATIC_LINKED_LIST_MAX_SIZE_ALLOWED ((static_linked_list_size_t)(-3))

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Variables
 *****************************************************************************/

/******************************************************************************
 * Public functions
 *****************************************************************************/

static_linked_list_resp_t static_linked_list_init(static_linked_list_handle_t* handle,
                                                  static_linked_list_init_t* init)
{
    static_linked_list_resp_t resp = STATIC_LINKED_LIST_INVALID_PARM;
    static_linked_list_size_t index;

    if (NULL != handle && NULL != init
        && init->max_number_of_elements <= STATIC_LINKED_LIST_MAX_SIZE_ALLOWED) {
        handle->max_number_of_elements = init->max_number_of_elements;
        handle->size_of_element = init->size_of_element;
        handle->static_data = init->static_data;
        handle->static_link_table = init->static_link_table;
        // Reset link table to default link
        for (index = 0; index < handle->max_number_of_elements; index++) {
            handle->static_link_table[index] = STATIC_LINKED_LIST_DEFAULT_LINK;
        }
        handle->head = STATIC_LINKED_LIST_END_OF_LIST;
        handle->current_number_of_elements = 0;
        resp = STATIC_LINKED_LIST_SUCCESS;
    }

    return resp;
}

static_linked_list_resp_t static_linked_list_insert(static_linked_list_handle_t* handle,
                                                    void* new_element_data,
                                                    uint32_t position)
{
    static_linked_list_resp_t resp = STATIC_LINKED_LIST_INVALID_PARM;
    static_linked_list_size_t insert_index = 0;
    static_linked_list_size_t* previous_ptr = &(handle->head);
    static_linked_list_size_t iter;

    if (position > handle->current_number_of_elements) {
        resp = STATIC_LINKED_LIST_INVALID_POSITION;
    } else if (NULL != handle && NULL != new_element_data) {
        // Search for an unoccupied position
        while (insert_index < handle->max_number_of_elements
               && handle->static_link_table[insert_index] != STATIC_LINKED_LIST_DEFAULT_LINK) {
            insert_index++;
        }
        if (handle->static_link_table[insert_index] != STATIC_LINKED_LIST_DEFAULT_LINK) {
            // No empty space left
            resp = STATIC_LINKED_LIST_OVERFLOW;
        } else {
            // Insert the element
            for (iter = handle->head; position > 0 && iter != STATIC_LINKED_LIST_END_OF_LIST;
                 position--) {
                previous_ptr = &(handle->static_link_table[iter]);
                iter = handle->static_link_table[iter];
            }
            handle->static_link_table[insert_index] = (*previous_ptr);
            (*previous_ptr) = insert_index;
            memcpy(&(handle->static_data[insert_index * handle->size_of_element]),
                   new_element_data,
                   handle->size_of_element);
            handle->current_number_of_elements++;
            resp = STATIC_LINKED_LIST_SUCCESS;
        }
    }

    return resp;
}

static_linked_list_resp_t static_linked_list_remove(static_linked_list_handle_t* handle,
                                                    uint32_t position)
{
    static_linked_list_resp_t resp = STATIC_LINKED_LIST_INVALID_PARM;
    static_linked_list_size_t* previous_ptr = &(handle->head);
    static_linked_list_size_t iter;

    if (position > handle->current_number_of_elements) {
        resp = STATIC_LINKED_LIST_INVALID_POSITION;
    } else if (NULL != handle) {
        for (iter = handle->head; position > 0 && iter != STATIC_LINKED_LIST_END_OF_LIST;
             position--) {
            previous_ptr = &(handle->static_link_table[iter]);
            iter = handle->static_link_table[iter];
        }
        (*previous_ptr) = handle->static_link_table[iter];
        handle->static_link_table[iter] = STATIC_LINKED_LIST_DEFAULT_LINK;
        handle->current_number_of_elements--;
        resp = STATIC_LINKED_LIST_SUCCESS;
    }

    return resp;
}

static_linked_list_resp_t static_linked_list_get(static_linked_list_handle_t* handle,
                                                 void* output_data,
                                                 uint32_t position)
{
    static_linked_list_resp_t resp = STATIC_LINKED_LIST_INVALID_PARM;
    static_linked_list_size_t link = handle->head;

    if (position >= handle->current_number_of_elements) {
        resp = STATIC_LINKED_LIST_INVALID_POSITION;
    } else if (NULL != handle && NULL != output_data) {
        // Iterate to the element
        for (; position > 0 && link != STATIC_LINKED_LIST_END_OF_LIST; position--) {
            link = handle->static_link_table[link];
        }
        if (link >= STATIC_LINKED_LIST_MAX_SIZE_ALLOWED) {
            resp = STATIC_LINKED_LIST_INVALID_POSITION;
        } else {
            // Get element
            memcpy(output_data,
                   &(handle->static_data[link * handle->size_of_element]),
                   handle->size_of_element);
            resp = STATIC_LINKED_LIST_SUCCESS;
        }
    }

    return resp;
}

static_linked_list_resp_t static_linked_list_reset(static_linked_list_handle_t* handle)
{
    static_linked_list_resp_t resp = STATIC_LINKED_LIST_INVALID_PARM;

    if (NULL != handle) {
        handle->head = STATIC_LINKED_LIST_END_OF_LIST;
        handle->current_number_of_elements = 0;
    }

    return resp;
}

static_linked_list_resp_t static_linked_list_for_each(static_linked_list_handle_t* handle,
                                                      static_linked_list_iterate_cb cb)
{
    static_linked_list_resp_t resp = STATIC_LINKED_LIST_INVALID_PARM;
    static_linked_list_size_t iter;
    static_linked_list_size_t position = 0;

    if (NULL != handle && NULL != cb) {
        // Iterate to the element
        for (iter = handle->head; iter != STATIC_LINKED_LIST_END_OF_LIST;
             iter = handle->static_link_table[iter]) {
            cb(position, &(handle->static_data[iter * handle->size_of_element]));
            position++;
        }
        resp = STATIC_LINKED_LIST_SUCCESS;
    }

    return resp;
}
