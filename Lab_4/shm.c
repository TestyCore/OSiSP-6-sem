#include <stdio.h>
#include <sys/shm.h>
#include "shm.h"

int create_shared_memory(size_t size) {
    return shmget(IPC_PRIVATE, size, IPC_CREAT | 0666);
}

int *attach_shared_memory(int shmid) {
    return shmat(shmid, NULL, 0);
}

void detach_shared_memory(int *mem) {
    shmdt(mem);
}

void remove_shared_memory(int shmid) {
    shmctl(shmid, IPC_RMID, NULL);
}
