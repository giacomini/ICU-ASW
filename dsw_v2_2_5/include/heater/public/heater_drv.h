/**
 * \file	heater_drv.h
 *
 * \brief	This file defines the public interface of the Heater driver
 *
 * \author	Jesus Fernandez Conde, <jesus.fc@aut.uah.es>
 *
 * \internal
 * Created: 	28-february-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2016, Jesus Fernandez Conde
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef HEATERS_H
#define HEATERS_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>


/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_HEATERS Heaters
 *  @{
 */

/**
 *  @defgroup DSW_HEATERS_ERROR_CODES Error Codes
 *  @{
 * \brief	Error codes returned by the Heater driver functions
 */


/* Same as SPI driver */
#define ICU_HEATER_SPI_ERROR_DEV_ID_ERR	 			(1UL<<1)
#define ICU_HEATER_SPI_ERROR_RD_PARITY_ERR	 		(1UL<<2)
#define ICU_HEATER_SPI_ERROR_TX_OVERFLOW	 		(1UL<<5)
#define ICU_HEATER_SPI_ERROR_APB_ERROR		 		(1UL<<6)
#define ICU_HEATER_SPI_ERROR_INVALID_INPUT_PARAM  	(1UL<<8)
#define ICU_HEATER_SPI_ERROR_TIME_OUT 		 		(1UL<<9)

#define ICU_HEATER_ERROR_INVALID_INPUT_PARAM		(1UL<<16)


/// @cond ALTERNATIVE_ERROR_CODES
/* Same as SPI driver */
#define HEATER_SPI_ERROR_DEV_ID_ERR	 			ICU_HEATER_SPI_ERROR_DEV_ID_ERR
#define HEATER_SPI_ERROR_RD_PARITY_ERR	 		ICU_HEATER_SPI_ERROR_RD_PARITY_ERR
#define HEATER_SPI_ERROR_TX_OVERFLOW	 		ICU_HEATER_SPI_ERROR_TX_OVERFLOW
#define HEATER_SPI_ERROR_APB_ERROR		 		ICU_HEATER_SPI_ERROR_APB_ERROR
#define HEATER_SPI_ERROR_INVALID_INPUT_PARAM  	ICU_HEATER_SPI_ERROR_INVALID_INPUT_PARAM
#define HEATER_SPI_ERROR_TIME_OUT 		 		ICU_HEATER_SPI_ERROR_TIME_OUT

#define HEATER_ERROR_INVALID_INPUT_PARAM		ICU_HEATER_ERROR_INVALID_INPUT_PARAM
/// @endcond

/**
 * @}
 */


/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 *  @defgroup DSW_HEATERS_ERROR_TYPES Types
 *  @{
 */

/**
 * \enum	icu_heater_id_t
 *
 * \brief	This enumerate defines the values to be used as first parameter in
 * 			all the functions of the driver that require an identifier of the
 * 			heater
 */
typedef enum
{
	HEATER_FPA = 0,
	HEATER_SA
}
icu_heater_id_t;
/**
 * @}
 */

/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/


/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_HEATERS_API Public Functions
 *  @{
 *  \brief	Public functions provided by the Heater Drivers
 */


/**
 * \brief	This function initializes the Heater driver; It sends SPI commands
 * 			to HEATER_CONFx with the constant values declared in HW/SW ICD
 * 			(0x018F8800 and 0x018F8801 respectively).
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_HEATER_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_HEATER_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_HEATER_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_HEATER_SPI_ERROR_APB_ERROR
 * 				- ICU_HEATER_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_HEATER_SPI_ERROR_TIME_OUT
 */
uint32_t icu_heater_initialize(void);



/**
 * \brief	This function allows switching on a selected heater by configuring
 *          the PWM duty cycle through an SPI Command
 *
 * \param[in]	identifier				Identifier of the heater (see Enum type
 * 					icu_heater_id_t)
 * \param[in]	duty_cycle_in_ticks		Duty cycle expressed as an integer
 * 					number in the range [0..1024]. Value=0 means Duty Cycle=0%,
 * 					value=1024 means Duty Cycle=100%
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_HEATER_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_HEATER_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_HEATER_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_HEATER_SPI_ERROR_APB_ERROR
 * 				- ICU_HEATER_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_HEATER_SPI_ERROR_TIME_OUT
 * 				- ICU_HEATER_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_heater_switch_on( const icu_heater_id_t identifier,
								const uint32_t duty_cycle_in_ticks
								);

/**
 * \brief	This function allows switching off a selected heater by configuring
 * 			a PWM duty cycle of 0% through an SPI command
 *
 * \param[in]	identifier				Identifier of the heater (see Enum type
 * 				icu_heater_id_t)
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_HEATER_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_HEATER_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_HEATER_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_HEATER_SPI_ERROR_APB_ERROR
 * 				- ICU_HEATER_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_HEATER_SPI_ERROR_TIME_OUT
 * 				- ICU_HEATER_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_heater_switch_off(const icu_heater_id_t identifier);


/**
 * \brief	This function resets the Heater driver functionality
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_HEATER_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_HEATER_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_HEATER_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_HEATER_SPI_ERROR_APB_ERROR
 * 				- ICU_HEATER_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_HEATER_SPI_ERROR_TIME_OUT
 */
uint32_t icu_heater_reset(void);

/**
 * @}
 */


/**
 * @}
 */


#endif



