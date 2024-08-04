#include "shm_rwlock.h"


int shm_rwlock_init(const char *name, size_t size, shared_memory_t **shm_ptr, void *buf, write_evt_cb_t cb) {
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return SM_FAILURE;
    }

    // 设置共享内存对象的大小
    if (ftruncate(shm_fd, sizeof(shared_memory_t)) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return SM_FAILURE;
    }

    // 映射共享内存
    *shm_ptr = mmap(NULL, sizeof(shared_memory_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (*shm_ptr == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return SM_FAILURE;
    }

    close(shm_fd);

    // 初始化读写锁
    pthread_rwlockattr_t attr;
    pthread_rwlockattr_init(&attr);
    pthread_rwlockattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);

    if (pthread_rwlock_init(&((*shm_ptr)->rwlock), &attr) != 0) {
        perror("pthread_rwlock_init");
        munmap(*shm_ptr, sizeof(shared_memory_t));
        return SM_FAILURE;
    }

    pthread_rwlockattr_destroy(&attr);

    // 设置共享内存结构体
    (*shm_ptr)->buf_size = size;
    (*shm_ptr)->last_writer_pid = -1;
    (*shm_ptr)->last_write_size = 0;
    (*shm_ptr)->write_evt_cb = cb; // 设置回调函数
    return SM_SUCCESS;
}

int shm_rwlock_destroy(const char *name, shared_memory_t *shm_ptr) {
    // 销毁读写锁
    if (pthread_rwlock_destroy(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_destroy");
        return SM_FAILURE;
    }

    // 解除映射
    if (munmap(shm_ptr, sizeof(shared_memory_t)) == -1) {
        perror("munmap");
        return SM_FAILURE;
    }

    // 删除共享内存对象
    if (shm_unlink(name) == -1) {
        perror("shm_unlink");
        return SM_FAILURE;
    }

    return SM_SUCCESS;
}

int shm_rwlock_read(shared_memory_t *shm_ptr, void *src, char *buffer, size_t size) {
    if (size > shm_ptr->buf_size) {
        fprintf(stderr, "Read size exceeds buffer size\n");
        return SM_FAILURE;
    }

    // 获取读锁
    if (pthread_rwlock_rdlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_rdlock");
        return SM_FAILURE;
    }

    memcpy(buffer, src, size);

    // 释放锁
    if (pthread_rwlock_unlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_unlock");
        return SM_FAILURE;
    }

    return SM_SUCCESS;
}

int shm_rwlock_write(shared_memory_t *shm_ptr, void *dest, const void *buffer, size_t size, size_t cb_argc, void*cb_arg) {
    if (size > shm_ptr->buf_size) {
        fprintf(stderr, "Write size exceeds buffer size\n");
        return SM_FAILURE;
    }
    //printf("start write devmem %p, size %u\n", dest, size);
    // 获取写锁
    if (pthread_rwlock_wrlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_wrlock");
        return SM_FAILURE;
    }
    /*char *d = dest, *s = buffer;
    for (int i = 0; i < size; i++) {
       printf("&dest[%d] %p , val %c\n",i, d + i, s[i]);
       d[i] = s[i];
    }*/
    memcpy(dest, buffer, size);

    // 更新写进程 PID 和写入大小
    shm_ptr->last_writer_pid = getpid();
    shm_ptr->last_write_size = size;

    // 释放锁
    if (pthread_rwlock_unlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_unlock");
        return SM_FAILURE;
    }
    
    // 调用回调函数（如果已注册）
    //if (shm_ptr->write_evt_cb) {
    //    shm_ptr->write_evt_cb(cb_argc, cb_arg);
    //}    
    //printf("end write devmem %p, size %u\n", dest, size);
    return SM_SUCCESS;
}
