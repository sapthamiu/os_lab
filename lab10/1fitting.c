#include<stdio.h>
#include<stdlib.h>
typedef enum{
	BEST_FIT, FIRST_FIT, NEXT_FIT, WORST_FIT
}Strategy;
typedef struct mab{
	int offset; //starting point
	int size; //total size of block
	int allocated; //1 if allocated, 0 if free
	struct mab* next; //pointer to next block
	struct mab* prev; //pointer to prev block
}Mab;
//check if memory is available
Mab* memCheck(Mab* m, int size, Strategy strategy){
	Mab* cur = m;
	Mab* result = NULL;
	switch(strategy){
		case BEST_FIT:
			while(cur){
				if(!cur->allocated && cur->size >= size)
					if(!result || cur->size < result->size)
						result = cur;
				cur = cur->next;
			} 
			return result;
		case FIRST_FIT:
			while(cur){
				if(!cur->allocated && cur->size >= size)
					return cur;
				cur = cur->next;
			}
			break;
		case NEXT_FIT:
			static Mab* last_alloc = NULL;
			if(!last_alloc) last_alloc = m;
			Mab* start = last_alloc;
			do{
				if(!last_alloc->allocated && last_alloc->size >= size){
					result = last_alloc;
					last_alloc = last_alloc->next ? last_alloc->next : m; //wrap around
					return result;
				}
			}while(last_alloc != start);
			return NULL;
		case WORST_FIT:
			while(cur){
				if(!cur->allocated && cur->size >= size)
					if(!result || cur->size > result->size)
						result = cur;
				cur = cur->next;
			}
			return result;
	}
	return NULL;
}

Mab* memSplit(Mab* block, int size){
	if (block->size <= size) return block;

    Mab* newBlock = (Mab*)malloc(sizeof(Mab));
    newBlock->offset = block->offset + size;
    newBlock->size = block->size - size;
    newBlock->allocated = 0;

    newBlock->next = block->next;
    newBlock->prev = block;
    if (block->next) block->next->prev = newBlock;
    block->next = newBlock;

    block->size = size;

    return block;
}

//allocate a memory block
Mab* memAlloc(Mab* m, int size, Strategy strategy){
	Mab* block = memCheck(m, size, strategy);
	if(!block) return NULL;
	//split if larger than needed
	block = memSplit(block, size);
	block->allocated = 1;
	return block;
}
Mab* memMerge(Mab* block){
	// Merge with next block if free
    if (block->next && block->next->allocated == 0) {
        Mab* nextBlock = block->next;
        block->size += nextBlock->size;
        block->next = nextBlock->next;
        if (nextBlock->next)
            nextBlock->next->prev = block;
        free(nextBlock); // free the block metadata, not actual memory
    }

    // Merge with previous block if free
    if (block->prev && block->prev->allocated == 0) {
        Mab* prevBlock = block->prev;
        prevBlock->size += block->size;
        prevBlock->next = block->next;
        if (block->next)
            block->next->prev = prevBlock;
        free(block);
        block = prevBlock;
    }

    return block;
}
//free memory block
Mab* memFree(Mab* block){
	if (block == NULL) return NULL;

		block->allocated = 0;  // mark as free
		block = memMerge(block);  // merge with neighbors if possible

		return block;
}
//merge two memory blocks

int main() {
    Mab* memory = malloc(sizeof(Mab));
    memory->offset = 0;
    memory->size = 1000;
    memory->allocated = 0;
    memory->next = memory->prev = NULL;

    Mab* a = memAlloc(memory, 200, FIRST_FIT);
    Mab* b = memAlloc(memory, 100, FIRST_FIT);
    Mab* c = memAlloc(memory, 300, FIRST_FIT);

    printf("Before free:\n");
    for (Mab* temp = memory; temp; temp = temp->next)
        printf("[%s: %d bytes]\n", temp->allocated ? "USED" : "FREE", temp->size);

    memFree(b); // free the middle block

    printf("\nAfter freeing middle block:\n");
    for (Mab* temp = memory; temp; temp = temp->next)
        printf("[%s: %d bytes]\n", temp->allocated ? "USED" : "FREE", temp->size);

    return 0;	
}