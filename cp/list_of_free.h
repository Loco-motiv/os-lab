#ifndef LIST_OF_FREE
#define LIST_OF_FREE

#include "lib.h"


typedef struct {
    block *blocks;
    uint64_t *free;
    uint64_t byte_count;
} list_allocator;


list_allocator* list_alloc_create(uint64_t byte_count);
list_allocator* list_alloc_create_with_block_size(uint64_t block_count, uint64_t block_size);
void list_alloc_destroy(list_allocator *ba);

void* list_alloc_allocate(list_allocator *ba, uint64_t byte_count);
void* list_alloc_deallocate(list_allocator *ba, void *block);

void list_alloc_print(list_allocator ba);

#endif // LIST_OF_FREE