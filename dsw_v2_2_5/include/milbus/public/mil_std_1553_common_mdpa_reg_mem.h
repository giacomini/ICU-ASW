/**
 * \file 	mil_std_1553_common_mdpa_reg_mem.h
 *
 * \brief	This file declares different common structures handled by the MILBUS
 * 			1553 Drivers
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	25-august-2015
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2015, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef MIL_STD_1553_COMMON_MDPA_REG_MEM_H
#define MIL_STD_1553_COMMON_MDPA_REG_MEM_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/**
 *  @addtogroup DSW_MILBUS_1553 MILBUS 1553
 *  @{
 */

/**
 *  @defgroup DSW_MILBUS_1553_COMMON Common
 *  @{
 */

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

#define BLOCK_SIZE_IN_BYTES 64
#define BLOCK_SIZE_IN_WORDS16      32
#define BLOCK_SIZE_IN_WORDS32      16
#define PROGRAM_AREA_BYTES_SIZE (0x1FFFFUL*4UL)

/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

typedef unsigned int	mil_std_1553_reg_field_t;

/**
 * \brief	Format of the Configuration register
 *
 * \union	MIL1553ConfigurationRegister_t
 */
typedef union
{

    volatile mil_std_1553_reg_field_t all_bits;
    struct
    {
        volatile mil_std_1553_reg_field_t spare:4,
                 dw_16_en:1,
                 cmd_rst_en:1,
                 lock_en:1,
                 mem_area:9,
                 err_1553_mask:1,
                 it_sync_mask:1,
                 it_trok_mask:1,
                 ext_sub_ad:5,
                 wd_size:1,
                 ext_area:1,
                 cdt_cir_buf:1,
                 br_cst_en:1,
                 dbc_en:1,
                 time_out:1,
                 mode:2;
    };
}
MIL1553ConfigurationRegister_t;

/**
 * \brief	Format of the Ext Memory Base Address register
 *
 * \union	MIL1553ExtMemoryBaseAddressRegister_t
 */
typedef union
{

    volatile mil_std_1553_reg_field_t all_bits;
    struct
    {
        volatile mil_std_1553_reg_field_t spare:6,
                 ext_mem_ad:26;
    };

}
MIL1553ExtMemoryBaseAddressRegister_t;

/**
 * \brief	Format of the Command Status register
 *
 * \union	MIL1553CommandStatusRegister_t
 */
typedef union
{

    volatile mil_std_1553_reg_field_t all_bits;
    struct
    {
        volatile mil_std_1553_reg_field_t last_cmd_ad:17,
                 err_par_ad:1,
                 spare_1:7,
                 busy_flag:1,
                 rst_st:1,
                 spare_2:2,
                 int_cmd:1,
                 go_stop:1,
                 rst_1553:1;
    };

}
MIL1553CommandStatusRegister_t;

/**
 * \brief	Format of the Nominal IRQ register
 *
 * \union	MIL1553NominalInterruptRegister_t
 */
typedef union
{

    volatile mil_std_1553_reg_field_t all_bits;
    struct
    {
        volatile mil_std_1553_reg_field_t spare:23,
                          switch_sub_ad:5,
                          it_switch:1,
                          it_dbc:1,
                          it_sync:1,
                          it_trok:1;
    };

}
MIL1553NominalInterruptRegister_t;


/**
 * \brief	Format of the Error IRQ register
 *
 * \union	MIL1553ErrorInterruptRegister_t
 */
typedef union
{
    volatile mil_std_1553_reg_field_t all_bits;
    struct
    {
        volatile mil_std_1553_reg_field_t spare:27,
                          err_rt_add:1,
                          cw_error:1,
                          err_inst:1,
                          err_1553:1,
                          err_mem:1;
    };

}
MIL1553ErrorInterruptRegister_t;


/**
 * \brief	Format of the Reset IRQ register
 *
 * \union	MIL1553ResetInterruptRegister_t
 */
typedef union {

    volatile mil_std_1553_reg_field_t all_bits;
    struct {
        volatile mil_std_1553_reg_field_t spare:31,
                          rst_com:1;
    };

}MIL1553ResetInterruptRegister_t;


/**
 * \brief	Format of the Program Counter register
 *
 * \union	MIL1553ProgramCounterRegister_t
 */
typedef union {

    volatile mil_std_1553_reg_field_t all_bits;
    struct {
        volatile mil_std_1553_reg_field_t spare:16,
                          prog_cnt:16;
    };

}
MIL1553ProgramCounterRegister_t;

/**
 * \brief	Format of the Command Word
 *
 * \union	CommandWord_t
 */
typedef union {

    volatile uint16_t all_bits;
    struct
    {
        volatile uint16_t rt_address:5,
                 tx_rx:1,
                 subaddress:5,
                 data_word_count:5;
    };

}CommandWord_t;


/**
 * \brief	Format of the Memory Area
 *
 * \struct	MemoryAreaDataBlock_t
 */
typedef struct {

    volatile uint32_t words[BLOCK_SIZE_IN_WORDS32];

}
MemoryAreaDataBlock_t;

/**
 * \brief	Structure used by the MILBUS 1553 drivers to manage the different
 * 			registers
 *
 * \struct	MIL1553RegisterMap_t
 */
typedef struct
{
	volatile MIL1553ConfigurationRegister_t configuration_reg;
	volatile MIL1553ExtMemoryBaseAddressRegister_t extended_memory_base_address_reg;
	volatile MIL1553CommandStatusRegister_t command_status_reg;
	volatile MIL1553NominalInterruptRegister_t nominal_interrupt_reg;
	volatile MIL1553ErrorInterruptRegister_t error_interrupt_reg;
	volatile MIL1553ResetInterruptRegister_t reset_interrupt_reg;
	volatile MIL1553ProgramCounterRegister_t program_pointer;
	volatile uint32_t rx_table_index_reg;
	volatile uint32_t tx_table_index_reg;
}
MIL1553RegisterMap_t;

/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/


/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/
/**
 * @}
 */
/**
 * @}
 */


#endif // MIL_STD_1553_COMMON_MDPA_REG_MEM_H
