/*
 * spidev.c -- simple synchronous userspace interface to SPI devices
 *
 * Copyright (C) 2006 SWAPP
 *	Andrea Paterniani <a.paterniani@swapp-eng.it>
 * Copyright (C) 2007 David Brownell (simplification, cleanup)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>

#include <linux/kernel.h>
#include <linux/slab.h>		/* kmalloc() */
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/fcntl.h>
#include <linux/device.h>
#include <linux/mutex.h>
#include <linux/poll.h>

#include <linux/spi/spi.h>
#include <linux/delay.h>

#include <asm/uaccess.h>
#include <asm/atomic.h>

#include "radio.h"
#include "sx1276-LoRaMisc.h"
#include "sx1276-LoRa.h"
#include "rf.h"


/*
 * This supports acccess to SPI devices using normal userspace I/O calls.
 * Note that while traditional UNIX/POSIX I/O semantics are half duplex,
 * and often mask message boundaries, full SPI support requires full duplex
 * transfers.  There are several kinds of internal message boundaries to
 * handle chipselect management and other protocol options.
 */

#define RF_MAJOR	1278
#define RF_MINOR	0

int rf_major = RF_MAJOR;
int rf_minor = RF_MINOR;

module_param(rf_major, int, S_IRUGO);
module_param(rf_minor, int, S_IRUGO);


/* Bit masks for spi_device.mode management.  Note that incorrect
 * settings for some settings can cause *lots* of trouble for other
 * devices on a shared bus:
 *
 *  - CS_HIGH ... this device will be active when it shouldn't be
 *  - 3WIRE ... when active, it won't behave as it should
 *  - NO_CS ... there will be no explicit message boundaries; this
 *	is completely incompatible with the shared bus model
 *  - READY ... transfers may proceed when they shouldn't.
 *
 * REVISIT should changing those flags be privileged?
 */
#define SPI_MODE_MASK		(SPI_CPHA | SPI_CPOL | SPI_CS_HIGH \
				| SPI_LSB_FIRST | SPI_3WIRE | SPI_LOOP \
				| SPI_NO_CS | SPI_READY)

//struct atomic_t available = ATOMIC_INIT(1);

static struct rf_data *rf_device = NULL;

static unsigned bufsiz = 4096;
module_param(bufsiz, uint, S_IRUGO);
MODULE_PARM_DESC(bufsiz, "data bytes in biggest supported SPI message");

/*-------------------------------------------------------------------------*/

void radio_init(struct rf_data *rf)
{
	SX1276Init(rf->spi, true);
}

void radio_deinit(struct rf_data *rf)
{
	SX1276DeInit(rf->spi);
}

int radio_write(struct rf_data *rf, size_t len)
{
	return SX1276WriteBlock(rf, len, 0);
}

int radio_read(struct rf_data *rf, size_t len)
{
	return SX1276ReadBlock(rf, len, 0);
}

/*-------------------------------------------------------------------------*/
static ssize_t
rf_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
	struct rf_data	*rf;
	ssize_t			status = 0;

	/* chipselect only toggles at start or end of operation */
	if (count > bufsiz)
		return -EMSGSIZE;

	rf = filp->private_data;

	mutex_lock(&rf->buf_lock);
	status = radio_read(rf, count);
	if (status > 0) {
		unsigned long	missing;

		missing = copy_to_user(buf, rf->buffer, status);
		if (missing == status)
			status = -EFAULT;
		else
			status = status - missing;
	}
	mutex_unlock(&rf->buf_lock);

	return status;
}

ssize_t
static rf_write(struct file *filp, const char __user *buf,
		size_t count, loff_t *f_pos)
{
	struct rf_data	*rf;
	ssize_t			status = 0;
	unsigned long		missing;
 
	/* chipselect only toggles at start or end of operation */
	if (count > bufsiz) {
		return -EMSGSIZE;
	}

	rf = filp->private_data;

	mutex_lock(&rf->buf_lock);
	missing = copy_from_user(rf->buffer, buf, count);
	if (missing == 0) {
		status = radio_write(rf, count);
	} else
		status = -EFAULT;
	mutex_unlock(&rf->buf_lock);

	return status;

}

#if 1
static long
rf_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int	err = 0;
	int retval = 0;
	int tmp;
	struct rf_data *rf;

	/* Check type and command number */
	if (_IOC_TYPE(cmd) != RF_IOC_MAGIC)
		return -ENOTTY;
	if (_IOC_NR(cmd) > RF_IOC_MAXNR)
		return -ENOTTY;

	/* Check access direction once here; don't repeat below.
	 * IOC_DIR is from the user perspective, while access_ok is
	 * from the kernel perspective; so they look reversed.
	 */
	if (_IOC_DIR(cmd) & _IOC_READ)
		err = !access_ok(VERIFY_WRITE,
				(void __user *)arg, _IOC_SIZE(cmd));
	if (err == 0 && _IOC_DIR(cmd) & _IOC_WRITE)
		err = !access_ok(VERIFY_READ,
				(void __user *)arg, _IOC_SIZE(cmd));
	if (err)
		return -EFAULT;

	/* use the buffer lock here for triple duty:
	 *  - prevent I/O (from us) so calling spi_setup() is safe;
	 *  - prevent concurrent SPI_IOC_WR_* from morphing
	 *    data fields while SPI_IOC_RD_* reads them;
	 *  - SPI_IOC_MESSAGE needs the buffer locked "normally".
	 */
	rf = filp->private_data;
	mutex_lock(&rf->buf_lock);

	switch (cmd) {

	case RF_IOC_RESET:
		SX1276Reset();
		break;

	case RF_IOC_SLEEP:
		SX1276Sleep();
		break;
	/* read requests */
	case RF_IOC_GET_PWR:
		retval = __put_user( (int)SX1276LoRaGetRFPower(),
					(int __user *)arg);
		break;
	case RF_IOC_GET_FREQ:
		retval = __put_user( (int)SX1276LoRaGetRFFrequency(),
					(int __user *)arg);
		break;
	case RF_IOC_GET_SF:
		retval = __put_user( (int)SX1276LoRaGetSpreadingFactor(),
					(int __user *)arg);
		break;
	case RF_IOC_GET_BW:
		retval = __put_user( (int)SX1276LoRaGetSignalBandwidth(),
					(int __user *)arg);
		break;

	/* write requests */
	case RF_IOC_SET_PWR:
		retval = __get_user(tmp, (int __user *)arg);
		if (retval == 0) {
			SX1276LoRaSetRFPower(tmp);
		}
		break;
	case RF_IOC_SET_FREQ:
		retval = __get_user(tmp, (int __user *)arg);
//		printk(KERN_ALERT "ioctl freq = %u\n", tmp);
		if (retval == 0) {
			SX1276LoRaSetRFFrequency(tmp);
		}
		break;
	case RF_IOC_SET_SF:
		retval = __get_user(tmp, (int __user *)arg);
		if (retval == 0) {
			SX1276LoRaSetSpreadingFactor(tmp);
		}
		break;
	case RF_IOC_SET_BW:
		retval = __get_user(tmp, (int __user *)arg);
		if (retval == 0) {
			SX1276LoRaSetSignalBandwidth(tmp);
		}
		break;

	default:
		retval = -ENOTTY;
		break;
	}

	mutex_unlock(&rf->buf_lock);
	return retval;
}
#endif

#if 0
static void rf_spi_setup(struct spi_device *spi)
{
	spi->mode = SPI_MODE_0;
//	spi->mode = SPI_MODE_0 | SPI_LOOP;
	spi->bits_per_word = 8;
	spi->max_speed_hz = 1000000;
	spi_setup(spi);
}
#endif

static int rf_open(struct inode *inode, struct file *filp)
{
	struct rf_data	*rf;

	if (rf_device->devt != inode->i_rdev) {
		pr_debug("spidev: nothing for minor %d\n", iminor(inode));
		printk(KERN_ALERT "spidev: nothing for minor %d\n", iminor(inode));
		return -ENXIO;
	}

	rf = rf_device;

	if (! atomic_dec_and_test(&rf->available)) {
		atomic_inc(&rf->available);
		printk(KERN_ALERT "RF device already opened!\n");
		return -EBUSY;
	}

	mutex_init(&rf->buf_lock);

	if (!rf->buffer) {
		rf->buffer = kmalloc(bufsiz, GFP_KERNEL);
		if (!rf->buffer) {
			dev_dbg(&rf->spi->dev, "open/ENOMEM\n");
			printk(KERN_ALERT "open/ENOMEM\n");
			return -ENOMEM;
		}
	}

#if 0
	struct spi_device *spi;
	spi = spi_dev_get(rf->spi);
	rf_spi_setup(spi);
	spi_dev_put(spi);
#endif

	filp->private_data = rf;
	nonseekable_open(inode, filp);
	radio_init(rf);
	printk(KERN_ALERT "rf_open ok!\n");

	return 0;	/* success */
}

static int rf_release(struct inode *inode, struct file *filp)
{
	struct rf_data  *rf;

	rf = filp->private_data;
	filp->private_data = NULL;

	if (rf->buffer) {
		kfree(rf->buffer);
		rf->buffer = NULL;
	} else {
		printk(KERN_ALERT "rf_release warn: rf->buffer == NULL!\n");
	}

	atomic_inc(&rf->available);

	radio_deinit(rf);

	if (rf->spi == NULL) {
		printk(KERN_ALERT "rf_release warn: rf->spi == NULL!\n");
		kfree(rf);
	}


	printk(KERN_ALERT "rf_released!\n");
	return 0;
}

static unsigned int rf_poll(struct file *filp, poll_table *wait)
{
	uint32_t mask = 0;
	struct rf_data *rf = filp->private_data;
	
	mutex_lock(&rf->buf_lock);
	poll_wait(filp, &rf->inq,  wait);
	poll_wait(filp, &rf->outq, wait);
#if 0
	if (readable) {
		mask |= POLLIN | POLLRDNORM;
	}
	if (writable) {
		mask |= POLLOUT | POLLWRNORM;
	}
#endif
	mutex_lock(&rf->buf_lock);
	return mask;
}

static int rf_fasync(int fd, struct file *filp, int mode)
{
	struct rf_data *rf = filp->private_data;
	return fasync_helper(fd, filp, mode, &rf->async_queue);
}

static const struct file_operations rf_fops = {
	.owner = THIS_MODULE,
	.write = rf_write,
	.read  = rf_read,
	.open  = rf_open,
	.release = rf_release,
	.unlocked_ioctl = rf_ioctl,
	.poll = rf_poll,
	.fasync = rf_fasync,
};

/*-------------------------------------------------------------------------*/

static struct class *rf_class;

static int __devinit rf_probe(struct spi_device *spi)
{
	struct rf_data	*rf;
	int			status;

	struct device *dev;

	/* Allocate driver data */
	rf = kzalloc(sizeof(*rf), GFP_KERNEL);
	if (!rf)
		return -ENOMEM;

	/* Initialize the driver data */
	rf->spi = spi;
	atomic_set(&rf->available, 1);

	rf->devt = MKDEV(rf_major, rf_minor);

	dev = device_create(rf_class, &spi->dev, rf->devt,
			rf, "spidev%d.%d", spi->master->bus_num, spi->chip_select);

	status = IS_ERR(dev) ? PTR_ERR(dev) : 0;

	if (status == 0) {
		rf_device = rf;
		spi_set_drvdata(spi, rf);
		printk(KERN_ALERT "rf_probe OK!\n");
	} else {
		kfree(rf);
		printk(KERN_ALERT "rf_probe error!\n");
	}

	return status;
}

static int __devexit rf_remove(struct spi_device *spi)
{
	struct rf_data	*rf = spi_get_drvdata(spi);

	/* make sure ops on existing fds can abort cleanly */
	rf->spi = NULL;
	spi_set_drvdata(spi, NULL);

	device_destroy(rf_class, rf->devt);
	kfree(rf);
	printk(KERN_ALERT "rf_removed!\n");

	return 0;
}

static struct spi_driver rf_spi_driver = {
	.driver = {
		.name = "rf",
		.owner = THIS_MODULE,
	},
	.probe = rf_probe,
	.remove = __devexit_p(rf_remove),
};

static int __init rf_init(void)
{
	int status;

	status = register_chrdev(RF_MAJOR, "rf-cdev", &rf_fops);
	if (status < 0)
		return status;

	rf_class = class_create(THIS_MODULE, "rf-class");
	if (IS_ERR(rf_class)) {
		unregister_chrdev(RF_MAJOR, rf_spi_driver.driver.name);
		return PTR_ERR(rf_class);
	}

	status = spi_register_driver(&rf_spi_driver);
	if (status < 0) {
		class_destroy(rf_class);
		unregister_chrdev(RF_MAJOR, rf_spi_driver.driver.name);
		printk(KERN_ALERT "rf_init error!\n");
	} else {
		printk(KERN_ALERT "rf_init ok!\n");
	}

	return status;

}

static void __exit rf_exit(void)
{
	spi_unregister_driver(&rf_spi_driver);
	class_destroy(rf_class);
	unregister_chrdev(RF_MAJOR, rf_spi_driver.driver.name);
	printk(KERN_ALERT "rf_exit!\n");

}


module_init(rf_init);
module_exit(rf_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lichen");
