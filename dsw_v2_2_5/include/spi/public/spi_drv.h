/**
 * \file	spi_drv.h
 *
 * \brief	This file defines the public interface of the LED driver
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	13-january-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef ICU_SPI_DRV_H
#define ICU_SPI_DRV_H



/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/**
 *  @defgroup DSW_SPI SPI
 *  @{
 */


/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/
/**
 *  @defgroup DSW_SPI_CONST Constants
 *  @{
 *  \brief	Error codes returned by the SPI Driver functions
 */


#define PWMS_CLK_CONF4_ADDR		0x44U
#define PWMS_CLK_CONF4_DATA		0x8003U

/**
 * @}
 */

/**
 *  @defgroup DSW_SPI_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the SPI Driver functions
 */

#define ICU_SPI_ERROR_DEV_ID_ERR	 		(1UL<<1)
#define ICU_SPI_ERROR_RD_PARITY_ERR	 		(1UL<<2)
#define ICU_SPI_ERROR_RX_OVERFLOW	 		(1UL<<3)
#define ICU_SPI_ERROR_RX_UNDERFLOW	 		(1UL<<4)
#define ICU_SPI_ERROR_TX_OVERFLOW	 		(1UL<<5)
#define ICU_SPI_ERROR_APB_ERROR		 		(1UL<<6)
#define ICU_SPI_ERROR_INVALID_INPUT_PARAM  	(1UL<<8)
#define ICU_SPI_ERROR_TIME_OUT 		 		(1UL<<9)
#define ICU_SPI_ERROR_NO_DATA_AVAILABLE	 	(1UL<<10)


/// @cond ALTERNATIVE_ERROR_CODES
#define SPI_ERROR_DEV_ID_ERR	 		ICU_SPI_ERROR_DEV_ID_ERR
#define SPI_ERROR_RD_PARITY_ERR	 		ICU_SPI_ERROR_RD_PARITY_ERR
#define SPI_ERROR_RX_OVERFLOW	 		ICU_SPI_ERROR_RX_OVERFLOW
#define SPI_ERROR_RX_UNDERFLOW	 		ICU_SPI_ERROR_RX_UNDERFLOW
#define SPI_ERROR_TX_OVERFLOW	 		ICU_SPI_ERROR_TX_OVERFLOW
#define SPI_ERROR_APB_ERROR		 		ICU_SPI_ERROR_APB_ERROR
#define SPI_ERROR_INVALID_INPUT_PARAM  	ICU_SPI_ERROR_INVALID_INPUT_PARAM
#define SPI_ERROR_TIME_OUT 		 		ICU_SPI_ERROR_TIME_OUT
#define SPI_ERROR_NO_DATA_AVAILABLE	 	ICU_SPI_ERROR_NO_DATA_AVAILABLE
/// @endcond

/**
 * @}
 */


/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/


/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_SPI_API Public Functions
 *  @{
 *  \brief	Public functions provided by the SPI Driver
 */


/**
 * \brief	This function initializes SPI driver, including sending SPI commands
 * 			for PWM clocks configuration
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- 	ICU_SPI_ERROR_DEV_ID_ERR
 *				- 	ICU_SPI_ERROR_RD_PARITY_ERR
 *				- 	ICU_SPI_ERROR_RX_OVERFLOW
 *				- 	ICU_SPI_ERROR_RX_UNDERFLOW
 *				- 	ICU_SPI_ERROR_TX_OVERFLOW
 *				- 	ICU_SPI_ERROR_APB_ERR
 *				- 	ICU_SPI_ERROR_TIME_OUT
 *
 */
uint32_t icu_spi_ctrl_initialize( void );

/**
 * \brief	This function writes an SPI command, with concurrent access control.
 *
 * \param[in] address	address to write
 * \param[in] data		data to write
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- 	ICU_SPI_ERROR_INVALID_INPUT_PARAM
 *				- 	ICU_SPI_ERROR_DEV_ID_ERR
 *				- 	ICU_SPI_ERROR_RD_PARITY_ERR
 *				- 	ICU_SPI_ERROR_RX_OVERFLOW
 *				- 	ICU_SPI_ERROR_RX_UNDERFLOW
 *				- 	ICU_SPI_ERROR_TX_OVERFLOW
 *				- 	ICU_SPI_ERROR_APB_ERR
 *				- 	ICU_SPI_ERROR_TIME_OUT
 */
uint32_t icu_spi_ctrl_write(const uint8_t address, const  uint16_t data);


/**
 * \brief	This function reads the results of an SPI command, with concurrent
 * 			access control
 *
 * \param[in] address	address to read
 * \param[out] data	User variable where the data to read will be stored
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- 	ICU_SPI_ERROR_INVALID_INPUT_PARAM
 *				- 	ICU_SPI_ERROR_DEV_ID_ERR
 *				- 	ICU_SPI_ERROR_RD_PARITY_ERR
 *				- 	ICU_SPI_ERROR_RX_OVERFLOW
 *				- 	ICU_SPI_ERROR_RX_UNDERFLOW
 *				- 	ICU_SPI_ERROR_TX_OVERFLOW
 *				- 	ICU_SPI_ERROR_APB_ERR
 *				- 	ICU_SPI_ERROR_TIME_OUT
 *				-	SPI_ERROR_NO_DATA_AVAILABLE
 */
uint32_t icu_spi_ctrl_read(const uint8_t address, uint16_t * data);

/**
 * @}
 */

/**
 * @}
 */


#endif // ICU_SPI_DRV_H
