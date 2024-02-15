#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>

int *counter;
pid_t child_pid = 0;

void handle_sigint(int sig) {
    pid_t pid;

    if (child_pid > 0) {
        printf("\nTerminating old process. PID: %d\n", child_pid);
        kill(child_pid, SIGTERM);
    }
    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    } else if (pid == 0) { 
        printf("New process created. PID: %d\n", getpid());
    } else {
        child_pid = pid; 
    }
}

int main() {
    int shmid;

    shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    if (shmid < 0) {
        perror("shmget");
        exit(1);
    }

    counter = shmat(shmid, NULL, 0);
    if (counter == (int *) -1) {
        perror("shmat");
        exit(1);
    }

    *counter = 0;

    signal(SIGINT, handle_sigint);

    printf("Initial process. PID: %d\n", getpid());

    while (1) {
        printf("Counter: %d\n", (*counter)++);
        sleep(1); 
    }

    shmdt(counter);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
