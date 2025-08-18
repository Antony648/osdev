#ifndef	HEAP_CREAM_H
#define HEAP_CREAM_H
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#define MAX_4KB_HEAP  5
void heap_cream_init(uintptr_t* karray);
void* heap_cream_malloc(uintptr_t*karray,size_t size);
bool heap_cream_free(uintptr_t* karray,void* ptr);
void heap_cream_destroy(uintptr_t * karray);

#endif
