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

typedef void (*write_evt_cb_t)(size_t argc, void *arg);
// 共享内存结构体
typedef struct {
    pthread_rwlock_t rwlock;
    size_t buf_size;
    pid_t last_writer_pid; // 写进程的 PID
    size_t last_write_size; // 写入数据的大小
    write_evt_cb_t write_evt_cb;
} shared_memory_t;


// 初始化共享内存和读写锁
int shm_rwlock_init(const char *name, size_t size, shared_memory_t **shm_ptr, void *buf, write_evt_cb_t cb);

// 销毁共享内存和读写锁
int shm_rwlock_destroy(const char *name, shared_memory_t *shm_ptr);

// 读共享内存数据
int shm_rwlock_read(shared_memory_t *shm_ptr, void *src, char *buffer, size_t size);

// 写共享内存数据
int shm_rwlock_write(shared_memory_t *shm_ptr, void* dest, const void *buffer, size_t size, size_t cb_argc, void*cb_arg);

#endif // SHARED_MEMORY_RWLOCK_H
