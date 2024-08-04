#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/mm.h>
#include <linux/gfp.h>          // alloc_page
#include <linux/ioctl.h>
#include <linux/slab.h>
#include <linux/atomic.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <linux/device.h>
#include <linux/signal.h>
#define DEVICE_NAME "mykcm"
#define IOCTL_NOTIFY _IO('M', 1) // IOCTL 命令，用于通知
#define PAGE_ORDER 2
#define MAX_SIZE (PAGE_SIZE << PAGE_ORDER)
static struct page *page = NULL;
static int major;
static struct fasync_struct *async_queue = NULL;
static struct class *device_class = NULL; // 添加设备类的全局变量声明
static struct device *device = NULL;



static int device_fasync(int fd, struct file *filep, int mode)
{
    return fasync_helper(fd, filep, mode, &async_queue);
}

static int device_open(struct inode *inode, struct file *file) {
   struct mm_struct *mm = current->mm;

   int minor = MINOR(inode->i_rdev);

   printk("%s: major=%d, minor=%d\n", __func__, MAJOR(inode->i_rdev), minor);
 
   printk("client: %s (%d)\n", current->comm, current->pid);
   printk("code  section: [0x%lx   0x%lx]\n", mm->start_code, mm->end_code);
   printk("data  section: [0x%lx   0x%lx]\n", mm->start_data, mm->end_data);
   printk("brk   section: s: 0x%lx, c: 0x%lx\n", mm->start_brk, mm->brk);
   printk("mmap  section: s: 0x%lx\n", mm->mmap_base);
   printk("stack section: s: 0x%lx\n", mm->start_stack);
   printk("arg   section: [0x%lx   0x%lx]\n", mm->arg_start, mm->arg_end);
   printk("env   section: [0x%lx   0x%lx]\n", mm->env_start, mm->env_end);    
   return 0;
}

static int device_release(struct inode *inode, struct file *file) {
    device_fasync(-1, file, 0);
    return 0;
}

static long device_ioctl(struct file *file, unsigned int cmd, unsigned long arg) {
    int ret = 0;
    switch (cmd) {
        case IOCTL_NOTIFY: {
        
            if (async_queue) {
                kill_fasync(&async_queue, SIGIO, POLL_IN);
            }
            break;
        }
        default:
            ret = -EINVAL;
            break;
    }

    return ret;
}




static int device_mmap(struct file *file, struct vm_area_struct *vma) {
    unsigned long size;
    unsigned long pfn_start;
    void *virt_start;
    int ret;

    pfn_start = page_to_pfn(page) + vma->vm_pgoff;
    virt_start = page_to_virt(page) + (vma->vm_pgoff << PAGE_SHIFT);

    /* 映射大小不超过实际物理页 */
    size = min(((1 << PAGE_ORDER) - vma->vm_pgoff) << PAGE_SHIFT,
               vma->vm_end - vma->vm_start);

    printk("phys_start: 0x%lx, offset: 0x%lx, vma_size: 0x%lx, map size:0x%lx\n",
           pfn_start << PAGE_SHIFT, vma->vm_pgoff << PAGE_SHIFT,
           vma->vm_end - vma->vm_start, size);

    if (size <= 0) {
        printk("%s: offset 0x%lx too large, max size is 0x%lx\n", __func__,
               vma->vm_pgoff << PAGE_SHIFT, MAX_SIZE);
        return -EINVAL;
    }

    // 外层vm_mmap_pgoff已经用信号量保护了 
    // down_read(&mm->mmap_sem);
    ret = remap_pfn_range(vma, vma->vm_start, pfn_start, size, vma->vm_page_prot);
    // up_read(&mm->mmap_sem);

    if (ret) {
        printk("remap_pfn_range failed, vm_start: 0x%lx\n", vma->vm_start);
    }
    else {
        printk("map kernel 0x%px to user 0x%lx, size: 0x%lx\n",
               virt_start, vma->vm_start, size);
    }

    return ret;
}




static struct file_operations fops = {
    .open = device_open,
    .release = device_release,
    .unlocked_ioctl = device_ioctl,
    .mmap = device_mmap,
    .fasync = device_fasync,    
};

static int __init device_init(void) {
    //int ret;

    // 注册字符设备
    major = register_chrdev(0, DEVICE_NAME, &fops);
    if (major < 0) {
        printk(KERN_ALERT "Failed to register character device with major number %d\n", major);
        return major;
    }

    // 分配内核缓冲区
    page = alloc_pages(GFP_KERNEL, PAGE_ORDER);
    if (!page) {
        printk("alloc_page failed\n");
        return -ENOMEM;
    }

    printk("mykkcm buffer physical address: %lx, virtual address: %px\n",
           page_to_pfn(page) << PAGE_SHIFT, page_to_virt(page));
// 创建设备类
    device_class = class_create(THIS_MODULE, DEVICE_NAME);
    if (IS_ERR(device_class)) {
        printk(KERN_ALERT "Failed to create device class\n");
        __free_pages(page, PAGE_ORDER);
        unregister_chrdev(major, DEVICE_NAME);
        return PTR_ERR(device_class);
    }

    // 创建设备
    device = device_create(device_class, NULL, MKDEV(major, 0), NULL, DEVICE_NAME);
    if (IS_ERR(device)) {
        printk(KERN_ALERT "Failed to create device\n");
        class_destroy(device_class);
        __free_pages(page, PAGE_ORDER);
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
    
	__free_pages(page, PAGE_ORDER);
    
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
