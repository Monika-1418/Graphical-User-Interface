/**
 * \file spwusb_kernel.h
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
 * This file contains declarations for the kernel interface for
 * the STAR-Dundee USB Linux Driver.
 *
 * Copyright &copy; 2012 STAR-Dundee Ltd.
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

#ifndef SPWUSB_KERNEL_H_
#define SPWUSB_KERNEL_H_

/**
 * Driver details
 */
#define STAR_SPW_USB_VERSION_NAME   "SpaceWire USB Driver for Linux"
#define STAR_SPW_USB_VERSION_AUTHOR "David Paterson, STAR-Dundee Ltd."
#define STAR_USB_MODULE_NAME "star_spw_usb"
#define STAR_USB_CLASS_NAME "star_spw_usb%02d"
#define STAR_USB_MINOR_BASE 0

/* Device vendor ID */
#define SPW_USB_STAR_DUNDEE_VENDOR_ID (0x1825)

/* Device product IDs */
#define SPW_USB_ROUTER_MK2S       (0x1101)
#define SPW_USB_BRICK_MK2         (0x1102)
#define SPW_USB_LINK_ANALYSER_MK2 (0x1103)
#define SPW_USB_RTC               (0x1104)
#define SPW_USB_EGSE              (0x1105)
#define SPW_USB_SPLT              (0x1106)
#define SPW_USB_STAR_FIRE         (0x1107)
#define SPW_USB_WBS_II            (0x1108)
#define SPW_USB_BRICK_MK3         (0x1109)
#define SPW_USB_CONF_TESTER       (0x110a)
#define SPW_USB_STAR_FIRE_MK3     (0x110b)
#define SPW_USB_LINK_ANALYSER_MK3 (0x110c)
#define SPW_USB_CONF_TESTER_MK2   (0x110d)
#define SPW_USB_EGSE_MK2          (0x110e)

/* Link speed event units that define characteristics */
#define SPW_USB_LINK_EVENT_100000 (1)
#define SPW_USB_LINK_EVENT_200000 (2)

/* Amount of time to wait after device reset in milliseconds */
#define SPW_USB_POST_RESET_DELAY_0_MS (0)
#define SPW_USB_POST_RESET_DELAY_2_MS (2)


/**
 * Message print control levels
 */
#define STAR_MESSAGE_LEVEL_ERROR (1)
#define STAR_MESSAGE_LEVEL_WARNING (2)
#define STAR_MESSAGE_LEVEL_DEBUG (3)


/**
 * Error return codes
 */
extern const int STAR_NO_SUCH_FILE_ERROR;
extern const int STAR_NO_MEMORY_ERROR;
extern const int STAR_BAD_ADDRESS_ERROR;
extern const int STAR_NO_SUCH_DEVICE_ERROR;
extern const int STAR_INVALID_ARG_ERROR;
extern const int STAR_NOT_A_TTY_ERROR;
extern const int STAR_ENDPOINT_SHUTDOWN;
extern const int STAR_INVALID_OPERATION;

/**
 * Additional device information structure
 */
typedef struct
{
    char *device_name;          /* device name string */
    int device_type;            /* device type value */

    int num_spw_channels;       /* number of channels on this device */
    int num_unused_channels;    /* number of additional channels on the device */

    int txrx_device;            /* device can transmit and / or receive SpW packets */

    int config_device;          /* device can be configured */

    int link_event_units;       /* units to be used in link speed event calculation */

    int post_reset_delay_ms;    /* amount of time in ms to wait after reset */

} star_device_info;


/**
 * Additional device information table
 */
extern const star_device_info star_device_info_table[];


/**
 * Kernel usb_intfc function prototypes
 */
extern int spw_usb_register_driver(void);
extern void spw_usb_deregister_driver(void);
extern int spw_usb_register_device(void *kern_intfc);
extern void spw_usb_deregister_device(void *kern_intfc);

extern void spw_usb_set_intfdata(void *usb_intfc, void *data);
extern void *spw_usb_get_intfdata(void *usb_intfc);
extern void spw_usb_enable_autosuspend(void *kern_intfc);
extern void spw_usb_disable_autosuspend(void *kern_intfc);
extern void *spw_usb_alloc_urb(void);
extern void spw_usb_free_urb(void *urb);
extern void spw_fill_bulk_urb(void *usb_dev, void *urb, unsigned int pipe, void *buffer, int length, void *context, int direction, int ZLP);
extern int spw_usb_submit_urb(void *urb);
extern int spw_usb_kill_urb(void *urb);
extern int spw_usb_control_msg(void *kern_intfc, unsigned int pipe, unsigned char request, unsigned short value,
                               unsigned short index, int direction, void *data, unsigned short size);
extern int spw_usb_bulk_msg(void *kern_intfc, unsigned int pipe, int direction, void *data, int size, int *readSz, int timeout);

extern void *spw_kern_alloc(unsigned int size);
extern void spw_kfree(const void *p);
extern void spw_zero_mem(void *p, unsigned int size);

extern void *spw_create_mutex(void);
extern void *spw_create_wait_queue(void);

extern int spw_down(void *sem);
extern void spw_up(void *sem);
extern void spw_sema_init(void *sem, int val);

extern void spw_msleep(unsigned int msecs);

extern void spw_init_waitqueue_head(void *q);
extern int spw_wait_for_event(void *q);
extern void spw_wake_up_interruptible(void *q);

extern void *spw_create_workqueue(const char *name);
extern void spw_init_work(void *work);
extern void spw_queue_work(void *kern_intfc, void *xfer_info);
extern void spw_flush_workqueue(void *kern_intfc);
extern void spw_destroy_workqueue(void *kern_intfc);

extern long spw_copy_to_user(void *to, const void *from, unsigned long n);
extern long spw_copy_from_user(void *to, const void *from, unsigned long n);


/**
 * Additional support function prototypes
 */
extern int spw_check_vendor_device(void *usb_intfc);
extern void *spw_init_kernel_structs(void *priv_data, void *usb_intfc, char *name);
extern void *spw_init_xfer_info(void *buff);
extern void spw_release_kernel_structs(void *kern_intfc);
extern int spw_get_num_usb_endpoints(void *usb_intfc);
extern void spw_get_usb_endpoint_info(void *usb_intfc, int endptno, int *address, int *max_packet, int *direction);

extern void spw_print_msg(int level, char *fmt, ...);
extern int spw_strlen(char *str);
extern void spw_strncpy(char *to, const char *from, int len);

extern int spw_urb_get_status(void *urb);


/**
 * Driver internal function prototypes
 */
extern int star_usb_init(void);
extern void star_usb_exit(void);

extern int star_usb_probe(void *usb_intfc);
extern void star_usb_disconnect(void *usb_intfc);
extern int star_usb_suspend(void *usb_intfc, unsigned int state);
extern int star_usb_resume(void *usb_intfc);

extern void *star_usb_open(void *priv_data, int minor);
extern int star_usb_release(void *file);
extern int star_usb_read(void *file, char *buff, unsigned int count);
extern int star_usb_write(void *file, const char *buff, unsigned int count);

extern int star_usb_ioctl(void *file, unsigned int cmd, unsigned long arg);

extern void star_usb_callback(void *context, int xfer_status, unsigned int actual_length, void *urb);
extern void star_usb_worker(void *ptr);

#endif /* SPWUSB_KERNEL_H_ */
