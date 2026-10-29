# Custom Malloc (Linked-List Implementation)

This project is a custom implementation of the standard C memory allocation library (`malloc`, `calloc`, `realloc`, and `free`).

It relies on the `mmap` (and `munmap`) system calls and uses a **Segregated Free List** architecture (commonly known as **Buckets**) to ensure optimal allocation and deallocation performance.

## Features

* **Standard Allocation**: Complete implementation of `malloc`, `free`, `realloc`, and `calloc`.
* **Linked-List Architecture**
* **Memory Optimization**:
  * *Auto-Cleanup*: As soon as a 4096-byte page (composed of multiple smaller blocks) is entirely freed, it is returned to the OS via `munmap`.


## Compilation and Usage

A `Makefile` is provided to easily build and manage the project.

To compile the shared library (`libmalloc.so`):
```bash
make library
```

To clean up the generated build and test files:
```bash
make clean
```

To use this implementation instead of the standard glibc `malloc`, you simply need to set the `LD_PRELOAD` environment variable when launching your executable:
```bash
LD_PRELOAD=./libmalloc.so ./your_program
```

## Testsuite

This project comes with a **very extensive testsuite** that relies on a functional bash script as well as unit tests written with the **Criterion** framework and analyzed with **AddressSanitizer (ASan)**.

The tests cover:
* Basic allocation tests (`malloc`, `free`).
* Data integrity checks with `calloc` (zero-initialization).
* Memory resizing tests with `realloc` (expanding and shrinking, data copy verification).


To build the library and run the entire test suite automatically, run:
```bash
make check
```


## Limitations and Future Improvements

* Currently, the implementation is potentially not *thread-safe* (there are no mutexes protecting the global variable `g_list_free`). Adding locks (such as `pthread_mutex_t`) would make it safe to use in a multi-threaded context.
