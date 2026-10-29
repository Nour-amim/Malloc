#ifndef PAGE_MANAGE_H
#define PAGE_MANAGE_H

#include <stddef.h>

struct blocks
{
    char status;
    size_t size;
    struct blocks *next;
    struct blocks *prev;
};

// Align size given to a multiple of long double
size_t align(size_t size, char *err);
// Create a new block form given block, + offset after it
struct blocks *block_division(struct blocks *block, size_t offset);
// Create a new page of size based on the given size
struct blocks *blocks_new(size_t size);
// Add a new page at the end of blocks based on the size
struct blocks *add_page(struct blocks *blocks, size_t size);
// Return the beginning og the page where the pointer is located
void *page_begin(void *ptr, size_t page_size);
// Check if blocks are neighbours
char are_adjacent(struct blocks *b1, struct blocks *b2);
// Check if freeing the block can remove pages, if so, remove the pages freed
int remove_pages(struct blocks *block, size_t page_size);

#endif // PAGE_MANAGE_H
