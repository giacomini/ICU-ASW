/**
 *  @file
 *  config.h
 *
 *  @brief contains the table of user defined configuration
 *  parameters specific for the RTEMS API.
 *
 *  Project: RTEMS - Real-Time Executive for Multiprocessor Systems. Partial Modifications by RTEMS Improvement Project (Edisoft S.A.)
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  Version | Date        | Name         | Change history
 *  179     | 17/09/2008  | hsilva       | original version
 *  559     | 17/11/2008  | mcoutinho    | IPR 69
 *  5273    | 01/11/2009  | mcoutinho    | IPR 843
 *  8320    | 21/06/2010  | mcoutinho    | IPR 451
 *  9872    | 18/03/2011  | aconstantino | SPR 2819
 *  $Rev: 12479 $ | $Date: 2014-02-12 12:21:11 +0000 (Wed, 12 Feb 2014) $| $Author: hsilva $ | SWFMECA 19_20_21_22_23_24_25
 *
 **/

/**
 *  @addtogroup RTEMS_API RTEMS API
 *  @{
 */

/**
 *  @addtogroup RTEMS_API_COMMON Common
 *  @{
 */

#ifndef _RTEMS_RTEMS_CONFIG_H
#define _RTEMS_RTEMS_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <rtems/rtems/types.h>
#include <rtems/rtems/tasks.h>

   /**
    *  @brief The following records define the Configuration Table.  The
    *  information contained in this table is required in all
    *  RTEMS systems, whether single or multiprocessor.  This
    *  table primarily defines the following:
    *  \n
    *     + required number of each object type
    */
   typedef struct
   {

      /**
       * @brief maximum number of tasks in the system
       */
      uint32_t maximum_tasks;

      /**
       * @brief maximum number of timers in the system
       */
      uint32_t maximum_timers;

      /**
       * @brief maximum number of semaphores in the system
       */
      uint32_t maximum_semaphores;

      /**
       * @brief maximum number of maximum message queues in the system
       */
      uint32_t maximum_message_queues;
      
      /**
       * @brief maximum number of rate monotonic periods
       */
      uint32_t maximum_periods;

      /**
       * @brief number of user initialization tasks
       */
      uint32_t number_of_initialization_tasks;

      /**
       * @brief table of the user initialization tasks
       */
      rtems_initialization_tasks_table *User_initialization_tasks_table;

      /**
       * @brief flag to enable/disable secure task blockage/suspension with semaphore inherit/ceiling
       */
      uint32_t semaphore_secure_block_suspend;

   } rtems_api_configuration_table;

#ifdef __cplusplus
}
#endif

#endif
/* end of include file */

/**
 *  @}
 */

/**
 *  @}
 */
