//Brandon Renner - OS Producer/Consumer Assignment

//Produer Program

#include <iostream>
#include <cstdlib>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <cstring>

//structure for shared memory
struct SharedData {
    int table [2];
    int counter;
    int temp;
};

int main() {
    //name for shared memory
    const char* shm_name = "/shared_memory";
    //name for shared semaphore
    const char* sem_name = "/semaphore";

    //create and open shared memory block by name, with read/write permissions
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    //truncate shared memory to size of desired structure
    ftruncate(shm_fd, sizeof(SharedData));

    //map shared data for use by program
    SharedData *shared_data = (SharedData *)mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shared_data == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    //open shared semaphore with desired name
    sem_t *sem = sem_open(sem_name, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    
    //-----------------Producer functionality--------------------
    
    //set counter to zero
    shared_data->counter = 0;

    //set temp to zero - DETERMINES HOW MANY CRITICAL SECTIONS EXECUTE
    shared_data->temp = 0;

    //initialize random number seed
    srand(23109);

    //initialize producer index
    int in = 0;

    while(shared_data->temp < 15) {
        //produce next item
        int nextp = rand() % 10 + 1;

        //check buffer, if full busy wait
        while(shared_data->counter == 2) {
            continue;  //busy wait
        }

        //write to buffer - critical section with semaphore usage
        if (sem_wait(sem) == -1) {
            perror("sem_wait");
            exit(EXIT_FAILURE);
        }

        shared_data->table[in] = nextp;
        std::cout << "Producer produced data: " << shared_data->table[in] << std::endl;
        shared_data->counter++;
        shared_data->temp++;

        if (sem_post(sem) == -1) {
            perror("sem_post");
            exit(EXIT_FAILURE);
        }

        //update producer index
        in = (in + 1) % 2;
    }

    // Close semaphore
    if (sem_close(sem) == -1) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }

    return 0;
}