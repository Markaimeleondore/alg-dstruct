#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <assert.h>
#ifndef __MEMALLOCATOR_H__E71A34CB
#define __MEMALLOCATOR_H__E71A34CB

#define structsize sizeof(mb)


#ifdef __cplusplus
extern "C" {
#endif

/*
Реализуйте систему управления памятью, построенную на списках пустых блоков. Используйте
двухсвязный список и стратегию “лучший подходящий”.
Напишите функции выделения и освобождения памяти блоками произвольного размера.
Сравните производительность вашей системы с стандартным malloc/free
*/



typedef struct mem_block
{
    int avail;
    int size;
    struct mem_block *next;
    struct mem_block *prev;
} mb;


static int mem_allowed = 0; //Memory required size
       int init_checker = 0;

static mb* mem_beginning; //First block init



int memgetblocksize()
{
    return structsize;
}

int memgetminimumsize()
{
    return structsize;
}




int current_free_memory()
{

    if(mem_beginning == NULL)
        return -1;

    mb* cur = mem_beginning;
    int cur_size = 0;
    while(cur != NULL)
    {
        if(cur->avail == 1)
        {
            cur_size += (cur->size + sizeof(mb));
        }
        cur = cur->next;
    }
    return cur_size;

}





// Init memory system with memory block pMemory.
int meminit(void *pMemory, int size)
{
    if(pMemory == NULL || size < structsize)
        return -1;

    if(init_checker)
        return -1;


    mem_beginning = (mb*)pMemory;

    mem_beginning->size = size - structsize ;
    mem_beginning->avail = 1;
    mem_beginning->next = NULL;
    mem_beginning->prev = NULL;

    mem_allowed = size;

    init_checker = 1;

    return size;
}



// You can implement memory leak checks here
void memdone()
{
    if(mem_beginning == NULL)
        return;

    int cur_mem_size = current_free_memory();

    if(cur_mem_size == -1)
        return;

    if(cur_mem_size < mem_allowed)
    {
        fprintf(stderr, "Oh no, where is my memory?! %d allowed, but only %d availiable now \n", mem_allowed, cur_mem_size);
    }
    init_checker = 0;
    mem_allowed = 0;
    mem_beginning = NULL;
}





mb* seekin_for_the_best_fittin(int size)
{
    if(mem_beginning == NULL)
    {
        fprintf(stderr, "Fuck my life\n");
        return NULL;
    }


    mb* cur = mem_beginning;
    mb* cur_best = NULL;
    int cur_best_abs = mem_allowed + 1;

    while(cur)
    {
        if(cur->avail == 1)
        {
            if(cur->size >= size && cur_best_abs > abs(size - cur->size)) //Best-fitting detection
            {
                    cur_best = cur;
                    cur_best_abs = abs(size - cur_best->size);
            }
        }
        cur = cur->next;
    }
        //printf("Size %d", cur_best->size);
        return cur_best;
}




//void* correcting_pointer_for_memloc(void* p)
//{
//    mb* cur = mem_beginning;
//    void* cur_ptr = NULL;
//    while(cur)
//    {
//        cur_ptr = (void*)(cur + 1);
//        if(cur_ptr == p)
//        {
//            return cur_ptr;
//        }
//        cur = cur->next;
//    }
//    return NULL;
//}





// Allocate memory block of size 'size'.
// Returns pointer to memory block is success, 0 otherwise
void *memalloc(int size)
{
    mb* best_fit = seekin_for_the_best_fittin(size);


    if(best_fit == NULL)
    {
        fprintf(stderr, "NEM, honey\n");
        return 0;
    }

    void* best_fit_ptr = (void*)(best_fit + 1);

   // printf("%d",best_fit->size);

    if(best_fit->size > structsize + size)
    {
        mb* trash_block = (mb*)((char*)best_fit_ptr + size);

        trash_block->size = best_fit->size - structsize - size;
        trash_block->next = best_fit->next;
        trash_block->prev = best_fit;
        trash_block->avail = 1;

        if(best_fit->next != NULL)
        {
            best_fit->next->prev = trash_block;
        }
        best_fit->next = trash_block;
    }


    best_fit->size = size;
    best_fit->avail = 0;

    return best_fit_ptr == NULL ? 0 : best_fit_ptr;
}



int does_point_to_the_block(void* p)
{
    if(p == NULL)
    {
        fprintf(stderr,"Haha, nice try!\n");
        return 0;
    }



    mb* cur = mem_beginning;

    mb* pointed_block = (mb*)p - 1;

    while(cur)
    {
        if(cur == pointed_block)
        {
            if(pointed_block == NULL)
            {
                fprintf(stderr,"Sir, U've missed with the pointer!\n");
                return 0;
            }
            if(pointed_block->avail == 1)
            {
                fprintf(stderr,"This block is already availiable!\n");
                return 0;
            }

            return 1;
        }

        cur = cur->next;
    }
    return 0;
}





// Free memory previously allocated by memalloc
void memfree(void *p)
{

    mb* pointed_block = does_point_to_the_block(p) ? (mb*)p - 1 : NULL;
    if(!pointed_block)
        return;

    //----------merging------------//

    pointed_block->avail = 1;
    mb* new_free_block = pointed_block; //Block for extra memory and for probable merging
    //new_free_block->avail = 1;


    if(pointed_block->prev != NULL && pointed_block->prev->avail == 1)
    {

        int pointed_block_size;
        if(pointed_block->next)
            pointed_block_size = (char*)pointed_block->next - (char*)(pointed_block->prev + 1);
        else
            pointed_block_size = (char*)(mem_beginning + 1) + mem_allowed - (char*)(pointed_block->prev + 1);

        pointed_block->prev->next = pointed_block->next;

        if(pointed_block->next)
        {
        pointed_block->next->prev = pointed_block->prev;
        }


        pointed_block->prev->size = pointed_block_size;

        new_free_block = pointed_block->prev;

    }


    if(new_free_block->next != NULL && new_free_block->next->avail == 1)
    {
        int new_block_size;

        if(new_free_block->next->next)
            new_block_size = (char*)new_free_block->next->next - (char*)(new_free_block + 1);
        else
            new_block_size = (char*)(mem_beginning + 1) + mem_allowed - (char*)(new_free_block + 1);

        if(new_free_block->next->next != NULL)
        {
            new_free_block->next->next->prev =  new_free_block;
        }


        new_free_block->next =  new_free_block->next->next;

        new_free_block->size = new_block_size;
    }


    //lost parts grabbing
    void* edge_of_this_block_ptr = (void*)((char*)(new_free_block + 1) + new_free_block->size);
    void* edge_of_mem_ptr = (void*)((char*)mem_beginning + mem_allowed);

    if(new_free_block->next)
    {
        if(edge_of_this_block_ptr != new_free_block->next)
            new_free_block->size += (char*)new_free_block->next - (char*)edge_of_this_block_ptr;
    }
    else
    {
        if(edge_of_mem_ptr != edge_of_this_block_ptr)
            new_free_block->size += ((char*)mem_beginning + mem_allowed - (char*)edge_of_this_block_ptr);
    }
}


// Return minimum size in bytes of the memory pool to allocate 0-bytes block
// use case:
// void *p   = 0;
// void *ptr = malloc(memgetminimumsize() + 1);
// meminit(ptr, memgetminimumsize() + 1)
// p = memalloc(1); // Success!
// memfree(p);
// memdone();
// free(ptr);




// Returns size in bytes of additional information per allocation
// use case:
// void *p1 = 0, *p2 = 0;
// int  memsize = memgetminimumsize() + memgetblocksize() + 2;
// void *ptr = malloc(memsize);
// meminit(ptr, memsize())
// p1 = memalloc(1); // Success!
// p2 = memalloc(1); // Success!
// memfree(p2);
// memfree(p1);
// memdone();
// free(ptr);



#ifdef __cplusplus
}
#endif

#endif

