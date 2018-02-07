/**
 * \file	active_wait.h
 *
 * \brief	This file defines the public interface of the active wait module
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


#ifndef ACTIVEWAIT_H
#define ACTIVEWAIT_H


/**
 *  @defgroup DSW_COMMON Common
 *  @{
 */
/**
 *  @defgroup DSW_COMMON_ACTIVE_WAIT Active Wait
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
 *  @defgroup DSW_COMMON_ACTIVE_WAIT_CONST Constants
 *  @{
 */
/** 500 milliseconds*/
#define TIME_500MS			500000

/** 2500 microseconds*/
#define TIME_2500US			2500

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
 *  @defgroup DSW_COMMON_ACTIVE_WAIT_API Public Functions
 *  @{
 *  \brief	Public functions provided by the Active Wait module of the Common
 *  		Package
 */

/**
 * \brief	This function performs an active wait (empty body "for" loop) for
 * 			specified number of microseconds
 *
 * \param[in]	microsecs	Time to wait in microseconds
 *
 * \return void
 */
void icu_active_wait_usecs(uint32_t microsecs);


/**
 * \brief	This function performs an active wait of one microsecond, calling
 *          assembler function that implements a loop of NOPs
 * \return void
 */
void icu_active_wait_1_microsec(void);

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
