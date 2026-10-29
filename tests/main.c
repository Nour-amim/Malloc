
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>

#include "../src/allocation.h"
#define malloc my_malloc
#define free my_free
#define calloc my_calloc
#define realloc my_realloc

// #include "../src/malloc.c"
#undef malloc
#undef free
#undef calloc
#undef realloc

static struct blocks g_pages = { .status = 0,
                                 .size = 0,
                                 .next = NULL,
                                 .prev = NULL };
int main(void)
{
    g_pages.next = blocks_new(10);
    g_pages.next->prev = &g_pages;
    printf("g_pages : %p", (void *)&g_pages);
    printf("g_pages : %p", (void *)g_pages.next->prev);
    munmap(g_pages.next, g_pages.next->size + sizeof(struct blocks));
    return 0;
}
