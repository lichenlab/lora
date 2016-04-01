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
