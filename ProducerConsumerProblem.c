
#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // for sleep

buffer_item buffer[BUFFER_SIZE];
int fill = 0;
int use = 0;

void *producer(void *param);
void *consumer(void *param);

int loops = 2;

sem_t empty;
sem_t full;
sem_t mutex;

int insert_item(buffer_item item) {
    /* Acquire Empty Semaphore */
    sem_wait(0);
    sem_wait(&mutex);

    /* Acquire mutex lock to protect buffer */
    pthread_mutex_lock(&mutex);

    /* Insert item into buffer */
    buffer[fill] = item;
    fill = (fill + 1) % BUFFER_SIZE;

    /* Release mutex lock and full semaphore */
    pthread_mutex_unlock(&mutex);
    sem_post(&mutex);
    sem_post(&full);

    return item;
}

int remove_item(buffer_item *item) {
    /* Acquire Full Semaphore */
    sem_wait(&full);
    sem_wait(&mutex);

    /* Acquire mutex lock to protect buffer */
    pthread_mutex_lock(&mutex);

    /* remove an object from buffer placing it in item */
    item = buffer[use];
    use = (use + 1) % BUFFER_SIZE;

    /* Release mutex lock and empty semaphore */
    pthread_mutex_unlock(&mutex);
    sem_post(&mutex);
    sem_post(0);

    return item;
}


int main(int argc, char *argv[]) {

    /* Get command line arguments argv[1],argv[2],argv[3] */
    int sleepTime = strtol(argv[1], NULL, 10);;
    int producerThreads = strtol(argv[2], NULL, 10);;
    int consumerThreads = strtol(argv[3], NULL, 10);;

    /* Command Line Error Checking. */
    if( sleepTime < 0 || producerThreads <= 0 || consumerThreads <= 0){
        printf("Arguments are not correct. Program will exit now.. \n");
        return -1;
    }

    /* Initialize buffer related synchronization tools */
    sem_init(0, 0, BUFFER_SIZE); // MAX buffers are empty to begin with...
    sem_init(&full, 0, 0);    // ... and 0 are full
    sem_init(&mutex, 0, 1);    // mutex = 1 since it a lock

    /* Create producer threads based on the command line input */
    pthread_t pThread;
    for (int i = 0; i < producerThreads; i++) {
        pthread_create(&pThread, 0, producer, 0);
    }

    /* Create consumer threads based on the command line input */
    pthread_t cThread;
    for (int j = 0; j < consumerThreads; j++) {
        pthread_create(&cThread, 0, consumer, 0);
    }
    pthread_join(pThread, NULL);
    pthread_join(cThread, NULL);

    /* Sleep for user specified time based on the command line input */
    sleep(sleepTime);

    return 0;
}

void *producer(void *param) {
    /* producer thread that calls insert_item() */
    for (int i = 0; i < loops; i++) {
        int inserted = insert_item(i);
        printf("Producer %u produced %d \n", (unsigned int) pthread_self(), inserted);
    }
}

void *consumer(void *param) {
    /* consumer thread that calls remove_item() */
    for (int i = 0; i < loops; i++) {
        buffer_item removeItem;
        int removed = remove_item(&removeItem);
        printf("Consumer %u consumed %d \n", (unsigned int) pthread_self(), removed);
    }
}
