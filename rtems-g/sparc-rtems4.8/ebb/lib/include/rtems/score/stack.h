/**
 *  @file
 *  stack.h
 *
 *  @brief contains all information about the thread
 *  Stack Handler.  This Handler provides mechanisms which can be used to
 *  initialize and utilize stacks.
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
 *  5273    | 01/11/2009  | mcoutinho    | IPR 843
 *  8184    | 15/06/2010  | mcoutinho    | IPR 451
 *  9872    | 18/03/2011  | aconstantino | SPR 2819
 *  $Rev: 12218 $ | $Date: 2013-11-19 16:05:49 +0000 (Tue, 19 Nov 2013) $| $Author: hsilva $ | Change description: SW-FMECA_17: Task stack underflow and overflow verification
 *
 **/

/**
 *  @addtogroup SUPER_CORE Super Core
 *  @{
 */

#ifndef _RTEMS_SCORE_STACK_H
#define _RTEMS_SCORE_STACK_H

/**
 *  @defgroup ScoreStack Stack Handler
 *
 *   @brief This handler encapsulates functionality which is used in the management
 *  of thread stacks.
 */
/**@{*/

#ifdef __cplusplus
extern "C"
{
#endif

   /**
    *  @brief minimum stack size which every
    *  thread must exceed.
    */
#define STACK_MINIMUM_SIZE  CPU_STACK_MINIMUM_SIZE

   /**
    *  @brief stack header for overflow verification.
    */
#define STACK_BOUND_1 		 0xAAAAAAAA
#define STACK_BOUND_2 		 0x77777777
#define STACK_BOUND_SIZE 	 8 /* size in bytes = sizeof(STACK_BOUND_1) + sizeof(STACK_BOUND_2) */
#define I32_STACK_BOUND_SIZE 2 /* number of uint32_t in the size */

#define STACK_WRAPPER_SIZE 	 16 /* size in bytes = 2 * (sizeof(STACK_BOUND_1) + sizeof(STACK_BOUND_2)) */
#define I32_STACK_WRAPPER_SIZE 	4 /* number of uint32_t in the size */

   /**
    *  @brief control block used to manage each stack.
    */
   typedef struct
   {
      /** This is the stack size. */
      size_t size;
      /** This is the stack size considering the header and the footer. */
      size_t size_with_header_and_footer;
      /** This is the low memory address of stack. */
      void *area;
      /** This is the low memory address of the stack at the footer. */
      uint32_t *area_at_footer;
      /** This is the low memory address of the stack at the header. */
      uint32_t *area_at_header;
   } Stack_Control;

#ifndef __RTEMS_APPLICATION__
#include <rtems/score/stack.inl>
#endif

#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */

/**
 *  @}
 */
