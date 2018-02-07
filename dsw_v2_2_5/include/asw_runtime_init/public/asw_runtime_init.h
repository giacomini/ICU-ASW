/**
 * \file	asw_runtime_init.h
 *
 * \brief	This file defines the public interface of the ASW Runtime Init
 * 			driver
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

#ifndef PUBLIC__ASW_RUNTIME_INIT_H
#define PUBLIC__ASW_RUNTIME_INIT_H

/**
 *  @defgroup DSW_ASW_RUNTIME_INIT ASW Runtime Init
 *  @{
 */

/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/*PROTECTED REGION ID(public_asw_runtime_init_h) ENABLED START*/

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
 *  @defgroup DSW_ASW_RUNTIME_INIT_API Public Functions
 *  @{
 *  \brief	Public functions provided by the ASW Runtime Init Driver
 */


/**
 * \brief ASW start up function compatible with RTEMS runtime initialization.
 *
 * \return void
 */
void icu_runtime_init_dsw_startup(void);

/**
 * \brief Function that returns the CDPU version value.
 *
 * \return CDPU Version
 */
uint32_t icu_runtime_init_get_cdpu_version(void);

/**
 * \brief Function for setting the 5MHz clock divider
 *
 * \param[in] value 5MHz clock divider value
 *
 * \return void
 */
void icu_runtime_init_5mhz_clk_gen(uint32_t value);

/**
 * \brief Function for setting the GP_CLOCK clock divider
 *
 * \param[in] value GP_CLOCK clock divider value
 *
 * \return void
 */
void icu_runtime_init_gp_clk_gen(uint32_t value);

/**
 * \brief Function for setting the SpW clock divider and SpW clock multiplier
 *
 * \param[in] value SpW clock divider and SpW clock multiplier value
 *
  * \return void
 */
void icu_runtime_init_spw_clk_gen(uint32_t value);

/**
 * \brief Function for setting Bypass MIL-Bus PLL and Bypass SpW PLL
 *
 * \param[in] value Bypass MIL-Bus PLL and Bypass SpW PLL value
 *
  * \return void
 */
void icu_runtime_init_set_pll_bypass(uint32_t value);

/**
 * \brief Provides the information about the GP_CLOCK clock PLL lock status
 * 		and SpW Clock PLL lock status.
 *
 * \return PLL Status
 */
uint32_t icu_runtime_init_get_pll_status(void);

/**
 * \brief Activates or deactivates the EDAC in RAM
 *
 * \param[in] activate	TRUE for activating the EDAC in RAM, FALSE for
 * 				deactivating
 *
 * \return void
 */
void icu_runtime_activate_EDAC(bool_t activate);

/**
 * \brief Activates or deactivates the data and code cache
 *
 * \param[in] activate	TRUE for activating the CACHE, FALSE for deactivating
 *
 * \return void
 */
void icu_runtime_activate_cache(bool_t activate);

/**
 * @}
 */
/**
 * @}
 */

/*PROTECTED REGION END*/


#endif // PUBLIC__ASW_RUNTIME_INIT_H
