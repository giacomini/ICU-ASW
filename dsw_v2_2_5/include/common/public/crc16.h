/**
 * \file	crc16.h
 *
 * \brief	functions for handling of the crc 16bits.
 *
 * \author	Alberto Carrasco Gallardo, <acarrasco@srg.aut.uah.es>
 * \author	Oscar Rodriguez Polo, <opolo@aut.uah.es>
 * \author  Pablo Parra Espada, <pablo.parra@aut.uah.es>
 *
 * \internal
 * Created: 	30/04/2012
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2012, Alberto Carrasco Gallardo
 *			  Copyright (c) 2012, Oscar Rodriguez Polo
 * 			  Copyright (c) 2012, Pablo Parra Espada
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 * This software is provided under the terms of the GNU General Public v2
 * Licence. A full copy of the GNU GPL is provided in the file COPYING
 * found in the development root of the project.
 *
 */

#ifndef PUBLIC__CRC16_IFACE_V1_H
#define PUBLIC__CRC16_IFACE_V1_H

/**
 *  @addtogroup DSW_COMMON Common
 *  @{
 */
/**
 *  @defgroup DSW_CRC_16 CRC 16
 *  @{
 */


#include <public/basic_types.h>

/*PROTECTED REGION ID(public_crc16_iface_h) ENABLED START*/

/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/
/**
 *  @defgroup DSW_CRC_16_API Public Functions
 *  @{
 *  \brief	Public functions provided by the CRC 16 module of the Common
 *  		Package
 */
/**
 * \brief	initialize the CRC table
 *
 * \return void
 */
void crc16_init(void);

/**
 * \brief	function to set seed and generate the CRC
 *
 * \param[in]	num_bytes	number of bytes to make the calculation of the CRC.
 * \param[in]	p_data	Address from where the CRC shall be calculated.
 * \param[in]	seed	seed for the calculation.
 *
 * \return	CRC value of the memory area checked.
 */
uint16_t crc16_set_seed_and_calculate_CRC(uint32_t num_bytes,
									const uint8_t *p_data , uint16_t seed);

/**
 * \brief	function to generate the CRC
 *
 * \param[in]	num_bytes	number of bytes to make the calculation of the CRC.
 * \param[in]	p_data	Address from where the CRC shall be calculated.
 *
 * \return	CRC value of the memory area checked.
 */
uint16_t crc16_calculate_CRC(uint32_t  num_bytes, const uint8_t * p_data);

/**
 * \brief	function to check the CRC
 *
 * \param[in]	num_bytes	number of bytes to make the calculation of the CRC.
 * \param[in]	p_data	Address from where the CRC shall be calculated.
 * \param[in]	crc		waited crc value.
 *
 * \return 	TRUE if the calculation is the same as the crc passed as parameter,
 * 			FALSE otherwise.
 */
bool_t crc16_check_CRC(uint32_t  num_bytes, const uint8_t * p_data, uint16_t crc);


/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */

/*PROTECTED REGION END*/


#endif // PUBLIC__CRC16_IFACE_V1_H
