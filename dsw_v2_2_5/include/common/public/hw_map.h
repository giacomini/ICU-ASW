/**
 * \file	hw_map.h
 *
 * \brief	This file defines the HW memory map as seen from CPU
 *
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


#ifndef HWMAP_H_
#define HWMAP_H_


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>


/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @addtogroup DSW_COMMON Common
 *  @{
 */
/**
 *  @defgroup DSW_HW_MAP HW Map
 *  @{
 * \brief MEMORY MAP
 * - Module/Register Name 						Address Address (hex)	Size
 * - Program Memory (Boot PROM)					0x0000_0000-0x0000_FFFF	64 Kbytes
 * - unused 									0x0001_0000-0x0FFF_FFFF
 * - ASW EEPROM									0x1000_0000-0x001F_FFFF	2 Mbytes (1)
 * - unused 									0x1020_0000-0x1FFF_FFFF
 * - FPGA Registers and memory areas (I/O)		0x2000_0000-0x201F_FFFF	4 Mbytes
 * - unused 									0x2020_0000-0x3FFF_FFFF
 * - Instructions/Data Memory (External SRAM)	0x4000_0000-0x407F_FFFF	8 Mbytes
 * - unused 									0x4080_0000-0x7FFF_FFFF
 * - APB Bridge 1								0x8000_0000-0x8FFF_FFFC
 * - Hardware Debug Support (DSU)				0x9000_0000-0x9FFF_FFFC
 * - APB Bridge 2								0xA000_0000-0xAFFF_FFFC
 * - unused 									0xB000_0000-0xFFFF_FFFF
 *
 * (1) Both EEPROM banks (of 2 Mbytes each) are mapped in same address space
 */

/** First address within PROM */
#define PROM_BASE_ADDRESS  	     0x00000000UL
/** First address outside PROM */
#define PROM_END_ADDRESS   	     0x00010000UL
/** First address within EEPROM */
#define EEPROM_BASE_ADDRESS      0x10000000UL
/** First address outside EEPROM */
#define EEPROM_END_ADDRESS       0x10400000UL
/** First address within FPGA */
#define FPGA_BASE_ADDRESS	     0x20000000UL
/** First address outside FPGA */
#define FPGA_END_ADDRESS  	     0x20200000UL
/** First address within RAM */
#define RAM_BASE_ADDRESS  	     0x40000000UL
/** First address outside RAM */
#define RAM_END_ADDRESS   	     0x40800000UL
/** First address within APB BUS 1 */
#define APB_BUS1_BASE_ADDRESS    0x80000000UL
/** First address within APB BUS 2 */
#define APB_BUS2_BASE_ADDRESS    0xA0000000UL


#ifdef DSW_UNIT_TESTING

uint32_t simulated_mdpa_apb1[0x400] __attribute__ ((aligned (4096)));

#undef APB_BUS1_BASE_ADDRESS
#define APB_BUS1_BASE_ADDRESS	    (uint32_t)simulated_mdpa_apb1   /* First address within APB BUS 1 */

#endif


/**
 * @}
 */
/**
 * @}
 */

#endif /* HWMAP_H_ */




