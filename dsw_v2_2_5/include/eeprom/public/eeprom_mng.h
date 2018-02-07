/**
 * \file	eeprom_mng.h
 *
 * \brief	This file defines the public interface of the EEPROM driver
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

#ifndef PUBLIC__DSW_EEPROM_MNG_IFACE_V1_H
#define PUBLIC__DSW_EEPROM_MNG_IFACE_V1_H

/**
 *  @defgroup DSW_EEPROM EEPROM Manager
 *  @{
 */

/*PROTECTED REGION ID(public_dsw_eeprom_mng_iface_v1_h) ENABLED START*/


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_EEPROM_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the EEPROM Driver functions
 */

#define ICU_EEPROM_MNG_BLOCK_UNWRITABLE_READ_CHECK	(1UL)
#define ICU_EEPROM_MNG_BLOCK_UNWRITABLE_TIME_CHECK	(1UL<<1)
#define ICU_EEPROM_MNG_BLOCK_UNREADABLE				(1UL<<2)
#define ICU_EEPROM_MNG_LIMITS_ERROR					(1UL<<3)
#define ICU_EEPROM_MNG_SELFTEST_FAILED				(1UL<<4)
#define ICU_EEPROM_MNG_INVALID_INPUT_PARAM			(1UL<<5)

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
 *  @defgroup DSW_EEPROM_ERROR_API Public Functions
 *  @{
 *  \brief	Public functions provided by the EEPROM Driver
 */

/**
 * \brief	This function initializes the EEPROM driver (EEPROM init to
 * 			disabled state); This function must be called before the rest of
 * 			the EEPROM driver functions
 *
 * \return	void
 */
void icu_eeprom_init(void);



/**
* \brief	This function reads a number of bytes from EEPROM and copies them
* 			in the user buffer passed as a parameter. No constraint on valid
* 			start and end address inside EEPROM or byte alignment
*
* \param[in]	address	Address where the data to read are stored
*
* \param[in]	num_of_bytes	Number of bytes to read, range [1, 4KBytes]
*
* \param[out]	data	Buffer where the data shall be copied
*
* \param[out]	p_failing_addr	EEPROM address where the failure was found,
* 								set to 0 otherwise
*
* \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_EEPROM_MNG_INVALID_INPUT_PARAM
 * 				- ICU_EEPROM_MNG_LIMITS_ERROR
 * 				- ICU_EEPROM_MNG_BLOCK_UNREADABLE
*
*/
uint32_t icu_eeprom_read_bytes(const uint32_t address,
								  const uint32_t num_of_bytes,
								  uint8_t * data,
								  uint32_t * p_failing_addr);

/**
* \brief	This function takes a number of bytes from the user buffer passed
* 			as a parameter and copies them to EEPROM. No constraint on valid
* 			start and end address inside EEPROM or byte alignment
*
* \param[in]	address	Address where the data shall be copied
*
* \param[in]	num_of_bytes	Number of bytes to be copied, range [1, 4KBytes]
*
* \param[in]	data	buffer with the data copied
*
* \param[out]	p_failing_addr	EEPROM address where the failure was found,
* 								set to 0 otherwise
*
* \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_EEPROM_MNG_INVALID_INPUT_PARAM
 * 				- ICU_EEPROM_MNG_LIMITS_ERROR
 * 				- ICU_EEPROM_MNG_BLOCK_UNWRITABLE_READ_CHECK
 * 				- ICU_EEPROM_MNG_BLOCK_UNWRITABLE_TIME_CHECK
*
*/
uint32_t icu_eeprom_write_bytes(const uint32_t address,
								   const uint32_t num_of_bytes,
								   uint8_t * data,
								   uint32_t * p_failing_addr);

/**
 * \brief This function calculates the CRC of a number of bytes of EEPROM.
 * 		  The algorithm used for the CRC calculation is the PUS standard Annex
 * 		  1 (i.e. the packet CRC, not the TC(6,9) xor-based checksum calculation).
 * 		  This function is especially suited for the File Manager drive
 *
 * \param[in] address	Address where from which the calculated crc begins
 *
 * \param[in] size	Number of bytes to calculate the crc, range [1, 1 MByte
 * 			(2^20)]
 *
 * \param[out] crc	Calculated CRC
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_EEPROM_MNG_INVALID_INPUT_PARAM
 * 				- ICU_EEPROM_MNG_LIMITS_ERROR
 * 				- ICU_EEPROM_MNG_BLOCK_UNREADABLE
 */
uint32_t icu_eeprom_calculate_crc(const uint32_t address,
											uint32_t size, uint16_t * crc);

/**
 * \brief This function does self-test, checking the 4 bytes stored in the
 * 		last 4 positions of EEPROM are equal to 0xDE, 0xAD, 0xBE, 0xEF
 *
 * \param[out] stored_values 4 bytes read from the EEPROM, in any case
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_EEPROM_MNG_INVALID_INPUT_PARAM
 * 				- ICU_EEPROM_MNG_SELFTEST_FAILED

 */
uint32_t icu_eeprom_self_test(uint8_t stored_values[]);


/**
 * \brief	This function provides information about EEPROM state
 * 			(enabled/disabled)
 *
 * \return	The function returns TRUE if EEPROM is enabled, FALSE if it is
 * 			disabled
 */
bool_t icu_eeprom_is_eeprom_enabled(void);

/**
 * @}
 */
/**
 * @}
 */

/*PROTECTED REGION END*/




#endif // PUBLIC__DSW_EEPROM_MNG_IFACE_V1_H
