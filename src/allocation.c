#include "allocation.h"

#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
void *allocate(size_t size, struct blocks *list)
{
    if (list == NULL)
        return NULL;
    struct blocks *block = list;
    char err = 0;
    size = align(size, &err);
    if (err == 1)
    {
        errno = ENOMEM;
        return NULL;
    }
    while (block != NULL)
    {
        if (block->status == 1 && block->size >= size)
        {
            if (block->size > size + sizeof(struct blocks))
            {
                size_t old_size = block->size;
                block->size = size;
                struct blocks *new_block =
                    block_division(block, block->size + sizeof(struct blocks));
                new_block->status = 1;
                new_block->size = old_size - (size + sizeof(struct blocks));
                new_block->next = block->next;
                new_block->prev = block;
                if (block->next != NULL)
                    block->next->prev = new_block;
                block->next = new_block;
            }
            block->status = 0;
            return (block + 1);
        }
        block = block->next;
    }
    struct blocks *new_list = add_page(list, size);
    if (new_list == NULL)
        return NULL;
    return allocate(size, new_list);
}

void free_space(void *ptr)
{
    if (ptr == NULL)
        return;
    struct blocks *block = (struct blocks *)ptr - 1;
    block->status = 1;
    if (block->prev != NULL && block->prev->status == 1
        && are_adjacent(block->prev, block) == 0)
    {
        struct blocks *prev = block->prev;
        prev->size += block->size + sizeof(struct blocks);
        prev->next = block->next;
        if (block->next != NULL)
            block->next->prev = prev;
        block = prev;
    }
    if (block->next != NULL && block->next->status == 1
        && are_adjacent(block, block->next) == 0)
    {
        struct blocks *next = block->next;
        block->size += next->size + sizeof(struct blocks);
        block->next = next->next;
        if (block->next != NULL)
            block->next->prev = block;
        next = block;
    }
    size_t page_size = sysconf(_SC_PAGESIZE);
    remove_pages(block, page_size);
}
