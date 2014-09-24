#pragma once

#ifndef __MEMORY_MANAGER_H__
#define __MEMORY_MANAGER_H__

//author Hitesh Vyas
// DO NOT CHANGE THIS HEADER FILE
namespace MemoryManager
{

  // Initialize any data needed to manage the memory pool
  void initializeMemoryManager(void);

  // return a pointer inside the memory pool
  // If no chunk can accommodate aSize call OnAllocFail()
  void* allocate(int aSize);

  // Free up a chunk previously allocated
  void deallocate(void* aPointer);

  // Will scan the memory pool and return the total free space remaining
  int freeRemaining(void);

  // Will scan the memory pool and return the largest free space remaining
  int largestFree(void);

  // will scan the memory pool and return the smallest free space remaining
  int smallestFree(void);

  // Call if no space is left for the allocation request
  void onOutOfMemory(void);

  // Call if a pointer over run condition is detected
  void onOverrunDetected(void);

  // If the caller makes any illegal request your code should call this 
  // provided failure function (which will not return):
  void onIllegalOperation(const char* fmt,...);


};


#endif  // __MEMORY_MANAGER_H__
