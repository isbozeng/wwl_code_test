#ifndef CIRCULAR_QUEUE_H
#define CIRCULAR_QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

#if defined(CIR_QUE_DISABLE_ATOMIC)
    #include <stdatomic.h>

    typedef atomic_ulong cir_que_atomic_t;

#else
    typedef uint32_t cir_que_atomic_t;
#endif


// 数据结构定义
typedef struct {
    int *buffer;
    size_t size;
    cir_que_atomic_t head;
    cir_que_atomic_t tail;
} CircularQueue;

/**
 * @brief 初始化队列
 * 
 * @param q 指向 CircularQueue 结构的指针
 * @param size 队列的大小
 * @return int 成功返回 0，失败返回 -1
 */
int queue_init(CircularQueue *q, size_t size);

/**
 * @brief 销毁队列
 * 
 * @param q 指向 CircularQueue 结构的指针
 */
void queue_destroy(CircularQueue *q);

/**
 * @brief 入队操作
 * 
 * @param q 指向 CircularQueue 结构的指针
 * @param value 要入队的整数值
 * @return int 成功返回 0，失败返回 -1
 */
int queue_enqueue(CircularQueue *q, int value);

/**
 * @brief 出队操作
 * 
 * @param q 指向 CircularQueue 结构的指针
 * @param value 指向整数的指针，用于存储出队的值
 * @return int 成功返回 0，失败返回 -1
 */
int queue_dequeue(CircularQueue *q, int *value);

/**
 * @brief 判断队列是否为空
 * 
 * @param q 指向 CircularQueue 结构的指针
 * @return bool 队列为空返回 true，否则返回 false
 */
bool queue_is_empty(CircularQueue *q);

/**
 * @brief 判断队列是否满
 * 
 * @param q 指向 CircularQueue 结构的指针
 * @return bool 队列满返回 true，否则返回 false
 */
bool queue_is_full(CircularQueue *q);

#ifdef __cplusplus
}
#endif

#endif // CIRCULAR_QUEUE_H
