#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include "circular_queue.h"

#define QUEUE_SIZE 10

CircularQueue queue;

void* producer(void* arg) {
    while (1) {
        int value = rand() % 100;
        while (queue_enqueue(&queue, value) != 0) {
            // 队列满，等待一段时间后重试
            usleep(1000);
        }
        printf("Produced: %d\n", value);
        usleep(100000); // 模拟生产数据的时间
    }
    return NULL;
}

void* consumer(void* arg) {
    int value;
    while (1) {
        while (queue_dequeue(&queue, &value) != 0) {
            // 队列空，等待一段时间后重试
            usleep(1000);
        }
        printf("Consumed: %d\n", value);
        usleep(150000); // 模拟消费数据的时间
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    
    if (queue_init(&queue, QUEUE_SIZE) != 0) {
        fprintf(stderr, "Failed to initialize queue\n");
        return EXIT_FAILURE;
    }

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
