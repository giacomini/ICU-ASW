/**
 * \file 	mil_std_1553_bc_reg_mem.h
 *
 * \brief	This file declares different structures handled by the BC Driver
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

#ifndef ICU_MIL_STD_1553_BC_MDPA_REG_MEM_H
#define ICU_MIL_STD_1553_BC_MDPA_REG_MEM_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>
#include <public/mil_std_1553_common_mdpa_reg_mem.h>


/**
 *  @addtogroup DSW_MILBUS_1553 MILBUS 1553
 *  @{
 */

/**
 *  @addtogroup DSW_MILBUS_1553_BC BC
 *  @{
 */
/**
 *  @defgroup DSW_MILBUS_1553_BC_REG_MEM Bus Controller Structures
 *  @{
 */


/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

#define PROGRAM_AREA_OFFSET (0x40000UL)

/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

typedef unsigned int	mil_std_1553_bc_reg_field_t;

/**
 * \union	StatusWord_t
 *
 * \brief	Defines the structure of the Status Words
 */
typedef union
{
	volatile uint16_t all_bits;
	struct
	{
		volatile uint16_t rt_address :5, // Bit 15
							message_error :1,
							instrumentation :1,
							service_request :1,
							reserved :3,
							broadcast_command :1,
							busy :1,
							subsystem_flag :1,
							dynamic_bus_control:1,
							terminal_flag:1;
	};
}
 StatusWord_t;

 /**
  * \union	ControlWordRegister_t
  *
  * \brief	Defines the structure of the Control Word register
  */
typedef union
{
	volatile mil_std_1553_bc_reg_field_t all_bits;
	struct
	{
		volatile mil_std_1553_bc_reg_field_t cwerr :1,
		memerr :1,
		ttoflg :1,
		mbusy :1,
		excerr :1,
		lp :1,
		nr1 :1,
		nr2 :1,
		ad1 :1,
		ad2 :1,
		hiwrd :1,
		lowrd :1,
		wwrd :1,
		stw1 :1,
		stw2 :1,
		dblflg :1,
		unused :16;
	};

} ControlWordRegister_t;


/**
 * \union	BCContextBlock_t
 *
 * \brief	Defines the structure of the Context Block
 */
typedef struct
{
	uint16_t program_address;
	uint16_t data_buffer_address;
	uint16_t control_word_address;
	uint16_t control_word;
}
BCContextBlock_t;


/**
 * \enum	MsgInfoFlagsMasks_t
 *
 * \brief	Masks used to define the message flags
 */
typedef enum
{
	BC_MSG_INFO_REPEAT_ON_ERROR_ENABLE_MASK = 0x1,//!< BC_MSG_INFO_REPEAT_ON_ERROR_ENABLE_MASK
	BC_MSG_INFO_LAST_MSG_MASK = 0x2,              //!< BC_MSG_INFO_LAST_MSG_MASK
	BC_MSG_INFO_BUS_ID_MASK = 0x4,                //!< BC_MSG_INFO_BUS_ID_MASK
	BC_MSG_INFO_STOP_ON_ERROR = 0x8               //!< BC_MSG_INFO_STOP_ON_ERROR
}
MsgInfoFlagsMasks_t;

/**
 * \enum	BCInstructionOpCode_t
 *
 * \brief	Operation Codes of the all Instruction Blocks
 */
typedef enum
{
	OP_CODE_WRITE = 0x0,
	OP_CODE_NOP = 0x1,
	OP_CODE_READ = 0x2,
	OP_CODE_CALL = 0x4,
	OP_CODE_JUMP = 0x6,
	OP_CODE_RETURN = 0x8,
	OP_CODE_WAIT = 0xA,
	OP_CODE_RT_EXCHANGE = 0xC,
	OP_CODE_RT_TO_RT_EXCHANGE = 0xE
}
BCInstructionOpCode_t;


/**
 * \union	BCInstructionType_t
 *
 * \brief	Format of the type of all Instruction Blocks
 */
typedef union
{
	uint16_t all_bits;
	struct //NOP
	{
		uint16_t opcode :4,
				unused :2,
				illegal :1,
				unused2 :9;
	} nop;
	struct //WRITE/READ
	{
		uint16_t opcode :4,
				unused :2,
				illegal :1,
				chb :1,
				sync :1,
				unused2 :2,
				prog_data :1,
				reg :4;
	} write_read;
	struct //CALL/JUMP
	{
		uint16_t opcode :4,
				unused :2,
				illegal :1,
				chb :1,
				sync :1,
				extcnd :1,
				errcnd :1,
				reg_pgm :1,
				reg :4;
	} call_jum;
	struct //RETURN/WAIT
	{
		uint16_t opcode :4,
				unused :2,
				illegal :1,
				chb :1,
				sync :1,
				rtc :1, //used only for a WAIT instruction
				tim_ext :1, //used only for a WAIT instruction (depends on RTC bit)
				inte :1, //used only for a RETURN instruction
				unused2 :4;
	} return_wait;
	struct //RT EXCHANGE
	{
		uint16_t opcode :4,
				unused :2,
				illegal :1,
				chb :1,
				sync :1,
				busid :1,
				excmsk :1,
				rep :1,
				unused2 :4;
	} rt_exchange;

}
BCInstructionType_t;

/**
 * \union	BCInstructionParameters_t
 *
 * \brief	Format of the parameters used by the different Instruction Blocks
 */
typedef union
{
	struct //WRITE/READ/CALL/JUMP
	{
		uint16_t memory_address;
		uint16_t unused1[4];
	};
	struct //RT EXCHANGE
	{
		CommandWord_t command_word;
		uint16_t control_word_address;
		uint16_t data_buffer_address;
		uint16_t unused2[2];
	};
	struct //NOP/RETURN/WAIT
	{
		uint16_t fix_parameter; //1000H
		uint16_t unused3[4];
	};
	//RT to RT is not implemented
}
BCInstructionParameters_t;

/**
 * struct	BCInstructionBlock_t
 *
 * \brief
 */
typedef struct
{
	BCInstructionType_t type;
	BCInstructionParameters_t parameters;
}
BCInstructionBlock_t;



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


#endif // ICU_MIL_STD_1553_BC_MDPA_REG_MEM_H
