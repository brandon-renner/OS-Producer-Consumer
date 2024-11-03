//Consumer Program for OS 

#include <stdbool.h>
#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

#include "shared_memory.h"

int main() {

        int            fd;
        const char     *shmpath = "/my_shared_memory";
        size_t         len;
        struct shmbuf  *shmp;

        /* Open the existing shared memory object and map it into the caller's address space. */

        fd = shm_open(shmpath, O_RDWR, 0);
        if (fd == -1) {
                errExit("shm_open");
        }

        shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (shmp == MAP_FAILED) {
                errExit("mmap");
        }

        /* proceed with consumer operation */

        while (true) {

                while (shmp->counter == 0) {
                        printf("Buffer is empty");
                        printf("\n");
                        continue;   // busy wating
                }

                //critical section: change counter, get item from buffer, nextc, with semaphore useage

                sem_wait(&shmp->sem);
                int nextc = shmp->table[shmp->out];
                printf("integer ");
                printf("%d", nextc);
                printf(" was consumed");
                printf("\n");
                shmp->table[shmp->out] = 0;
                shmp->counter--;
                sem_post(&shmp->sem);

                //change out index
                shmp->out = (shmp->out + 1) % TABLE_SIZE;
        }
}
