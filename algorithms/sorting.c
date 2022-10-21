/*******************************************************************************
* Includes
******************************************************************************/

#include "sorting.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*******************************************************************************
* Definitions
******************************************************************************/

typedef uint32_t number;

/*******************************************************************************
* Prototypes
******************************************************************************/



/*******************************************************************************
* Variables
******************************************************************************/



/*******************************************************************************
* Codes Static
******************************************************************************/



/*******************************************************************************
* Codes API
******************************************************************************/

void insertion_sort(number* array, size_t size)
{
    number temp;
    size_t i, j;

    for (i = 1; i < size; i++) {
        temp = array[i];
        for (j = i; j > 0 && array[j - 1] > temp; j--) {
            array[j] = array[j - 1];
        }
        array[j] = temp;
    }
}

void shell_sort(number* array, uint32_t size)
{
    size_t gap, i, j;
    number temp;

    // Hibbard sequence: gap = 2*k - 1
    for (gap = 1; gap < size; gap = (gap << 1) | 1);
    while (gap > 0) {
        gap >>= 1;
        for (i = gap; i < size; i++) {
            temp = array[i];
            for (j = i; j >= gap && array[j - gap] > temp; j -= gap) {
                array[j] = array[j - gap];
            }
            array[j] = temp;
        }
    }
}
