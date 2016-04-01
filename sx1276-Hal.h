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
 * \file       sx1276-Hal.h
 * \brief      SX1276 Hardware Abstraction Layer
 *
 * \version    2.0.B2 
 * \date       May 6 2013
 * \author     Gregory Cristian
 *
 * Last modified by Miguel Luis on Jun 19 2013
 */
#ifndef __SX1276_HAL_H__
#define __SX1276_HAL_H__

#include <linux/spi/spi.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include "../mach/mx28_pins.h"

#define RST_GPIO	MXS_PIN_TO_GPIO(PINID_AUART0_RX)
#define DIO0_GPIO	MXS_PIN_TO_GPIO(PINID_AUART0_TX)
#define DIO1_GPIO	MXS_PIN_TO_GPIO(PINID_AUART1_RX)
#define DIO2_GPIO	MXS_PIN_TO_GPIO(PINID_AUART1_TX)

#define SPI_CLK		MXS_PIN_TO_GPIO(PINID_AUART2_RX)
#define SPI_MOSI	MXS_PIN_TO_GPIO(PINID_AUART2_RX)
#define SPI_MISO	MXS_PIN_TO_GPIO(PINID_AUART3_TX)
#define SPI_NSS		MXS_PIN_TO_GPIO(PINID_AUART3_TX)


/*!
 * DIO state read functions mapping
 */
#define DIO0                                        SX1276ReadDio0( )
#define DIO1                                        SX1276ReadDio1( )
#define DIO2                                        SX1276ReadDio2( )
#define DIO3                                        SX1276ReadDio3( )
#define DIO4                                        SX1276ReadDio4( )
#define DIO5                                        SX1276ReadDio5( )

// RXTX pin control see errata note
#define RXTX( txEnable )                            SX1276WriteRxTx( txEnable );

#define GET_TICK_COUNT( )                           ( jiffies )
#define TICK_RATE_MS( ms )                          ( ms )

typedef enum
{
    RADIO_RESET_OFF,
    RADIO_RESET_ON,
}tRadioResetState;

/*!
 * \brief Initializes the radio interface I/Os
 */
int SX1276InitIo( void );
void SX1276DeInitIo(void);

void SX1276InitSpi(struct spi_device *spi);
/*!
 * \brief Set the radio reset pin state
 * 
 * \param state New reset pin state
 */
void SX1276SetReset( uint8_t state );

/*!
 * \brief Writes the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [IN]: data New register value
 */
void SX1276Write( uint8_t addr, uint8_t data );

/*!
 * \brief Reads the radio register at the specified address
 *
 * \param [IN]: addr Register address
 * \param [OUT]: data Register value
 */
void SX1276Read( uint8_t addr, uint8_t *data );

/*!
 * \brief Writes multiple radio registers starting at address
 *
 * \param [IN] addr   First Radio register address
 * \param [IN] buffer Buffer containing the new register's values
 * \param [IN] size   Number of registers to be written
 */
int SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

/*!
 * \brief Reads multiple radio registers starting at address
 *
 * \param [IN] addr First Radio register address
 * \param [OUT] buffer Buffer where to copy the registers data
 * \param [IN] size Number of registers to be read
 */
int SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size );

/*!
 * \brief Writes the buffer contents to the radio FIFO
 *
 * \param [IN] buffer Buffer containing data to be put on the FIFO.
 * \param [IN] size Number of bytes to be written to the FIFO
 */
void SX1276WriteFifo( uint8_t *buffer, uint8_t size );

/*!
 * \brief Reads the contents of the radio FIFO
 *
 * \param [OUT] buffer Buffer where to copy the FIFO read data.
 * \param [IN] size Number of bytes to be read from the FIFO
 */
void SX1276ReadFifo( uint8_t *buffer, uint8_t size );

/*!
 * \brief Gets the SX1276 DIO0 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
static inline uint8_t SX1276ReadDio0( void )
{
	return gpio_get_value(DIO0_GPIO) ? 1 : 0;
}

/*!
 * \brief Gets the SX1276 DIO1 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
static inline uint8_t SX1276ReadDio1( void )
{

	return gpio_get_value(DIO1_GPIO) ? 1 : 0;
}

/*!
 * \brief Gets the SX1276 DIO3 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
static inline uint8_t SX1276ReadDio2( void )
{
	return 0;
}

/*!
 * \brief Gets the SX1276 DIO4 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
static inline uint8_t SX1276ReadDio3( void )
{

	return 0;
}

/*!
 * \brief Gets the SX1276 DIO2 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
static inline uint8_t SX1276ReadDio4( void )
{

	return 0;
}

/*!
 * \brief Gets the SX1276 DIO5 hardware pin status
 *
 * \retval status Current hardware pin status [1, 0]
 */
static inline uint8_t SX1276ReadDio5( void )
{

	return 0;
}

/*!
 * \brief Writes the external RxTx pin value
 *
 * \remark see errata note
 *
 * \param [IN] txEnable [1: Tx, 0: Rx]
 */
static inline void SX1276WriteRxTx( uint8_t txEnable )
{

}

#endif //__SX1276_HAL_H__
