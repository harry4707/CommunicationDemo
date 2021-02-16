//
// Created by 姚雨 on 2021/2/16.
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


int g_buffer[BUFFSIZE];
unsigned short in = 0;// the point of producer
unsigned short out = 0;// the point of consumer
unsigned short produce_id = 0;
unsigned short consume_id = 0;

sem_t g_sem_full;//the count of buffer can be used
sem_t g_sem_empty; // the count of production can be consumer
pthread_mutex_t  g_mutex; // mutex
pthread_cond_t g_cond;    // global condition var
pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];
int production_num = 0;

void *consume (void *arg) {
    int i;
    int num = (int)arg;
    while (1)
    {
        pthread_mutex_lock(&g_mutex);
        while(production_num == 0) {
            printf("the queue is empty consumer-%d begin wait a condtion ...\n", num);
            pthread_cond_wait(&g_cond, &g_mutex);
        }
        for (i = 0; i < BUFFSIZE; i++)
        {
            printf("%02d ", i);
            if (g_buffer[i] == -1)
                printf("%s", "null");
            else
                printf("%d", g_buffer[i]);
            if (i == out) {
                printf("\t<--consume-%d", num);
            }
            printf("\n");
        }
        consume_id = g_buffer[out];
        printf("consumer-%d begin consume product %d\n", num, consume_id);
        g_buffer[out] = -1;
        out = (out + 1) % BUFFSIZE; // to move the point to next position of production
        printf("consumer-%d end consumer product %d \n", num, consume_id++);
        --production_num;
        pthread_cond_signal(&g_cond);
        pthread_mutex_unlock(&g_mutex);
        sleep(5);
    }
}

void *produce(void *arg) {
    int num = (int) arg;
    int i;
    while (1) {
        pthread_mutex_lock(&g_mutex);
        while(production_num >= BUFFSIZE) {
            printf("the queue is full producer-%d begin wait a condtion ...\n", num);
            pthread_cond_wait(&g_cond, &g_mutex);
        }
        for (i = 0; i < BUFFSIZE; i++)
        {
            printf("%02d ", i);
            if (g_buffer[i] == -1)
                printf("%s", "null");
            else
                printf("%d", g_buffer[i]);

            if (i == in)
                printf("\t<--produce-%d", num);

            printf("\n");
        }
        printf("producer-%d begin produce product %d\n", num, produce_id);
        g_buffer[in] = produce_id;
        in = (in + 1) % BUFFSIZE;
        printf("producer-%d end produce product %d\n", num, produce_id++);
        ++production_num ;
        printf("producer-%d signal ...\n", num);
        pthread_cond_signal(&g_cond);
        pthread_mutex_unlock(&g_mutex);
        printf("producer-%d mutex_unlock ...\n", num);
        printf("producer-%d production_num is %d ...\n", num, production_num);
        sleep(1);
    }
}

int main(void)
{
    int i;
    // init the globalArray
    for (i = 0; i < BUFFSIZE; i++)
        g_buffer[i] = -1;
    pthread_cond_init(&g_cond, NULL);
    pthread_mutex_init(&g_mutex, NULL);


    for (i = 0; i < CONSUMERS_COUNT; i++)
        pthread_create(&g_thread[i], NULL, consume, (void *)i);

    for (i = 0; i < PRODUCERS_COUNT; i++)
        pthread_create(&g_thread[CONSUMERS_COUNT + i], NULL, produce, (void *)i);

    //
    for (i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; i++)
        pthread_join(g_thread[i], NULL);

    pthread_mutex_destroy(&g_mutex); // frees the resources allocated for mutex
    pthread_cond_destroy(&g_cond);
    return 0;
}
