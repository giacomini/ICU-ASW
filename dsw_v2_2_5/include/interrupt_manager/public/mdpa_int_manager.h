/**
 * \file	mdpa_int_manager.h
 *
 * \brief  This file defines the public interface of the MPDA Timers driver
 *
 *
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	26-january-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef ICU_MDPA_INT_MANAGER_H
#define ICU_MDPA_INT_MANAGER_H



/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>


/**
 *  @addtogroup DSW_IRQ_MANAGER IRQ Manager
 *  @{
 */

/**
 *  @defgroup DSW_MDPA_IRQ_MANAGER MDPA IRQ Manager
 *  @{
 *
 */

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_MDPA_IRQ_MANAGER_PRIMARY Primary IRQ Controller constants
 *  @{
 *  \brief	Constants to be used in PRIMARY INT CONTROLLER functions (bits)
 * 			- 15  NMI connected to external Pin
 * 			- 14  DSU trace buffer
 * 			- 13  Time Tick (from OBT)
 * 			- 12  Milbus Interrupt from HP CIC
 * 			- 11  HP Communication Interrupt Controller
 * 			- 10  Parallel I/O (3), configured through ISEL3
 * 			- 9   Timer 2
 * 			- 8   UART 2
 * 			- 7   Parallel I/O (2), configured through ISEL2
 * 			- 6   UART 1
 * 			- 5   SpaceWire Module 1
 * 			- 4   SpaceWire Module 2
 * 			- 3   Timer 1
 * 			- 2   LP Communication Interrupt Controller
 * 			- 1   AHB Status register (lowest)
 * bits [12-11], [5-4] and [2] are not implemented for force/clear functions
 * bits [12] and [2] have to be cleared using HPC/LPC functions(*), see below
 * interrupt is masked when its corresponding bit is 0 (reset value)
 * interrupt is enabled when its corresponding bit is 1
 * (*) See Section 3.8 of MDPA-AST-UM-0092 document for more details
 * IMPORTANT NOTE: Relationship between FPGA interrupts and MDPA interrupts:
 *
 * 	FPGA IRQ3 connected to Parallel I/O 2:
 *
 * 	-	rtems_interrupt_catch(fpga_irq3_handler_irq, LEON_TRAP_TYPE(7), NULL);
 *
 * 	FPGA IRQ2 connected to Parallel I/O 3:
 *
 * 	-	rtems_interrupt_catch(fpga_irq2_handler_irq, LEON_TRAP_TYPE(10), NULL);
 */

#define MDPA_PIC_INT_TIMER_1 		(1UL<<3)
#define MDPA_PIC_INT_PARALLEL_IO2 	(1UL<<7)
#define MDPA_PIC_INT_TIMER_2 		(1UL<<9)
#define MDPA_PIC_INT_PARALLEL_IO3 	(1UL<<10)
#define MDPA_PIC_INT_MILBUS		 	(1UL<<12)

/**
 * @}
 */

/**
 *  @defgroup DSW_MDPA_IRQ_MANAGER_HP High Priority IRQ Controller constants
 *  @{
 *  \brief	Constants to be used in HIGH PRIORITY INT CONTROLLER functions (bits)
 *  	- 11 : Milbus_2 C53It (nominal interrupt)
 *  	- 10 : Milbus_2 C53Rst (reset interrupt)
 *  	- 9  : Milbus_2 C53Er (error interrupt )
 *  	- 8  : Milbus_1 C53It (nominal interrupt )
 *  	- 7  : Milbus_1 C53Rst (reset interrupt)
 *  	- 6  : Milbus_1 C53Err (error interrupt )
 * All the Milbus interrupts have to be cleared in the IP blocks
 * there is no function to clear pending interrupts here
 *
 */

#define MDPA_HPC_INT_MILBUS1_ERROR	 	(1UL<<6)
#define MDPA_HPC_INT_MILBUS1_RESET	 	(1UL<<7)
#define MDPA_HPC_INT_MILBUS1_NOMINAL 	(1UL<<8)
#define MDPA_HPC_INT_MILBUS2_ERROR	 	(1UL<<9)
#define MDPA_HPC_INT_MILBUS2_RESET	 	(1UL<<10)
#define MDPA_HPC_INT_MILBUS2_NOMINAL 	(1UL<<11)


/**
 * @}
 */

/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/


/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_MDPA_IRQ_MANAGER_API Public Functions
 *  @{
 *  \brief	Public functions provided by the MDPA module of the IRQ Manager
 *
 */


/**
 * \brief	This function initializes the MDPA part of interrupt manager driver.
 * 			All MDPA interrupts are cleared and initialized to disabled state
 *
 * \return	void
 */
void icu_intmngr_mdpa_init(void);


/*-------------------------PRIMARY INT CONTROLLER-----------------------------*/

/**
 * \brief	This function sets the interrupt mask for MPDA PIC (Primary
 * 			Interrupt Controller).Bits set to 1 in mask will generate interrupt,
 * 			bits set to 0 will not
 *
 * \param[in] mask	value for the mask
 *
 * \return	void
 */
void icu_intmngr_mdpa_pic_set_irq_mask(const uint16_t mask);

/**
 * \brief	This function gets the interrupt mask for MPDA PIC.Bits set to 1
 * 			in mask will generate interrupt, bits set to 0 will not
 *
 * \return	The value of the mask
 */
uint16_t icu_intmngr_mdpa_pic_get_irq_mask(void);


/**
 * \brief	This function sets the interrupt level for MPDA PIC interrupts.
 * 			Bits set to 1 in mask will set interrupt level to 1, bits set to 0
 * 			will set interrupt level to 0
 *
 * \param[in] mask	value for the level mask
 *
 * \return	void
 */
void icu_intmngr_mdpa_pic_set_irq_level(const uint16_t mask);

/**
 * \brief	This function gets the interrupt level (0 or 1) for MPDA PIC
 * 			interrupts
 *
 * \return	The value of the level mask
 */
uint16_t icu_intmngr_mdpa_pic_get_irq_level(void);

/**
 * \brief	This function gets pending interrupts for MPDA PIC
 *
 * \return	The value of the pending register
 */
uint32_t icu_intmngr_mdpa_pic_get_irq_pending(void);

/**
 * \brief	This function forces interrupts for MPDA PIC. Bits set to 1 in
 * 			force_bits will force interrupt, bits set to 0 will not
 *
 * \param force_bits	mask with the IRQ bits to force
 *
 * \return	void
 */
void icu_intmngr_mdpa_pic_force_irq(const uint32_t force_bits);

/**
 * \brief	This function clears pending interrupts for MPDA PIC. Bits set to 1
 * 			in clear_bits will clear interrupt, bits set to 0 will not
 *
 * \param clear_bits	mask with the IRQ bits to clear
 *
 * \return void
 */
void icu_intmngr_mdpa_pic_clear_irq_pending(const uint32_t clear_bits);


/*---------------------HIGH PRIORITY INT CONTROLLER---------------------------*/


/**
 * \brief	This function sets the interrupt mask for HPC (High Priority
 * 			Controller) MPDA. Bits set to 1 in mask will generate interrupt,
 * 			bits set to 0 will not
 *
 * \param[in] mask	value for the mask
 *
 * \return	void
 */
void icu_intmngr_mdpa_hpc_set_irq_mask (const uint32_t mask);

/**
 * \brief	This function gets the interrupt mask for HPC MPDA. Bits set to 1
 * 			in mask will generate interrupt, bits set to 0 will not
 *
 * \return	the mask register value
 */
uint32_t icu_intmngr_mdpa_hpc_get_irq_mask(void);

/**
 * \brief	This function gets pending interrupts for MPDA HPC
 *
 * \return	the pending register value
 */
uint32_t icu_intmngr_mdpa_hpc_get_irq_pending(void);


/**
 * @}
 */
/**
 * @}
 */

/**
 * @}
 */


#endif // ICU_MDPA_INT_H
