#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <sys/types.h>

int create_shared_memory(size_t size);
int *attach_shared_memory(int shmid);
void detach_shared_memory(int *mem);
void remove_shared_memory(int shmid);

#endif
