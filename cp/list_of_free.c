#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "list_of_free.h"


list_allocator* list_alloc_create(uint64_t byte_count) {
    void *memory = calloc(byte_count, 1);
    assert(memory != NULL);

    block *blocks = calloc(byte_count, sizeof(block));
    assert(blocks != NULL);

    for (uint64_t i = 0; i < byte_count; ++i) {
        blocks[i].memory = memory + i;
    }

    uint64_t *free = calloc(byte_count, sizeof(uint64_t));
    for (uint64_t i = 0; i < byte_count; ++i) {
        free[i] = byte_count - i;
    }

    list_allocator* ba = calloc(sizeof(list_allocator), 1);

    ba->blocks = blocks;
    ba->free = free;
    ba->byte_count = byte_count;

    return ba;
}

list_allocator* list_alloc_create_with_block_size(uint64_t block_count, uint64_t block_size) {
    return list_alloc_create(block_count * block_size);
}

void list_alloc_destroy(list_allocator *ba) {
    free(ba->blocks[0].memory);
    free(ba->blocks);
    free(ba->free);
}

void* list_alloc_allocate(list_allocator *ba, uint64_t bytes_needed) {
    void *result = NULL;

    for (uint64_t i = 0; i < ba->byte_count; ++i) {
        if (ba->free[i] < bytes_needed) {
            continue;
        }

        result = ba->blocks[i].memory;
        for (uint64_t j = 0; j < bytes_needed; ++j) {
            ba->free[i + j] = 0;
            ba->blocks[i + j].taken = bytes_needed - j;
        }

        break;
    }

    return result;
}

void* list_alloc_deallocate(list_allocator *ba, void *block) {
    for (uint64_t i = 0; i < ba->byte_count; ++i) {
        if (block != ba->blocks[i].memory) continue;

        uint64_t bytes_taken = ba->blocks[i].taken;
        for (uint64_t j = 0; j < bytes_taken; ++j) {
            ba->blocks[i + j].taken = 0;
            ba->free[i + j] = bytes_taken - j;
        }

        if (i + bytes_taken < ba->byte_count && ba->free[i + bytes_taken] != 0) {
            for (uint64_t j = bytes_taken; j > 1; --j) {
                ba->free[i + j - 1] = ba->free[i + j] + 1;
            }
        }

        ++i;
        for (; i > 1 && ba->free[i - 1 - 1] != 0; --i) {
            ba->free[i - 1 - 1] = ba->free[i - 1] + 1;
        }

        break;
    }

    return NULL;
}

void list_alloc_print(list_allocator ba) {
    printf("ba = {\n");
    printf("\tfree = {\n\t\t");
    for (uint64_t i = 0; i < ba.byte_count; ++i) {
        printf("%"PRIu64", ", ba.free[i]);
    }
    printf("\b\b \b\n\t}\n");


    printf("\tbyte_count = %"PRIu64"\n", ba.byte_count);

    printf("}\n");
}