/**
 * \file pcimk2_kernel.h
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
 * This file contains declarations for the kernel interface for
 * the STAR-Dundee SpaceWire PCI Mk2 Linux Driver.
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

#ifndef PCIMK2_KERNEL_H_
#define PCIMK2_KERNEL_H_

/**
 * Driver details
 * ==============
 */
#define STAR_SPW_PCI_VERSION_NAME   "SpaceWire PCI, cPCI, PCIe and PXI Driver for Linux"
#define STAR_SPW_PCI_VERSION_AUTHOR "David Paterson, STAR-Dundee Ltd."
#define STAR_DUNDEE_PCI_VENDOR_ID (0x1b5e)
#define STAR_PCI_MK2_DEVICE_ID (0x0001)
#define STAR_PCIE_DEVICE_ID (0x0002)
#define STAR_CPCI_MK2_DEVICE_ID (0x0003)
#define STAR_RECORDER_DEVICE_ID (0x0004)
#define STAR_PXI_INTERFACE_DEVICE_ID (0x0005)
#define STAR_PXI_RMAP_DEVICE_ID (0x0006)
#define STAR_PXI_ROUTER_8_DEVICE_ID (0x0007)
#define STAR_PXI_ROUTER_12_DEVICE_ID (0x0008)
#define STAR_PCI_MK3_DEVICE_ID (0x000a)
#define STAR_PXI_INTERFACE_MK2_DEVICE_ID (0x000b)
#define STAR_PXI_RMAP_MK2_DEVICE_ID (0x000c)
#define STAR_PXI_ROUTER_MK2_DEVICE_ID (0x000d)
#define STAR_PCI_MODULE_NAME "star_spw_pci"


/**
 * Message print control levels
 * ============================
 */
#define STAR_MESSAGE_LEVEL_ERROR (1)
#define STAR_MESSAGE_LEVEL_WARNING (2)
#define STAR_MESSAGE_LEVEL_DEBUG (3)


/**
 * DMA direction enum
 * ==================
 */
typedef enum
{
    STAR_DMA_FROM_SPW = 0,
    STAR_DMA_TO_SPW,
    STAR_DMA_TO_FROM_SPW

} star_dma_direction;


/**
 * Error return codes
 * ==================
 */
extern const int STAR_NO_SUCH_FILE_ERROR;
extern const int STAR_NO_MEMORY_ERROR;
extern const int STAR_BAD_ADDRESS_ERROR;
extern const int STAR_NO_SUCH_DEVICE_ERROR;
extern const int STAR_INVALID_ARG_ERROR;
extern const int STAR_NOT_A_TTY_ERROR;


/**
 * Additional device information structure
 */
typedef struct
{
    char *device_name;      /* device name string */
    int device_type;        /* device type value */

    int num_spw_channels;   /* number of channels on this device */

} star_device_info;


/**
 * Additional device information table
 */
extern const star_device_info star_device_info_table[];


/**
 * Kernel interface function prototypes
 * ====================================
 */
extern int spw_alloc_chrdev_region(void *dev, unsigned int baseminor, unsigned int count, const char *name);
extern void spw_unregister_chrdev_region(unsigned int from, unsigned int count);
extern void spw_cdev_init(void *cdev);
extern int spw_cdev_add(void *p, unsigned int dev, unsigned int count);
extern void spw_cdev_del(void *ptr);
extern void *spw_class_create(void);
extern void spw_class_destroy(void *cls);
extern void *spw_device_create(void *class, void *parent, unsigned int devt, void *device, char *devname);
extern void spw_device_destroy(void *class, unsigned int devt, char *devname);
extern unsigned int spw_mkdev(int major, int minor);
extern int spw_get_major(unsigned int dev);
extern int spw_get_minor(unsigned int dev);

extern int spw_pci_register_driver(void);
extern void spw_pci_unregister_driver(void);
extern int spw_pci_enable_device(void *dev);
extern void spw_pci_disable_device(void *dev);
extern int spw_pci_request_region(void *pdev, int bar, const char *res_name);
extern void spw_pci_release_regions(void *pdev);
extern unsigned long spw_pci_resource_start(void *pdev, int bar);
extern unsigned long spw_pci_resource_len(void *pdev, int bar);
extern unsigned long spw_pci_resource_flags(void *pdev, int bar);
extern void spw_pci_set_master(void *dev);
extern void spw_pci_set_drvdata(void *pdev, void *data);
extern void *spw_pci_get_drvdata(void *pdev);

extern int spw_request_irq(void *pdev, const char *name, void *dev);
extern void spw_free_irq(void *pdev, void *dev_id);

extern void *spw_kern_alloc(unsigned int size);
extern void *spw_dma_alloc(unsigned int size);
extern void spw_kfree(const void *p);
extern void spw_memcpy(void *to, const void *from, unsigned int size);
extern void spw_memcpy_toio(void *to, const void *from, unsigned int size);
extern void spw_memcpy_fromio(void *to, const void *from, unsigned int size);
extern void spw_zero_mem(void *p, unsigned int size);
extern void *spw_ptr_align(void *p, int align);

extern int spw_down(void *sem);
extern void spw_up(void *sem);
extern void spw_sema_init(void *sem, int val);

extern void spw_spin_lock_init(void *lock);
extern void spw_spin_lock_irqsave(void *ptr, unsigned long *flags);
extern void spw_spin_unlock_irqrestore(void *ptr, unsigned long flags);

extern void spw_msleep(unsigned int msecs);

extern void spw_init_waitqueue_head(void *q);
extern int spw_wait_for_event(void *q, unsigned int *val);
extern void spw_wake_up_interruptible(void *q);

extern void *spw_create_workqueue(const char *name);
extern void spw_init_work(void *work, void *func);
extern void spw_queue_work(void *ptr);
extern void spw_flush_workqueue(void *ptr);
extern void spw_destroy_workqueue(void *ptr);

extern int spw_dma_set_mask(void *dev);
extern int spw_dma_set_coherent_mask(void *dev);
extern unsigned long spw_dma_map_single(void *dev, void *cpu_addr, unsigned int size, star_dma_direction direction);
extern void spw_dma_unmap_single(void *dev, unsigned long dma_addr, unsigned int size, star_dma_direction direction);
extern void *spw_dma_alloc_coherent(void *dev, unsigned int size, volatile unsigned long *dma_handle);
extern void spw_dma_free_coherent(void *dev, unsigned int size, void *cpu_addr, unsigned long dma_handle);
extern void *spw_pci_iomap(void *dev, int bar, unsigned long size);
#if 0
extern void *spw_ioremap_nocache(unsigned long offset, unsigned long size);
#endif

extern long spw_copy_to_user(void *to, const void *from, unsigned long n);
extern long spw_copy_from_user(void *to, const void *from, unsigned long n);

extern void spw_set_bit(int nr, volatile unsigned long *addr);
extern void spw_clear_bit(int nr, volatile unsigned long *addr);

extern void spw_mb(void);
extern void spw_rmb(void);
extern void spw_wmb(void);

#if 0
extern unsigned char spw_readb(const volatile void *addr);
extern unsigned short spw_readw(const volatile void *addr);
extern unsigned int spw_readl(const volatile void *addr);
extern void spw_writeb(unsigned char v, volatile void *addr);
extern void spw_writew(unsigned short v, volatile void *addr);
extern void spw_writel(unsigned int v, volatile void *addr);
#endif

extern unsigned char spw_ioread8(void *addr);
extern unsigned short spw_ioread16(void *addr);
extern unsigned int spw_ioread32(void *addr);
extern void spw_iowrite8(unsigned char v, void *addr);
extern void spw_iowrite16(unsigned short v, void *addr);
extern void spw_iowrite32(unsigned int v, void *addr);

extern int spw_pci_read_config_byte(void *dev, int where, unsigned char *val);
extern int spw_pci_read_config_word(void *dev, int where, unsigned short *val);
extern int spw_pci_read_config_dword(void *dev, int where, unsigned int *val);
extern int spw_pci_write_config_byte(void *dev, int where, unsigned char val);
extern int spw_pci_write_config_word(void *dev, int where, unsigned short val);
extern int spw_pci_write_config_dword(void *dev, int where, unsigned int val);

extern void spw_print_msg(int level, char *fmt, ...);
extern void spw_snprintf(char *buf, int size, const char *fmt, ...);
extern int spw_strlen(char *str);
extern void spw_strncpy(char *to, const char *from, int len);


/**
 * Additional support function prototypes
 * ======================================
 */
extern void *spw_init_kernel_structs(char *name, void *pPriv);
extern void spw_release_kernel_structs(void *ptr);
extern int spw_check_vendor_device(void *pdev);
extern int spw_get_device_id(void *pdev);
extern void *spw_get_dev(void *pdev);
extern void *spw_get_cdev_addr(void *ptr);
extern void *spw_create_mutex(void);
extern void *spw_create_wait_queue(void);


/**
 * Driver internal function prototypes
 * ===================================
 */
extern int star_pci_init(void);
extern void star_pci_exit(void);

extern int star_pci_probe(void *pDev);
extern void star_pci_remove(void *pDev);
extern int star_pci_suspend(void *pDev, unsigned int state);
extern int star_pci_resume(void *pDev);

extern void *star_pci_open(void *pPriv);
extern int star_pci_release(void *pFile);
extern int star_pci_read(void *pFile, char *buff, unsigned int count);
extern int star_pci_write(void *pFile, const char *buff, unsigned int count);

extern int star_pci_ioctl(void *pFile, unsigned int cmd, unsigned long arg);

extern int star_pci_interrupt_handler(int irq, void *pDevId);
extern void star_pci_worker(void *pPrivPtr);

#endif /* PCIMK2_KERNEL_H_ */
