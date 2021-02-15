//
// Created by 姚雨 on 2021/2/15.
//

#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <semaphore.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

//
#define  ERR_EXIT(m)
#define CONSUMERS_COUNT 2   //the count of consumer
#define PRODUCERS_COUNT 2   //the count of producer
#define BUFFSIZE 5

//do {
//    perror(m);// to display the message of error
//    exit(EXIT_FAILURE);
//}   while(0)


int g_buffer[BUFFSIZE];
unsigned short in = 0;// the point of production
unsigned short out = 0;// the point of consumer
unsigned short producer_id = 0;
unsigned short consumer_id = 0;

sem_t g_sem_full;//the count of buffer can be used
sem_t g_sem_empty; // the count of production can be consumer
pthread_mutex_t  g_mutex; // mutex sempher
pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

void *consume (void *arg) {
    int i;
    int num = (int)arg;
    while (1)
    {
        printf("%d wait buffer not empty\n", num);
        sem_wait(&g_sem_empty);
        pthread_mutex_lock(&g_mutex);
        for (i = 0; i < BUFFSIZE; i++)
        {
            printf("%02d ", i);
            if (g_buffer[i] == -1)
                printf("%s", "null");
            else
                printf("%d", g_buffer[i]);
            if (i == out) {
                printf("\t<--consume");
            }
            printf("\n");
        }
        //produce()操作(生产产品)
        consumer_id = g_buffer[out];
        printf("%d begin consume product %d\n", num, consume_id);
        g_buffer[out] = -1;
        out = (out + 1) % BUFFSIZE; // to move the point to next position of production
        printf("%d end consumer product %d \n", num, consumer_id);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_full);// use the semaphore to communicate reciprocally
        sleep(1);
    }
    return NULL;
}

void *produce(void *arg) {
    int num = (int) arg;
    int i;
    while (1) {
        printf("%d wait buffer not full\n", num);
        sem_wait(&g_sem_full);
        pthread_mutex_lock(&g_mutex);
        for (i = 0; i < BUFFSIZE; i++)
        {
            printf("%02d ", i);
            if (g_buffer[i] == -1)
                printf("%s", "null");
            else
                printf("%d", g_buffer[i]);

            if (i == in)
                printf("\t<--produce");

            printf("\n");
        }
        printf("%d begin produce product %d\n", num, producer_id);
        g_buffer[in] = producer_id;
        in = (in + 1) % BUFFSIZE;
        printf("%d end produce product %d\n", num, producer_id++);
        pthread_mutex_unlock(&g_mutex);
        sem_post(&g_sem_empty);
        sleep(5);
    }
}

int main(void)
{
    int i;
    // init the globalArray
    for (i = 0; i < BUFFSIZE; i++)
        g_buffer[i] = -1;
    sem_init(&g_sem_full, 0, BUFFSIZE);// sem_full to notify the producer
    sem_init(&g_sem_empty, 0, 0); // sem_empty to notify the consumer

    pthread_mutex_init(&g_mutex, NULL);


    for (i = 0; i < CONSUMERS_COUNT; i++)
        pthread_create(&g_thread[i], NULL, consume, (void *)i);

    for (i = 0; i < PRODUCERS_COUNT; i++)
        pthread_create(&g_thread[CONSUMERS_COUNT + i], NULL, produce, (void *)i);

    //
    for (i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; i++)
        pthread_join(g_thread[i], NULL);

    sem_destroy(&g_sem_full);
    sem_destroy(&g_sem_empty);//destroys the unnamed semaphore at the address pointed to by sem.
    pthread_mutex_destroy(&g_mutex); // frees the resources allocated for mutex

    return 0;
}
