
/**
 * \file	obt.h
 *
 * \brief	This file defines the public interface of the OBT driver
 *
 * \author	Jesus Fernandez Conde, <jesus.fc@aut.uah.es>
 *
 * \internal
 * Created: 	08-january-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Jesus Fernandez Conde
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef OBT_H
#define OBT_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/**
 *  @defgroup DSW_OBT OBT
 *  @{
 */

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_OBT_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the OBT driver functions
 */

#define ICU_OBT_ERROR_SELFTEST_FAILED		(1UL)
#define ICU_OBT_ERROR_INVALID_INPUT_PARAM	(1UL<<1)


/// @cond ALTERNATIVE_ERROR_CODES
#define OBT_ERROR_SELFTEST_FAILED		ICU_OBT_ERROR_SELFTEST_FAILED
#define OBT_ERROR_INVALID_INPUT_PARAM	ICU_OBT_ERROR_INVALID_INPUT_PARAM
/// @endcond

/**
 * @}
 */

/**
 *  @defgroup DSW_OBT_CONST	Constant
 *  @{
 */

/** 24-bit subsecond value located in 24 msb of 32-bit register */
#define OBT_MAX_OBT_FINE_VALUE 	0xFFFFFF00UL

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
 *  @defgroup DSW_OBT_API Public Functions
 *  @{
 *  \brief	Public functions provided by the OBT Driver
 */



/**
 * \brief	This function preloads a predefined OBT value,
 *          but it is not yet transferred to the active OBT
 *          registers until it is explicitly triggered
 *
 * \param[in]	obt_coarse	Over-second counter value
 * \param[in]	obt_fine	Sub-second counter value (max value = 0xFFFFFF00),
 *                          located in the 24 MSBs of 32-bit parameter
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- OBT_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_obt_preload_obt( uint32_t obt_coarse,
								uint32_t obt_fine );



/**
 * \brief	This function triggers the loading of the preloaded OBT
 *
 * \return	void
 */
void icu_obt_trigger_load_preloaded_obt(void);



/**
 * \brief	This function performs a latch of the current OBT value
 *
 * \return	void
 */
void icu_obt_latch_obt(void);



/**
 * \brief	This function reads a previously latched OBT.
 *
 * \param[out]	obt_coarse		Over-second counter value
 * \param[out]	obt_fine		Sub-second counter value (max value = 0xFFFFFF00),
 *                          	located in the 24 MSBs of 32-bit parameter
 * \param[out]	already_read	true if latched value was already read, false
 * 								otherwise
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- OBT_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_obt_read_latched_obt( uint32_t* obt_coarse,
									uint32_t* obt_fine,
									bool_t* already_read );



/**
 * \brief	This function writes directly a new OBT value
 *
 * \param[in]	obt_coarse	Over-second counter value
 * \param[in]	obt_fine	Sub-second counter value (max value = 0xFFFFFF00),
 *                          located in the 24 MSBs of 32-bit parameter
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- OBT_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_obt_write_current_obt( uint32_t obt_coarse,
								uint32_t obt_fine );



/**
 * \brief	This function gets the current value of the OBT Timer
 *
 * \param[out]	obt_coarse	Over-second counter value
 * \param[out]	obt_fine	Sub-second counter value (max value = 0xFFFFFF00),
 *                          located in the 24 MSBs of 32-bit parameter
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- OBT_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_obt_read_current_obt( uint32_t* obt_coarse,
									uint32_t* obt_fine );



/**
 * \brief	This function provides information about OBT synchronization
 *
 * \return	The function returns TRUE if OBT was synchronized in the last
 *          second, FALSE if not
 *
 */
bool_t icu_obt_is_obt_synchronized( void );



/**
 * \brief	This function performs the OBT self-test.
 *          NOTE: This function performs an active wait of 2.5 milliseconds,
 *          therefore it is responsibility of application code to execute this
 *          function while interrupts are disabled, otherwise self-test may fail
 *          if this function is interrupted.
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- OBT_ERROR_SELFTEST_FAILED
 */
uint32_t icu_obt_self_test( void );

/**
 * @}
 */

/**
 * @}
 */

#endif
