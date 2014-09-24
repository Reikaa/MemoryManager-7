#include "MemoryManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "Main.h"

//author Hitesh Vyas
namespace MemoryManager {

const int MM_POOL_SIZE = 65536;
char MM_pool[MM_POOL_SIZE];
using namespace std;

void initializeMemoryManager(void) {
	char* temp = MM_pool;
	temp = (char*) malloc(MM_POOL_SIZE * sizeof(char));
	int* t = (int*) MM_pool;
	*t = -65531;
}

// return a pointer inside the memory pool
// If no chunk can accommodate aSize call onOutOfMemory()
void* allocate(int aSize) {
	int MM_Index = 0;
	//convert memory to int pointer to store integers
	int* int_pointer = (int*) MM_pool;
	//get the chunk size at that index
	int chunkSize = int_pointer[MM_Index];
	char* returnPointer;
	//Initialize the pointer to return
	returnPointer = &MM_pool[MM_Index];

	//while it is in the memory pool
	while (MM_Index < MM_POOL_SIZE) {
		/*
		 * if chunk size is -ve, it is free
		 */
		if (chunkSize < 0) {
			//get the free size +ve
			chunkSize = -chunkSize;

			//if we can accomodate the input
			if ((chunkSize - 4) >= aSize) {
				//assign chunk size

				int_pointer = (int*) &MM_pool[MM_Index];
				//assign chunk size
				*int_pointer = aSize;
				//go to the chuck
				MM_Index = MM_Index + 4;

				//return address
				returnPointer = &MM_pool[MM_Index];

				//update next chunk which is unoccupied
				MM_Index = MM_Index + aSize;
				int_pointer = (int*) &MM_pool[MM_Index];
				*int_pointer = aSize - chunkSize;

				return (void*) returnPointer;

			} else if (chunkSize == aSize) {
				//if chunk size perfectly fits in no need to update next chunk size

				int_pointer = (int*) &MM_pool[MM_Index];
				//update the size of chunk in memory loop
				*int_pointer = aSize;
				MM_Index = MM_Index + 4;

				returnPointer = &MM_pool[MM_Index + 4];
				return (void*) returnPointer;

			} else {
				//if chuck size is smaller, go to next chunk
				MM_Index = MM_Index + 4;
				MM_Index = MM_Index + chunkSize;
			}
		} else {
			//if filled memory, go to next chunk
			MM_Index = MM_Index + 4;
			MM_Index = MM_Index + chunkSize;
		}
		//recalculate the values of next chunk
		int_pointer = (int*) &MM_pool[MM_Index];
		chunkSize = *int_pointer;
	}

	//should not come here
	cout << "outof memory " << endl;
	//return garbage
	return (void*) returnPointer;

}

// Free up a chunk previously allocated
void deallocate(void* aPointer) {

	int MM_index = 0;
	int* chunk_pointer = (int*) MM_pool;
	int chuck_size = *chunk_pointer;
	int* to_deallocate = (int*) aPointer;
	int* previous_chuck=0;

	while (MM_index < MM_POOL_SIZE) {

		//if chuck is allocated
		if (chuck_size > 0) {
			//the chuck
			chunk_pointer = (int*) &MM_pool[MM_index + 4];
			//if they are same
			if (to_deallocate == chunk_pointer) {

				chunk_pointer = (int*) &MM_pool[MM_index];
				//make it free
				*chunk_pointer = -(*chunk_pointer);
				//free the pointer
				free(aPointer);
				//if next and/or previous chunk is free (recalculate size)
				MM_index = MM_index + 4;
				MM_index = MM_index + chuck_size;
				//next chuck
				int* next_chuck = (int *) &MM_pool[MM_index];
				if (MM_index < MM_POOL_SIZE && *next_chuck < 0) {
					//update chuck size
					*chunk_pointer = *chunk_pointer + *next_chuck;
				}
				//if previous chuck is available
				if(previous_chuck){
					if(*previous_chuck<0){
						//update the chuck size
						*previous_chuck=*previous_chuck+*chunk_pointer;
					}
				}

				return;

			} else {
				//if it is not the chunk to deallocate,go to next chunk
				MM_index = MM_index + 4;
				MM_index = MM_index + chuck_size;
				//update the values
				previous_chuck=chunk_pointer;
				chunk_pointer = (int*) &MM_pool[MM_index];
				chuck_size = *chunk_pointer;

			}
		} else {
			//if the chuck is free, go to the next chunk
			chuck_size = -chuck_size;
			MM_index = MM_index + 4;
			MM_index = MM_index + chuck_size;
			//update the values
			previous_chuck=chunk_pointer;
			chunk_pointer = (int*) &MM_pool[MM_index];
			chuck_size = *chunk_pointer;
		}
	}
	//control should never come to this point
	cout << "should not come here" << endl;
}

//---
//--- support routines
//---

// Will scan the memory pool and return the total free space remaining
int freeRemaining(void) {
	//initialize the variables
	int free_memory = 0;
	int MM_index= 0;
	//chunk pointer
	int* chunk = (int*) MM_pool;
	//initial chunk size
	int chunk_size = *chunk;

	while (MM_index < MM_POOL_SIZE) {
		//if the chunk is deallocated
		if (chunk_size < 0) {
			chunk_size = -chunk_size;
			//update the free memory size
			free_memory = free_memory + chunk_size;
		}
		//go to next chunk
		MM_index = MM_index + 4;
		MM_index = MM_index + chunk_size;
		//update the pointer to next chunk and its size
		chunk = (int*) &MM_pool[MM_index];
		chunk_size = *chunk;
	}

	return free_memory;
}

// Will scan the memory pool and return the largest free space remaining
int largestFree(void) {
	//index
	int MM_index = 0;
	//1st chunk pointer
	int* chunk = (int*) MM_pool;
	//initial chunk size
	int chunk_size = *chunk;
	//largest chunk free , let it be 0
	int largest_free = 0;

	while (MM_index < MM_POOL_SIZE) {
		//if chunk is free
		if (chunk_size < 0) {
			chunk_size = -chunk_size;
			//if the size is greater than the largest chunk size so far
			if (largest_free < chunk_size) {
				//update it
				largest_free = chunk_size;
			}
		}
		//go to next chunk
		MM_index = MM_index + 4;
		MM_index = MM_index + chunk_size;
		//update the pointer to the next chunk
		chunk = (int*) &MM_pool[MM_index];
		chunk_size = *chunk;
	}
	return largest_free;
}

// will scan the memory pool and return the smallest free space remaining
int smallestFree(void) {
	//index to memory pool
	int MM_index = 0;
	//first chunk
	int* chunk = (int*) MM_pool;
	//initial chunk size
	int chunk_size = *chunk;
	//let smallest be whole chunk
	int smallest_free = 65536;

	while (MM_index < MM_POOL_SIZE) {
		//if chunk is available
		if (chunk_size < 0) {
			chunk_size = -chunk_size;
			//if smallest chunk is greater than the current chunk
			if (smallest_free > chunk_size) {
				//update it
				smallest_free = chunk_size;
			}
		}
		//next chunk
		MM_index = MM_index + 4;
		MM_index = MM_index + chunk_size;
		//update pointer to the next chunk
		chunk = (int*) &MM_pool[MM_index];
		chunk_size = *chunk;
	}

	return smallest_free;
}
}
