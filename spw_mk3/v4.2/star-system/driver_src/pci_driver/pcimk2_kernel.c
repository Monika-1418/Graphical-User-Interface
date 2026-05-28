/**
 * \file pcimk2_kernel.c
 *
 * \brief Linux kernel interface for the Linux SpaceWire PCI Driver.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains kernel interface functions for
 * the STAR-Dundee SpaceWire PCI Mk2 Linux Driver.
 *
 * Copyright &copy 2013 STAR-Dundee Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/kernel.h>
#include <linux/module.h>

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif

/* Only allow 2.6.0 through 4.14.3 kernels */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0) /* < 2.6 */
    #error "This kernel is too old: not supported by this driver"
#endif
#if LINUX_VERSION_CODE > KERNEL_VERSION(5,5,8) /* > 5.5.8 */
    #ifndef STAR_TRY_NEWER_KERNEL
        #error "This kernel is too recent: not supported by this driver."
        #error "Please contact STAR-Dundee to check if a newer version of the driver is available."
    #else
        #warning "This kernel is too recent: not supported by this driver."
        #warning "Please contact STAR-Dundee to check if a newer version of the driver is available."
    #endif
#endif

#include <linux/init.h>
#include <linux/pci.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/interrupt.h>
#include <linux/dma-mapping.h>
#include <linux/sched.h>
#include <linux/workqueue.h>
#include <linux/wait.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 7))
#include <linux/delay.h>
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18))
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 26))
#include <linux/semaphore.h>
#else
#include <asm/semaphore.h>
#endif
#if (LINUX_VERSION_CODE <= KERNEL_VERSION(2, 6, 17))
#include <linux/devfs_fs_kernel.h>
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0))
#include <linux/sched/signal.h>
#endif

#include "pcimk2_kernel.h"


/* Must be able to access PCI functionality */
#ifndef CONFIG_PCI
    #error "This driver needs PCI support to be available"
#endif

/* If power management support is enabled */
#ifdef CONFIG_PM

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 11))
    #define STAR_PCI_SUSPEND_STATE_TYPE pm_message_t
#else
    #define STAR_PCI_SUSPEND_STATE_TYPE u32
#endif
#endif

/* __devinit and __devexit were removed in kernel 3.8 */
#ifndef __devinit
    #define __devinit
#endif
#ifndef __devexit
    #define __devexit
#endif
#ifndef __devexit_p
    #define __devexit_p
#endif


/* Specify the license used by the driver */
MODULE_LICENSE("GPL");

/* Specify the driver author */
MODULE_AUTHOR("David Paterson, STAR-Dundee, support@star-dundee.com");

/* Provide a description of the module */
MODULE_DESCRIPTION("STAR-Dundee STAR-System PCI Driver for Linux");

/* Specify the version of the driver */
MODULE_VERSION("4.0");


/**
 * Forward declarations of driver-specific functions
 * =================================================
 */
static int __init spw_pci_init(void);
static void __exit spw_pci_exit(void);

int __devinit spw_pci_probe(struct pci_dev *pDev, const struct pci_device_id *pId);
void __devexit spw_pci_remove(struct pci_dev *pDev);
#ifdef CONFIG_PM
int spw_pci_suspend(struct pci_dev *dev, STAR_PCI_SUSPEND_STATE_TYPE state);
int spw_pci_resume(struct pci_dev *pDev);
#endif

int spw_pci_open(struct inode *inode, struct file *filp);
int spw_pci_release(struct inode *inode, struct file *filp);
ssize_t spw_pci_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
ssize_t spw_pci_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
int spw_pci_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#ifdef HAVE_UNLOCKED_IOCTL
long spw_pci_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#ifdef HAVE_COMPAT_IOCTL
long spw_pci_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 19))
irqreturn_t spw_interrupt_handler(int irq, void *pDevId);
#else
irqreturn_t spw_interrupt_handler(int irq, void *pDevId, struct pt_regs *pRegs);
#endif

void spw_pci_worker(struct work_struct *work);


/**
 * Error return codes
 * ==================
 */
const int STAR_NO_SUCH_FILE_ERROR = -ENOENT;    /* 2.6.0 - latest */
const int STAR_NO_MEMORY_ERROR = -ENOMEM;       /* 2.6.0 - latest */
const int STAR_BAD_ADDRESS_ERROR = -EFAULT;     /* 2.6.0 - latest */
const int STAR_NO_SUCH_DEVICE_ERROR = -ENODEV;  /* 2.6.0 - latest */
const int STAR_INVALID_ARG_ERROR = -EINVAL;     /* 2.6.0 - latest */
const int STAR_NOT_A_TTY_ERROR = -ENOTTY;       /* 2.6.0 - latest */


/**
 * Set the functions called when initialising and exiting the driver
 */
module_init(spw_pci_init);
module_exit(spw_pci_exit);

/**
 * The level of informational messages to output.
 */
int message_level = STAR_MESSAGE_LEVEL_WARNING;
module_param(message_level, int, S_IRUSR|S_IWUSR);
MODULE_PARM_DESC(message_level, "The level of informational messages to output (1 = error, 2 = warning, 3 = debug, 0 for no messages)");


/**
 * The list of devices supported by this driver.
 */
static struct pci_device_id star_pci_ids[] =
{
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PCI_MK2_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PCIE_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_CPCI_MK2_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_RECORDER_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PXI_INTERFACE_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PXI_RMAP_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PXI_ROUTER_8_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PXI_ROUTER_12_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PCI_MK3_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PXI_INTERFACE_MK2_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PXI_RMAP_MK2_DEVICE_ID) },
    { PCI_DEVICE(STAR_DUNDEE_PCI_VENDOR_ID, STAR_PXI_ROUTER_MK2_DEVICE_ID) },
    { 0, },
};

/**
 * Export device ID table to user space for hotplug and module loading systems.
 */
MODULE_DEVICE_TABLE(pci, star_pci_ids);


/**
 * Additional device information table
 * (must be same length as, and have matching entries to device ID table above)
 */
const star_device_info star_device_info_table[] =
{
        { "SpaceWire PCI Mk2", 8, 4 },
        { "SpaceWire PCIe", 11, 4 },
        { "SpaceWire cPCI Mk2", 14, 4 },
        { "SpaceWire Recorder", 18, 4 },
        { "SpaceWire PXI Interface", 21, 5 },
        { "SpaceWire PXI RMAP", 22, 5 },
        { "SpaceWire PXI 8 Port Router", 23, 9 },
        { "SpaceWire PXI 12 Port Router", 24, 9 },
        { "SpaceWire PCI Mk3", 27, 4 },
        { "SpaceWire PXI Interface Mk2", 29, 5 },
        { "SpaceWire PXI RMAP Mk2", 30, 5 },
        { "SpaceWire PXI Router Mk2", 31, 9 },
        { 0 },
};


/**
 * The callbacks to be called to deal with events relating to our device.
 */
struct pci_driver star_pci_driver =
{
    .name = STAR_PCI_MODULE_NAME,
    .id_table = star_pci_ids,
    .probe = spw_pci_probe,
    .remove = __devexit_p(spw_pci_remove),
    #ifdef CONFIG_PM
    .suspend = spw_pci_suspend,
    .resume = spw_pci_resume,
    #endif /* CONFIG_PM */
};


/**
 * The file operations supported by this device.
 */
struct file_operations star_pci_fileops =
{
    .owner = THIS_MODULE,
    .read = spw_pci_read,
    .write = spw_pci_write,
    #if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
    .ioctl = spw_pci_ioctl,
    #endif /* older IOCTL */
    #ifdef HAVE_UNLOCKED_IOCTL
    .unlocked_ioctl = spw_pci_unlocked_ioctl,
    #endif /* HAVE_UNLOCKED_IOCTL */
    #ifdef HAVE_COMPAT_IOCTL
    .compat_ioctl = spw_pci_compat_ioctl,
    #endif /* HAVE_COMPAT_IOCTL */
    .open = spw_pci_open,
    .release = spw_pci_release,
};


/**
 * Driver-kernel interface structure
 */
typedef struct
{
    void *pPriv;                    /* pointer to driver internal struct */

    struct cdev cDevice;            /* the character device associated with this PCI device */

    struct workqueue_struct *workQ; /* interrupt bottom half work queue */
    struct work_struct workTask;    /* bottom half worker task */

} star_kernel_intfc;


/**
 * Mapping of kernel functions to version-independent ones
 * =======================================================
 *
 * Note : the kernel versions supported are given for each call
 */
int spw_alloc_chrdev_region(void *dev, unsigned int baseminor, unsigned int count, const char *name)
{
    return alloc_chrdev_region((dev_t *)(dev), baseminor, count, name); /* 2.6.0 - latest */
}


void spw_unregister_chrdev_region(unsigned int from, unsigned int count)
{
    unregister_chrdev_region((dev_t)(from), count); /* 2.6.0 - latest */
}


void spw_cdev_init(void *cdev)
{
    cdev_init((struct cdev *)(cdev), &star_pci_fileops);    /* 2.6.0 - latest */
    ((struct cdev *)(cdev))->owner = THIS_MODULE;           /* 2.6.0 - latest */
}


int spw_cdev_add(void *p, unsigned int dev, unsigned int count)
{
    return cdev_add((struct cdev *)(p), (dev_t)(dev), count);   /* 2.6.0 - latest */
}


void spw_cdev_del(void *ptr)
{
    cdev_del(&(((star_kernel_intfc *)(ptr))->cDevice)); /* 2.6.0 - latest */
}


/*
 * Notes on classes, devices, creating '/dev' files etc.
 * =====================================================
 *
 * 2.6.0 - 2.6.1
 * -------------
 * 'class_register' to register class - type 'struct class' allocated
 * 'devfs_mk_cdev' to add device file, returns int
 * 'devfs_remove' to delete device file
 * 'class_unregister' to delete class
 *
 *
 * 2.6.2 - 2.6.12
 * --------------
 * 'class_simple_create' to create object of type 'struct class_simple'
 * 'devfs_mk_cdev' and 'class_simple_device_add' to add device file, returns type 'struct class_device *'
 * 'class_simple_device_remove' to delete device file
 * 'class_simple_destroy' to delete class
 *
 * 2.6.13 - 2.6.17
 * ---------------
 * 'class_create' to create object of type 'struct class'
 * 'devfs_mk_cdev' and 'class_device_create' to add device file, returns type 'struct class_device *'
 * 'class_device_destroy' to delete device file
 * 'class_destroy' to delete class
 *
 * 2.6.18 - 2.6.26
 * ---------------
 * 'class_create' to create object of type 'struct class'
 * 'device_create' to add device file, returns type 'struct device *'
 * 'device_destroy' to delete device file
 * 'class_destroy' to delete class
 *
 * 2.6.27 - latest
 * --------------
 * 'class_create' to create object of type 'struct class'
 * 'device_create' to add device file, returns type 'struct device *'
 * 'device_destroy' to delete device file
 * 'class_destroy' to delete class
 *
 * Note : the difference between pre and post-2.6.27 is a change in the number of parameters for calling
 * device_create, which uses varargs!  Extreme caution is advised if making any changes in this area...
 */

void *spw_class_create(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13))
    return class_create(THIS_MODULE, STAR_PCI_MODULE_NAME);         /* 2.6.13 - latest */
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2))
    return class_simple_create(THIS_MODULE, STAR_PCI_MODULE_NAME);  /* 2.6.2 - 2.6.12 */
#else
    struct class *cls = spw_kzalloc(sizeof(struct class), GFP_KERNEL);
    cls->name = name;
    if (class_register(cls))                                        /* 2.6.0 - 2.6.1 */
    {
        spw_kfree(cls);
        return NULL;
    }
    return cls;
#endif
}


void spw_class_destroy(void *cls)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13))
    class_destroy((struct class *)(cls));               /* 2.6.13 - latest */
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2))
    class_simple_destroy((struct class_simple *)(cls)); /* 2.6.2 - 2.6.12 */
#else
    class_unregister((struct class *)(cls));            /* 2.6.0 - 2.6.1 */
    spw_kfree((struct class *)(cls));
#endif
}


void *spw_device_create(void *class, void *parent, unsigned int devt, void *device, char *devname)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 27))
    return device_create(class, (struct device *)(parent), (dev_t)(devt), NULL, devname);   /* 2.6.27 - latest (note : change to varargs param list from 2.6.26!!) */
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18))
    return device_create(class, (struct device *)(parent), (dev_t)(devt), devname);         /* 2.6.18 - 2.6.26 */
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13))
    devfs_mk_cdev(devt, S_IRUSR|S_IWUSR|S_IRGRP, devname);                                  /* 2.6.0 - 2.6.17 */
    return class_device_create(class, (struct device *)(parent), (dev_t)(devt), (struct device *)(device), devname); /* 2.6.13 - 2.6.25 (but switch to device_create for 2.6.18 ff) */
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2))
    devfs_mk_cdev(devt, S_IRUSR|S_IWUSR|S_IRGRP, devname);                                  /* 2.6.0 - 2.6.17 */
    return class_simple_device_add(class, (dev_t)(devt), (struct device *)(device), devname); /* 2.6.2 - 2.6.12 */
#else
    return devfs_mk_cdev(devt, S_IFCHR|S_IRUSR|S_IWUSR, devname);                           /* 2.6.0 - 2.6.17 */
#endif
}


void spw_device_destroy(void *class, unsigned int devt, char *devname)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18))
    device_destroy(class, (dev_t)(devt));               /* 2.6.18 - latest */
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 13))
    class_device_destroy(class, (dev_t)(devt));         /* 2.6.13 - 2.6.25 (but switch to device_destroy for 2.6.18 ff) */
#elif (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 2))
    class_simple_device_remove((dev_t)(devt));          /* 2.6.2 - 2.6.12 */
#else
    devfs_remove(devname);  /* 2.6.0 - 2.6.1 */
#endif
}


unsigned int spw_mkdev(int major, int minor)
{
    return MKDEV(major, minor); /* 2.6.0 - latest */
}


int spw_get_major(unsigned int dev)
{
    return MAJOR((dev_t)(dev)); /* 2.6.0 - latest */
}


int spw_get_minor(unsigned int dev)
{
    return MINOR((dev_t)(dev)); /* 2.6.0 - latest */
}


int spw_pci_register_driver(void)
{
    return pci_register_driver(&star_pci_driver);   /* 2.6.0 - latest */
}


void spw_pci_unregister_driver(void)
{
    pci_unregister_driver(&star_pci_driver);    /* 2.6.0 - latest */
}


int spw_pci_enable_device(void *dev)
{
    return pci_enable_device((struct pci_dev *)(dev));  /* 2.6.0 - latest */
}


void spw_pci_disable_device(void *dev)
{
    pci_disable_device((struct pci_dev *)(dev));    /* 2.6.0 - latest */
}


int spw_pci_request_region(void *pdev, int bar, const char *res_name)
{
    return pci_request_region((struct pci_dev *)(pdev), bar, res_name); /* 2.6.0 - latest */
}


void spw_pci_release_regions(void *pdev)
{
    pci_release_regions((struct pci_dev *)(pdev));  /* 2.6.0 - latest */
}


unsigned long spw_pci_resource_start(void *pdev, int bar)
{
    return pci_resource_start((struct pci_dev *)(pdev), bar);   /* 2.6.0 - latest */
}


unsigned long spw_pci_resource_len(void *pdev, int bar)
{
    return pci_resource_len((struct pci_dev *)(pdev), bar); /* 2.6.0 - latest */
}


unsigned long spw_pci_resource_flags(void *pdev, int bar)
{
    return pci_resource_flags((struct pci_dev *)(pdev), bar);   /* 2.6.0 - latest */
}


void spw_pci_set_master(void *dev)
{
    pci_set_master((struct pci_dev *)(dev));    /* 2.6.0 - latest */
}


void spw_pci_set_drvdata(void *pdev, void *data)
{
    pci_set_drvdata((struct pci_dev *)(pdev), data); /* 2.6.0 - latest */
}


void *spw_pci_get_drvdata(void *pdev)
{
    return pci_get_drvdata((struct pci_dev *)(pdev));   /* 2.6.0 - latest */
}


int spw_request_irq(void *pdev, const char *name, void *dev)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18))
    return request_irq(((struct pci_dev *)(pdev))->irq, spw_interrupt_handler, IRQF_SHARED, name, dev); /* 2.6.18 - latest, with handler signature change from 2.6.20 */
#else
    return request_irq(((struct pci_dev *)(pdev))->irq, spw_interrupt_handler, SA_SHIRQ, name, dev);    /* 2.6.0 - 2.6.18 */
#endif
}


void spw_free_irq(void *pdev, void *dev_id)
{
    free_irq(((struct pci_dev *)(pdev))->irq, dev_id);  /* 2.6.0 - latest */
}


void *spw_kern_alloc(unsigned int size)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 14))
    return kzalloc(size, GFP_KERNEL);       /* 2.6.14 - latest */
#else
    void *ptr = kmalloc(size, GFP_KERNEL);  /* 2.6.0 - 2.6.13 */
    if (ptr != NULL)
    {
        spw_zero_mem(ptr, size);
    }
    return ptr;
#endif
}


void *spw_dma_alloc(unsigned int size)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 14))
    return kzalloc(size, GFP_KERNEL | __GFP_DMA);       /* 2.6.14 - latest */
#else
    void *ptr = kmalloc(size, GFP_KERNEL | __GFP_DMA);  /* 2.6.0 - 2.6.13 */
    if (ptr != NULL)
    {
        spw_zero_mem(ptr, size);
    }
    return ptr;
#endif
}


void spw_kfree(const void *p)
{
    kfree(p);   /* 2.6.0 - latest */
}


void spw_memcpy(void *to, const void *from, unsigned int size)
{
    memcpy(to, from, size); /* 2.6.0 - latest */
}


void spw_memcpy_toio(void *to, const void *from, unsigned int size)
{
    memcpy_toio(to, from, size);    /* 2.6.0 - latest */
}


void spw_memcpy_fromio(void *to, const void *from, unsigned int size)
{
    memcpy_fromio(to, from, size);  /* 2.6.0 - latest */
}


void spw_zero_mem(void *p, unsigned int size)
{
    memset(p, 0, size);     /* 2.6.0 - latest */
}


void *spw_ptr_align(void *p, int align)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 23))
    return PTR_ALIGN(p, align);                                                 /* 2.6.23 - latest */
#else
    return ((typeof(p))(ALIGN((unsigned long)(p), (unsigned long)(align))));    /* 2.6.0 - 2.6.22 */
#endif
}


int spw_down(void *sem)
{
    return down_interruptible((struct semaphore *)(sem));   /* 2.6.0 - latest */
}


void spw_up(void *sem)
{
    up((struct semaphore *)(sem));  /* 2.6.0 - latest */
}


void spw_sema_init(void *sem, int val)
{
    sema_init((struct semaphore *)(sem), val);  /* 2.6.0 - latest */
}


void spw_msleep(unsigned int msecs)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 7))
    msleep(msecs);                              /* 2.6.7 - latest */
#else
     set_current_state(TASK_UNINTERRUPTIBLE);   /* 2.6.0 - 2.6.6 - no 'msleep' or equivalent available */
     schedule_timeout(msecs_to_jiffies(msecs));
#endif
}


void spw_init_waitqueue_head(void *q)
{
    init_waitqueue_head((wait_queue_head_t *)(q));  /* 2.6.0 - latest */
}


int spw_wait_for_event(void *q, unsigned int *val)
{
    DEFINE_WAIT(wait);                              /* 2.6.0 - latest */
    prepare_to_wait(q, &wait, TASK_INTERRUPTIBLE);  /* 2.6.0 - latest */

    if (*val == 0)
    {
        schedule();                                 /* 2.6.0 - latest */
    }

    finish_wait(q, &wait);                          /* 2.6.0 - latest */

    if (signal_pending(current))                    /* 2.6.0 - latest */
    {
        return -ERESTARTSYS;                        /* 2.6.0 - latest */
    }

    return 0;
}


void spw_wake_up_interruptible(void *q)
{
    wake_up_interruptible((wait_queue_head_t *)(q));    /* 2.6.0 - latest */
}


void *spw_create_workqueue(const char *name)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 6))
    return create_singlethread_workqueue(name); /* 2.6.6 - latest */
#else
    return create_workqueue(name);              /* 2.6.0 - latest  (but switch to create_singlethread_workqueue for 2.6.6 ff) */
#endif
}


void spw_init_work(void *work, void *func)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20))
    INIT_WORK((struct work_struct *)(work), (work_func_t)(func));   /* 2.6.20 - latest */
#else
    INIT_WORK((struct work_struct *)(work), func, work);            /* 2.6.0 - 2.6.19 */
#endif
}


void spw_queue_work(void *ptr)
{
    queue_work(((star_kernel_intfc *)(ptr))->workQ, &(((star_kernel_intfc *)(ptr))->workTask)); /* 2.6.0 - latest */
}


void spw_flush_workqueue(void *ptr)
{
    if (((star_kernel_intfc *)(ptr))->workQ != NULL)
    {
        flush_workqueue(((star_kernel_intfc *)(ptr))->workQ);   /* 2.6.0 - latest */
    }
}


void spw_destroy_workqueue(void *ptr)
{
    if (((star_kernel_intfc *)(ptr))->workQ != NULL)
    {
        destroy_workqueue(((star_kernel_intfc *)(ptr))->workQ); /* 2.6.0 - latest */
    }
}


int spw_dma_set_mask(void *dev)
{
    return dma_set_mask((struct device *)(dev), DMA_BIT_MASK(32));   /* 2.6.0 - latest */
}


int spw_dma_set_coherent_mask(void *dev)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34))
    return dma_set_coherent_mask((struct device *)(dev), DMA_BIT_MASK(32));   /* 2.6.34 - latest */
#else
    return 0;   /* not available before 2.6.34 */
#endif
}


unsigned long spw_dma_map_single(void *dev, void *cpu_addr, unsigned int size, star_dma_direction direction)
{
    unsigned long addr;
    enum dma_data_direction dir;

    switch (direction)
    {
    case STAR_DMA_FROM_SPW :
        dir = DMA_FROM_DEVICE;
        break;

    case STAR_DMA_TO_SPW :
        dir = DMA_TO_DEVICE;
        break;

    case STAR_DMA_TO_FROM_SPW :
        dir = DMA_BIDIRECTIONAL;
        break;

    default:
        return 0;
    }

    addr = dma_map_single((struct device *)(dev), cpu_addr, size, dir); /* 2.6.0 - latest */

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 5))                     /* did not exist before 2.6.5 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 27))
    if (dma_mapping_error((struct device *)(dev), addr))                /* 2.6.27 - latest */
#else
    if (dma_mapping_error(addr))                                        /* 2.6.5 - 2.6.26 */
#endif
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "dma_map_single error : addr 0x%08x, len 0x%08x, dir %d\n", cpu_addr, size, dir);
        return 0;
    }
#endif

    return addr;
}


void spw_dma_unmap_single(void *dev, unsigned long dma_addr, unsigned int size, star_dma_direction direction)
{
    enum dma_data_direction dir = (direction == STAR_DMA_FROM_SPW) ? DMA_FROM_DEVICE : DMA_TO_DEVICE;   /* 2.6.0 - latest */

    dma_unmap_single((struct device *)(dev), dma_addr, size, dir);  /* 2.6.0 - latest */
}


void *spw_dma_alloc_coherent(void *dev, unsigned int size, volatile unsigned long *dma_handle)
{
    return dma_alloc_coherent((struct device *)(dev), size, (dma_addr_t *)(dma_handle), GFP_KERNEL | __GFP_DMA);    /* 2.6.0 - latest */
}


void spw_dma_free_coherent(void *dev, unsigned int size, void *cpu_addr, unsigned long dma_handle)
{
    dma_free_coherent((struct device *)(dev), size, cpu_addr, dma_handle);  /* 2.6.0 - latest */
}


void *spw_pci_iomap(void *dev, int bar, unsigned long size)
{
    return pci_iomap((struct pci_dev *)(dev), bar, size);  /* 2.6.0 - latest */
}


#if 0
void *spw_ioremap_nocache(unsigned long offset, unsigned long size)
{
    return ioremap_nocache(offset, size);   /* 2.6.0 - latest */
}
#endif


long spw_copy_to_user(void __user *to, const void *from, unsigned long n)
{
    return copy_to_user(to, from, n);   /* 2.6.0 - latest */
}


long spw_copy_from_user(void *to, const void __user *from, unsigned long n)
{
    return copy_from_user(to, from, n); /* 2.6.0 - latest */
}


void spw_set_bit(int nr, volatile unsigned long *addr)
{
    set_bit(nr, addr);  /* 2.6.0 - latest */
}


void spw_clear_bit(int nr, volatile unsigned long *addr)
{
    clear_bit(nr, addr);    /* 2.6.0 - latest */
}


void spw_mb(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 31))
    mb();       /* 2.6.31 - latest */
#else
    barrier();  /* 2.6.0 - 2.6.30 */
#endif
}


void spw_rmb(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 31))
    rmb();      /* 2.6.31 - latest */
#else
    barrier();  /* 2.6.0 - 2.6.30 */
#endif
}


void spw_wmb(void)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 31))
    wmb();      /* 2.6.31 - latest */
#else
    barrier();  /* 2.6.0 - 2.6.30 */
#endif
}


unsigned char spw_ioread8(void *addr)
{
    return ioread8(addr); /* 2.6.0 - latest */
}


unsigned short spw_ioread16(void *addr)
{
    return ioread16(addr); /* 2.6.0 - latest */
}


unsigned int spw_ioread32(void *addr)
{
    return ioread32(addr); /* 2.6.0 - latest */
}


void spw_iowrite8(unsigned char v, void *addr)
{
    iowrite8(v, addr);    /* 2.6.0 - latest */
}


void spw_iowrite16(unsigned short v, void *addr)
{
    iowrite16(v, addr);    /* 2.6.0 - latest */
}


void spw_iowrite32(unsigned int v, void *addr)
{
    iowrite32(v, addr);    /* 2.6.0 - latest */
}


int spw_pci_read_config_byte(void *dev, int where, u8 *val)
{
    return pci_read_config_byte((struct pci_dev *)(dev), where, val);   /* 2.6.0 - latest */
}


int spw_pci_read_config_word(void *dev, int where, u16 *val)
{
    return pci_read_config_word((struct pci_dev *)(dev), where, val);   /* 2.6.0 - latest */
}


int spw_pci_read_config_dword(void *dev, int where, u32 *val)
{
    return pci_read_config_dword((struct pci_dev *)(dev), where, val);  /* 2.6.0 - latest */
}


int spw_pci_write_config_byte(void *dev, int where, u8 val)
{
    return pci_write_config_byte((struct pci_dev *)(dev), where, val);  /* 2.6.0 - latest */
}


int spw_pci_write_config_word(void *dev, int where, u16 val)
{
    return pci_write_config_word((struct pci_dev *)(dev), where, val);  /* 2.6.0 - latest */
}


int spw_pci_write_config_dword(void *dev, int where, u32 val)
{
    return pci_write_config_dword((struct pci_dev *)(dev), where, val); /* 2.6.0 - latest */
}


void spw_print_msg(int level, char *fmt, ...)
{
    va_list argp;
    char msg[512];

    va_start(argp, fmt);
    vsnprintf(msg, 512, fmt, argp);
    va_end(argp);

    if ((level == STAR_MESSAGE_LEVEL_ERROR) && (message_level >= STAR_MESSAGE_LEVEL_ERROR))
    {
        printk("%sSTAR-PCI : %s", KERN_ERR, msg);      /* 2.6.0 - latest */
    }
    else
    if ((level == STAR_MESSAGE_LEVEL_WARNING) && (message_level >= STAR_MESSAGE_LEVEL_WARNING))
    {
        printk("%sSTAR-PCI : %s", KERN_WARNING, msg);  /* 2.6.0 - latest */
    }
    else
    if ((level == STAR_MESSAGE_LEVEL_DEBUG) && (message_level >= STAR_MESSAGE_LEVEL_DEBUG))
    {
        printk("%sSTAR-PCI : %s", KERN_DEBUG, msg);    /* 2.6.0 - latest */
    }
}


extern void spw_snprintf(char *buf, int size, const char *fmt, ...)
{
    va_list argp;

    va_start(argp, fmt);
    vsnprintf(buf, size, fmt, argp);
    va_end(argp);
}


int spw_strlen(char *str)
{
    return strlen(str); /* 2.6.0 - latest */
}


void spw_strncpy(char *to, const char *from, int len)
{
    strncpy(to, from, len); /* 2.6.0 - latest */
}


/**
 * Additional support functions
 * ============================
 *
 * Note : these only use functions above, and do not interface with the kernel directly
 */
void *spw_init_kernel_structs(char *name, void *pPriv)
{
    star_kernel_intfc *intfc = spw_kern_alloc(sizeof(star_kernel_intfc));
    if (intfc == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Failed to alloc intfc struct\n");
        return NULL;
    }

    /* create workqueue for interrupt processing */
    intfc->workQ = spw_create_workqueue(name);
    if (intfc->workQ == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Failed to create workqueue\n");
        spw_kfree(intfc);
        return NULL;
    }

    /* create worker task for queuing */
    spw_init_work(&(intfc->workTask), spw_pci_worker);

    intfc->pPriv = pPriv;

    return intfc;
}


void spw_release_kernel_structs(void *ptr)
{
    star_kernel_intfc *intfc = ptr;

    if (intfc == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "NULL interface pointer in release_kernel_structs!\n");
        return;
    }

    spw_flush_workqueue(intfc);
    spw_destroy_workqueue(intfc);

    spw_kfree(ptr);
}


int spw_check_vendor_device(void *pdev)
{
    struct pci_dev *pci_device = (struct pci_dev *)(pdev); /* 2.6.0 - latest */

    unsigned short vendor_id = pci_device->vendor;
    unsigned short device_id = pci_device->device;

    /* get last index into device ID table */
    int devIDidx = sizeof(star_pci_ids) / sizeof(struct pci_device_id) - 2;

    /* compare vendor and device IDs to see if we really should handle this device */
    for (; devIDidx>=0; --devIDidx)
    {
        if ((star_pci_ids[devIDidx].vendor == vendor_id) && (star_pci_ids[devIDidx].device == device_id))
        {
            /* return table index of matching device */
            return devIDidx;
        }
    }

    /* no match found - return negative value */
    return -1;
}


int spw_get_device_id(void *pdev)
{
    return ((struct pci_dev *)(pdev))->device;
}


void *spw_get_dev(void *pdev)
{
    return &(((struct pci_dev *)(pdev))->dev);
}


void *spw_get_cdev_addr(void *ptr)
{
    return &(((star_kernel_intfc *)(ptr))->cDevice);
}


void *spw_create_mutex(void)
{
    struct semaphore *sem = spw_kern_alloc(sizeof(struct semaphore));
    if (sem == NULL)
    {
        return NULL;
    }

    spw_sema_init(sem, 1);

    return sem;
}


void *spw_create_wait_queue(void)
{
    wait_queue_head_t *wq = spw_kern_alloc(sizeof(wait_queue_head_t));
    if (wq == NULL)
    {
        return NULL;
    }

    spw_init_waitqueue_head(wq);

    return wq;
}


/**
 * Module init and exit fns
 * ========================
 */
static int __init spw_pci_init(void)
{
    return star_pci_init();
}


static void __exit spw_pci_exit(void)
{
    star_pci_exit();
}


/**
 * PCI driver-specific functions
 * =============================
 */
int __devinit spw_pci_probe(struct pci_dev *pDev, const struct pci_device_id *pId)
{
    return star_pci_probe(pDev);
}


void __devexit spw_pci_remove(struct pci_dev *pDev)
{
    return star_pci_remove(pDev);
}


#ifdef CONFIG_PM
int spw_pci_suspend(struct pci_dev *pDev, STAR_PCI_SUSPEND_STATE_TYPE state)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 12))
    return star_pci_suspend(pDev, state.event); /* 2.6.12 - latest */
#else
    return star_pci_suspend(pDev, state);       /* 2.6.0 - 2.6.11 */
#endif
}


int spw_pci_resume(struct pci_dev *pDev)
{
    return star_pci_resume(pDev);
}
#endif


/**
 * File operation and IOCTL functions
 * ==================================
 */
int spw_pci_open(struct inode *inode, struct file *filp)
{
    star_kernel_intfc *pIntfc;
    void *pPriv;

    /* get device data pointer */
    pIntfc = container_of(inode->i_cdev, star_kernel_intfc, cDevice);   /* 2.6.0 - latest */
    if ((pIntfc == NULL) || IS_ERR(pIntfc))
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "container_of failed in 'open' : %ld\n", PTR_ERR(pIntfc));
        return STAR_INVALID_ARG_ERROR;
    }

    /* get driver internal data pointer */
    pPriv = pIntfc->pPriv;
    if (pPriv == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'open' : %ld\n", PTR_ERR(pPriv));
        return STAR_NO_SUCH_DEVICE_ERROR;
    }

    /* save channel info in private data */
    filp->private_data = star_pci_open(pPriv);  /* 2.6.0 - latest (for ->private_data) */
    if (filp->private_data == NULL)
    {
        return STAR_NO_SUCH_FILE_ERROR;
    }

    return 0;
}


int spw_pci_release(struct inode *inode, struct file *filp)
{
    void *pFile = filp->private_data;   /* 2.6.0 - latest */

    if (pFile == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'release' : %ld\n", PTR_ERR(pFile));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    return star_pci_release(pFile);
}


ssize_t spw_pci_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    void *pFile = filp->private_data;   /* 2.6.0 - latest (for ->private_data) */
    ssize_t len;

    if ((pFile == NULL) || IS_ERR(pFile))
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'read' : %ld\n", PTR_ERR(pFile));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    len = star_pci_read(pFile, buff, count);
    if (len > 0)
    {
        *offp += len;
    }

    return len;
}


ssize_t spw_pci_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    void *pFile = filp->private_data;   /* 2.6.0 - latest (for ->private_data) */
    ssize_t len;

    if ((pFile == NULL) || IS_ERR(pFile))
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'write' : %ld\n", PTR_ERR(pFile));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    len = star_pci_write(pFile, buff, count);
    if (len > 0)
    {
        *offp += len;
    }

    return len;
}


long spw_pci_common_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    void *pFile = filp->private_data;   /* 2.6.0 - latest (for ->private_data) */

    if (pFile == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'ioctl' : %ld\n", PTR_ERR(pFile));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    return star_pci_ioctl(pFile, cmd, arg);
}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
int spw_pci_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    return spw_pci_common_ioctl(filp, cmd, arg);
}
#endif


#ifdef HAVE_UNLOCKED_IOCTL
long spw_pci_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return spw_pci_common_ioctl(filp, cmd, arg);
}
#endif


#ifdef HAVE_COMPAT_IOCTL
long spw_pci_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return spw_pci_common_ioctl(filp, cmd, arg);
}
#endif


/**
 * Interrupt service routine
 * =========================
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 19))
irqreturn_t spw_interrupt_handler(int irq, void *pDevId)    /* 2.6.19 - latest */
#else
irqreturn_t spw_interrupt_handler(int irq, void *pDevId, struct pt_regs *pRegs) /* 2.6.0 - 2.6.18 */
#endif
{
    if (star_pci_interrupt_handler(irq, pDevId))
    {
        return IRQ_NONE;    /* 2.6.0 - latest */
    }
    else
    {
        return IRQ_HANDLED; /* 2.6.0 - latest */
    }
}


/**
 * Interrupt worker task
 * =====================
 */
void spw_pci_worker(struct work_struct *work)
{
    star_kernel_intfc *pIntfc;
    void *pPriv;

    /* get device data pointer */
    pIntfc = container_of(work, star_kernel_intfc, workTask);   /* 2.6.0 - latest */
    if ((pIntfc == NULL) || IS_ERR(pIntfc))
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "container_of failed in worker : %ld\n", PTR_ERR(pIntfc));
        return;
    }

    /* get driver internal data pointer */
    pPriv = pIntfc->pPriv;
    if (pPriv == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in worker : %ld\n", PTR_ERR(pPriv));
        return;
    }

    star_pci_worker(pPriv);
}
