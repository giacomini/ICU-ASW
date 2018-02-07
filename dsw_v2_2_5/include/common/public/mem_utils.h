/**
 * \file	mem_utils.h
 *
 * \brief	This file defines the public interface of the Memory Utilities module
 *
 * \author	Jesus Fernandez Conde, <jesus.fc@aut.uah.es>
 *
 * \internal
 * Created: 	08-april-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Jesus Fernandez Conde
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef MEMUTILS_H
#define MEMUTILS_H


/**
 *  @addtogroup DSW_COMMON Common
 *  @{
 */
/**
 *  @defgroup DSW_MEM_UTILS Memory Utilities
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
 *  @defgroup DSW_MEM_UTILS_ERRO_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the Memory Utilities module of the Common
 *  		package
 */

/** Read_block_and_check error code*/
#define ICU_MEM_UTILS_BLOCK_UNREADABLE (1UL<<1)


/// @cond ALTERNATIVE_ERROR_CODES
/* Alternative value of ICU_MEM_UTILS_BLOCK_UNREADABLE*/
#define MEM_UTILS_BLOCK_UNREADABLE ICU_MEM_UTILS_BLOCK_UNREADABLE
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
 *  @defgroup DSW_MEM_UTILS_API Public Functions
 *  @{
 *  \brief	Public functions provided by the Memory utilities module of the
 *  		Common package
 */



/**
 * \brief	This function copies specified number of bytes from source to
 * 			destination
 *
 *
 * \param[in, out]	destination	Destination buffer where the data will be copied
 *
 * \param[in]	source	Buffer with the data to be copied
 *
 * \param[in]	num_of_bytes	Number of bytes to be copied into the
 * 					destination	buffer
 *
 * \return	void
 */
void icu_mem_utils_mem_copy (uint8_t destination[], uint8_t source[], const uint32_t num_of_bytes);


/**
 * \brief	This function copies specified number of bytes from source to
 * 			destination, waiting 1 microsecond between consecutive write
 * 			accesses
 *
 *
 * \param[in, out]	destination	Destination buffer where the data will be copied
 *
 * \param[in]	source	Buffer with the data to be copied
 *
 * \param[in]	num_of_bytes	Number of bytes to be copied into the
 * 					destination	buffer
 *
 * \return	void
 */
void icu_mem_utils_mem_copy_wait (uint8_t destination[], uint8_t source[], const uint32_t num_of_bytes);



/**
 * \brief	This function reads specified number of bytes from address twice:
 *          1. First time copies bytes to data
 *          2. Second time copies bytes to internal_buf
 *          After that, it compares all bytes read in data and internal_buf,
 *          updating p_failing_addr if they are different
 *          (first failing address found)
 *
 *
 * \param[in]	address Address where the information to copy
 *
 * \param[in]	num_of_bytes	Number of bytes to copy
 *
 * \param[in, out]	data	Buffer where the information is copied the first
 * 					time
 *
 * \param[in, out]	internal_buf	Buffer where the information is copied the
 * 					second time
 *
 * \param[out]	p_failing_addr Stores the address where a comparison error,
 * 					between data and internal_buf, was produced; If there is
 * 					no error then the initial value is not modified
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- MEM_UTILS_BLOCK_UNREADABLE
 *
 */
uint32_t icu_mem_utils_read_block_and_check(uint8_t* address,
										  const uint32_t num_of_bytes,
										  uint8_t data[],
										  uint8_t internal_buf[],
										  uint32_t* p_failing_addr);



/**
 * \brief	This function checks the if the address and size passed as
 * 			parameters are within the limits of ram
 *
 * 	\param[in]	address	address to be checked.
 *
 * 	\param[in]	size	size to be checked.
 *
 * 	\return	TRUE if address and size are within limits or FALSE if not
 */
bool_t icu_mem_utils_ram_check_limits(const uint32_t address, const uint32_t size);



/**
 * \brief	This function checks the if the address and size passed as
 * 			parameters are within the limits of EEPROM
 *
 * 	\param[in]	address	address to be checked
 *
 * 	\param[in]	size	size to be checked
 *
 * 	\return	TRUE if address and size are within limits or FALSE if not
 */
bool_t icu_mem_utils_eeprom_check_limits(const uint32_t address, const uint32_t size);



/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */


#endif
