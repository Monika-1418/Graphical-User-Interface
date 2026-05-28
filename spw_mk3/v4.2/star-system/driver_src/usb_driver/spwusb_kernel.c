/**
 * \file spwusb_kernel.c
 *
 * \brief Linux kernel interface for the STAR-Dundee USB Linux Driver.
 *
 * \author STAR-Dundee Ltd.\n
 *         STAR House\n
 *         166 Nethergate\n
 *         Dundee, DD1 4EE\n
 *         Scotland, UK\n
 *         e-mail: support@star-dundee.com
 *
 * This file contains kernel interface functions for
 * the STAR-Dundee USB Linux Driver.
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
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/workqueue.h>

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 18))
#include <linux/uaccess.h>
#else
#include <asm/uaccess.h>
#endif

#ifndef LINUX_VERSION_CODE
#include <linux/version.h>
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0))
#include <linux/sched/signal.h>
#endif

#include "spwusb_kernel.h"


/* Specify the license used by the driver */
MODULE_LICENSE("GPL");

/* Specify the driver author */
MODULE_AUTHOR("David Paterson, STAR-Dundee, support@star-dundee.com");

/* Provide a description of the module */
MODULE_DESCRIPTION("STAR-Dundee STAR-System USB Driver for Linux");

/* Specify the version of the driver */
MODULE_VERSION("4.0");


/**
 * Types which are dependent on kernel version
 */
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 12))
    #define STAR_USB_SUSPEND_STATE_TYPE pm_message_t
#else
    #define STAR_USB_SUSPEND_STATE_TYPE u32
#endif

/* __devinit and __devexit were removed in kernel 3.8 */
#ifndef __devinit
    #define __devinit
#endif
#ifndef __devexit
    #define __devexit
#endif


/**
 * Forward declarations of driver-specific functions
 */
static int __init spw_usb_init(void);
static void __exit spw_usb_exit(void);

int __devinit spw_usb_probe(struct usb_interface *usb_intfc, const struct usb_device_id *id);
void __devexit spw_usb_disconnect(struct usb_interface *usb_intfc);
int spw_usb_suspend(struct usb_interface *usb_intfc, STAR_USB_SUSPEND_STATE_TYPE state);
int spw_usb_resume(struct usb_interface *usb_intfc);

int spw_usb_open(struct inode *inode, struct file *filp);
int spw_usb_release(struct inode *inode, struct file *filp);
ssize_t spw_usb_read(struct file *filp, char __user *buff, size_t count, loff_t *offp);
ssize_t spw_usb_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
int spw_usb_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#ifdef HAVE_UNLOCKED_IOCTL
long spw_usb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#ifdef HAVE_COMPAT_IOCTL
long spw_usb_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 19))
void spw_usb_callback(struct urb *xfer_urb);
#else
void spw_usb_callback(struct urb *xfer_urb, struct pt_regs *regs);
#endif

void spw_usb_worker(struct work_struct *work_ptr);


/**
 * Error return codes
 */
const int STAR_NO_SUCH_FILE_ERROR = -ENOENT;    /* 2.6.0 - latest */
const int STAR_NO_MEMORY_ERROR = -ENOMEM;       /* 2.6.0 - latest */
const int STAR_BAD_ADDRESS_ERROR = -EFAULT;     /* 2.6.0 - latest */
const int STAR_NO_SUCH_DEVICE_ERROR = -ENODEV;  /* 2.6.0 - latest */
const int STAR_INVALID_ARG_ERROR = -EINVAL;     /* 2.6.0 - latest */
const int STAR_NOT_A_TTY_ERROR = -ENOTTY;       /* 2.6.0 - latest */
const int STAR_ENDPOINT_SHUTDOWN = -ESHUTDOWN;  /* 2.6.0 - latest */
const int STAR_INVALID_OPERATION = -EPERM;      /* 2.6.0 - latest */


/**
 * Set the functions called when initialising and exiting the driver
 */
module_init(spw_usb_init);
module_exit(spw_usb_exit);


/**
 * The level of informational messages to output.
 */
int message_level = STAR_MESSAGE_LEVEL_DEBUG;
module_param(message_level, int, S_IRUSR|S_IWUSR);
MODULE_PARM_DESC(message_level, "The level of informational messages to output (1 = error, 2 = warning, 3 = debug, 0 for no messages)");


/**
 * The list of devices supported by this driver.
 */
static const struct usb_device_id star_usb_ids[] =
{
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_ROUTER_MK2S) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_BRICK_MK2) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_LINK_ANALYSER_MK2) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_RTC) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_EGSE) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_SPLT) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_STAR_FIRE) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_WBS_II) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_BRICK_MK3) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_CONF_TESTER) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_STAR_FIRE_MK3) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_LINK_ANALYSER_MK3) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_CONF_TESTER_MK2) },
    { USB_DEVICE(SPW_USB_STAR_DUNDEE_VENDOR_ID, SPW_USB_EGSE_MK2) },
    { 0, },
};


/**
 * Export device ID table to user space for module loading systems.
 */
MODULE_DEVICE_TABLE(usb, star_usb_ids);


/**
 * Additional device information table
 * (must be same length as USB ID table above)
 */
const star_device_info star_device_info_table[] =
{
        { "SpaceWire Router Mk2S", 7, 2, 0, 1, 1,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "SpaceWire Brick Mk2", 9, 2, 0, 1, 1,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "SpaceWire Link Analyser Mk2", 10, 2, 0, 0, 0,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "SpaceWire RTC", 12, 2, 0, 1, 1,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "SpaceWire EGSE", 13, 2, 0, 0, 0,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "SpaceWire Physical Layer Tester", 15, 2, 0, 1, 1,
            SPW_USB_LINK_EVENT_100000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "STAR Fire", 16, 2, 0, 1, 1,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "Wide Band Spectrometer II", 17, 2, 0, 1, 1,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "SpaceWire Brick Mk3", 19, 3, 0, 1, 1,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "SpaceWire Conformance Tester", 5, 2, 0, 0, 0,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_0_MS },
        { "STAR Fire Mk3", 26, 3, 0, 1, 1,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_2_MS },
        { "SpaceWire Link Analyser Mk3", 28, 3, 0, 0, 0,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_2_MS },
        { "SpaceWire Conformance Tester Mk2", 29, 3, 0, 0, 0,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_2_MS },
        { "SpaceWire EGSE Mk2", 30, 3, 0, 0, 0,
            SPW_USB_LINK_EVENT_200000,
            SPW_USB_POST_RESET_DELAY_2_MS },
        { 0 },
};


/**
 * The callbacks to be called to deal with events relating to our device.
 */
struct usb_driver star_usb_driver =
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16))
    .owner = THIS_MODULE,       /* 2.6.0 - 2.6.15 */
#endif
    .name = STAR_USB_MODULE_NAME,
    .id_table = star_usb_ids,
    .probe = spw_usb_probe,
    .disconnect = spw_usb_disconnect,
    .suspend = spw_usb_suspend,
    .resume = spw_usb_resume,
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 23))
    .reset_resume = spw_usb_resume,
#endif
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 24))
    .supports_autosuspend = 0,
#endif
};


/**
 * The file operations supported by this device.
 */
struct file_operations star_usb_fileops =
{
    .owner = THIS_MODULE,
    .read = spw_usb_read,
    .write = spw_usb_write,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
    .ioctl = spw_usb_ioctl,
#endif
#ifdef HAVE_UNLOCKED_IOCTL
    .unlocked_ioctl = spw_usb_unlocked_ioctl,
#endif /* HAVE_UNLOCKED_IOCTL */
#ifdef HAVE_COMPAT_IOCTL
    .compat_ioctl = spw_usb_compat_ioctl,
#endif /* HAVE_COMPAT_IOCTL */
    .open = spw_usb_open,
    .release = spw_usb_release,
};


/**
 * The device class definition.
 */
static struct usb_class_driver star_usb_class = {
    .name = STAR_USB_CLASS_NAME,
    .fops = &star_usb_fileops,
    .minor_base = STAR_USB_MINOR_BASE,
};


/**
 * Driver-kernel interface structure
 */
typedef struct
{
    void *priv_data;                    /* pointer to driver internal struct */

    struct usb_interface *usb_intfc;    /* the usb_intfc for this device */
    struct usb_device *usb_dev;         /* the usb device for this device */

    struct workqueue_struct *work_q;    /* interrupt bottom half work queue */

} star_kernel_intfc;


/**
 * Transfer info structure
 */
typedef struct
{
    void *xfer_buff;                    /* pointer to transfer buffer */

    struct work_struct work_task;       /* bottom half work task */

} star_xfer_info;


/**
 * Mapping of kernel functions to version-independent ones
 *
 * Note : the kernel versions supported are given for each call
 */
int spw_usb_register_driver(void)
{
    return usb_register(&star_usb_driver);  /* 2.6.0 - latest */
}


void spw_usb_deregister_driver(void)
{
    usb_deregister(&star_usb_driver);       /* 2.6.0 - latest */
}


int spw_usb_register_device(void *kern_intfc)
{
    return usb_register_dev(((star_kernel_intfc *)(kern_intfc))->usb_intfc, &star_usb_class);   /* 2.6.0 - latest */
}


void spw_usb_deregister_device(void *kern_intfc)
{
    usb_deregister_dev(((star_kernel_intfc *)(kern_intfc))->usb_intfc, &star_usb_class);    /* 2.6.0 - latest */
}


void spw_usb_set_intfdata(void *usb_intfc, void *data)
{
    usb_set_intfdata((struct usb_interface *)(usb_intfc), data);   /* 2.6.0 - latest */
}


void *spw_usb_get_intfdata(void *usb_intfc)
{
    return usb_get_intfdata((struct usb_interface *)(usb_intfc));  /* 2.6.0 - latest */
}


void spw_usb_enable_autosuspend(void *kern_intfc)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34))
    struct usb_device *usb_dev = ((star_kernel_intfc *)(kern_intfc))->usb_dev;  /* 2.6.0 - latest */
    usb_enable_autosuspend(usb_dev);
#endif
}


void spw_usb_disable_autosuspend(void *kern_intfc)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 34))
    struct usb_device *usb_dev = ((star_kernel_intfc *)(kern_intfc))->usb_dev;  /* 2.6.0 - latest */
    usb_disable_autosuspend(usb_dev);
#endif
}


void *spw_usb_alloc_urb(void)
{
    return usb_alloc_urb(0, GFP_KERNEL);    /* 2.6.0 - latest */
}


void spw_usb_free_urb(void *urb)
{
    usb_free_urb(urb);      /* 2.6.0 - latest */
}


void spw_fill_bulk_urb(void *kern_intfc, void *urb, unsigned int pipe, void *buffer, int length, void *context, int direction, int ZLP)
{
    struct urb *temp = urb;

    if (direction)
    {
        usb_fill_bulk_urb(urb, ((star_kernel_intfc *)(kern_intfc))->usb_dev,
                          usb_rcvbulkpipe((((star_kernel_intfc *)(kern_intfc))->usb_dev), pipe),
                          buffer, length, spw_usb_callback, context);   /* 2.6.0 - latest */

        temp->transfer_flags &= ~URB_ZERO_PACKET;
    }
    else
    {
        usb_fill_bulk_urb(urb, ((star_kernel_intfc *)(kern_intfc))->usb_dev,
                          usb_sndbulkpipe((((star_kernel_intfc *)(kern_intfc))->usb_dev), pipe),
                          buffer, length, spw_usb_callback, context);   /* 2.6.0 - latest */

        if (ZLP)
        {
            temp->transfer_flags |= URB_ZERO_PACKET;
        }
    }
}


int spw_usb_submit_urb(void *urb)
{
    return usb_submit_urb(urb, GFP_KERNEL);     /* 2.6.0 - latest */
}


int spw_usb_kill_urb(void *urb)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 8))
    usb_kill_urb(urb);              /* 2.6.8 - latest */
    return 0;
#else
    return usb_unlink_urb(urb);     /* 2.6.0 - 2.6.7 */
    /* note - "unlink" is synchronous provided the URB_ASYNC_UNLINK flag is not set, but */
    /* this action is deprecated from 2.6.8 onwards, and "kill" should be used instead */
#endif
}


int spw_usb_control_msg(void *kern_intfc, unsigned int pipe, unsigned char request, unsigned short value,
                        unsigned short index, int direction, void *data, unsigned short size)
{
    struct usb_device *usb_dev = ((star_kernel_intfc *)(kern_intfc))->usb_dev;  /* 2.6.0 - latest */

    if (direction)
    {
        return usb_control_msg(usb_dev, usb_rcvctrlpipe(usb_dev, pipe), request, 0xc0, value, index, data, size, 0);    /* 2.6.0 - latest */
    }
    else
    {
        return usb_control_msg(usb_dev, usb_sndctrlpipe(usb_dev, pipe), request, 0x40, value, index, data, size, 0);    /* 2.6.0 - latest */
    }
}


int spw_usb_bulk_msg(void *kern_intfc, unsigned int pipe, int direction, void *data, int size, int *readSz, int timeout)
{
    struct usb_device *usb_dev = ((star_kernel_intfc *)(kern_intfc))->usb_dev;  /* 2.6.0 - latest */

    if (direction)
    {
        return usb_bulk_msg(usb_dev, usb_rcvbulkpipe(usb_dev, pipe), data, size, readSz, timeout);    /* 2.6.0 - latest */
    }
    else
    {
        return usb_bulk_msg(usb_dev, usb_sndbulkpipe(usb_dev, pipe), data, size, readSz, timeout);    /* 2.6.0 - latest */
    }
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


void spw_kfree(const void *p)
{
    kfree(p);   /* 2.6.0 - latest */
}


void spw_zero_mem(void *p, unsigned int size)
{
    memset(p, 0, size);     /* 2.6.0 - latest */
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
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Failed to allocate wait queue\n");
        return NULL;
    }

    spw_init_waitqueue_head(wq);

    return wq;
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


int spw_wait_for_event(void *q)
{
    DEFINE_WAIT(wait);                              /* 2.6.0 - latest */
    prepare_to_wait(q, &wait, TASK_INTERRUPTIBLE);  /* 2.6.0 - latest */
    schedule();                                     /* 2.6.0 - latest */
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


void spw_init_work(void *work)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 20))
    INIT_WORK((struct work_struct *)(work), (work_func_t)(spw_usb_worker));   /* 2.6.20 - latest */
#else
    INIT_WORK((struct work_struct *)(work), spw_usb_worker, work);            /* 2.6.0 - 2.6.19 */
#endif
}


void spw_queue_work(void *kern_intfc, void *xfer_info)
{
    queue_work(((star_kernel_intfc *)(kern_intfc))->work_q, &(((star_xfer_info *)(xfer_info))->work_task));   /* 2.6.0 - latest */
}


void spw_flush_workqueue(void *kern_intfc)
{
    if (((star_kernel_intfc *)(kern_intfc))->work_q != NULL)
    {
        flush_workqueue(((star_kernel_intfc *)(kern_intfc))->work_q);   /* 2.6.0 - latest */
    }
}


void spw_destroy_workqueue(void *kern_intfc)
{
    if (((star_kernel_intfc *)(kern_intfc))->work_q != NULL)
    {
        destroy_workqueue(((star_kernel_intfc *)(kern_intfc))->work_q); /* 2.6.0 - latest */
    }
}


long spw_copy_to_user(void __user *to, const void *from, unsigned long n)
{
    return copy_to_user(to, from, n);   /* 2.6.0 - latest */
}


long spw_copy_from_user(void *to, const void __user *from, unsigned long n)
{
    return copy_from_user(to, from, n); /* 2.6.0 - latest */
}


/**
 * Additional support functions
 */
int spw_check_vendor_device(void *usb_intfc)
{
    struct usb_device *usb_dev = interface_to_usbdev(((struct usb_interface *)usb_intfc)); /* 2.6.0 - latest */

    unsigned short vendor_id = usb_dev->descriptor.idVendor;
    unsigned short product_id = usb_dev->descriptor.idProduct;

    /* get last index into device ID table */
    int devIDidx = sizeof(star_usb_ids) / sizeof(struct usb_device_id) - 2;

    /* compare vendor and product IDs to see if we really should handle this device */
    for (; devIDidx>=0; --devIDidx)
    {
        if ((star_usb_ids[devIDidx].idVendor == vendor_id) && (star_usb_ids[devIDidx].idProduct == product_id))
        {
            /* return table index of matching device */
            return devIDidx;
        }
    }

    /* no match found - return negative value */
    return -1;
}


void *spw_init_kernel_structs(void *priv_data, void *usb_intfc, char *name)
{
    star_kernel_intfc *kernel_interface = spw_kern_alloc(sizeof(star_kernel_intfc));
    if (kernel_interface == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Failed to alloc kernel usb_intfc struct\n");
        return NULL;
    }

    kernel_interface->priv_data = priv_data;
    kernel_interface->usb_intfc = usb_intfc;
    kernel_interface->usb_dev = usb_get_dev(interface_to_usbdev(((struct usb_interface *)usb_intfc))); /* 2.6.0 - latest */

    /* create workqueue for interrupt processing */
    kernel_interface->work_q = spw_create_workqueue(name);
    if (kernel_interface->work_q == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Failed to create workqueue\n");
        spw_kfree(kernel_interface);
        return NULL;
    }

    return kernel_interface;
}


void spw_release_kernel_structs(void *kern_intfc)
{
    if (kern_intfc != NULL)
    {
        usb_put_dev(((star_kernel_intfc *)(kern_intfc))->usb_dev); /* 2.6.0 - latest */

        spw_flush_workqueue(kern_intfc);
        spw_destroy_workqueue(kern_intfc);

        spw_kfree(kern_intfc);
    }
}


void *spw_init_xfer_info(void *buff)
{
    star_xfer_info *xfer_info = spw_kern_alloc(sizeof(star_xfer_info));
    if (xfer_info == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Failed to alloc star_xfer_info struct\n");
        return NULL;
    }

    spw_init_work(&(xfer_info->work_task));
    xfer_info->xfer_buff = buff;

    return xfer_info;
}


int spw_get_num_usb_endpoints(void *usb_intfc)
{
    struct usb_interface *interface = (struct usb_interface *)(usb_intfc);

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 5))
    return interface->cur_altsetting->desc.bNumEndpoints;   /* 2.6.5 - latest */
#else
    return interface->altsetting[interface->act_altsetting]->desc.bNumEndpoints;    /* 2.6.0 - 2.6.4 */
#endif
}


void spw_get_usb_endpoint_info(void *usb_intfc, int endptno, int *address, int *max_packet, int *direction)
{
    struct usb_interface *interface = (struct usb_interface *)(usb_intfc);
    struct usb_endpoint_descriptor *endpoint;

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 5))
    endpoint = &(interface->cur_altsetting->endpoint[endptno].desc);    /* 2.6.5 - latest */
#else
    endpoint = &(interface->altsetting[interface->act_altsetting]->endpoint[endptno].desc); /* 2.6.0 - 2.6.4 */
#endif

    *address = endpoint->bEndpointAddress & USB_ENDPOINT_NUMBER_MASK;                   /* 2.6.0 - latest */
    *max_packet = endpoint->wMaxPacketSize;                                             /* 2.6.0 - latest */
    *direction = (endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN;    /* 2.6.0 - latest */
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
        printk("%sSTAR-USB : %s", KERN_ERR, msg);      /* 2.6.0 - latest */
    }
    else
    if ((level == STAR_MESSAGE_LEVEL_WARNING) && (message_level >= STAR_MESSAGE_LEVEL_WARNING))
    {
        printk("%sSTAR-USB : %s", KERN_WARNING, msg);  /* 2.6.0 - latest */
    }
    else
    if ((level == STAR_MESSAGE_LEVEL_DEBUG) && (message_level >= STAR_MESSAGE_LEVEL_DEBUG))
    {
        printk("%sSTAR-USB : %s", KERN_DEBUG, msg);    /* 2.6.0 - latest */
    }
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
 * Module init and exit functions
 */
static int __init spw_usb_init(void)
{
    return star_usb_init();
}


static void __exit spw_usb_exit(void)
{
    star_usb_exit();
}


/**
 * USB driver-specific functions
 */
int __devinit spw_usb_probe(struct usb_interface *usb_intfc, const struct usb_device_id *id)
{
    return star_usb_probe(usb_intfc);
}


void __devexit spw_usb_disconnect(struct usb_interface *usb_intfc)
{
    star_usb_disconnect(usb_intfc);
}


int spw_usb_suspend(struct usb_interface *usb_intfc, STAR_USB_SUSPEND_STATE_TYPE state)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 12))
    return star_usb_suspend(usb_intfc, state.event); /* 2.6.12 - latest */
#else
    return star_usb_suspend(usb_intfc, state);       /* 2.6.0 - 2.6.11 */
#endif
}


int spw_usb_resume(struct usb_interface *usb_intfc)
{
    return star_usb_resume(usb_intfc);
}


/**
 * File operation and IOCTL functions
 */
int spw_usb_open(struct inode *inode, struct file *filp)
{
    struct usb_interface *usb_intfc;
    void *priv_data;

    /* get this device's minor number */
    int minor = iminor(inode);      /* 2.6.0 - latest */

    /* and find the associated USB usb_intfc */
    usb_intfc = usb_find_interface(&star_usb_driver, minor);        /* 2.6.0 - latest */
    if (usb_intfc == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Unable to find USB usb_intfc for minor %d\n", minor);
        return STAR_NO_SUCH_DEVICE_ERROR;
    }

    /* get the private data pointer */
    priv_data = usb_get_intfdata(usb_intfc);    /* 2.6.0 - latest */
    if (priv_data == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Unable to access driver data for minor %d\n", minor);
        return STAR_NO_SUCH_DEVICE_ERROR;
    }

    /* save channel info in private data */
    filp->private_data = star_usb_open(priv_data, minor);  /* 2.6.0 - latest (for ->private_data) */
    if (filp->private_data == NULL)
    {
        return STAR_NO_SUCH_FILE_ERROR;
    }

    return 0;
}


int spw_usb_release(struct inode *inode, struct file *filp)
{
    void *file = filp->private_data;   /* 2.6.0 - latest (for ->private_data) */

    if (file == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'release' : %ld\n", PTR_ERR(file));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    return star_usb_release(file);
}


ssize_t spw_usb_read(struct file *filp, char __user *buff, size_t count, loff_t *offp)
{
    void *file = filp->private_data;   /* 2.6.0 - latest (for ->private_data) */
    ssize_t len;

    if ((file == NULL) || IS_ERR(file))
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'read' : %ld\n", PTR_ERR(file));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    len = star_usb_read(file, buff, count);
    if (len > 0)
    {
        *offp += len;
    }

    return len;
}


ssize_t spw_usb_write(struct file *filp, const char __user *buff, size_t count, loff_t *offp)
{
    void *file = filp->private_data;   /* 2.6.0 - latest (for ->private_data) */
    ssize_t len;

    if ((file == NULL) || IS_ERR(file))
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'write' : %ld\n", PTR_ERR(file));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    len = star_usb_write(file, buff, count);
    if (len > 0)
    {
        *offp += len;
    }

    return len;
}


long spw_usb_common_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    void *file = filp->private_data;   /* 2.6.0 - latest (for ->private_data) */

    if (file == NULL)
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "Invalid ptr in 'ioctl' : %ld\n", PTR_ERR(file));
        return STAR_NO_SUCH_FILE_ERROR;
    }

    return star_usb_ioctl(file, cmd, arg);
}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 35))
int spw_usb_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
    return spw_usb_common_ioctl(filp, cmd, arg);
}
#endif


#ifdef HAVE_UNLOCKED_IOCTL
long spw_usb_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return spw_usb_common_ioctl(filp, cmd, arg);
}
#endif


#ifdef HAVE_COMPAT_IOCTL
long spw_usb_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return spw_usb_common_ioctl(filp, cmd, arg);
}
#endif


#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 19))
void spw_usb_callback(struct urb *xfer_urb)                         /* 2.6.19 - latest */
#else
void spw_usb_callback(struct urb *xfer_urb, struct pt_regs *regs)   /* 2.6.0 - 2.6.18 */
#endif
{
    star_usb_callback(xfer_urb->context, xfer_urb->status, xfer_urb->actual_length, xfer_urb);  /* 2.6.0 - latest */
}


int spw_urb_get_status(void *urb)
{
    struct urb *temp = urb;

    return temp->status;
}


void spw_usb_worker(struct work_struct *work_ptr)
{
    star_xfer_info *xfer_info;

    /* get transfer info pointer */
    xfer_info  = container_of(work_ptr, star_xfer_info, work_task);   /* 2.6.0 - latest */
    if ((xfer_info == NULL) || IS_ERR(xfer_info))
    {
        spw_print_msg(STAR_MESSAGE_LEVEL_ERROR, "container_of failed in worker : %ld\n", PTR_ERR(xfer_info));
        return;
    }

    star_usb_worker(xfer_info->xfer_buff);
}
