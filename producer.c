// Producer Program for OS


#include <iostream>
#include <cstdlib> 

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "Shared_memory.h"

using std::cout; using std::endl;  

int main() {

        int            fd;
        char           *shmpath;
        struct shmbuf  *shmp;

        shmpath = "~/cs23001/OS_code/temp";
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


        /* Initialize semaphore as process-shared, with value 0, then signal */
        if (sem_init(&shmp->sem, 1, 0) == -1) {
                errExit("sem_init");
        }

        sem_post(&shmp->sem);

        /* initialize in out counter */

        shmp->in = 0;
        shmp->out = 0;
        shmp->counter = 0;

        /* proceed with producer operation */

        srand(23109);
        while (true) {
                //produce item nextp
                int nextp = rand() % 10 + 1;  

                //check to make sure buffer isnt full
                while (shmp->counter == TABLE_SIZE) { 
                        continue;
                }

                //critical section: write to buffer, change counter variable, with semaphore usage

                sem_wait(&shmp->sem);
                shmp->table[shmp->in] = nextp;
                cout << "integer " << nextp << " was produced" << endl;
                shmp->counter++;
                sem_post(&shmp->sem);

                shmp->in = (shmp->in + 1) % TABLE_SIZE;
        }
}