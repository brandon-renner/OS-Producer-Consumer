//Producer Program for OS 

#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <stddef.h>
#include <string.h>

#include "shared_memory.h" 

int main() {

        int            fd;
        const char     *shmpath = "my_shared_memory";
        struct shmbuf  *shmp;

       /* Create shared memory object and set its size to the size of our structure. */

       /* Create shared memory object and set its size to the size of our structure. */

        fd = shm_open(shmpath, O_CREAT | O_RDWR, 0666);
        if (fd == -1) {
                errExit("shm_open");
        }

        if (ftruncate(fd, sizeof(struct shmbuf)) == -1) {
                errExit("ftruncate");
        }


        /* Map the object into the caller's address space. */

        shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (shmp == MAP_FAILED) {
                errExit("mmap");
        }


        /* Initialize semaphore as process-shared, with value 0, then signal to increment to 1*/
        if (sem_init(&shmp->sem, 1, 0) == -1) {
                errExit("sem_init");
        }

        sem_post(&shmp->sem);

        /* initialize: in out counter */

        shmp->in = 0;
        shmp->out = 0;
        shmp->counter = 0;

        /* proceed with producer operation */

        srand(23109);
        while (true) {
                //produce item nextp
                int nextp = rand() % 10 + 1;

                //check buffer, busy wait if full
                while (shmp->counter == TABLE_SIZE) {
                        printf("Buffer is full");
                        printf("\n");  
                        continue;   // busy wating
                }

                //critical section: write to buffer, change counter variable, with semaphore usage

                sem_wait(&shmp->sem);
                shmp->table[shmp->in] = nextp;
                printf("integer ");
                printf("%d", nextp);
                printf(" was produced");
                printf("\n");
                shmp->counter++;
                sem_post(&shmp->sem);

                //update in index
                shmp->in = (shmp->in + 1) % TABLE_SIZE;
        }
}