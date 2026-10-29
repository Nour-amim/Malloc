#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#define my_malloc malloc
#define my_free free
#define my_calloc calloc
#define my_realloc realloc

#include "../src/malloc.h"

#undef malloc
#undef free

// #include "../src/utils.h"

Test(malloc_testsuite, int_arrray)
{
    int *arr = my_malloc(10 * sizeof(int));
    cr_assert_not_null(arr, "fail malloc");
    arr[0] = 7;
    cr_expect(eq(int, arr[0], 7));
    my_free(arr);
}
Test(malloc_testsuite, char_arrray)
{
    char *arr = my_malloc(10 * sizeof(char));
    cr_assert_not_null(arr, "fail malloc");
    memset(arr, 'a', 10);
    // list_print(arr);
    my_free(arr);
}
Test(malloc_testsuite, float_arrray)
{
    float *arr = my_malloc(10 * sizeof(float));
    cr_assert_not_null(arr, "fail malloc");
    arr[3] = 1.5;
    cr_assert(arr[3] == 1.5);
    my_free(arr);
}
Test(malloc_testsuite, big_char_arrray)
{
    char *arr = my_malloc(100000 * sizeof(char));
    cr_assert_not_null(arr, "fail malloc");
    memset(arr, 0x11, 100000);
    my_free(arr);
}
Test(malloc_testsuite, multiple_malloc)
{
    char **arr = my_malloc(8 * sizeof(char *));
    cr_assert_not_null(arr, "fail malloc");
    for (size_t i = 0; i < 8; i++)
    {
        arr[i] = my_malloc(10000 * (i + 1) + i);
        cr_assert_not_null(arr[i]);
        memset(arr[i], i + 'a', 10000 * (i + 1) + i);
        // printf("%zu iteration\n", i);
        // printf("%c\n", arr[i][10000*(i+1) - i-1]);
    }
    for (size_t i = 0; i < 8; i++)
        my_free(arr[i]);
    my_free(arr);
}
Test(malloc_testsuite, multiple_malloc_free)
{
    char **arr = my_malloc(10 * sizeof(char *));
    cr_assert_not_null(arr, "fail malloc");
    for (size_t i = 0; i < 10; i++)
    {
        arr[i] = my_malloc(10 * (i + 1) + i);
        cr_assert_not_null(arr[i]);
        memset(arr[i], i + 'a', 10 * (i + 1) + i);
        // printf("%c\n", arr[i][10*(i+1) - i-1]);
        my_free(arr[i]);
    }
    my_free(arr);
}
