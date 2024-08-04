#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <pthread.h>
#include "circular_queue.h"
#define QUE_IS_VALID(b) ((b) != NULL && (b)->buffer != NULL && (b)->size > 0)
#ifdef CIR_QUE_DISABLE_ATOMIC
#define CIR_QUE_INIT(var, val)        (var) = (val)
#define CIR_QUE_LOAD(var, type)       (var)
#define CIR_QUE_STORE(var, val, type) (var) = (val)
#else
#define CIR_QUE_INIT(var, val)        atomic_init(&(var), (val))
#define CIR_QUE_LOAD(var, type)       atomic_load_explicit(&(var), (type))
#define CIR_QUE_STORE(var, val, type) atomic_store_explicit(&(var), (val), (type))
#endif


// 初始化队列
int queue_init(CircularQueue *q, size_t size) {
    q->buffer = (int *)malloc(size * sizeof(int));
    if (q->buffer == NULL) {
        return -1; // 内存分配失败
    }
    // if(pthread_spin_init(&q->lock, PTHREAD_PROCESS_SHARED)!=0) {
    //     return -1;
    // }
    q->size = size;
    CIR_QUE_INIT(q->head, 0);
    CIR_QUE_INIT(q->tail, 0);
    return 0;
}

// 销毁队列
void queue_destroy(CircularQueue *q) {
    if (QUE_IS_VALID(q)) {
        free(q->buffer);
        // pthread_spin_destroy(&q->lock);
    }  
}



// 入队操作
int queue_enqueue(CircularQueue *q, int value) {
    if (!QUE_IS_VALID(q)) {
        return -1;
    }

    if (queue_is_full(q)) {
        return -1; // 队列满
    }
    size_t tail = CIR_QUE_LOAD(q->tail, memory_order_acquire); 
    // pthread_spin_lock(&q->lock);
    q->buffer[tail] = value;
    // pthread_spin_unlock(&q->lock);
    size_t next_tail = (tail + 1) % q->size;   
    CIR_QUE_STORE(q->tail, next_tail, memory_order_release);
    return 0;
}

// 出队操作
int queue_dequeue(CircularQueue *q, int *value) {
    if (!QUE_IS_VALID(q)) {
        return -1;
    }    
    if (queue_is_empty(q)) {
        return -1; // 队列空
    }  
    size_t head = CIR_QUE_LOAD(q->head, memory_order_acquire);     
    *value = q->buffer[head];
    CIR_QUE_STORE(q->head, (head + 1) % q->size, memory_order_release);    

    return 0;
}

// 判断队列是否为空
bool queue_is_empty(CircularQueue *q) {
    if (!QUE_IS_VALID(q)) {
        return false;
    }    
    size_t head = CIR_QUE_LOAD(q->head, memory_order_relaxed);
    size_t tail = CIR_QUE_LOAD(q->tail, memory_order_relaxed);   
    return head == tail;
}

// 判断队列是否满
bool queue_is_full(CircularQueue *q) {
    if (!QUE_IS_VALID(q)) {
        return false;
    }
    size_t head = CIR_QUE_LOAD(q->head, memory_order_relaxed);
    size_t tail = CIR_QUE_LOAD(q->tail, memory_order_relaxed);
    return ((tail + 1) % q->size) == head;
}
