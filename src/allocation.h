#ifndef ALLOCATION_H
#define ALLOCATION_H

#include <stddef.h>

#include "page_manage.h"

// Main fuction for allocation, take the size asked by the user and allocate it
// Calls other methods from page_manage.h if needed (align size, create page...)
void *allocate(size_t size, struct blocks *list);
// Function for free, check if previous and next blocks are free
// If neighbours are free, they are merged
// Calls remove_pages from page_manage.h to munmap if needed
void free_space(void *ptr);

#endif // ALLOCATION_H
