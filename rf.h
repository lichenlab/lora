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


/*
 * This supports acccess to SPI devices using normal userspace I/O calls.
 * Note that while traditional UNIX/POSIX I/O semantics are half duplex,
 * and often mask message boundaries, full SPI support requires full duplex
 * transfers.  There are several kinds of internal message boundaries to
 * handle chipselect management and other protocol options.
 */
#ifndef _RF_H_ 
#define _RF_H_

#include <linux/ioctl.h>


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

struct rf_data {

	/* buffer is NULL unless this device is open (users > 0) */
	u8 *buffer;

	dev_t				devt;
	atomic_t			available;
	wait_queue_head_t 	inq, outq;

	struct mutex		buf_lock;
	struct spi_device	*spi;
	struct fasync_struct *async_queue;
	struct completion done;
};


/*
 * Ioctl definitions
 */

/* Use 'k' as magic number */
#define RF_IOC_MAGIC  'k'
/* Please use a different 8-bit number in your code */

/*
 * S means "Set" through a ptr,
 * G means "Get": reply by setting through a pointer
 */
 
#define RF_IOC_RESET    _IO(RF_IOC_MAGIC,  0)
#define RF_IOC_SLEEP    _IO(RF_IOC_MAGIC,  1)

#define RF_IOC_SET_PWR	_IOW(RF_IOC_MAGIC, 2, int)
#define RF_IOC_SET_FREQ	_IOW(RF_IOC_MAGIC, 3, int)
#define RF_IOC_SET_SF	_IOW(RF_IOC_MAGIC, 4, int)
#define RF_IOC_SET_BW	_IOW(RF_IOC_MAGIC, 5, int)

#define RF_IOC_GET_PWR	_IOR(RF_IOC_MAGIC, 6, int)
#define RF_IOC_GET_FREQ	_IOR(RF_IOC_MAGIC, 7, int)
#define RF_IOC_GET_SF	_IOR(RF_IOC_MAGIC, 8, int)
#define RF_IOC_GET_BW	_IOR(RF_IOC_MAGIC, 9, int)

#define RF_IOC_MAXNR 9

#endif /* _RF_H_ */
