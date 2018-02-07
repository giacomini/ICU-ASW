/**
 * \file	reset.h
 *
 * \brief	This file defines the public interface of the RESET driver
 *
 * \author	Jesus Fernandez Conde, <jesus.fc@aut.uah.es>
 *
 * \internal
 * Created: 	08-january-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcal�.
 * Copyright: Copyright (c) 2016, Jesus Fernandez Conde
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef RESET_H
#define RESET_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/**
 *  @defgroup DSW_RESET Reset
 *  @{
 */



/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/


/**
 *  @defgroup DSW_RESET_SOURCES Reset Sources
 *  @{
 *  \brief	Possible reset sources
 */

/* possible reset sources */
#define RESET_WATCHDOG_RST_SOURCE 	(1UL<<4)
#define RESET_SW_RST_SOURCE 		(1UL<<3)
#define RESET_1553_RST_SOURCE 		(1UL<<2)
#define RESET_CPULOCK_RST_SOURCE 	(1UL<<1)
#define RESET_POWERUP_RST_SOURCE 	(1UL)

/**
 * @}
 */


/**
 *  @defgroup DSW_RESET_ERROR_CODES Error Codes
 *  @{
 */


#define ICU_RESET_ERROR_SELFTEST_FAILED		(1UL)
#define ICU_RESET_ERROR_INVALID_INPUT_PARAM	(1UL<<1)

/// @cond ALTERNATIVE_ERROR_CODES
#define RESET_ERROR_SELFTEST_FAILED		ICU_RESET_ERROR_SELFTEST_FAILED
#define RESET_ERROR_INVALID_INPUT_PARAM	ICU_RESET_ERROR_INVALID_INPUT_PARAM
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
 *  @defgroup DSW_RESET_API Public Functions
 *  @{
 *  \brief	Public functions provided by the Reset Driver
 */


/**
 * \brief	This function reads the Reset Source from RESET_SOURCE register
 *
 * \return	RESET_SOURCE register value
 */
uint32_t icu_reset_get_reset_source( void );



/**
 * \brief	This function clears the specified Reset Source (can be multiple)
 *
 * \param[in]	reset_source	Reset Source(s) to be cleared
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RESET_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_reset_clear_reset_source( uint32_t reset_source );



/**
 * \brief	This function sets the mask for Reset Sources. Those bits set to 1
 *          in the mask are enabled (will generate Reset)
 *
 * \param[in]	reset_mask	Reset mask to be set
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RESET_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_reset_set_mask( uint32_t reset_mask );


/**
 * \brief	This function reads mask for Reset Sources from RESET_MASK register
 *
 * \return	RESET_MASK register value
 */
uint32_t icu_reset_get_mask(void);



/**
 * \brief	This function generates a SW Reset
 *
 * \return	void
 */
void icu_reset_generate_sw_reset(void);



/**
 * \brief	This function generates a Backplane SW Reset (MDPA and FPGA not
 * 			affected)
 *
 *\return	void
 */
void icu_reset_generate_bp_sw_reset(void);


/**
 * \brief	This function sets the information for the forced warm Reset (SW)
 *
 * \param[in]	reset_cause	Cause of Reset
 *
 * \param[in]	dr_addr	Death Report address
 *
 * \return	void
 */
void icu_reset_set_forced_warm_reset_info ( uint32_t reset_cause,
											uint32_t dr_addr ) ;



/**
 * \brief	This function gets the information for the forced warm Reset (SW)
 *
 * \param[out]	p_reset_source	Cause of Reset
 *
 * \param[out]	p_dr_addr		Death Report address
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RESET_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_reset_get_forced_warm_reset_info ( uint32_t* p_reset_source,
												uint32_t* p_dr_addr );

/**
 * @}
 */

/**
 * @}
 */


#endif
