#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include "circular_queue.h"

#define QUEUE_SIZE 10

CircularQueue queue;
sem_t data_ready;  // 信号量，用于通知消费者线程有数据
void* producer(void* arg) {
    while (1) {
        int value = rand() % 1000;
        while (queue_enqueue(&queue, value) != 0) {
            // 队列满，等待一段时间后重试
            usleep(10000);
        }
        sem_post(&data_ready);
        printf("Produced: %d\n", value);
        usleep(100000); // 模拟生产数据的时间
    }
    return NULL;
}

void* consumer(void* arg) {
    int value;
    while (1) {
        sem_wait(&data_ready); // 等待有数据
        while (queue_dequeue(&queue, &value) == 0) {
            printf("Consumed: %d\n", value);
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    
    if (queue_init(&queue, QUEUE_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize queue\n");
        return EXIT_FAILURE;
    }
    // 初始化信号量
    sem_init(&data_ready, 0, 0); // 初始值为0，表示开始时没有数据
    pthread_t producer_thread, consumer_thread;

    if (pthread_create(&producer_thread, NULL, producer, NULL) != 0) {
        fprintf(stderr, "Failed to create producer thread\n");
        return EXIT_FAILURE;
    }

    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        fprintf(stderr, "Failed to create consumer thread\n");
        return EXIT_FAILURE;
    }

    // 等待线程完成
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    queue_destroy(&queue);

    return EXIT_SUCCESS;
}
