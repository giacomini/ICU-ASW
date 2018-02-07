/**
 * \file	led_drv.h
 *
 * \brief	This file defines the public interface of the LED driver
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	08-feb-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef LEDS_H
#define LEDS_H


#include <public/basic_types.h>


/**
 *  @defgroup DSW_LEDS LEDs
 *  @{
 */


/**
 *  @defgroup DSW_LEDS_CONST Constants
 *  @{
 */


/**LED 1 Identifier*/
#define	LED_1  (0UL)
/**LED 2 Identifier*/
#define	LED_2  (1UL)
/**LED 3 Identifier*/
#define	LED_3  (2UL)
/**LED 4 Identifier*/
#define	LED_4  (3UL)
/**LED 5 Identifier*/
#define	LED_5  (4UL)

/**
 * @}
 */
/**
 *  @defgroup DSW_LEDS_ERROR_CODES LEDs
 *  @{
 *  \brief	error codes returned by driver functions
 */

#define	ICU_LED_ERROR_MAX_DUTY_CYCLE_EXCEEDED  	(1UL)
#define	ICU_LED_SPI_ERROR_DEV_ID_ERR	 		(1UL<<1)
#define	ICU_LED_SPI_ERROR_RD_PARITY_ERR	 		(1UL<<2)
#define	ICU_LED_SPI_ERROR_TX_OVERFLOW	 		(1UL<<5)
#define	ICU_LED_SPI_ERROR_APB_ERR		 		(1UL<<6)
#define	ICU_LED_SPI_ERROR_INVALID_INPUT_PARAM  	(1UL<<8)
#define	ICU_LED_SPI_ERROR_TIME_OUT 		 		(1UL<<9)

#define	ICU_LED_ERROR_INVALID_INPUT_PARAM  		(1UL<<16)
#define	ICU_LED_ERROR_SPI_COMM_IN_COURSE  		(1UL<<17)
#define	ICU_LED_ERROR_STATUS_RUNNING  			(1UL<<18)
#define	ICU_LED_ERROR_STATUS_STOPPED  			(1UL<<19)


/// @cond ALTERNATIVE_ERROR_CODES
#define	LED_ERROR_MAX_DUTY_CYCLE_EXCEEDED  	ICU_LED_ERROR_MAX_DUTY_CYCLE_EXCEEDED
#define	LED_SPI_ERROR_DEV_ID_ERR	 		ICU_LED_SPI_ERROR_DEV_ID_ERR
#define	LED_SPI_ERROR_RD_PARITY_ERR	 		ICU_LED_SPI_ERROR_RD_PARITY_ERR
#define	LED_SPI_ERROR_TX_OVERFLOW	 		ICU_LED_SPI_ERROR_TX_OVERFLOW
#define	LED_SPI_ERROR_APB_ERR		 		ICU_LED_SPI_ERROR_APB_ERR
#define	LED_SPI_ERROR_INVALID_INPUT_PARAM  	ICU_LED_SPI_ERROR_INVALID_INPUT_PARAM
#define	LED_SPI_ERROR_TIME_OUT 		 		ICU_LED_SPI_ERROR_TIME_OUT

#define	LED_ERROR_INVALID_INPUT_PARAM  		ICU_LED_ERROR_INVALID_INPUT_PARAM
#define	LED_ERROR_SPI_COMM_IN_COURSE  		ICU_LED_ERROR_SPI_COMM_IN_COURSE
#define	LED_ERROR_STATUS_RUNNING  			ICU_LED_ERROR_STATUS_RUNNING
#define	LED_ERROR_STATUS_STOPPED  			ICU_LED_ERROR_STATUS_STOPPED
/// @endcond


/**
 * @}
 */


//LED_MAXIMUM_DUTY, LED_SYNC_DELAY, LED_CURRENT_LEVEL, LED_DUTY,
// * 			LED_STATUS, LED_IRQ_PENDING)

/**
 *  @defgroup DSW_LEDS_ERROR_TYPE Structures
 *  @{
 */


/**
 * \struct	icu_led_drv_status_t
 *
 * \brief	Defines different status information about the LEDs
 */
typedef struct
{
	uint32_t maximum_duty;
	uint32_t sync_delay;
	uint32_t current_level;
	uint32_t duty;
	uint32_t status;
	uint32_t irq_pending;
}
icu_led_drv_status_t;

/**
 * @}
 */

/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_LEDS_ERROR_CODES_API Public Functions
 *  @{
 *  \brief	Public functions provided by the LED Drivers
 */


/**
 * \brief	This function is used to initialize the LED Module with a fixed
 * 			that is assumed to be constant during all mission. Maximum duty
 * 			cycle is related to the value above which the LEDs might be damaged.
 * 			The current expected value in the HW design is 50%. Synchronism
 * 			delay is related to the amount of time that that goes from the LED
 * 			Current PWM to the LED Acquisition PWM, in order to maximize
 * 			usefulness of analog TM for LED signals. Moreover, it sends SPI
 * 			commands to LED_CONFx with the constant values declared in HW/SW
 * 			ICD (see referred document).
 *
 * \param[in] max_duty_cycle	LED maximum duty cycle, as a 10-bit unsigned integer
 * 				with LSB=0,09765625%
 * \param[in] delay	LED synchronism delay, as a 12-bit unsigned integer with
 * 				LSB=100ns
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_LED_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_LED_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_LED_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_LED_SPI_ERROR_APB_ERR
 * 				- ICU_LED_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_LED_SPI_ERROR_TIME_OUT
 *				- ICU_LED_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_led_drv_initialize (const uint32_t max_duty_cycle, const uint32_t delay);

/**
 * \brief	This function checks if starting a LED is possible, and if so it
 * 			performs the "Start LED excitation" sequence. After the commands
 * 			are sent, it checks if the status of the LED Manager indicates if
 * 			it was successful or not. This function performs small internal
 * 			waits, but it is not blocking, it returns immediately after the
 * 			procedure is finished.
 *
 * \param[in] identifier
 * \param[in] current_level
 * \param[in] duty_cycle
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_LED_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_LED_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_LED_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_LED_SPI_ERROR_APB_ERR
 * 				- ICU_LED_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_LED_SPI_ERROR_TIME_OUT
 *				- ICU_LED_ERROR_INVALID_INPUT_PARAM
 *				- ICU_LED_ERROR_STATUS_STOPPED
 *				- ICU_LED_ERROR_STATUS_RUNNING
 *				- ICU_LED_ERROR_SPI_COMM_IN_COURSE
 */
uint32_t icu_led_drv_start_excitation (const uint32_t identifier,
				const uint8_t current_level, const uint32_t duty_cycle);

/**
 * \brief	Stop the LED excitation. It leaves the LEDs in an un-powered and
 * 			un-selected	configuration
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_LED_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_LED_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_LED_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_LED_SPI_ERROR_APB_ERR
 * 				- ICU_LED_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_LED_SPI_ERROR_TIME_OUT
 * 				- ICU_LED_ERROR_SPI_COMM_IN_COURSE
 * 				- ICU_LED_ERROR_STATUS_RUNNING
 */
uint32_t icu_led_drv_stop_excitation (void);


/**
 * \brief	Reset the FPGA LED functionality. It also stops the LED excitation.
 * 			It leaves the LEDs in an un-powered and un-selected configuration
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_LED_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_LED_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_LED_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_LED_SPI_ERROR_APB_ERR
 * 				- ICU_LED_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_LED_SPI_ERROR_TIME_OUT
 * 				- ICU_LED_ERROR_SPI_COMM_IN_COURSE
 *				- ICU_LED_ERROR_STATUS_RUNNING
 */
uint32_t icu_led_drv_reset (void);

/**
 * \brief	Gets the contents of register that contain state info
 * 			(LED_MAXIMUM_DUTY, LED_SYNC_DELAY, LED_CURRENT_LEVEL, LED_DUTY,
 * 			LED_STATUS, LED_IRQ_PENDING)
 *
 * \param[out] 	p_status_regs	User structure where different information
 * 					about the status of the LED will be stored
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_LED_ERROR_INVALID_INPUT_PARAM
 */
uint32_t icu_led_drv_get_status (icu_led_drv_status_t * p_status_regs);


/**
 * @}
 */

/**
 * @}
 */


#endif


