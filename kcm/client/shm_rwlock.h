#ifndef SHARED_MEMORY_RWLOCK_H
#define SHARED_MEMORY_RWLOCK_H

#include <pthread.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 错误码
#define SM_SUCCESS 0
#define SM_FAILURE -1

// 共享内存结构体
typedef struct {
    pthread_rwlock_t rwlock;
    void *buf;
    size_t buf_size;
} shared_memory_t;

// 初始化共享内存和读写锁
int shm_rwlock_init(const char *name, size_t size, shared_memory_t **shm_ptr, void *buf);

// 销毁共享内存和读写锁
int shm_rwlock_destroy(const char *name, shared_memory_t *shm_ptr);

// 读共享内存数据
int shm_rwlock_read(shared_memory_t *shm_ptr, char *buffer, size_t size);

// 写共享内存数据
int shm_rwlock_write(shared_memory_t *shm_ptr, const char *buffer, size_t size);

#endif // SHARED_MEMORY_RWLOCK_H
