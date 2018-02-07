/**
 *  @file
 *  bsp.h
 *
 *  @brief This include file contains all SPARC simulator definitions.
 *
 *  Project: RTEMS - Real-Time Executive for Multiprocessor Systems. Partial Modifications by RTEMS Improvement Project (Edisoft S.A.)
 *
 *  COPYRIGHT (c) 1989-1998.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  Ported to ERC32 implementation of the SPARC by On-Line Applications
 *  Research Corporation (OAR) under contract to the European Space
 *  Agency (ESA).
 *
 *  ERC32 modifications of respective RTEMS file: COPYRIGHT (c) 1995.
 *  European Space Agency.
 *
 *  Version | Date        | Name         | Change history
 *  179     | 17/09/2008  | hsilva       | original version
 *  554     | 17/11/2008  | mcoutinho    | IPR 66
 *  4341    | 15/09/2009  | mcoutinho    | IPR 606
 *  5273    | 01/11/2009  | mcoutinho    | IPR 843
 *  5477    | 09/11/2009  | mcoutinho    | IPR 862
 *  6524    | 09/03/2010  | mcoutinho    | IPR 1944
 *  8183    | 15/06/2010  | mcoutinho    | IPR 451
 *  9872    | 18/03/2011  | aconstantino | SPR 2819
 *  $Rev: 12190 $ | $Date: 2013-11-11 17:57:57 +0000 (Mon, 11 Nov 2013) $| $Author: hsilva $ | SW-FMECA 25
 *
 **/

/**
 *  @addtogroup SPARC_LEON2_BSP SPARC LEON2 BSP
 *  @{
 */

#ifndef _BSP_H
#define _BSP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <rtems.h>
#include <leon.h>
#include <rtems/score/cpu.h>
#include <sharedBSPs.h>
#include <sharedSPARC.h>
   
   /* SPARC CPU variant: LEON2 */
#define LEON2 1

#if (CPU_PROVIDES_IDLE_THREAD_BODY == TRUE)

   /**
    *  @brief entry point to the BSP idle thread
    *
    *  Some SPARC implementations have low power, sleep, or idle modes.  This
    *  tries to take advantage of those models.
    */
   void _CPU_Thread_Idle_body(void);

#endif /* CPU_PROVIDES_IDLE_THREAD_BODY */

   /**
    * @brief determine if this BSP has clock driver
    */
#define BSP_HAS_CLOCK_DRIVER                         TRUE

   /**
    * @brief determine if this BSP needs support at clock tick
    */
#define BSP_CLOCK_DRIVER_NEEDS_SUPPORT_AT_CLOCK_TICK FALSE


   /**
    * @brief Leon2 has instruction cache
    */
#define HAS_INSTRUCTION_CACHE                        TRUE

   /* BSP specific interrupt mask functionality */
#define BSP_mask_interrupt(_source) \
        LEON_Mask_interrupt(_source)

   /* BSP specific interrupt unmask functionality */
#define BSP_unmask_interrupt(_source) \
        LEON_Unmask_interrupt(_source)

   /* BSP specific interrupt is masked functionality */
#define BSP_interrupt_is_masked(_source) \
        LEON_Is_interrupt_masked(_source)
   
#ifdef __cplusplus
}
#endif

#endif

/**  
 *  @}
 */
