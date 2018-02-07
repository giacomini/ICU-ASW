/**
 * \file	tm_acq_manager_reg_mem.h
 *
 * \brief	This file declares different structures handled by the TM
 * 			Acquisition Driver
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	1-march-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef TM_ACQ_MANAGER_FPGA_REG_MEM_H
#define TM_ACQ_MANAGER_FPGA_REG_MEM_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/**
 *  @defgroup DSW_TM_MANAGER TM Manager
 *  @{
 */

/**
 *  @defgroup DSW_TM_MANAGER_REG_MEM TM Manager Structures
 *  @{
 */

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_TM_MANAGER_REG_MEM_CONST Constants
 *  @{
 */

/** Defines the maximum number of Telemetry Identifiers handled by the TM Drv*/
#define MAX_NUMBER_TM_IDS (256UL)

/**
 * @}
 */

/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 *  @defgroup DSW_TM_MANAGER_REG_MEM_TYPES Structures
 *  @{
 *  \brief	Structure of the different registers handled by the TM Acquisition
 *  		Driver
 */

typedef unsigned int	tm_acq_reg_field_t;

/**
 * \union	ACQ_CTRL_STATUS_REG_t
 *
 * \brief	Defines the structure for the CTRL STATUS register
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:25,
		synchronized_tm_disable:1,
		cyclic_enable:1,
		tm_buffer_data_ready:1,
		tm_buffer_toggle:1,
		tm_buffer_lock:1,
		start_stop_state:1,
		start_stop_command:1;
	};
}
ACQ_CTRL_STATUS_REG_t;

/**
 * \struct	ACQ_ASYNC_TMID_EXE_REG_t
 *
 * \brief	Defines the structure for the ASYNC TM ID EXE register
 */
typedef struct
{
	volatile tm_acq_reg_field_t tm_id_to_acquire;
}
ACQ_ASYNC_TMID_EXE_REG_t;

/**
 * \union	ACQ_ASYNC_TM_DATA_REG_t
 *
 * \brief	Defines the structure for the ASYNC TM DATA register
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:16,
		tm_error:1,
		tm_data:15;
	};
}
ACQ_ASYNC_TM_DATA_REG_t;

/**
 * \union	ACQ_ASYNC_CYCLIC_PERIOD_REG_t
 *
 * \brief	Defines the structure for the ASYNC CYCLIC PERIOD register
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:16,
		cyclic_period:16;
	};
}
ACQ_ASYNC_CYCLIC_PERIOD_REG_t;

/**
 * \union	ACQ_TIMEOUT_SYNCH_REG_t
 *
 * \brief	Defines the structure for the TIMEOUT SYNCH REG register
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:12,
		synch_tm_timeout:20;
	};
}
ACQ_TIMEOUT_SYNCH_REG_t;

/**
 * \struct	ACQ_CONFIG_SETTLING_0_1_REG_t
 *
 * \brief	Defines the structure for the SETTLING_0_1 register
 */
typedef struct
{
       volatile tm_acq_reg_field_t settling_time_1_0;
}
ACQ_CONFIG_SETTLING_0_1_REG_t;

/**
 * \struct	ACQ_CONFIG_SETTLING_2_3_REG_t
 *
 * \brief	Defines the structure for the SETTLING_0_1 register
 */
typedef struct
{
       volatile tm_acq_reg_field_t settling_time_3_2;
}
ACQ_CONFIG_SETTLING_2_3_REG_t;

/**
 * \union	ACQ_MUX_OPEN_t
 *
 * \brief	Defines the structure for the MUX OPEN register
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:5,
		mux_open_spi_dev_id:4,
		mux_open_spi_address:7,
		mux_open_spi_write_data:16;
	};
}
ACQ_MUX_OPEN_t;

/**
 * \union	ACQ_TABLES_MAX_LENGTH_t
 *
 * \brief	Defines the structure for the MAX LENGTH EXTERNAL TABLES register
 */
typedef struct
{
	volatile tm_acq_reg_field_t max_length_external_tables;
}
ACQ_TABLES_MAX_LENGTH_t;

/**
 * \union	ACQ_ERROR_IRQ_REG_t
 *
 * \brief	Defines the structure for the ERROR IRQ registers
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:19,
		mux_open_error:1,
		trigger_change_error:1,
		apb_address_not_implemented:1,
		tm_buffer_uncorrectable_edac_error:1,
		tm_id_buffer_uncorrectable_edac_error:1,
		invalid_tm_id:1,
		eot_not_found:1,
		adc_error:1,
		synch_tm_acq_timeout:1,
		table_acq_period_underflow:1,
		table_acq_buffer_lick_error:1,
		spi_data_read_parity_error:1,
		spi_device_id_error:1;
	};
}
ACQ_ERROR_IRQ_REG_t;


/**
 * \union	ACQ_TABLE_ACQ_IRQ_REG_t
 *
 * \brief	Defines the structure for the TABLE IRQ registers
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:31,
		table_acq_iteration_finished:1;
	};
}
ACQ_TABLE_ACQ_IRQ_REG_t;

/**
 * \union	ACQ_ASYNC_ACQ_IRQ_REG_t
 *
 * \brief	Defines the structure for the ASYNCH IRQ registers
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
	struct
	{
		volatile tm_acq_reg_field_t reserved:31,
		async_acq_iteration_finished:1;
	};
}
ACQ_ASYNC_ACQ_IRQ_REG_t;

/**
 * \struct	ACQ_TM_ID_TABLE_REG_t
 *
 * \brief	Defines the structure for the TM ID TABLE register
 */
typedef struct
{
	volatile tm_acq_reg_field_t tm_id_to_acquire;
}
ACQ_TM_ID_TABLE_REG_t;

/**
 * \struct	ACQ_TM_ID_TABLE_REG_t
 *
 * \brief	Defines the structure for the TM OUT TABLE register
 */
typedef union
{
	volatile tm_acq_reg_field_t all_bits;
		struct
		{
			volatile tm_acq_reg_field_t data_1:16,
			data_0:16;
		};
}
ACQ_TM_OUT_TABLE_REG_t;

/**
 * \struct	ICU_TM_ACQ_MANAGER_REG_t
 *
 * \brief	Structure used by the TM Acquisition Driver to manage its different
 * 			registers
 */
typedef struct
{
	/*Offset 0x00*/
	ACQ_CTRL_STATUS_REG_t control_status;
	/*Offset 0x04*/
	ACQ_ASYNC_TMID_EXE_REG_t async_tm_exe;
	/*Offset 0x08*/
	ACQ_ASYNC_TM_DATA_REG_t async_tm_data;
	/*Offset 0x0C*/
	ACQ_ASYNC_CYCLIC_PERIOD_REG_t cyclic_period;
	/*Offset 0x10*/
	ACQ_TIMEOUT_SYNCH_REG_t synch_time_out;
	/*Offset 0x14*/
	ACQ_CONFIG_SETTLING_0_1_REG_t config_settling_0_1;
	/*Offset 0x18*/
	ACQ_CONFIG_SETTLING_2_3_REG_t  config_settling_2_3;
	/*Offset 0x1C*/
	ACQ_MUX_OPEN_t mux_open;
	/*Offset 0x20*/
	ACQ_TABLES_MAX_LENGTH_t tables_max_length;
	/*Offset 0x24*/
	tm_acq_reg_field_t reserved[7];
	/*Offset 0x40*/
	ACQ_ERROR_IRQ_REG_t error_acq_irq_pending;
	/*Offset 0x44*/
	ACQ_ERROR_IRQ_REG_t error_acq_irq_test;
	/*Offset 0x48*/
	ACQ_ERROR_IRQ_REG_t error_acq_irq_mask;
	/*Offset 0x4C*/
	ACQ_ERROR_IRQ_REG_t error_acq_irq_status;
	/*Offset 0x50*/
	ACQ_TABLE_ACQ_IRQ_REG_t table_acq_irq_pending;
	/*Offset 0x54*/
	ACQ_TABLE_ACQ_IRQ_REG_t table_acq_irq_test;
	/*Offset 0x58*/
	ACQ_TABLE_ACQ_IRQ_REG_t table_acq_irq_mask;
	/*Offset 0x5C*/
	ACQ_TABLE_ACQ_IRQ_REG_t table_acq_irq_status;
	/*Offset 0x60*/
	ACQ_ASYNC_ACQ_IRQ_REG_t async_acq_irq_pending;
	/*Offset 0x64*/
	ACQ_ASYNC_ACQ_IRQ_REG_t async_acq_irq_test;
	/*Offset 0x68*/
	ACQ_ASYNC_ACQ_IRQ_REG_t async_acq_irq_mask;
	/*Offset 0x6C*/
	ACQ_ASYNC_ACQ_IRQ_REG_t async_acq_irq_status;
	/*Offset 0x70*/
	tm_acq_reg_field_t unimplemented[228];
	/*Offset 0x400*/
	ACQ_TM_ID_TABLE_REG_t tm_id_table[MAX_NUMBER_TM_IDS];
	/*Offset 0x800*/
	ACQ_TM_OUT_TABLE_REG_t tm_table[MAX_NUMBER_TM_IDS/2];
}
ICU_TM_ACQ_MANAGER_REG_t;



/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */



#endif // TM_ACQ_MANAGER_FPGA_REG_MEM_H
