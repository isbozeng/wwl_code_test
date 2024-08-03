#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "shm_rwlock.h"

#define DEVICE_PATH "/dev/mykcm"
#define IOCTL_NOTIFY _IO('M', 1) 
#define BUFFER_SIZE 256
#define EPOLL_SIZE 2
#define SHM_NAME "/myshm"



static shared_memory_t *shm_ptr;

static void handle_events(int epoll_fd, int device_fd) {
    struct epoll_event events[EPOLL_SIZE];
    int nfds = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);

    for (int i = 0; i < nfds; i++) {
        if (events[i].events & EPOLLIN) {
            if (events[i].data.fd == STDIN_FILENO) {
                char buffer[BUFFER_SIZE];
                uint32_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
                if (bytes_read > 0) {

		    // 获取写锁
		    if (pthread_rwlock_wrlock(&(shm_ptr->rwlock)) != 0) {
			perror("pthread_rwlock_wrlock");
			exit(1);
		    }

		    memcpy(&shm_ptr->buf[4], buffer, bytes_read);
		    memcpy(&shm_ptr->buf[0], &bytes_read, sizeof(bytes_read));
                    // 通知设备
                    int pid = getpid();
                    if (ioctl(device_fd, IOCTL_NOTIFY, &pid) < 0) {
                        perror("ioctl");
                        exit(1);
                    }
		    // 释放锁
		    if (pthread_rwlock_unlock(&(shm_ptr->rwlock)) != 0) {
			perror("pthread_rwlock_unlock");
			exit(1);
		    }
                }
            }else if (events[i].data.fd == device_fd) {
            	 pthread_rwlock_rdlock(&(shm_ptr->rwlock));
                uint32_t *ready_len = (void*)&shm_ptr->buf[0];
                pthread_rwlock_unlock(&(shm_ptr->rwlock));
            	for(uint32_t i = 0; i < *ready_len; i++) {
		    pthread_rwlock_rdlock(&(shm_ptr->rwlock));
            	    char *p_data= &shm_ptr->buf[4 + i];
            	    pthread_rwlock_unlock(&(shm_ptr->rwlock));
                    printf("Received from process: %c\n", *p_data);
                }
            }
            
        }
        if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
            printf("Error or hang up on fd %d\n", events[i].data.fd);
        }
    }
}

int main() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd < 0) {
        perror("epoll_create1");
        return 1;
    }


    // 打开设备文件
    int device_fd = open(DEVICE_PATH, O_RDWR);
    if (device_fd < 0) {
        perror("open");
        close(epoll_fd);
        return 1;
    }
    // 映射设备文件
    void *dev_mem = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, device_fd, 0);
    if (dev_mem == MAP_FAILED) {
        perror("dev mmap");
        close(device_fd);
        close(epoll_fd);
        return 1;
    }



    // 初始化读写锁
    int ret = shm_rwlock_init(SHM_NAME, BUFFER_SIZE, &shm_ptr, dev_mem);
    if (ret == -1) {
        perror("shm_rwlock_init");
        munmap(shm_ptr->buf, BUFFER_SIZE);
        close(device_fd);
        close(epoll_fd);
        return 1;
    }
    
    // 监听标准输入
    struct epoll_event stdin_event = {
        .events = EPOLLIN,
        .data.fd = STDIN_FILENO
    };
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_event) < 0) {
        perror("epoll_ctl");
        pthread_rwlock_destroy(&shm_ptr->rwlock);
        close(device_fd);
        close(epoll_fd);
        return 1;
    }

    struct epoll_event device_event = {
        .events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET,
        .data.fd = device_fd
    };
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, device_fd, &device_event) < 0) {
        perror("epoll_ctl");
        pthread_rwlock_destroy(&shm_ptr->rwlock);
        close(device_fd);
        close(epoll_fd);
        return 1;
    }

    // 处理事件
    while (1) {
        handle_events(epoll_fd, device_fd);
    }

    // 清理资源
    munmap(shm_ptr->buf, BUFFER_SIZE);
    pthread_rwlock_destroy(&shm_ptr->rwlock);
    close(device_fd);
    close(epoll_fd);
    return 0;
}
