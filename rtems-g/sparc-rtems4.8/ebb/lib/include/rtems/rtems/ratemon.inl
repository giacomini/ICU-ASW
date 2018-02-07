/**
 *  @file
 *  ratemon.inl
 *
 *  @brief static inline  implementation of the inlined
 *  routines in the Rate Monotonic Manager.
 *
 *  Project: RTEMS - Real-Time Executive for Multiprocessor Systems. Partial Modifications by RTEMS Improvement Project (Edisoft S.A.)
 *
 *  COPYRIGHT (c) 1989-2007.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  Version | Date        | Name         | Change history
 *  179     | 17/09/2008  | hsilva       | original version
 *  4546    | 28/09/2009  | mcoutinho    | IPR 778
 *  5273    | 01/11/2009  | mcoutinho    | IPR 843
 *  8184    | 15/06/2010  | mcoutinho    | IPR 451
 *  9872    | 18/03/2011  | aconstantino | SPR 2819
 *  $Rev: 12196 $ | $Date: 2013-11-13 15:25:07 +0000 (Wed, 13 Nov 2013) $| $Author: hsilva $ | SW-FMECA_5_6_7: Rate Monotonic Deadline
 *
 **/

/**
 *  @addtogroup RTEMS_API RTEMS API
 *  @{
 */

/**
 *  @addtogroup RTEMS_API_RATEMON Rate Monotonic Manager
 *  @{
 */

#ifndef _RTEMS_RTEMS_RATEMON_INL
#define _RTEMS_RTEMS_RATEMON_INL


/**
 *  @brief allocate rate monotonic
 *  
 *  This function allocates a period control block from
 *  the inactive chain of free period control blocks.
 *
 *  @return returns the allocated rate monotonic object
 */
RTEMS_INLINE_ROUTINE Rate_monotonic_Control *_Rate_monotonic_Allocate(void)
{
   /* return the allocated rate monotonic object with the rate
    * monotonic object information table */
   return (Rate_monotonic_Control *) _Objects_Allocate(&_Rate_monotonic_Information);
}


/**
 *  @brief free rate monotonic
 *  
 *  This routine allocates a period control block from
 *  the inactive chain of free period control blocks.
 *
 *  @param[in] the_period the period to free
 */
RTEMS_INLINE_ROUTINE void _Rate_monotonic_Free(
                                               Rate_monotonic_Control *the_period
                                               )
{
   /* free the rate monotonic object information */
   _Objects_Free(&_Rate_monotonic_Information , &the_period->Object);
}


/**
 *  @brief get rate monotonic
 *  
 *  This function maps period IDs to period control blocks.
 *  If ID corresponds to a local period, then it returns
 *  the_period control pointer which maps to ID and location
 *  is set to OBJECTS_LOCAL.  Otherwise, location is set
 *  to OBJECTS_ERROR and the_period is undefined.
 *
 *  @param[in] id the rate monotonic id
 *  @param[out] location the rate monotonic object location
 *
 *  @return returns the rate monotonic object or NULL if there is none
 */
RTEMS_INLINE_ROUTINE Rate_monotonic_Control *_Rate_monotonic_Get(
                                                                 Objects_Id id ,
                                                                 Objects_Locations *location
                                                                 )
{
   /* return the rate monotonic object information with:
    *   the rate monotonic object information
    *   the id of the period
    *   the address to place the location of the period */
   return (Rate_monotonic_Control *) _Objects_Get(&_Rate_monotonic_Information ,
                                                  id , location);
}


/**
 *  @brief announce the passage of a clock tick to the Rate Monotonic Handler
 *
 *  This routine is invoked at each clock tick to update the ticks of the
 *  rate monotonic deadline chain.
 */
RTEMS_INLINE_ROUTINE void _Rate_monotonic_Deadline_Tickle_ticks(void)
{
   /* announce the passage of a clock tick to the rate monotonic deadlines chain */
   _Rate_monotonic_Deadline_Tickle(&_Rate_monotonic_Deadline_Ticks_chain);
}


/**
 *  @brief initialize a rate monotonic deadline
 *
 *  This routine initializes the specified rate monotonic deadline. The deadline is
 *  made inactive, its initial value being 0.
 *
 *  @param[in] the_deadline the deadline to initialize
 *  @param[in] rtmon_id parent rate monotonic object id
 */
RTEMS_INLINE_ROUTINE void _Rate_monotonic_Deadline_Initialize(
                                                              Rate_monotonic_Deadline_Control *the_deadline ,
                                                              Objects_Id rtmon_id
                                                              )
{
   /* initialize the deadline fields */

   /* the initial state is inactive */
   the_deadline->state = RATE_MONOTONIC_DEADLINE_INACTIVE;

   /* the initial deadline */
   the_deadline->deadline = 0;

   /* the initial routine */
   the_deadline->routine = NULL;
   
   /* the parent rate monotonic id */
   the_deadline->rtmon_id = rtmon_id;
   
   /* initial task execution time */
   the_deadline->executionTime = 0;

   /* initial task maximum execution time */
   the_deadline->maximumExecutionTime = 0;

   /* initial task minimum execution time */
   the_deadline->minimumExecutionTime = 0;
}


/**
 *  @brief reset deadline to active state and given deadline
 *
 *  This routine reactivates the specified rate monotonic deadline. The deadline is
 *  made active, its initial value being deadline.
 *
 *  @param[in] the_deadline the deadline to activate
 *  @param[in] deadline the deadline to set the deadline to
 */
RTEMS_INLINE_ROUTINE void _Rate_monotonic_Deadline_Reactivate(
                                                              Rate_monotonic_Deadline_Control *the_deadline,
                                                              uint32_t deadline
                                                              )
{
   /* reset state to active */
   the_deadline->state = RATE_MONOTONIC_DEADLINE_ACTIVE;

   /* reset deadline to given value */
   the_deadline->deadline = deadline;
}


/**
 *  @brief determine the next deadline node
 *
 *  This routine returns a pointer to the deadline node following
 *  THE_DEADLINE on the rate monotonic deadline chain.
 *
 *  @param[in] the_deadline
 *
 *  @return returns the next watchdog on the chain
 */
RTEMS_INLINE_ROUTINE Rate_monotonic_Deadline_Control *_Rate_monotonic_Deadline_Next(
                                                                                    Rate_monotonic_Deadline_Control *the_deadline
                                                                                    )
{
   /* get the next deadline */
   return ( (Rate_monotonic_Deadline_Control *) the_deadline->Node.next );
}


/**
 *  @brief get the first deadline node of a rate monotonic deadline chain
 *
 *  This routine returns a pointer to the first deadline node
 *  on the rate monotonic deadline chain HEADER.
 *
 *  @param[in] header the chain header
 *
 *  @return returns the first deadline on the chain
 */
RTEMS_INLINE_ROUTINE Rate_monotonic_Deadline_Control *_Rate_monotonic_Deadline_First(
                                                                                     Chain_Control *header
                                                                                     )
{
   /* get the first deadline of the chain */
   return ( (Rate_monotonic_Deadline_Control *) header->first );
}

#endif
/* end of include file */

/**  
 *  @}
 */

/**
 *  @}
 */
