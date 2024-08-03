#include "shm_rwlock.h"

int shm_rwlock_init(const char *name, size_t size, shared_memory_t **shm_ptr, void *buf) {
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

    // 设置共享内存结构体的buf指针
    (*shm_ptr)->buf = buf;
    (*shm_ptr)->buf_size = size;

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

int shm_rwlock_read(shared_memory_t *shm_ptr, char *buffer, size_t size) {
    if (size > shm_ptr->buf_size) {
        fprintf(stderr, "Read size exceeds buffer size\n");
        return SM_FAILURE;
    }

    // 获取读锁
    if (pthread_rwlock_rdlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_rdlock");
        return SM_FAILURE;
    }

    memcpy(buffer, shm_ptr->buf, size);

    // 释放锁
    if (pthread_rwlock_unlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_unlock");
        return SM_FAILURE;
    }

    return SM_SUCCESS;
}

int shm_rwlock_write(shared_memory_t *shm_ptr, const char *buffer, size_t size) {
    if (size > shm_ptr->buf_size) {
        fprintf(stderr, "Write size exceeds buffer size\n");
        return SM_FAILURE;
    }

    // 获取写锁
    if (pthread_rwlock_wrlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_wrlock");
        return SM_FAILURE;
    }

    memcpy(shm_ptr->buf, buffer, size);

    // 释放锁
    if (pthread_rwlock_unlock(&(shm_ptr->rwlock)) != 0) {
        perror("pthread_rwlock_unlock");
        return SM_FAILURE;
    }

    return SM_SUCCESS;
}
