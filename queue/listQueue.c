//
// Created by 姚雨 on 2021/2/16.
//
#include "stdio.h"
#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "unistd.h"
#include "pthread.h"
#include "time.h"
#include "list.h"

//the count of producer and consumer
#define PRODUCER_MAX 3
#define CONSUMER_MAX 5

//mutex
pthread_mutex_t mutex;

//condition
pthread_cond_t cond;

//table
LIST *list

void *producer(void *args) {
    int *num;// int point var
    while (1) {
        num = (int *)malloc(sizeof(int));
        *num = rand()%100 + 1;
        pthread_mutex_lock(&mutex);// lock
        list_insert(list, num);
        pthread_mutex_unlock(&mutex);//unlock
        pthread_cond_signal(&cond);//notify the producer to release resource
        printf("%ld -> 生产成功 -- %d \n", pthread_self(), *num);
        sleep(1);

    }
}

void *consumer(void *args) {
    int num;
    while (1) {
        pthread_mutex_lock(&mutex);
        while (list->size == 0) {
            printf("%ld -> 等待生产\n",pthread_self());
            pthread_cond_wait(&cond, &mutex);
        }
        num = *(int *)list_value(list);
        list_delete(list);
        pthread_mutex_unlock(&mutex);
        printf("%ld -> 消费成功 -- %d\n", pthread_self(), num);
        sleep(rand()%3 + 1);

    }
}

int main(int argc, char *argv[]) {
    pthread_mutex_init(&mutex, NULL);// init mutex
    pthread_cond_init(&cond, NULL); // init condition
    //初始化任务链表
    list = list_init();
    if(list == NULL)
    {
        perror("list_init error");
        exit(1);
    }
    int ret;
    pthread_t producer_tid[PRODUCER_MAX];
    pthread_t consumer_tid[CONSUMER_MAX];

    for(int i = 0; i < PRODUCER_MAX; i++) {
        ret = pthread_create(producer_tid + i; NULL, producer, NUll);
        if(ret != 0) {
            printf("pthread_create error:%s\n", strerror(ret));
            return -1;
        }
    }

    for (int i = 0; i < CONSUMER_MAX; i ++) {
        ret = pthread_create(consumer_tid + i, NULL, consumer, NULL);
        if(ret != 0) {
            printf("pthread_create error:%s\n", strerror(ret));
            return -1;
        }
    }
    for (int i = 0; i < PRODUCER_MAX; i++) {
        pthread_join(producer_tid[i], NULL);
    }
    for (int i = 0; i < CONSUMER_MAX; i++) {
        pthread_join(consumer_tid[i], NULL);
    }
    list_detory(list);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    return 0;
}
