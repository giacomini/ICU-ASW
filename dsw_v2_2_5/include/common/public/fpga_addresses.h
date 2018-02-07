/**
 * \file	fpga_addresses.h
 *
 * \brief	This file defines a set of macros to access to the content of the
 * 			all registers of the different FPGA modules handled by the DSW
 *
 * \author	Jesus Fernandez Conde, <jesus.fc@aut.uah.es>
 *
 * \internal
 * Created: 	07-january-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Jesus Fernandez Conde
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef FPGAADDR_H_
#define FPGAADDR_H_


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

/**
 *  @addtogroup DSW_COMMON Common
 *  @{
 */
/**
 *  @defgroup DSW_FPGA_ADDRESSES FPGA Addresses
 *  @{
 */

#include <public/basic_types.h>
#include <public/hw_map.h>



/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/


/**
 *  @defgroup DSW_FPGA_ADDRESSES_RESET Reset Registers
 *  @{
 */

/*** Reset I/F ***/
#define FPGA_RESET_BASE_ADDRESS			FPGA_BASE_ADDRESS // + 0x0000

#define FPGA_REG_RESET_SOURCE			(*((volatile uint32_t *)(FPGA_RESET_BASE_ADDRESS )))  // + 0x00
#define FPGA_REG_RESET_MASK				(*((volatile uint32_t *)(FPGA_RESET_BASE_ADDRESS + 0x04UL)))
#define FPGA_REG_RESET_SW				(*((volatile uint32_t *)(FPGA_RESET_BASE_ADDRESS + 0x08UL)))
#define FPGA_REG_RESET_BP_SW			(*((volatile uint32_t *)(FPGA_RESET_BASE_ADDRESS + 0x0CUL)))
#define FPGA_REG_GENERAL_PURPOSE_1		(*((volatile uint32_t *)(FPGA_RESET_BASE_ADDRESS + 0x10UL)))
#define FPGA_REG_GENERAL_PURPOSE_2		(*((volatile uint32_t *)(FPGA_RESET_BASE_ADDRESS + 0x14UL)))
/**
 * @}
 */

/**
 *  @defgroup DSW_FPGA_ADDRESSES_WD Watchdog Registers
 *  @{
 */

/*** Watchdog I/F ***/
#define FPGA_WATCHDOG_BASE_ADDRESS		(FPGA_BASE_ADDRESS + 0x0080UL)

#define FPGA_REG_WD_VALUE  				(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS ))) // + 0x00
#define FPGA_REG_WD_LIMITS 				(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x04UL)))
#define FPGA_REG_WD_START_STOP 			(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x08UL)))
#define FPGA_REG_WD_REFRESH 			(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x0CUL)))
#define FPGA_REG_WD_CONFIG_LOCK 		(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x10UL)))
#define FPGA_REG_WD_STATUS		 		(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x14UL)))
#define FPGA_REG_WD_T1_IRQ_PENDING 		(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x18UL)))
#define FPGA_REG_WD_T1_IRQ_TEST 		(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x1CUL)))
#define FPGA_REG_WD_T1_IRQ_MASK 		(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x20UL)))
#define FPGA_REG_WD_T1_IRQ_STATUS 		(*((volatile uint32_t *)(FPGA_WATCHDOG_BASE_ADDRESS + 0x24UL)))
/**
 * @}
 */
/**
 *  @defgroup DSW_FPGA_ADDRESSES_INT_IRQ Internal IRQ Registers
 *  @{
 */
/*** IRQs I/F ***/
#define FPGA_REG_IRQ_MNG_BASE_ADDRESS 	(FPGA_BASE_ADDRESS + 0x100UL)

#define FPGA_REG_INTERNAL_IRQ_1_STATUS	(*((volatile uint32_t *)(FPGA_REG_IRQ_MNG_BASE_ADDRESS ))) // + 0x00
#define FPGA_REG_INTERNAL_IRQ_2_STATUS	(*((volatile uint32_t *)(FPGA_REG_IRQ_MNG_BASE_ADDRESS + 0x04UL)))
#define FPGA_REG_INTERNAL_IRQ_3_STATUS	(*((volatile uint32_t *)(FPGA_REG_IRQ_MNG_BASE_ADDRESS + 0x08UL)))
#define FPGA_REG_INTERNAL_IRQ_4_STATUS	(*((volatile uint32_t *)(FPGA_REG_IRQ_MNG_BASE_ADDRESS + 0x0CUL)))
/**
 * @}
 */
/**
 *  @defgroup DSW_FPGA_ADDRESSES_TIMERS	Timer 1 & 2 Registers
 *  @{
 */

/*** FPGA Timers I/F ***/
#define FPGA_GPT_BASE_ADDRESS			(FPGA_BASE_ADDRESS + 0x0180UL)

#define FPGA_REG_GPT1_CTRL_STATUS 		(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS ))) // + 0x00
#define FPGA_REG_GPT1_LOAD				(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x04UL)))
#define FPGA_REG_GPT1_VALUE				(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x08UL)))
#define FPGA_REG_GPT1_TOUT_IRQ_PENDING	(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x0CUL)))
#define FPGA_REG_GPT1_TOUT_IRQ_TEST		(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x10UL)))
#define FPGA_REG_GPT1_TOUT_IRQ_MASK		(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x14UL)))
#define FPGA_REG_GPT1_TOUT_IRQ_STATUS	(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x18UL)))

#define FPGA_REG_GPT2_CTRL_STATUS 		(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x80UL)))
#define FPGA_REG_GPT2_LOAD				(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x84UL)))
#define FPGA_REG_GPT2_VALUE				(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x88UL)))
#define FPGA_REG_GPT2_TOUT_IRQ_PENDING	(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x8CUL)))
#define FPGA_REG_GPT2_TOUT_IRQ_TEST		(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x90UL)))
#define FPGA_REG_GPT2_TOUT_IRQ_MASK		(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x94UL)))
#define FPGA_REG_GPT2_TOUT_IRQ_STATUS	(*((volatile uint32_t *)(FPGA_GPT_BASE_ADDRESS + 0x98UL)))
/**
 * @}
 */

/**
 *  @defgroup DSW_FPGA_ADDRESSES_MEMORIES Memory Registers
 *  @{
 */
/*** MEMORIES I/F ***/
#define	FPGA_REG_MEM_BASE_ADDRESS		(FPGA_BASE_ADDRESS + 0x0280UL)

#define	FPGA_REG_BOARD_CONFIG			(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS ))) // + 0x00
#define FPGA_REG_BOOT_MEM_IRQ_PENDING	(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x10UL)))
#define FPGA_REG_BOOT_MEM_IRQ_TEST		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x14UL)))
#define FPGA_REG_BOOT_MEM_IRQ_MASK		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x18UL)))
#define FPGA_REG_BOOT_MEM_IRQ_STATUS	(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x1CUL)))
#define	FPGA_REG_EEPROM_CONTROL			(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x20UL)))
#define	FPGA_REG_EEPROM_CONFIG			(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x24UL)))
#define FPGA_REG_EEPROM_IRQ_PENDING		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x28UL)))
#define FPGA_REG_EEPROM_IRQ_TEST		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x2CUL)))
#define FPGA_REG_EEPROM_IRQ_MASK		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x30UL)))
#define FPGA_REG_EEPROM_IRQ_STATUS		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x34UL)))
#define FPGA_REG_FPGA_ADDR_IRQ_PENDING	(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x38UL)))
#define FPGA_REG_FPGA_ADDR_IRQ_TEST		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x3CUL)))
#define FPGA_REG_FPGA_ADDR_IRQ_MASK		(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x40UL)))
#define FPGA_REG_FPGA_ADDR_IRQ_STATUS	(*((volatile uint32_t *)(FPGA_REG_MEM_BASE_ADDRESS + 0x44UL)))
/**
 * @}
 */
/**
 *  @defgroup DSW_FPGA_ADDRESSES_OBT OBT Registers
 *  @{
 */

/*** OBT I/F ***/
#define FPGA_OBT_BASE_ADDRESS				(FPGA_BASE_ADDRESS + 0x4000UL)

#define FPGA_REG_OBT_SYNC_STATUS			(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS ))) // + 0x00
#define FPGA_REG_OBT_SYNC_CONTROL			(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x04UL)))
#define FPGA_REG_OBT_COARSE_MIRROR			(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x10UL)))
#define FPGA_REG_OBT_FINE_MIRROR			(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x14UL)))
#define FPGA_REG_OBT_COARSE_DELAYED_WRITE	(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x18UL)))
#define FPGA_REG_OBT_FINE_DELAYED_WRITE		(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x1CUL)))
#define FPGA_REG_OBT_COARSE_LATCH			(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x20UL)))
#define FPGA_REG_OBT_FINE_LATCH				(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x24UL)))
#define FPGA_REG_OBT_COARSE_LATCH_SYNC		(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x28UL)))
#define FPGA_REG_OBT_FINE_LATCH_SYNC		(*((volatile uint32_t *)(FPGA_OBT_BASE_ADDRESS + 0x2CUL)))
/**
 * @}
 */

/**
 *  @defgroup DSW_FPGA_ADDRESSES_LED LED Registers
 *  @{
 */

/*** LED I/F ***/
#define	FPGA_REG_LED_BASE_ADDRESS			(FPGA_BASE_ADDRESS + 0x8000UL)

#define	FPGA_REG_LED_LOCK					(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS ))) // + 0x00
#define	FPGA_REG_LED_MAXIMUM_DUTY			(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x04UL)))
#define	FPGA_REG_LED_SYNC_DELAY				(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x08UL)))
#define	FPGA_REG_LED_CURRENT				(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x0CUL)))
#define	FPGA_REG_LED_DUTY					(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x10UL)))
#define	FPGA_REG_LED_STATUS					(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x14UL)))
#define	FPGA_REG_LED_ERROR_IRQ_PENDING		(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x20UL)))
#define	FPGA_REG_LED_ERROR_IRQ_TEST			(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x24UL)))
#define	FPGA_REG_LED_ERROR_IRQ_MASK			(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x28UL)))
#define	FPGA_REG_LED_ERROR_IRQ_STATUS		(*((volatile uint32_t *)(FPGA_REG_LED_BASE_ADDRESS + 0x2CUL)))
/**
 * @}
 */

/**
 *  @defgroup DSW_FPGA_ADDRESSES_MOTORS FWA & GWA Motors Registers
 *  @{
 */
/*** FWA & GWA Motors I/F ***/
#define FPGA_MOTORS_BASE_ADDRESS 				(FPGA_BASE_ADDRESS+0xC000UL)

#define FPGA_MOTORS_CONTROL_STATUS				(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS ))) // + 0x00
#define FPGA_MOTORS_DIRECT_PHASE				(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0004UL)))
#define FPGA_MOTORS_PROFILE_SIZE				(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0008UL)))
#define FPGA_MOTORS_SPW_CONTROL_STATUS			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x000CUL)))
#define FPGA_MOTORS_STEPS_EXECUTED	 			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0010UL)))
#define FPGA_MOTORS_DIRECT_STEP_EXE				(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0014UL)))
#define FPGA_MOTORS_CURRENT_STEP_EXE 			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0018UL)))
#define FPGA_MOTORS_HOME_SENSOR_CONFIG 			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x001CUL)))
#define FPGA_MOTORS_ERROR_IRQ_PENDING			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0040UL)))
#define FPGA_MOTORS_ERROR_IRQ_TEST				(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0044UL)))
#define FPGA_MOTORS_ERROR_IRQ_MASK				(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0048UL)))
#define FPGA_MOTORS_ERROR_IRQ_STATUS			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x004CUL)))
#define FPGA_MOTORS_PROFILE_IRQ_PENDING			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0050UL)))
#define FPGA_MOTORS_PROFILE_IRQ_TEST			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0054UL)))
#define FPGA_MOTORS_PROFILE_IRQ_MASK			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0058UL)))
#define FPGA_MOTORS_PROFILE_IRQ_STATUS			(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x005CUL)))
#define FPGA_MOTORS_PROFILE_LUT_TABLE_PHASE_A	(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0400UL)))
#define FPGA_MOTORS_PROFILE_LUT_TABLE_PHASE_B	(*((volatile uint32_t *)(FPGA_MOTORS_BASE_ADDRESS + 0x0800UL)))

/**
 * @}
 */

/**
 *  @defgroup DSW_FPGA_ADDRESSES_SPI SPI Registers
 *  @{
 */
#define FPGA_SPI_BASE_ADDRESS 				(FPGA_BASE_ADDRESS + 0x10000UL)

#define FPGA_REG_SPI_STATUS 				(*((volatile uint32_t *)(FPGA_SPI_BASE_ADDRESS ))) // + 0x00
#define FPGA_REG_SPI_CTRL_READ_WRITE 		(*((volatile uint32_t *)(FPGA_SPI_BASE_ADDRESS + 0x04UL)))
#define FPGA_REG_SPI_FREQUENCY 				(*((volatile uint32_t *)(FPGA_SPI_BASE_ADDRESS + 0x08UL)))
#define FPGA_REG_SPI_IRQ_PENDING 			(*((volatile uint32_t *)(FPGA_SPI_BASE_ADDRESS + 0x0CUL)))
#define FPGA_REG_SPI_IRQ_TEST		 		(*((volatile uint32_t *)(FPGA_SPI_BASE_ADDRESS + 0x10UL)))
#define FPGA_REG_SPI_IRQ_MASK		 		(*((volatile uint32_t *)(FPGA_SPI_BASE_ADDRESS + 0x14UL)))
#define FPGA_REG_SPI_IRQ_STATUS		 		(*((volatile uint32_t *)(FPGA_SPI_BASE_ADDRESS + 0x18UL)))
/**
 * @}
 */

/**
 *  @defgroup DSW_FPGA_ADDRESSES_TM TM Acquisition Registers
 *  @{
 */
#define FPGA_TM_ACQ_BASE_ADDRESS 			(FPGA_BASE_ADDRESS+0x14000UL)

#define FPGA_TM_ACQ_CONTROL_STATUS			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS ))) // + 0x00
#define FPGA_TM_ACQ_ASYNC_TM_EXE			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0004UL)))
#define FPGA_TM_ACQ_ASYNC_TM_DATA			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0008UL)))
#define FPGA_TM_ACQ_CYCLIC_PERIOD			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x000CUL)))
#define FPGA_TM_ACQ_TIMEOUT_PERIOD  		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0010UL)))
#define FPGA_TM_ACQ_CONFIG_SETTLINE_0_1		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0014UL)))
#define FPGA_TM_ACQ_CONFIG_SETTLINE_2_3		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0018UL)))
#define FPGA_TM_ACQ_MUX_OPEN		 		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x001CUL)))
#define FPGA_TM_ACQ_ERROR_IRQ_PENDING		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0040UL)))
#define FPGA_TM_ACQ_ERROR_IRQ_TEST			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0044UL)))
#define FPGA_TM_ACQ_ERROR_IRQ_MASK			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0048UL)))
#define FPGA_TM_ACQ_ERROR_IRQ_STATUS		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x004CUL)))
#define FPGA_TM_ACQ_TABLE_IRQ_PENDING		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0050UL)))
#define FPGA_TM_ACQ_TABLE_IRQ_TEST			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0054UL)))
#define FPGA_TM_ACQ_TABLE_IRQ_MASK			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0058UL)))
#define FPGA_TM_ACQ_TABLE_IRQ_STATUS		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x005CUL)))
#define FPGA_TM_ACQ_ASYNC_IRQ_PENDING		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0060UL)))
#define FPGA_TM_ACQ_ASYNC_IRQ_TEST			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0064UL)))
#define FPGA_TM_ACQ_ASYNC_IRQ_MASK			(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0068UL)))
#define FPGA_TM_ACQ_ASYNC_IRQ_STATUS		(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x006CUL)))
#define FPGA_TM_ACQ_TMID_TABLE				(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0400UL)))
#define FPGA_TM_ACQ_TM_TABLE				(*((volatile uint32_t *)(FPGA_TM_ACQ_BASE_ADDRESS + 0x0800UL)))
/**
 * @}
 */


/**
 * @}
 */
/**
 * @}
 */



#endif /* FPGAADDR_H_ */




