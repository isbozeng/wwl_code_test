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
#include <signal.h>

#include "shm_rwlock.h"
#define BUFFER_SIZE 512

#define DEVICE_PATH "/dev/mykcm"
#define IOCTL_NOTIFY _IO('M', 1) 
#define EPOLL_SIZE 2
#define SHM_NAME "/myshm"



static shared_memory_t *shm_ptr;
static char *dev_mem = NULL;
static void write_evt_notify(size_t argc, void *arg) {
	if (argc > 0) {
		int32_t *p_fd = arg;
		if (ioctl(*p_fd, IOCTL_NOTIFY) < 0) {
		   perror("ioctl");
		   exit(1);
		}  
	}

}
// 处理接收到的信号的函数
void signal_handler(int signum) {

    // 在这里执行接收到信号后的处理逻辑
    pthread_rwlock_rdlock(&(shm_ptr->rwlock));
    pid_t from_pid = shm_ptr->last_writer_pid;
    size_t read_cnt = shm_ptr->last_write_size;
    pthread_rwlock_unlock(&(shm_ptr->rwlock));
    
    if (from_pid != getpid()) {
        printf("Received signal %d from %u data cnt %u virt addr %p\n", signum, 
        shm_ptr->last_writer_pid, read_cnt, dev_mem);
		for (size_t i = 0; i < read_cnt; i++) {
			pthread_rwlock_rdlock(&(shm_ptr->rwlock));
			char data = dev_mem[i];
			pthread_rwlock_unlock(&(shm_ptr->rwlock));
		   	printf("%c", data);
		}    
    }

}

static void handle_events(int epoll_fd, int device_fd) {
    struct epoll_event events[EPOLL_SIZE];
    int nfds = epoll_wait(epoll_fd, events, EPOLL_SIZE, -1);

    for (int i = 0; i < nfds; i++) {
        if (events[i].events & EPOLLIN) {
            if (events[i].data.fd == STDIN_FILENO) {
                char buffer[BUFFER_SIZE] = { 0 };
                uint32_t bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
                if (bytes_read > 0) {
					shm_rwlock_write(&(shm_ptr->rwlock), dev_mem, buffer, bytes_read, 1, &device_fd);
					/*if (ioctl(device_fd, IOCTL_NOTIFY) < 0) {
					   perror("ioctl");
					   exit(1);
					}*/  
                }
            }
        }
        if (events[i].events & EPOLLERR || events[i].events & EPOLLHUP) {
            printf("Error or hang up on fd %d\n", events[i].data.fd);
        }
    }
}

int main() {

	struct sigaction sa = { 0 };

    // 设置信号处理函数
    sa.sa_handler = signal_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGIO, &sa, NULL) == -1) {
        perror("Error setting signal handler");
        return 1;
    }
    
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
    // 设置设备为异步模式
    if (fcntl(device_fd, F_SETFL, O_ASYNC) == -1) {
        perror("Failed to set async mode");
        close(device_fd);
        return 1;
    }

    // 将进程的PID告知内核，以便接收信号
    if (fcntl(device_fd, F_SETOWN, getpid()) == -1) {
        perror("Failed to set owner");
        close(device_fd);
        return 1;
    }
    
    // 映射设备文件

    #ifndef MY_SM_BUF_SIZE
    dev_mem = mmap(NULL, (size_t)getpagesize(), PROT_READ | PROT_WRITE, MAP_SHARED, device_fd, 0);
    if (dev_mem == MAP_FAILED) {
        perror("dev mmap");
        close(device_fd);
        close(epoll_fd);
        return 1;
    }
    printf("kernel to virt addr %p\n",dev_mem);
	#endif


    // 初始化读写锁
    int ret = shm_rwlock_init(SHM_NAME, BUFFER_SIZE, &shm_ptr, dev_mem, write_evt_notify);
    if (ret == -1) {
        perror("shm_rwlock_init");
        munmap(dev_mem, BUFFER_SIZE);
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
    	shm_rwlock_destroy(SHM_NAME, shm_ptr);
        munmap(dev_mem, BUFFER_SIZE);
    
        close(device_fd);
        close(epoll_fd);
        return 1;
    }


    // 处理事件
    while (1) {
        handle_events(epoll_fd, device_fd);
    }

    // 清理资源
    munmap(dev_mem, BUFFER_SIZE);
	shm_rwlock_destroy(SHM_NAME, shm_ptr);
    close(device_fd);
    close(epoll_fd);
    return 0;
}
