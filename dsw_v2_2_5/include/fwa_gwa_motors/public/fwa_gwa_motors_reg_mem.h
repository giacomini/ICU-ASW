/**
 * \file	fwa_gwa_motors_reg_mem.h
 *
 * \brief	This file declares different structures handled by the FWA & GWA
 * 			motor driver
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	1-march-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef FWA_GWA_MOTORS_FPGA_REG_MEM_H
#define FWA_GWA_MOTORS_FPGA_REG_MEM_H

/**
 *  @defgroup DSW_MOTORS FWA GWA Motors
 *  @{
 */

/**
 *  @defgroup DSW_MOTORS_REG_MEM FWA GWA Structures
 *  @{
 */


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>



/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/


/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/
/**
 *  @defgroup DSW_MOTORS_REG_MEM_TYPES Structures
 *  @{
 *  \brief	Structure of the different registers handled by the FWA & GWA Motors
 *  		Driver
 */


typedef unsigned int	fwa_gwa_motors_reg_field_t;

/**
 * \union	FPGA_MOTORS_CTRL_STATUS_t
 *
 * \brief	Defines the structure of the CTRL_STATUS register
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
	volatile fwa_gwa_motors_reg_field_t
		reserved:26,
		home_sensor_clk_enable:1,
		lut_enable:1,
		profile_load_enable:1,
		profile_exec_ctrl:1,
		drv_exec_ctrl:2;
	};
}
FPGA_MOTORS_CTRL_STATUS_t;

/**
 * \union	FPGA_MOTORS_DIRECT_PHASE_t
 *
 * \brief	Defines the structure of the DIRECT_PHASE register
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t current_phase_a:8,
		current_phase_b:8,
		direct_phase_a_b:16;
	};
}
FPGA_MOTORS_DIRECT_PHASE_t;

/**
 * \union	FPGA_MOTORS_PROFILE_SIZE_t
 *
 * \brief	Defines the structure of the PROFILE SIZE register
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t reserved:18,
		size:14;
	};
}
FPGA_MOTORS_PROFILE_SIZE_t;

/**
 * \union	FPGA_MOTORS_SPW_CTRL_STATUS_t
 *
 * \brief	Defines the structure of the SPW CTRL STATUS register
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t reserved:30,
		link_state:1,
		cmd_state:1;
	};
}
FPGA_MOTORS_SPW_CTRL_STATUS_t;

/**
 * \union	FPGA_MOTORS_STEPS_EXECUTED_t
 *
 * \brief	Defines the structure of the SPW STEPS EXECUTED register
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t reserved:18,
		steps:14;
	};
}
FPGA_MOTORS_STEPS_EXECUTED_t;


/**
 * \union	FPGA_MOTORS_STEP_EXE_t
 *
 * \brief	Defines the structure for the DIRECT and CURRENT STEP EXE registers
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t time:16,
		current_phase_a:8,
		current_phase_b:8;
	};
}
FPGA_MOTORS_STEP_EXE_t;


/**
 * \union	FPGA_MOTORS_ERROR_IRQ_t
 *
 * \brief	Defines the structure of the IRQ registers
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t reserved:19,
		spw_seq:1,
		spw_escape:1,
		spw_parity:1,
		spw_credit:1,
		spw_disconnect:1,
		reserved_2:1,
		spw_eep_detected:1,
		unexpected_profile_data_received:1,
		profile_fifo_edac_uncorrectable:1,
		profile_fifo_overflow:1,
		lut_edac_uncorrectable:1,
		size_error:1,
		profile_fifo_underflow:1;
	};
}
FPGA_MOTORS_ERROR_IRQ_t;

/**
 * \union	FPGA_MOTORS_PROFILE_EXE_IRQ_t
 *
 * \brief	Defines the structure of the PROFILE EXE IRQ registers
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t reserved:31,
		profile_executed_succesfully:1;
	};
}
FPGA_MOTORS_PROFILE_EXE_IRQ_t;


/**
 * \union	FPGA_MOTORS_PROFILE_LUT_TABLE_PHASE_t
 *
 * \brief	Defines the structure of the LUT TABLES registers
 */
typedef union
{
	volatile fwa_gwa_motors_reg_field_t all_bits;
	struct
	{
		volatile fwa_gwa_motors_reg_field_t
		reserved_1:6,
		phase_pwm_data_1:10,
		reserved_0:6,
		phase_pwm_data_0:10;
	};
}
FPGA_MOTORS_PROFILE_LUT_TABLE_PHASE_t;


/**
 * \struct	FPGA_MOTORS_REGISTERS_t
 *
 * \brief	Registers defined by the FWA GWA Motors IP Core and handled by the
 * 			FWA GWA Motors Driver
 */
typedef struct
{
	FPGA_MOTORS_CTRL_STATUS_t control_status;
	FPGA_MOTORS_DIRECT_PHASE_t direct_phase;
	FPGA_MOTORS_PROFILE_SIZE_t profile_size;
	FPGA_MOTORS_SPW_CTRL_STATUS_t spw_ctrl_status;
	FPGA_MOTORS_STEPS_EXECUTED_t steps_executed;
	FPGA_MOTORS_STEP_EXE_t direct_step_exe;
	FPGA_MOTORS_STEP_EXE_t current_step_exe;
	uint32_t reserved[9];
	FPGA_MOTORS_ERROR_IRQ_t error_irq_pending;
	FPGA_MOTORS_ERROR_IRQ_t error_irq_test;
	FPGA_MOTORS_ERROR_IRQ_t error_irq_mask;
	FPGA_MOTORS_ERROR_IRQ_t error_irq_status;
	FPGA_MOTORS_PROFILE_EXE_IRQ_t profile_exe_irq_pending;
	FPGA_MOTORS_PROFILE_EXE_IRQ_t profile_exe_irq_test;
	FPGA_MOTORS_PROFILE_EXE_IRQ_t profile_exe_irq_mask;
	FPGA_MOTORS_PROFILE_EXE_IRQ_t profile_exe_irq_status;
	/* LUTs in offsets 0x400 and 0x800 */
	uint32_t reserved2[232];
	FPGA_MOTORS_PROFILE_LUT_TABLE_PHASE_t profile_lut_table_a[128];
	uint32_t reserved3[128];
	FPGA_MOTORS_PROFILE_LUT_TABLE_PHASE_t profile_lut_table_b[128];
}
FPGA_MOTORS_REGISTERS_t;

/**
 * @}
 */

/**
 * @}
 */
/**
 * @}
 */


#endif /* FWA_GWA_MOTORS_FPGA_REG_MEM_H */
