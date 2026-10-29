#include "page_manage.h"

#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
size_t align(size_t size, char *err)
{
    if (size == 0)
        return 0;
    size_t ref = sizeof(long double);
    size_t mask;

    if (__builtin_uaddl_overflow(size, ref - 1, &mask) == 1)
    {
        *err = 1;
        return 0;
    }

    return mask & ~(ref - 1);
}

struct blocks *block_division(struct blocks *block, size_t offset)
{
    void *ptr = block;
    char *c_ptr = ptr;
    void *new_ptr = c_ptr + offset;
    struct blocks *final_block = new_ptr;
    return final_block;
}
struct blocks *blocks_new(size_t size)
{
    size_t page_size = sysconf(_SC_PAGESIZE);
    char err = 0;
    size_t aligned_size = align(size, &err);
    if (err == 1)
    {
        errno = ENOMEM;
        return NULL;
    }
    aligned_size += sizeof(struct blocks);
    if (aligned_size > page_size)
    {
        if (aligned_size % page_size == 0)
            page_size = (aligned_size / page_size) * page_size;
        else
            page_size = (aligned_size / page_size) * page_size + page_size;
    }
    void *addr = mmap(NULL, page_size, PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (addr == MAP_FAILED)
    {
        errno = ENOMEM;
        return NULL;
    }
    struct blocks *meta = addr;
    meta->status = 1;
    meta->size = page_size - sizeof(struct blocks);
    meta->next = NULL;
    meta->prev = NULL;

    return meta;
}

struct blocks *add_page(struct blocks *blocks, size_t size)
{
    if (blocks == NULL)
        return NULL;
    struct blocks *new_blocks = blocks_new(size);
    if (new_blocks == NULL)
        return NULL;
    struct blocks *curr_block = blocks;
    while (curr_block->next != NULL)
        curr_block = curr_block->next;
    curr_block->next = new_blocks;
    new_blocks->prev = curr_block;
    return blocks;
}

void *page_begin(void *ptr, size_t page_size)
{
    char *addr = ptr;
    return addr - ((size_t)ptr & (page_size - 1));
}

char are_adjacent(struct blocks *b1, struct blocks *b2)
{
    void *ptr1 = b1;
    char *c_ptr1 = ptr1;
    void *ptr2 = b2;
    char *c_ptr2 = ptr2;
    if (c_ptr1 + sizeof(struct blocks) + b1->size == c_ptr2)
        return 0;
    return 1;
}

int remove_pages(struct blocks *block, size_t page_size)
{
    struct blocks *page_b = page_begin(block, page_size);
    size_t total_size = block->size + sizeof(struct blocks);
    if (block != page_b || total_size < page_size)
        return 0;
    size_t pages_to_remove = total_size / page_size;
    struct blocks *to_remove = block;
    size_t offset = pages_to_remove * page_size;
    if (total_size % page_size != 0)
    {
        struct blocks *new_block = block_division(block, offset);
        new_block->status = 1;
        new_block->size = total_size - offset - sizeof(struct blocks);
        new_block->next = block->next;
        new_block->prev = block->prev;
        if (new_block->prev != NULL)
            new_block->prev->next = new_block;
        if (new_block->next != NULL)
            new_block->next->prev = new_block;
        block = new_block;
        return munmap(to_remove, offset);
    }
    block = block->next;
    if (block != NULL)
        block->prev = to_remove->prev;
    if (to_remove->prev != NULL)
        to_remove->prev->next = block;
    return munmap(to_remove, offset);
}
