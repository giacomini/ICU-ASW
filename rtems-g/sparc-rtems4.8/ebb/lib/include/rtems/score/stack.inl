/**
 *  @file
 *  stack.inl
 *
 *  @brief contains the static inline implementation of the inlined
 *  routines from the Stack Handler.
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

/**
 *  @addtogroup ScoreStack Stack Handler
 *  @{
 */

#ifndef _RTEMS_SCORE_STACK_INL
#define _RTEMS_SCORE_STACK_INL

#include <rtems/system.h>


/**
 *  @brief initialize the a stack control structure
 *  
 *  This routine initializes the_stack record to indicate that
 *  size bytes of memory starting at starting_address have been
 *  reserved for a stack.
 *
 *  @param[in] the_stack the stack structure to initialize
 *  @param[in] starting_address the starting address of the stack
 *  @param[in] size the size of the stack
 *
 */
RTEMS_INLINE_ROUTINE void _Stack_Initialize(
                                            Stack_Control *the_stack ,
                                            void *starting_address ,
                                            size_t size
                                            )
{
   /* set the stack area as the starting address */
   the_stack->area = starting_address;

   /* set the stack size */
   the_stack->size = size;
}

/**
 *  @brief initialize the a stack control structure (header and footer)
 *
 *  This routine initializes the_stack record to indicate that
 *  size bytes of memory starting at starting_address have been
 *  reserved for a stack, which includes header and footer.
 *
 *  @param[in] the_stack the stack structure to initialize
 *  @param[in] starting_address_at_footer the address of the stack footer
 *  @param[in] starting_address_at_header the address of the stack header
 *  @param[in] size_with_header_and_footer the size of the stack
 *
 */
RTEMS_INLINE_ROUTINE void _Stack_Initialize_Header_and_Footer(
                                            Stack_Control *the_stack ,
                                            uint32_t *starting_address_at_footer ,
                                            uint32_t *starting_address_at_header ,
                                            size_t size_with_header_and_footer
                                            )
{
   /* set the stack address at the footer */
   the_stack->area_at_footer = starting_address_at_footer;

   /* set the stack address at the header */
   the_stack->area_at_header = starting_address_at_header;

   /* set the footer to its 'constant' value */
   the_stack->area_at_footer[0] = STACK_BOUND_1;
   the_stack->area_at_footer[1] = STACK_BOUND_2;

   /* set the header to its 'constant' value */
   the_stack->area_at_header[0] = STACK_BOUND_1;
   the_stack->area_at_header[1] = STACK_BOUND_2;

   /* set the stack size */
   the_stack->size_with_header_and_footer = size_with_header_and_footer;
}


/**
 *  @brief determine if the stack is enough
 *  
 *  This function returns TRUE if size bytes is enough memory for
 *  a valid stack area on this processor, and FALSE otherwise.
 *
 *  @param[in] size the size to determine if it is enough
 *
 *  @return returns TRUE if the size is enough and FALSE otherwise
 */
RTEMS_INLINE_ROUTINE boolean _Stack_Is_enough(
                                              size_t size
                                              )
{
   /* if the size is larger than the minimum size, then it is enough */
   return ( size >= STACK_MINIMUM_SIZE );
}


/**
 *  @brief adjust the size of a stack
 *  
 *  This function increases the stack size to ensure that the thread
 *  has the desired amount of stack space after the initial stack
 *  pointer is determined based on alignment restrictions.
 *
 *  @param[in] size the size to adjust
 *
 *  @return returns the adjust size
 *
 *  @note The amount of adjustment for alignment is CPU dependent.
 */
RTEMS_INLINE_ROUTINE uint32_t _Stack_Adjust_size(
                                                 size_t size
                                                 )
{
   /* increase the size to ensure the aligment */
   return size + CPU_STACK_ALIGNMENT;
}

/**@}*/

#endif
/* end of include file */

/**  
 *  @}
 */
