/**
 * \file 	mil_std_1553_rt_reg_mem.h
 *
 * \brief	This file declares different structures handled by the RT Driver
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

#ifndef MIL_STD_1553_RT_REG_MEM_H
#define MIL_STD_1553_RT_REG_MEM_H

/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>
#include <public/mil_std_1553_common_mdpa_reg_mem.h>

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @addtogroup DSW_MILBUS_1553 MILBUS 1553
 *  @{
 */

/**
 *  @addtogroup DSW_MILBUS_1553_RT RT
 *  @{
 */
/**
 *  @defgroup DSW_MILBUS_1553_RT_REG_MEM Remote Terminal Structures
 *  @{
 */
/**
 *  @defgroup DSW_MILBUS_1553_RT_REG_MEM_CONST Constants
 *  @{
 */
/**
 * Maximum number of entries of the Command Area
 */
#define COMMAND_AREA_MAX_ENTRIES    1920U

/**
 * Number of blocks of the Data Area
 */
#define DATA_AREA_SIZE_IN_BLOCKS    2048U

/**
 * Offset inside of the Memory Area of the Command Area
 */
#define COMMAND_AREA_OFFSET 0x40000UL

/**
 * @}
 */

/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 *  @defgroup DSW_MILBUS_1553_RT_REG_MEM_TYPES Structures
 *  @{
 *  \brief	Different type of structures handled by the RT Driver
 */

typedef unsigned int	mil_std_1553_rt_reg_field_t;

/**
 * \union	RTControlWord_t
 *
 * \brief	Defines the format of a Control Word
 */
typedef union
{
    volatile uint16_t all_bits;
    struct
    {
        volatile uint16_t data_valid:1,
                          indirection_table:1,
                          rt_rt:1,
                          bus_id:1,
                          spare:1,
                          mem_err:1,
                          tx_time_out_bus_0:1,
						  tx_time_out_bus_1:1,
                          terminal_flag_inhibition:1,
                          higher_data_words_received:1,
                          lower_data_words_received:1,
                          undefined_cmd_received:1,
                          tx_rx_mode_code_illegal:1,
                          loop_test_err:1,
                          busy_app:1,
                          tx_time_out_act_bus:1;
    };

} RTControlWord_t;

/**
 * \union	RTInformationWord_t
 *
 * \brief	Defines the format of an Information Words
 */
typedef union
{

    volatile mil_std_1553_rt_reg_field_t all_bits;
    struct
    {
        volatile mil_std_1553_rt_reg_field_t data_blk_bnmber:11,
                          dating:21;

    };

}RTInformationWord_t;


/**
 * \union	RTCmdWordTableEntry_t
 *
 * \brief	Defines the format of an entry in the Command Area
 */
typedef struct
{

    CommandWord_t cmd_word;
    RTControlWord_t ctrl_word;
    RTInformationWord_t info_word;

}
RTCmdWordTableEntry_t;


/**
 * \union	RTCharacterizationWord_t
 *
 * \brief	Defines the format of a Characterization Word
 */
typedef union
{

    volatile mil_std_1553_rt_reg_field_t all_bits;
    struct {
        volatile mil_std_1553_rt_reg_field_t subaddress_tx_trok:1,
                          subaddress_tx_leg:1,
                          subaddress_tx_ssf:1,
                          subaddress_tx_sreq:1,
                          subaddress_rx_trok:1,
                          subaddress_rx_leg:1,
                          subaddress_rx_ssf:1,
                          subaddress_rx_sreq:1,
                          mode_code_tx_trok:1,
                          mode_code_tx_leg:1,
                          mode_code_tx_ssf:1,
                          mode_code_tx_sreq:1,
                          mode_code_rx_trok:1,
                          mode_code_rx_leg:1,
                          mode_code_rx_ssf:1,
                          mode_code_rx_sreq:1,
                          spare:16;
    };

}
RTCharacterizationWord_t;


/**
 * \union	RTIndirectionTableEntry_t
 *
 * \brief	Defines the format of an Indirection Table entry
 */
typedef union
{
    volatile mil_std_1553_rt_reg_field_t all_bits;
    struct
	{
        volatile mil_std_1553_rt_reg_field_t adupd:1,
                          current_block:10,
                          max_buffer_size:10,
                          beginning_buffer_block:11;
    };

}
RTIndirectionTableEntry_t;


/**
 * \struct	RTMemoryArea_t
 *
 * \brief	Defines a structure to handle all the areas handled by the RT Driver
 */
typedef struct
{
    MemoryAreaDataBlock_t rx_data_area[DATA_AREA_SIZE_IN_BLOCKS];
    MemoryAreaDataBlock_t tx_data_area[DATA_AREA_SIZE_IN_BLOCKS];
    RTCmdWordTableEntry_t	command_area[COMMAND_AREA_MAX_ENTRIES];

    struct
    {
        volatile uint16_t data;
        volatile uint16_t spare;
    }
    rx_mode_command[32];

    RTCharacterizationWord_t characterization_area[32];

    struct
    {
        volatile uint16_t data;
        volatile uint16_t spare;
    }
    tx_mode_command[32];

    uint32_t spare1[32];

    RTIndirectionTableEntry_t rx_address_table[2][32];

    RTIndirectionTableEntry_t tx_address_table[2][32];

    uint32_t spare2[61440];
}
RTMemoryArea_t;

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
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */

#endif // MIL_STD_1553_RT_REG_MEM_H
