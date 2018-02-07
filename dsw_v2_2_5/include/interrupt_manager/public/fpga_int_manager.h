/**
 * \file	fpga_int_manager.h
 *
 * \brief  This file defines the public interface of the FPGA Timers driver
 *
 * \author	Jesus Fernandez Conde, <jesus.fc@aut.uah.es>
 *
 * \internal
 * Created: 	08-june-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Jesus Fernandez Conde
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef ICU_FPGA_INT_MANAGER_H
#define ICU_FPGA_INT_MANAGER_H



/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/**
 *  @defgroup DSW_IRQ_MANAGER IRQ Manager
 *  @{
 */

/**
 *  @defgroup DSW_FPGA_IRQ_MANAGER FPGA IRQ Manager
 *  @{
 */

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_FPGA_IRQ_MANAGER_ERROR_CODES Error Codes
 *  @{
 * \brief	Error codes returned by the function of the FPGA module of the
 * 			IRQ Manager
 */

#define ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM	(1UL)

/// @cond ALTERNATIVE_ERROR_CODES
#define INT_MNGR_ERROR_INVALID_INPUT_PARAM	ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
/// @endcond


/**
 * @}
 */

/*  IMPORTANT NOTE: Relationship between FPGA interrupts and MDPA interrupts:

	FPGA IRQ3 connected to Parallel I/O 2:

	-> 	rtems_interrupt_catch(fpga_irq3_handler_irq, LEON_TRAP_TYPE(7), NULL);

	FPGA IRQ2 connected to Parallel I/O 3:

	-> rtems_interrupt_catch(fpga_irq2_handler_irq, LEON_TRAP_TYPE(10), NULL);

*/


/*
 * The following values are to be used as second parameter in all
 * functions of the driver related to FPGA IRQ2.
 * In case second parameter is "output", these values can be used
 * for comparison/checking
 */

/* ------------------- IRQ 2 -------------------------*/

/**
 *  @defgroup DSW_FPGA_IRQ_MANAGER_CONST Constants
 *  @{
 */

// BOOT_MEM
#define FPGA_IRQ2_BOOT_MEM_ACC_RANGE_ERR	(1UL<<3)
#define FPGA_IRQ2_BOOT_MEM_PROM_WR_ERR		(1UL<<2)
#define FPGA_IRQ2_BOOT_MEM_ENABLE_ALL		((1UL<<3)|(1UL<<2))
#define FPGA_IRQ2_BOOT_MEM_DISABLE_ALL		0UL
#define FPGA_IRQ2_BOOT_MEM_CLEAR_ALL		FPGA_IRQ2_BOOT_MEM_ENABLE_ALL

// EEPROM
#define FPGA_IRQ2_EEPROM_ACC_DISABLE_ERR	(1UL<<1)
#define FPGA_IRQ2_EEPROM_WR_ACC_ERR			(1UL<<0UL)
#define FPGA_IRQ2_EEPROM_ENABLE_ALL			((1UL<<1)|(1UL))
#define FPGA_IRQ2_EEPROM_DISABLE_ALL		0UL
#define FPGA_IRQ2_EEPROM_CLEAR_ALL			FPGA_IRQ2_EEPROM_ENABLE_ALL

// WATCHDOG T1
#define FPGA_IRQ2_WD_T1_EXPIRATION			(1UL)
#define FPGA_IRQ2_WD_T1_ENABLE_ALL			(1UL)
#define FPGA_IRQ2_WD_T1_DISABLE_ALL			0UL
#define FPGA_IRQ2_WD_T1_CLEAR_ALL			FPGA_IRQ2_WD_T1_ENABLE_ALL

// FPGA ADDRESS
#define FPGA_IRQ2_FPGA_ADDR_APBBUS_ERR		(1UL)
#define FPGA_IRQ2_FPGA_ADDR_ENABLE_ALL		(1UL)
#define FPGA_IRQ2_FPGA_ADDR_DISABLE_ALL		0UL
#define FPGA_IRQ2_FPGA_ADDR_CLEAR_ALL		FPGA_IRQ2_FPGA_ADDR_ENABLE_ALL

// TM ACQ
#define FPGA_IRQ2_TM_ACQ_ERR_MUX_OPEN			(1UL<<12)
#define FPGA_IRQ2_TM_ACQ_ERR_INTERNAL_TMID		(1UL<<10)
#define FPGA_IRQ2_TM_ACQ_ERR_TMBUF_EDAC			(1UL<<9)
#define FPGA_IRQ2_TM_ACQ_ERR_TMIDBUF_EDAC		(1UL<<8)
#define FPGA_IRQ2_TM_ACQ_ERR_INVALID_TMID		(1UL<<7)
#define FPGA_IRQ2_TM_ACQ_ERR_EOT_NOT_FOUND		(1UL<<6)
#define FPGA_IRQ2_TM_ACQ_ERR_ADC				(1UL<<5)
#define FPGA_IRQ2_TM_ACQ_ERR_SYNCACQ_TIMEOUT	(1UL<<4)
#define FPGA_IRQ2_TM_ACQ_ERR_PERIOD_UNDERFLOW	(1UL<<3)
#define FPGA_IRQ2_TM_ACQ_ERR_BUF_LOCK			(1UL<<2)
#define FPGA_IRQ2_TM_ACQ_ERR_SPI_RD_PARITY_ERR	(1UL<<1)
#define FPGA_IRQ2_TM_ACQ_ERR_SPI_DEV_ID_ERR		(1UL)
#define FPGA_IRQ2_TM_ACQ_ERR_ENABLE_ALL			((1UL<<12)|(1UL<<11)|(1UL<<10)|(1UL<<9)|(1UL<<8)|(1UL<<7)| \
												 (1UL<<6)|(1UL<<5)|(1UL<<4)|(1UL<<3)|(1UL<<2)|(1UL<<1)|(1UL))
#define FPGA_IRQ2_TM_ACQ_ERR_DISABLE_ALL		0UL
#define FPGA_IRQ2_TM_ACQ_ERR_CLEAR_ALL			FPGA_IRQ2_TM_ACQ_ERR_ENABLE_ALL

#define FPGA_IRQ2_TM_ACQ_NEW_BLOCK_AVAILABLE		(1UL)
#define FPGA_IRQ2_TM_ACQ_NEW_BLOCK_ENABLE_ALL		(1UL)
#define FPGA_IRQ2_TM_ACQ_NEW_BLOCK_DISABLE_ALL		0UL
#define FPGA_IRQ2_TM_ACQ_NEW_BLOCK_CLEAR_ALL		FPGA_IRQ2_TM_ACQ_NEW_BLOCK_ENABLE_ALL

#define FPGA_IRQ2_TM_ACQ_ASYNC_END_FINISHED			(1UL)
#define FPGA_IRQ2_TM_ACQ_ASYNC_END_ENABLE_ALL		(1UL)
#define FPGA_IRQ2_TM_ACQ_ASYNC_END_DISABLE_ALL		0UL
#define FPGA_IRQ2_TM_ACQ_ASYNC_END_CLEAR_ALL		FPGA_IRQ2_TM_ACQ_ASYNC_END_ENABLE_ALL



/*
 * The following values are to be used as second parameter in all
 * functions of the driver related to FPGA IRQ3.
 * In case second parameter is "output", these values can be used
 * for comparison/checking
 */

/* ------------------- IRQ 3 -------------------------*/

// MOTORS
#define FPGA_IRQ3_MOTOR_PROFILE_END_OK	(1UL)
#define FPGA_IRQ3_MOTOR_PROFILE_END_ENABLE_ALL		(1UL)
#define FPGA_IRQ3_MOTOR_PROFILE_END_DISABLE_ALL		0UL
#define FPGA_IRQ3_MOTOR_PROFILE_END_CLEAR_ALL		FPGA_IRQ3_MOTOR_PROFILE_END_ENABLE_ALL

#define FPGA_IRQ3_MOTOR_ERR_SPW_SEQ		(1UL<<12)
#define FPGA_IRQ3_MOTOR_ERR_SPW_ESC		(1UL<<11)
#define FPGA_IRQ3_MOTOR_ERR_SPW_PARITY	(1UL<<10)
#define FPGA_IRQ3_MOTOR_ERR_SPW_CREDIT	(1UL<<9)
#define FPGA_IRQ3_MOTOR_ERR_SPW_DISC	(1UL<<8)
#define FPGA_IRQ3_MOTOR_ERR_SPW_EEP		(1UL<<6)
#define FPGA_IRQ3_MOTOR_ERR_SPW_UNEXP	(1UL<<5)
#define FPGA_IRQ3_MOTOR_ERR_FIFO_EDAC	(1UL<<4)
#define FPGA_IRQ3_MOTOR_ERR_FIFO_OVERF	(1UL<<3)
#define FPGA_IRQ3_MOTOR_ERR_LUT_EDAC	(1UL<<2)
#define FPGA_IRQ3_MOTOR_ERR_PROF_SIZE	(1UL<<1)
#define FPGA_IRQ3_MOTOR_ERR_FIFO_UNDERF	(1UL)
#define FPGA_IRQ3_MOTOR_ERR_ENABLE_ALL	((1UL<<12)|(1UL<<11)|(1UL<<10)|(1UL<<9)|(1UL<<8)|(1UL<<7)| \
										 (1UL<<6)|(1UL<<5)|(1UL<<4)|(1UL<<3)|(1UL<<2)|(1UL<<1)|(1UL))
#define FPGA_IRQ3_MOTOR_ERR_DISABLE_ALL	0UL
#define FPGA_IRQ3_MOTOR_ERR_CLEAR_ALL	FPGA_IRQ3_MOTOR_ERR_ENABLE_ALL

// LEDs
#define FPGA_IRQ3_LED_ERR_APB_ERR		(1UL<<6)
#define FPGA_IRQ3_LED_ERR_TX_OVERFLOW	(1UL<<5)
#define FPGA_IRQ3_LED_ERR_RD_PARITY_ERR	(1UL<<2)
#define FPGA_IRQ3_LED_ERR_DEV_ID_ERR	(1UL<<1)
#define FPGA_IRQ3_LED_ERR_MAX_DUTY		(1UL)
#define FPGA_IRQ3_LED_ERR_ENABLE_ALL	((1UL<<6)|(1UL<<5)|(1UL<<2)|(1UL<<1)|(1UL))
#define FPGA_IRQ3_LED_ERR_DISABLE_ALL	0UL
#define FPGA_IRQ3_LED_ERR_CLEAR_ALL		FPGA_IRQ3_LED_ERR_ENABLE_ALL

// SPI
#define FPGA_IRQ3_SPI_END_APB_ERR		(1UL<<6)
#define FPGA_IRQ3_SPI_END_TX_OVERFLOW	(1UL<<5)
#define FPGA_IRQ3_SPI_END_RX_UNDERFLOW	(1UL<<4)
#define FPGA_IRQ3_SPI_END_RX_OVERFLOW	(1UL<<3)
#define FPGA_IRQ3_SPI_END_RD_PARITY_ERR	(1UL<<2)
#define FPGA_IRQ3_SPI_END_DEV_ID_ERR	(1UL<<1)
#define FPGA_IRQ3_SPI_END_FINISHED_OK   (1UL)
#define FPGA_IRQ3_SPI_END_ENABLE_ALL	((1UL<<6)|(1UL<<5)|(1UL<<4)|(1UL<<3)|(1UL<<2)|(1UL<<1)|(1UL))
#define FPGA_IRQ3_SPI_END_DISABLE_ALL	0UL
#define FPGA_IRQ3_SPI_END_CLEAR_ALL		FPGA_IRQ3_SPI_END_ENABLE_ALL

// TIMERS
#define FPGA_IRQ3_GPT1_TIMEOUT 			(1UL)
#define FPGA_IRQ3_GPT1_ENABLE_ALL		(1UL)
#define FPGA_IRQ3_GPT1_DISABLE_ALL		0UL
#define FPGA_IRQ3_GPT1_CLEAR_ALL		FPGA_IRQ3_GPT1_ENABLE_ALL

#define FPGA_IRQ3_GPT2_TIMEOUT 			(1UL)
#define FPGA_IRQ3_GPT2_ENABLE_ALL		(1UL)
#define FPGA_IRQ3_GPT2_DISABLE_ALL		0UL
#define FPGA_IRQ3_GPT2_CLEAR_ALL		FPGA_IRQ3_GPT2_ENABLE_ALL




/* FPGA IRQ bit values for checking/comparison in function icu_intmngr_fpga_irq_get_status() */
#define FPGA_IRQ2_TM_ACQ_ERR_STATUS_BIT			(1<<0UL)
#define FPGA_IRQ2_TM_ACQ_NEW_BLOCK_STATUS_BIT	(1<<1)
#define FPGA_IRQ2_TM_ACQ_ASYNC_END_STATUS_BIT	(1<<2)
#define FPGA_IRQ2_BOOT_MEM_STATUS_BIT			(1<<8)
#define FPGA_IRQ2_EEPROM_STATUS_BIT				(1<<9)
#define FPGA_IRQ2_WD_T1_STATUS_BIT				(1<<10)
#define FPGA_IRQ2_FPGA_ADDR_STATUS_BIT			(1<<11)

#define FPGA_IRQ3_MOTOR_PROFILE_END_STATUS_BIT	(1<<0UL)
#define FPGA_IRQ3_MOTOR_ERR_STATUS_BIT			(1<<1)
#define FPGA_IRQ3_LED_ERR_STATUS_BIT			(1<<2)
#define FPGA_IRQ3_SPI_END_STATUS_BIT			(1<<3)
#define FPGA_IRQ3_GPT1_STATUS_BIT				(1<<8)
#define FPGA_IRQ3_GPT2_STATUS_BIT				(1<<9)


/**
 *
 * \brief	These are the values  to be used as first parameter in
 *          icu_intmngr_fpga_irq_get_status() function
 *
 */
#define 	FPGA_IRQ2			(0UL)
#define 	FPGA_IRQ3			(1UL)
/**
 * @}
 */


/**
 *  @defgroup DSW_FPGA_IRQ_MANAGER_IRQ_MODULES IRQ Modules
 *  @{
 * \brief	These are the values to be used as first parameter in all
 *          functions of the driver related to FPGA IRQx (identifier of FPGA IRQx module)
 *
 */
#define 	FPGA_IRQ2_BOOT_MEM			(0UL)
#define 	FPGA_IRQ2_EEPROM			(1UL)
#define 	FPGA_IRQ2_WD_T1				(2UL)
#define 	FPGA_IRQ2_FPGA_ADDR			(3UL)
#define 	FPGA_IRQ2_TM_ACQ_ERR		(4UL)
#define 	FPGA_IRQ2_TM_ACQ_NEW_BLOCK	(5UL)
#define 	FPGA_IRQ2_TM_ACQ_ASYNC_END	(6UL)
#define 	FPGA_IRQ3_MOTOR_PROFILE_END (7UL)
#define 	FPGA_IRQ3_MOTOR_ERR			(8UL)
#define 	FPGA_IRQ3_LED_ERR 			(9UL)
#define 	FPGA_IRQ3_SPI_END 			(10UL)
#define 	FPGA_IRQ3_GPT1 				(11UL)
#define 	FPGA_IRQ3_GPT2 				(12UL)



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
 *  @defgroup DSW_FPGA_IRQ_MANAGER_API Public Functions
 *  @{
 *  \brief	Public functions provided by the IRQ Manager Driver
 */


/**
 * \brief	This function initializes the FPGA part of interrupt manager driver
 *          All FPGA interrupts are cleared and initialized to disabled state
 * \return	void
 */
void icu_intmngr_fpga_init(void);


/**
 * \brief	This function sets the interrupt mask for FPGA IRQx module specified as first parameter
 *			Bits set to 1 in mask will generate interrupt, bits set to 0UL will not
 *
 * \param[in]	irq_int_module	      Identifier of FPGA IRQx module
 * \param[in]	irq_int_module_mask   Interrupt mask to be set
 * \return	The function returns 0UL if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_intmngr_fpga_set_module_mask(const uint32_t irq_int_module,
												const uint32_t irq_int_module_mask);



/**
 * \brief	This function gets the interrupt mask for FPGA IRQx module specified as first parameter
 *			Bits set to 1 in mask will generate interrupt, bits set to 0UL will not
 *
 * \param[in]	irq_int_module	      Identifier of FPGA IRQx module
 * \param[out]	irq_int_module_mask  Interrupt mask currently set
 * \return	The function returns 0UL if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_intmngr_fpga_get_module_mask(const uint32_t irq_int_module,
												uint32_t* irq_int_module_mask);


/**
 * \brief	This function forces interrupt(s) for FPGA IRQx module specified as first parameter
 *			Interrupt(s) will be generated for bits set to 1 in second parameter.
 *
 * \param[in]	irq_int_module	      Identifier of FPGA IRQx module
 * \param[in]	irq_int_module_force Interrupt(s) to be generated
 * \return	The function returns 0UL if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_intmngr_fpga_force_module_int(const uint32_t irq_int_module,
												const uint32_t irq_int_module_force);


/**
 * \brief	This function gets the interrupt pending values for FPGA IRQx module specified as first parameter
 *			Bits set to 1 in pending mean interrupt is pending
 *
 * \param[in]	irq_int_module	        Identifier of FPGA IRQx module
 * \param[out]	irq_int_module_pending   Module interrupt pending
 * \return	The function returns 0UL if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_intmngr_fpga_get_module_pending(const uint32_t irq_int_module,
												uint32_t* irq_int_module_pending);


/**
 * \brief	This function clears pending interrupt(s) for FPGA IRQx module specified as first parameter
 *			Interrupt(s) will be cleared for bits set to 1 in second parameter.
 *
 * \param[in]	irq_int_module	        Identifier of FPGA IRQx module
 * \param[in]	irq_int_module_pending Interrupt(s) to be cleared
 * \return	The function returns 0UL if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_intmngr_fpga_clear_module_pending(const uint32_t irq_int_module,
													const uint32_t irq_int_module_pending);


/**
 * \brief	This function gets the interrupt status for FPGA IRQx module specified as first parameter
 *			Bits set to 1 in status mean interrupt is enabled and pending
 *
 * \param[in]	irq_int_module	        Identifier of FPGA IRQx module
 * \param[out]	irq_int_module_status   Module interrupt status
 * \return	The function returns 0UL if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_intmngr_fpga_get_module_status(const uint32_t irq_int_module,
													uint32_t* irq_int_module_status);


/**
 * \brief	This function gets the interrupt status for all FPGA IRQx interrupt modules of
 *          specified FPGA IRQ, one bit per module
 *
 * \param[in]	irq_int	        Identifier of FPGA IRQ
 * \param[out]	irq_int_status  IRQx Interrupt status
 * \return	The function returns 0UL if executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_INT_MNGR_ERROR_INVALID_INPUT_PARAM
 *
 */
uint32_t icu_intmngr_fpga_get_status(const uint32_t irq_int,
												uint32_t* irq_int_status);

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */


#endif // ICU_FPGA_INT_MANAGER_H


