
#include "linked_list.h"
#include <stdlib.h>

/******************************************************************************
 * Definitions
 *****************************************************************************/

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef struct {
    void* data;
    void* next;
} linked_list_element_struct_t;

/******************************************************************************
 * Prototypes
 *****************************************************************************/

/******************************************************************************
 * Variables
 *****************************************************************************/

/******************************************************************************
 * Public functions
 *****************************************************************************/

linked_list_resp_t linked_list_init(linked_list_handle_t* handle)
{
    linked_list_resp_t resp = LINKED_LIST_INVALID_PARM;

    if (NULL != handle) {
        handle->head = NULL;
        handle->number_of_elements = 0;
        resp = LINKED_LIST_SUCCESS;
    }

    return resp;
}

linked_list_resp_t linked_list_insert(linked_list_handle_t* handle,
                                      void* new_element_data,
                                      uint32_t position)
{
    linked_list_resp_t resp = LINKED_LIST_INVALID_PARM;
    linked_list_element_struct_t* new_element = NULL;
    linked_list_element_struct_t** link_pointer = &(handle->head);

    if (NULL != handle && NULL != new_element_data) {
        new_element =
            (linked_list_element_struct_t*)malloc(sizeof(linked_list_element_struct_t));
        if (NULL == new_element) {
            resp = LINKED_LIST_MEM_ALLOCATE_FAILED;
        } else {
            new_element->data = new_element_data;
            for (; position > 0 && (*link_pointer) != NULL; position--) {
                link_pointer = &((*link_pointer)->next);
            }
            if (position > 0) {
                resp = LINKED_LIST_INVALID_POSITION;
            } else {
                new_element->next = (*link_pointer);
                (*link_pointer) = new_element;
                handle->number_of_elements++;
                resp = LINKED_LIST_SUCCESS;
            }
        }
    }

    return resp;
}

linked_list_resp_t linked_list_remove(linked_list_handle_t* handle, uint32_t position, bool free_data)
{
    linked_list_resp_t resp = LINKED_LIST_INVALID_PARM;
    linked_list_element_struct_t** link_pointer = &(handle->head);
    linked_list_element_struct_t* element_to_free = NULL;

    if (NULL != handle) {
        for (; position > 0 && (*link_pointer) != NULL; position--) {
            link_pointer = &((*link_pointer)->next);
        }
        if ((*link_pointer) == NULL) {
            resp = LINKED_LIST_INVALID_POSITION;
        } else {
            element_to_free = *link_pointer;
            (*link_pointer) = (*link_pointer)->next;
            if (free_data) {
                free(element_to_free->data);
            }
            free(element_to_free);
            handle->number_of_elements--;
            resp = LINKED_LIST_SUCCESS;
        }
    }

    return resp;
}

linked_list_resp_t linked_list_get(linked_list_handle_t* handle, void** output_data, uint32_t position)
{
    linked_list_resp_t resp = LINKED_LIST_INVALID_PARM;
    linked_list_element_struct_t** link_pointer = &(handle->head);

    if (NULL != handle && NULL != output_data && NULL != output_data) {
        for (; position > 0 && (*link_pointer) != NULL; position--) {
            link_pointer = &((*link_pointer)->next);
        }
        if ((*link_pointer) == NULL) {
            resp = LINKED_LIST_INVALID_POSITION;
        } else {
            (*output_data) = (*link_pointer)->data;
            link_pointer = &((*link_pointer)->next);
            resp = LINKED_LIST_SUCCESS;
        }
    }

    return resp;
}

linked_list_resp_t linked_list_reset(linked_list_handle_t* handle, bool free_data)
{
    linked_list_resp_t resp = LINKED_LIST_INVALID_PARM;
    linked_list_element_struct_t* element_iter = handle->head;
    linked_list_element_struct_t* element_to_free = NULL;

    if (NULL != handle) {
        while (element_iter != NULL) {
            element_to_free = element_iter;
            element_iter = element_iter->next;
            if (free_data) {
                free(element_to_free->data);
            }
            free(element_to_free);
        }
        handle->head = NULL;
        handle->number_of_elements = 0;
        resp = LINKED_LIST_SUCCESS;
    }

    return resp;
}

linked_list_resp_t linked_list_for_each(linked_list_handle_t* handle, linked_list_iterate_cb cb)
{
    linked_list_resp_t resp = LINKED_LIST_INVALID_PARM;
    linked_list_element_struct_t* link_pointer;
    uint32_t position = 0;

    if (NULL != handle && NULL != cb) {
        for (link_pointer = handle->head; link_pointer != NULL;
             link_pointer = link_pointer->next) {
            cb(position, link_pointer->data);
            position++;
        }
        resp = LINKED_LIST_SUCCESS;
    }

    return resp;
}

/******************************************************************************
 * Internal code
 *****************************************************************************/
