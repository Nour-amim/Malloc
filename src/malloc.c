#include "malloc.h"

#include <errno.h>
#include <stddef.h>
#include <string.h>

#include "allocation.h"
#include "page_manage.h"
static struct blocks g_pages = { .status = 0,
                                 .size = 0,
                                 .next = NULL,
                                 .prev = NULL };
__attribute__((visibility("default"))) void *malloc(size_t size)
{
    if (g_pages.next == NULL)
        g_pages.next = blocks_new(size);
    if (g_pages.next == NULL)
        return NULL;
    g_pages.next->prev = &g_pages;
    void *res = allocate(size, g_pages.next);
    return res;
}

__attribute__((visibility("default"))) void free(void *ptr)
{
    free_space(ptr);
}

__attribute__((visibility("default"))) void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return malloc(size);
    if (size == 0)
    {
        free(ptr);
        return NULL;
    }
    struct blocks *src = (struct blocks *)ptr - 1;
    struct blocks *dst = malloc(size);
    if (dst == NULL)
        return NULL;
    size_t min = src->size;
    if (min > size)
        min = size;
    memcpy(dst, ptr, min);
    free(ptr);
    return dst;
}

__attribute__((visibility("default"))) void *calloc(size_t nmemb, size_t size)
{
    size_t res;
    if (__builtin_umull_overflow(nmemb, size, &res) == 1)
    {
        errno = ENOMEM;
        return NULL;
    }
    void *ptr = malloc(res);
    if (ptr == NULL)
        return NULL;
    void *ptr_res = memset(ptr, 0, res);
    return ptr_res;
}
