/**
 * \file	edac_mng.h
 *
 * \brief	This file defines the public interface of the EDAC Manager driver
 *
 * \author	Imanol Viana Sanchez, <iviana@srg.aut.uah.es>
 *
 * \internal
 * Created: 	08/11/2014
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2014, Imanol Viana Sanchez
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 * This software is provided under the terms of the GNU General Public v2
 * Licence. A full copy of the GNU GPL is provided in the file COPYING
 * found in the development root of the project.
 *
 */

#ifndef PUBLIC__EDAC_MNG_H
#define PUBLIC__EDAC_MNG_H

/*PROTECTED REGION ID(public_edac_mng_h) ENABLED START*/

/**
 *  @defgroup DSW_EDAC_MNG EDAC Manager
 *  @{
 */

/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/


#include <public/basic_types.h>

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_EDAC_MNG_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the EDAC Driver
 */

/** Invalid parameter error code*/
#define ICU_EDAC_ERROR_INVALID_INPUT_PARAM (1UL)


/// @cond ALTERNATIVE_ERROR_CODES
/* Alternative value of ICU_EDAC_ERROR_INVALID_INPUT_PARAM*/
#define EDAC_ERROR_INVALID_INPUT_PARAM ICU_EDAC_ERROR_INVALID_INPUT_PARAM
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
 *  @defgroup DSW_EDAC_MNG_ERROR_API Public Functions
 *  @{
 *  \brief	Public functions provided by the MDPA module of the Edac Driver
 */


/**
 * \brief	Modify the fields of the register %asr16, the register file EDAC
 * 			protection control register
 *
 * \param[in] file_ctrl_data	 Value to write in the file ctrl register
 *
 * \return	void
 */
void icu_edac_mng_write_register_file_ctrl(uint32_t file_ctrl_data);

/**
 * \brief	Return the fields of the register %asr16, the register file EDAC
 * 			protection control register
 *
 * \return	Read value for the file control register
 */
uint32_t icu_edac_mng_read_register_file_ctrl(void);

/**
 * \brief Function for returning the "failing address register" that contains
 * 		  the memory position that failed the EDAC check
 *
 * \return	Failing address register value
 */
uint32_t icu_edac_mng_get_fail_address(void);

/**
 * \brief Function for returning the "AHB status register" that contains
 * 		  information about the EDAC error
 *
 * \return	AHB status register value
 */
uint32_t icu_edac_mng_get_ahb_status(void);

/**
 * \brief Function that performs a scrubbing on a single failing position
 *
 * \param[out] p_AHB_status_reg		Contains information about the EDAC error
 *
 * \param[out] p_AHB_failing_addr	Contains the memory position that failed
 * 				EDAC check
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_EDAC_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_edac_mng_scrub_single_fail(uint32_t * p_AHB_status_reg,
		uint32_t * p_AHB_failing_addr);

/**
 * \brief	Clears the NE and CE bits in the AHB status register
 *
 * \return	void
 */
void icu_edac_mng_clear_ahb_status(void);

/*PROTECTED REGION END*/

/**
 * @}
 */
/**
 * @}
 */

/**
 * @}
 */


#endif // PUBLIC__EDAC_MNG_H
