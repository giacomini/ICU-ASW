/**
 * \file	fpga_timers.h
 *
 * \brief	This file defines the public interface of the FPGA Timer driver
 *
 * \author	Jesus Fernandez Conde, <jesus.fc@aut.uah.es>
 *
 * \internal
 * Created: 	08-january-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Jesus Fernandez Conde
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef FPGA_TIMERS_H
#define FPGA_TIMERS_H

/**
 *  @defgroup DSW_TIMERS Timers
 *  @{
 */

/**
 *  @defgroup DSW_FPGA_TIMERS FPGA Timers
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
 *  @defgroup DSW_FPGA_TIMERS_CONST Constans
 *  @{
 */

/**
 * Value to be used as first parameter in all the functions of the driver that
 * require an identifier of the FPGA timer 1
 */
#define	ICU_FPGA_TIMER_1 (1UL)

/**
 * Value to be used as first parameter in all the functions of the driver that
 * require an identifier of the FPGA timer 2
 */
#define	ICU_FPGA_TIMER_2 (2UL)

/// @cond ALTERNATIVE_TIMER_IDS
/* alternative values: these can be used as well */
#define FPGA_TIMERS_TIMER_1 ICU_FPGA_TIMER_1
#define FPGA_TIMERS_TIMER_2 ICU_FPGA_TIMER_2
/// @endcond

/**
 *  @defgroup DSW_FPGA_TIMERS_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the FPGA Timers driver functions
 */

/** Self Test failed error code*/
#define ICU_FPGA_TIMERS_ERROR_SELFTEST_FAILED		(1UL)
/** Invalid parameter error code */
#define ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM	(1UL<<1)


/// @cond ALTERNATIVE_ERROR_CODES
/* Alternative value of ICU_FPGA_TIMERS_ERROR_SELFTEST_FAILED*/
#define FPGA_TIMERS_ERROR_SELFTEST_FAILED		ICU_FPGA_TIMERS_ERROR_SELFTEST_FAILED
/* Alternative value of ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM*/
#define FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM	ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM
/// @endcond


/**
 * @}
 */

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
 *  @defgroup DSW_FPGA_TIMERS_API Public Functions
 *  @{
 *  \brief	Public functions provided by the FPGA Timers Driver
 */


/**
 * \brief	This function initializes the FPGA Timers driver.
 *
 * \return	void
 */
void icu_FPGA_timer_ctrl_init(void);


/**
 * \brief	This function starts the FPGA_TIMER specified as first parameter
 *
 * \param[in]	timer_id	Identifier of timer
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_FPGA_timer_ctrl_start_timer(uint32_t timer_id);



/**
 * \brief	This function stops the FPGA_TIMER specified as first parameter
 *
 * \param[in]	timer_id	Identifier of timer
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_FPGA_timer_ctrl_stop_timer(uint32_t timer_id);




/**
 * \brief	This function programs the FPGA_TIMER specified as first parameter in
 *          one-shot mode.
 *
 * \param[in]	timer_id	Identifier of timer
 *
 * \param[in]	trigger_interrupt	Flag to determine if interrupt is triggered
 * 					or not
 *
 * \param[in]	load_value_usecs	Initial timer counter value in microseconds
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_FPGA_timer_ctrl_program_one_shot_timer(uint32_t timer_id,
													bool_t trigger_interrupt,
													uint32_t load_value_usecs);



/**
 * \brief	This function programs the FPGA_TIMER specified as first parameter in
 *          periodic mode.
 *
 * \param[in]	timer_id	Identifier of timer
 *
 * \param[in]	trigger_interrupt	Flag to determine if interrupt is triggered
 * 				or not
 *
 * \param[in]	load_value_usecs	Initial timer counter value in microseconds
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_FPGA_timer_ctrl_program_periodic_timer(uint32_t timer_id,
													bool_t trigger_interrupt,
													uint32_t load_value_usecs);



/**
 * \brief	This function performs the FPGA_TIMER self-test
 *          NOTE: This function performs an active wait of 2.5 milliseconds,
 *          therefore it is responsibility of application code to execute this
 *          function while interrupts are disabled, otherwise self-test may fail
 *          if this function is interrupted.
 *
 * \param[in]	timer_id	Identifier of timer
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_FPGA_TIMERS_ERROR_SELFTEST_FAILED
 * 				- ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_FPGA_timer_ctrl_self_test(uint32_t timer_id);



/**
 * \brief	This function gets the status info of specified timer
 *
 * \param[in]	timer_id	 	An enumerate uint32_t is used as a parameter to
 * 				identify the timer
 *
 * \param[out]	is_running	 	A flag parameter to get the information about if
 * 				the timer is running  or not
 *
 * \param[out]	is_auto_reload	 A flag parameter to get the information about
 * 				if the automatically reload timer function is activated or not
 *
 * \param[out]	reload_value	 A parameter to get the information about the
 * 				timer  reload value
 *
 * \param[out]	current_value	 A parameter to get the information about the
 * 				timer counter value
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_FPGA_TIMERS_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_FPGA_timer_ctrl_get_status( uint32_t timer_id,
										bool_t * is_running,
										bool_t * is_auto_reload,
										uint32_t * reload_value,
										uint32_t * current_value
										);


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
