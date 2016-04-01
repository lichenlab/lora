/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND 
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, SEMTECH SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 * 
 * Copyright (C) SEMTECH S.A.
 */
/*! 
 * \file       sx1276-Hal.c
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       Nov 21 2012
 * \author     Miguel Luis
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/delay.h>

#include "platform.h"

#if defined( USE_SX1276_RADIO )

#include "sx1276-Hal.h"

/*!
 * SX1276 RESET I/O definitions
 */

/*!
 * SX1276 SPI NSS I/O definitions
 */

/*!
 * SX1276 DIO pins  I/O definitions
 */


struct gpio_info {
	uint32_t pin;
	char name[16];
};

static struct gpio_info sx1276_gpio[] = {
	{RST_GPIO, "sx1276-RST"},
	{DIO0_GPIO, "sx1276-DIO0"},
	{DIO1_GPIO, "sx1276-DIO1"},
};

static struct spi_device *spidev = NULL;



void SX1276InitSpi(struct spi_device *spi)
{
	spidev = spi;
}
#if 0
static irqreturn_t trigger(int irq, void *id)
{
	struct rf_data *rf = (struct rf_data *) id; 

//	if (rf->done != NULL) 
		complete(&rf->done);
	printk( "irq interrupt!\n");
	return IRQ_RETVAL(IRQ_HANDLED);
}
static int init_trigger(void *arg)
{
	int irq;

	gpio_free(DIO0_GPIO);
	if ( gpio_request(DIO0_GPIO, "DIO0_GPIO") ) {
		printk(KERN_ALERT  "gpio_request() error!\n");
		return -EBUSY;
	}
	gpio_direction_input(DIO0_GPIO);
	irq = gpio_to_irq(DIO0_GPIO);
	set_irq_type(irq, IRQF_TRIGGER_RISING);

	/* FIRQ enable */
	if ( request_irq(irq, trigger, IRQF_DISABLED, "DIO0-IRQ", arg) ) {
		gpio_free(DIO0_GPIO);
		printk(KERN_ALERT "request_irq() error!\n");
		return -EBUSY;
	}

	return 0;
}
static void deinit_trigger(void)
{
	free_irq( gpio_to_irq(DIO0_GPIO), NULL );
	gpio_free(DIO0_GPIO);
}
#endif

int SX1276InitIo( void )
{
    // Configure radio DIO as inputs
    // Configure DIO0
    // Configure DIO1
    // Configure DIO2
    // REAMARK: DIO3/4/5 configured are connected to IO expander
    // Configure DIO3 as input
    // Configure DIO4 as input
    // Configure DIO5 as input

	int i;

	for (i = 0; i < ARRAY_SIZE(sx1276_gpio); i++) {
		gpio_free( sx1276_gpio[i].pin );
		if ( gpio_request(sx1276_gpio[i].pin, sx1276_gpio[i].name) ) {
			printk(KERN_ALERT "gpio_request(): %s error!\n", sx1276_gpio[i].name);
			return -EBUSY;
		}
		gpio_direction_input(sx1276_gpio[i].pin);
	}

	return 0;

}

void SX1276DeInitIo(void)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(sx1276_gpio); i++)
		gpio_free( sx1276_gpio[i].pin );
}


void SX1276SetReset( uint8_t state )
{
    if( state == RADIO_RESET_ON )
    {
        // Set RESET pin to 0
	gpio_direction_output(RST_GPIO, 0);
    }
    else
    {
        // Configure RESET as input
//	gpio_direction_input(RST_GPIO);
	gpio_direction_output(RST_GPIO, 1);
   }
}

void SX1276Write( uint8_t addr, uint8_t data )
{
    SX1276WriteBuffer( addr, &data, 1 );
}

void SX1276Read( uint8_t addr, uint8_t *data )
{
    SX1276ReadBuffer( addr, data, 1 );
}

int SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	int res;

	uint8_t *buf = kzalloc(512, GFP_KERNEL);
	if (!buf)
		return -ENOMEM;
	buf[0] = addr | 0x80;
	memcpy(&buf[1], buffer, size);
	res = spi_write(spidev, buf, size+1);
	kfree(buf);
	return res;
}

/* lichen: spi_write_then_read() only support 32 Byte in the linux 
   source code, see /driver/spi/spi.c, the SPI_BUFSIZ micro,
   so we change the SPI_BUFSIZ to 0x100 (256) Bytes to support our
   use.
*/
int SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
	addr &= 0x7f;
	return spi_write_then_read(spidev, &addr, 1, buffer, size);
}

void SX1276WriteFifo( uint8_t *buffer, uint8_t size )
{
    SX1276WriteBuffer( 0, buffer, size );
}

void SX1276ReadFifo( uint8_t *buffer, uint8_t size )
{
    SX1276ReadBuffer( 0, buffer, size );
}

#if 0
inline uint8_t SX1276ReadDio0( void )
{

}

inline uint8_t SX1276ReadDio1( void )
{

}

inline uint8_t SX1276ReadDio2( void )
{

}

inline uint8_t SX1276ReadDio3( void )
{

}

inline uint8_t SX1276ReadDio4( void )
{

}

inline uint8_t SX1276ReadDio5( void )
{

}

inline void SX1276WriteRxTx( uint8_t txEnable )
{
    if( txEnable != 0 )
    {

    }
    else
    {

    }
}
#endif

#endif // USE_SX1276_RADIO
