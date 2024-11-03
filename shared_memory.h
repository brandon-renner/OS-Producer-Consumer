#ifndef SHARED_MEM_H
#define SHARED_MEM_H

#include <semaphore.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

#define TABLE_SIZE 2   /* Maximum size for exchanged string */

/* Define a structure that will be imposed on the shared memory object */

struct shmbuf {
    int in;                      /* in index */
    int out;                     /* out index */
    int counter;                 /* counter tracking variable */
    sem_t  sem;                  /* POSIX unnamed semaphore */
    int   table[TABLE_SIZE];     /* Data being transferred */
};

#endif