#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/atomic.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/signal.h>
#define DEVICE_NAME "mykcm"
#define IOCTL_NOTIFY _IO('M', 1) // IOCTL 命令，用于通知



static int major;
static char *kernel_buffer;
static size_t buffer_size = 4096;
static atomic_t target_pid  = ATOMIC_INIT(-1);
static DECLARE_WAIT_QUEUE_HEAD(wait_queue);
static struct class *device_class = NULL; // 添加设备类的全局变量声明
static struct device *device = NULL;


static int device_open(struct inode *inode, struct file *file) {
    
    return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    return 0;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int ret = 0;

    switch (cmd) {
        case IOCTL_NOTIFY: {
            int write_len = 0;

            if (get_user(write_len, (int __user *)arg)) {
                return -EFAULT; // 返回错误并退出函数
            }
		/*struct siginfo info;
		struct kernel_siginfo kinfo;
		memset(&info, 0, sizeof(info));
		info.si_signo = SIGIO;
		info.si_code = SI_KERNEL; // 用户空间发送的信号
		info.si_pid = current->pid; // 发送信号的进程PID
		info.si_value.sival_int = write_len;
		memcpy(&kinfo, &info, sizeof(struct kernel_siginfo));
               do_send_sig_info(SIGIO, &kinfo, current, 0);*/
        }
        default:
            ret = -EINVAL;
            break;
    }

    return ret;
}


/*static unsigned int device_poll(struct file *file, poll_table *wait) {
    unsigned int mask = 0;
    pid_t current_pid = task_pid_nr(current);

    poll_wait(file, &wait_queue, wait);

    int target = atomic_read(&target_pid);
    if (target != -1 && current_pid != target) {
        mask = POLLIN;
        atomic_set(&target_pid, -1);
    }

    return mask;
}*/

static int device_mmap(struct file *file, struct vm_area_struct *vma) {
    unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;
    size_t size = vma->vm_end - vma->vm_start;

    // 检查映射区域是否超出缓冲区大小
    if (offset + size > buffer_size)
        return -EINVAL;

    // 映射物理内存到用户空间
    if (remap_pfn_range(vma, vma->vm_start,
                        virt_to_phys(kernel_buffer) >> PAGE_SHIFT,
                        size, vma->vm_page_prot)) {
        return -EAGAIN; // 映射失败时返回错误代码
    }

    return 0; // 映射成功
}



static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl,
    .mmap = device_mmap,
 //   .poll = device_poll,
};

static int __init device_init(void) {
    int ret;

    // 注册字符设备
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register character device with major number %d\n", major);
        return major;
    }

    // 分配内核缓冲区
    kernel_buffer = kmalloc(buffer_size, GFP_KERNEL);
    if (!kernel_buffer) {
        printk(KERN_ALERT "Failed to allocate memory for kernel buffer\n");
        unregister_chrdev(major, DEVICE_NAME); // 释放已注册的设备
        return -ENOMEM;
    }
// 创建设备类
    device_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(device_class)) {
        printk(KERN_ALERT "Failed to create device class\n");
        kfree(kernel_buffer);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(device_class);
    }

    // 创建设备
    device = device_create(device_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(device)) {
        printk(KERN_ALERT "Failed to create device\n");
        class_destroy(device_class);
        kfree(kernel_buffer);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(device);
    }
    printk(KERN_INFO "mykcm successfully registered with major number %d\n", major);
    return 0;
}


static void __exit device_exit(void) {



    if (device) {
        device_destroy(device_class, MKDEV(major, 0));
    }
    if (device_class) {
        class_destroy(device_class);
    }
    if (kernel_buffer) {
        kfree(kernel_buffer);
        kernel_buffer = NULL; // 防止再次释放
    }

    if (major >= 0) {
        unregister_chrdev(major, DEVICE_NAME);
    }

    printk(KERN_INFO "mykcm with major number %d unregistered and memory freed\n", major);
}



module_init(device_init);
module_exit(device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bowen");
MODULE_DESCRIPTION("A kernel communiication module");
