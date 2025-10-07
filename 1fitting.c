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
// typedef Mab* MabPtr;
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
//allocate a memory block
Mab* memAlloc(Mab* m, int size, Strategy strategy){
	Mab* block = memCheck(m, size, strategy);
	if(!block) return NULL;
	//split if larger than needed
	if(block->size > size){
		Mab* newblock = (Mab*)malloc(sizeof(Mab));
		newblock->offset = block->offset + size;
		newblock->size = block->size - size;
		newblock->allocated = 0;

		newblock->next = block->next;
		newblock->prev= block;
		if(block->next) block->next->prev = newblock;
		block->next newblock;
	}
}
//free memory block
Mab* memFree(Mab* m){

}
//merge two memory blocks
Mab* memMerge(Mab* m){

}
Mab* memSplit(Mab* m, int size){

}