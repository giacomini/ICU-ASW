/**
 * \file	mdpa_timers.h
 *
 * \brief	This file defines the public interface of the MDPA Timer driver
 *
 * \author	Imanol Viana Sanchez, <iviana@srg.aut.uah.es>
 *
 * \internal
 * Created: 	Jan 14, 2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Imanol Viana Sánchez
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 * This software is provided under the terms of the GNU General Public v2
 * Licence. A full copy of the GNU GPL is provided in the file COPYING
 * found in the development root of the project.
 *
 */

#ifndef PUBLIC__MDPA_TIMERS_H
#define PUBLIC__MDPA_TIMERS_H


#include <public/basic_types.h>


/*PROTECTED REGION ID(public_mdpa_timers_h) ENABLED START*/

/**
 *  @addtogroup DSW_TIMERS Timers
 *  @{
 */

/**
 *  @defgroup DSW_MDPA_TIMERS MDPA Timers
 *  @{
 */

/**
 *  @defgroup DSW_MDPA_TIMERS_CONST Constant
 *  @{
 */
/*	values to be used as first parameter in all
 *          the functions of the driver that require an identifier of the LED
 */

/** Identifier of the MDPA Timer 1*/
#define 	ICU_MDPA_TIMER_1		(1UL)

/** Identifier of the MDPA Timer 2*/
#define 	ICU_MDPA_TIMER_2		(2UL)

/**
 * @}
 */

/**
 *  @defgroup DSW_MDPA_TIMERS_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the MDPA Timers Driver functions
 */

#define ICU_TIMER_SELF_TEST_ERROR				(1UL)
#define ICU_TIMER_INVALID_INPUT_PARAM_ERROR		(1UL << 1)


/**
 * @}
 */

/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/


/**
 *  @defgroup DSW_MDPA_TIMERS_API Public Functions
 *  @{
 *  \brief	Public functions provided by the MDPA Timers Driver
 */


/**
 * \brief	This function initializes the MDPA Timers driver,
 *          configuring the MDPA timer with a precision of 1 microsecond. This
 * 			function must be called before the rest of the driver functions.
 *
 * \return	void
 */
void icu_MDPA_timer_ctrl_init(void);

/**
 * \brief	Start a MDPA timer.
 *
 * \param[in] timer_id	 Parameter to identify the timer.
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TIMER_INVALID_INPUT_PARAM_ERROR
 */
uint32_t icu_MDPA_timer_ctrl_start_timer(uint32_t timer_id);

/**
 * \brief	Stop a MDPA timer.
 *
 * \param[in] timer_id	Parameter to identify the timer.
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TIMER_INVALID_INPUT_PARAM_ERROR
 */
uint32_t icu_MDPA_timer_ctrl_stop_timer(uint32_t timer_id);

/**
 * \brief	Program a one-shot MDPA timer.
 * \param[in] timer_id	Parameter to identify the timer.
 * \param[in] irq_flag	Flag parameter to determine if an interrupt is
 * 						triggered in the underflow.
 * \param[in] time_us Parameter used to set the timer value in microseconds
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TIMER_INVALID_INPUT_PARAM_ERROR
 */
uint32_t icu_MDPA_timer_ctrl_program_one_shot_timer(
												uint32_t timer_id,
												bool_t irq_flag,
												uint32_t time_us);

/**
 * \brief	Program a periodic MDPA timer.
 * \param[in] timer_id Parameter to identify the timer.
 * \param[in] apply_now Parameter flag to determine if the period is applied
 * 						immediately, or in the next underflow
 * \param[in] irq_flag Flag parameter to determine if an interrupt is
 * 						triggered in the underflow.
 * \param[in] time_us Parameter used to define the timer in microseconds
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TIMER_INVALID_INPUT_PARAM_ERROR
 */
uint32_t icu_MDPA_timer_ctrl_program_periodic_timer(
												uint32_t timer_id,
												bool_t apply_now,
												bool_t irq_flag,
												uint32_t time_us);

/**
 * \brief	Get the contents of register that contain state info
 *
 * \param[in] timer_id Parameter to identify the timer.
 * \param[out] is_enable	Parameter that specify if timer is enabled or not.
 * \param[out] is_auto_reload	Parameter that specify if timer counter will be
 * 		automatically reloaded after each underflow or not.
 * \param[out] reload_value		Value in the Timer Reload Value
 * 		Register
 * \param[out] current_value		Current value of Timer Counter
 * 		Register
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TIMER_INVALID_INPUT_PARAM_ERROR
 */
uint32_t icu_MDPA_timer_ctrl_get_status(uint32_t timer_id,
											bool_t * is_enable,
											bool_t * is_auto_reload,
											uint32_t * reload_value,
											uint32_t * current_value);
/**
 * \brief	Perform a self-test of a MDPA timer.
 *          NOTE: This function performs an active wait of 2.5 milliseconds,
 *          therefore it is responsibility of application code to execute this
 *          function while interrupts are disabled, otherwise self-test may fail
 *          if this function is interrupted.
 *
 * \param[in] timer_id Parameter to identify the timer.
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TIMER_SELF_TEST_ERROR
 * 				- ICU_TIMER_INVALID_INPUT_PARAM_ERROR
 */
uint32_t icu_MDPA_timer_ctrl_self_test(uint32_t timer_id);

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


#endif // PUBLIC__MDPA_TIMERS_H
