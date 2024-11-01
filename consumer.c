// Consumer Program for OS

#include <iostream>

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>

#include "shared_memory.h"

using std::cout; using std::endl;

int main() {

        int            fd;
        char           *shmpath;
        size_t         len;
        struct shmbuf  *shmp;

        shmpath = "~/cs23001/OS_code/temp";

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

                //check to make sure buffer isnt empty
                while (shmp->counter == 0) {  
                        continue;
                }

                //critical section: change counter, get item from buffer, nextc, with semaphore useage

                sem_wait(&shmp->sem);
                int nextc = shmp->table[shmp->out];
                cout << "integer " << nextc << " was consumed" << endl; 
                shmp->table[shmp->out] = 0;
                shmp->counter--;
                sem_post(&shmp->sem);

                shmp->out = (shmp->out + 1) % TABLE_SIZE; 
        }
}
