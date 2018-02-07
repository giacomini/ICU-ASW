
/**
 * \file	watchdog.h
 *
 * \brief	This file defines the public interface of the WATCHDOG driver
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


#ifndef WATCHDOG_H
#define WATCHDOG_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>


/**
 *  @defgroup DSW_WATCHDOG Watchdog
 *  @{
 */



/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_WATCHDOG_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the Watchdog Driver functions
 */


#define ICU_WATCHDOG_ERROR_SELFTEST_FAILED			(1UL)
#define ICU_WATCHDOG_ERROR_INVALID_INPUT_PARAM		(1UL<<1)

/// @cond ALTERNATIVE_ERROR_CODES
#define WATCHDOG_ERROR_SELFTEST_FAILED			ICU_WATCHDOG_ERROR_SELFTEST_FAILED
#define WATCHDOG_ERROR_INVALID_INPUT_PARAM		ICU_WATCHDOG_ERROR_INVALID_INPUT_PARAM
/// @endcond

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
 *  @defgroup DSW_WATCHDOG_API Public Functions
 *  @{
 *  \brief	Public functions provided by the Watchdog Driver
 */


/**
 * \brief	This function initializes the WATCHDOG driver
 *
 * \return 	void
 */
void icu_watchdog_init(void);


/**
 * \brief	This function starts the WATCHDOG count
 *
 * \return 	void
 *
 */
void icu_watchdog_start_count(void);



/**
 * \brief	This function stops the WATCHDOG count
 *
 * \return 	void
 */
void icu_watchdog_stop_count(void);



/**
 * \brief	This function sets the interval, in ms, from the last refresh to the
 *  		interrupt. The maximum interval is 65535
 *
 * \param[in]	interval_millisecs	Time interval in milliseconds
 *
 * \return 	void
 */
void icu_watchdog_set_interval_to_interrupt(const uint16_t interval_millisecs);



/**
 * \brief	This function sets the interval, in ms, from the interrupt to the
 * 			reset event. The maximum interval is 65535
 *
 * \param[in]	interval_millisecs	Time interval in milliseconds
 *
 * \return 	void
 */
void icu_watchdog_set_interval_from_interrupt_to_reset(const uint16_t interval_millisecs);




/**
 * \brief	This function Resets the count of the WD.
 *          Count for T1 (interrupt generation) will start over again.
 *
 * \return 	void
 */
void icu_watchdog_refresh(void);



/**
 * \brief	This function performs the Watchdog self-test
 *          NOTE: This function performs an active wait of 55 milliseconds,
 *          therefore it is responsibility of application code to execute this
 *          function while interrupts are disabled, otherwise self-test may fail
 *          if this function is interrupted.
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_WATCHDOG_ERROR_SELFTEST_FAILED
 */
uint32_t icu_watchdog_self_test(void);



/**
 * \brief	This function provides information about WD status
 *
 * \return	The function returns TRUE if WD is started, FALSE if it is stopped
 *
 */
bool_t icu_watchdog_is_watchdog_started(void);


/**
 * \brief	This function gets the current value of the WD Timer
 *
 * \param[out]	time_int_to_reset_ms	Time from interrupt to reset in
 * 					milliseconds
 * \param[out]	time_to_interrupt_ms 	Time to interrupt in milliseconds
 *
 * \return	The function returns 0 if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_WATCHDOG_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_watchdog_read_watchdog_counter(uint16_t* time_int_to_reset_ms,
									uint16_t* time_to_interrupt_ms );

/**
 * @}
 */
/**
 * @}
 */


#endif
