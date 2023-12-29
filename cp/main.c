#include <stdio.h>
#include <stdlib.h>

#include "list_of_free.h"
#include "time.h"
#include "buddy.h"

void test_buddy()
{
    clock_t start;
    printf("Buddy_allocator:\n");
    buddy_allocator *buda = buddy_create(1024 * 1024);
    int buddy_time;
    int buddy_time1;
    for (int i = 0; i < 1; i++)
    {
        void** blocks = calloc(100, 1024);
        for (int j = 0; j < 100; j++){
            start = clock();
            blocks[j] = buddy_allocate(buda, closest_pow2(rand() % 1000));
            buddy_time += (clock() - start);
        }
        for (int j = 0; j < 100; j++){
            start = clock();
            buddy_deallocate(buda, blocks[j]);
            buddy_time1 += (clock() - start);
        }
        free(blocks);
        printf("Time to allocate: %f, time to deallocate: %f", ((double) buddy_time) / CLOCKS_PER_SEC, ((double) buddy_time1) / CLOCKS_PER_SEC);
    }
    buddy_destroy(buda);
    free(buda);
    printf("\n");
}

void test_list_alloc()
{
    clock_t start;
    printf("Free lists allocator:\n");
    list_allocator *lisa = list_alloc_create(1024 * 1024);
    int buddy_time;
    int buddy_time1;
    for (int i = 0; i < 1; i++)
    {
        void** blocks = calloc(100, 1024);
        for (int j = 0; j < 100; j++){
            start = clock();
            blocks[j] = list_alloc_allocate(lisa, closest_pow2(rand() % 1000));
            buddy_time += (clock() - start);
        }
        for (int j = 0; j < 100; j++){
            start = clock();
            list_alloc_deallocate(lisa, blocks[j]);
            buddy_time1 += (clock() - start);
        }
        free(blocks);
        printf("Time to allocate: %f, time to deallocate: %f", ((double) buddy_time) / CLOCKS_PER_SEC, ((double) buddy_time1) / CLOCKS_PER_SEC);
    }
    list_alloc_destroy(lisa);
    free(lisa);
    printf("\n");
}

int main()
{
    srand(time(NULL)); 
    test_buddy();
    test_list_alloc();

    return 0;
}