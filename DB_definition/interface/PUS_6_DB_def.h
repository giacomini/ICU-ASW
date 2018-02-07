/**
 * \file	PUS_6_DB_def.h
 *
 * \brief	PUS Service 6 auxiliary variables and functions
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Nov 4, 2016
 */

#ifndef PUS_6_DB_DEF_H_
#define PUS_6_DB_DEF_H_

#include "ICU_config.h"
#include "db_type_def.h"

/**
 *  \ingroup PUS_6_SUPPORT_CONSTANTS
 *  \{
 */
/* redundant!   EUCL-TAST-ICD-1-003 V.5 */
#define MIN_NON_CONSEC_BLOCKS_TO_LOAD 1U //!< MIN_NON_CONSECUTIVE_BLOCKS_TO_LOAD
#define MAX_NON_CONSEC_BLOCKS_TO_LOAD 10U //!< MAX_NON_CONSECUTIVE_BLOCKS_TO_LOAD

/* TBC: for ICU RAM and EEPROM, for DPU not necessary (successive MDUMP)
 * ICU EEPROM_MAX_RD_WR_SIZE is 4096U see eeprom_mng.c
 */
#define MAX_MEM_RW_DATA_LENGTH 4096U
/* max size of MEMREG_LOAD/DUMP_TAB for DPU in Data Word*/
#define DPU_LOAD_MAX_DW 32U
/* FIXME because actual size are 23 and 27 */
#define DPU_MEMREG_TAB_1_MAX_DW 22U
#define DPU_MEMREG_TAB_2_16_MAX_DW 26U
#define DPU_MEMREG_TAB_MAX_LENGTH_BYTE (2U * (DPU_MEMREG_TAB_1_MAX_DW + (15U * DPU_MEMREG_TAB_2_16_MAX_DW)))
#define MAX_TM_6_6_DPU_DATA_LENGTH_BYTE (DPU_MEMREG_TAB_MAX_LENGTH_BYTE)
#define MAX_TM_6_6_DATA_LENGTH_BYTE 996U

/* SAU (Smallest Addressable Unit)
 * For DPUs memories, 4 Bytes except for SCEs (2 Bytes)
 * For ICU */
#define PUS6_DPU_DCU_SAU 4U
#define PUS6_SCE_SAU 2U
#define PUS6_ICU_RAM_EEPROM_PROM_SAU 1U     //FIXME
#define PUS6_ICU_REGISTER_SAU 4U  //FIXME
#define PUS6_DPU_BSW_SAU 4U

/**
 *\enum memory_ID_system_list
 *\brief List of all memory system managed by ICU ASW (MSB of memory ID)
 */
enum memory_ID_system_list {
	MEM_SYS_ICU_IDX = 0U,
	MEM_SYS_DPU1_BSW_IDX,
	MEM_SYS_DPU2_BSW_IDX,
	MEM_SYS_DPU1_IDX,
	MEM_SYS_DPU2_IDX,
};

/**
 *\enum memory_ID_DPU_subsys_list
 *\brief List of all memory subsystem of DPUs while in ASW (LSB of memory ID)
 */
enum memory_ID_DPU_subsys_list {
	MEM_SUBSYS_DPU_CPU_IDX = 0U,
	MEM_SUBSYS_DPU_DBB_IDX= 10U,
	MEM_SUBSYS_DPU_DRB_IDX = 20U,
	MEM_SUBSYS_DPU_DCU1_IDX = 30U,
	MEM_SUBSYS_DPU_DCU2_IDX = 31U,
	MEM_SUBSYS_DPU_DCU3_IDX = 32U,
	MEM_SUBSYS_DPU_DCU4_IDX = 33U,
	MEM_SUBSYS_DPU_DCU5_IDX = 34U,
	MEM_SUBSYS_DPU_DCU6_IDX = 35U,
	MEM_SUBSYS_DPU_DCU7_IDX = 36U,
	MEM_SUBSYS_DPU_DCU8_IDX = 37U,
	MEM_SUBSYS_DPU_SCE1_IDX = 40U,
	MEM_SUBSYS_DPU_SCE2_IDX = 41U,
	MEM_SUBSYS_DPU_SCE3_IDX = 42U,
	MEM_SUBSYS_DPU_SCE4_IDX = 43U,
	MEM_SUBSYS_DPU_SCE5_IDX = 44U,
	MEM_SUBSYS_DPU_SCE6_IDX = 45U,
	MEM_SUBSYS_DPU_SCE7_IDX = 46U,
	MEM_SUBSYS_DPU_SCE8_IDX = 47U,
};

/**
 *\enum mem_ID_ICU_subsys_list
 *\brief List of all memory subsystem of ICU and of DPUs while in boot (LSB of memory ID)
 */
enum mem_ID_ICU_subsys_list {
	MEM_SUBSYS_ICU_RAM_IDX = 1U,
	MEM_SUBSYS_ICU_PROM_IDX = 2U,
	MEM_SUBSYS_ICU_EEPROM_IDX = 3U,
	MEM_SUBSYS_ICU_REGISTER_IDX = 10U,
};

/**
 *\enum mem_ID_DPU_BSW_subsys_list
 *\brief List of all memory subsystem of ICU and of DPUs while in boot (LSB of memory ID)
 */
enum mem_ID_DPU_BSW_subsys_list {
	MEM_SUBSYS_DPU_BSW_RAM_IDX = 1U,
	MEM_SUBSYS_DPU_BSW_EEPROM_IDX = 2U,
};

/**
 * \}
 */

/**
 *  \ingroup PUS_6_SUPPORT_TYPES
 *  \{
 */

/**
 * \struct DPU_MEMREG_DUMP_tab_t
 * \brief Struct used to define the MEMREG tables from DPU memory dump
 */
typedef struct {
	/* cmd_ID needed to distinguish ASW and BSW */
	uint8_t DPU_MDUMP_cmd_id;
	uint16_t DPU_MDUMP_mem_ID;
	uint16_t DPU_MDUMP_ICU_counter;
	uint16_t DPU_MDUMP_requested_size;
	uint32_t MDUMP_start_address;
	uint16_t effective_mem_length; /* includes CRC!! in Bytes */
	uint8_t memory_data[1024]; /* 16 tables x 32 DW x 2 Bytes; no CRC */
} DPU_MEMREG_DUMP_tab_t;
/**
 * \}
 */

/**
 *  \ingroup PUS_6_SUPPORT_VARIABLES
 *  \{
 */
/*
 * Tables to host the memory dump from DPU to ICU, in response to a TC(6.5)
 * One for each DPU with ASW
 */

DPU_MEMREG_DUMP_tab_t DPU_MEMREG_DUMP_tab[2];

/*
 * this information should be necessary only for Service 6
 * FIXME: Insert upper and lower memories limits!!!!
 * ICU_DPU_memories_base_addresses[ICU_DPU_MEM_NUMBER] to {x,x,x};
 * ICU_DPU_memories_end_addresses[ICU_DPU_MEM_NUMBER] to {0xFFFF,0xFFFF,0xFFFF};
 */

/**
 * \}
 */

/**
 *  \ingroup PUS_6_SUPPORT_FUNCTIONS
 *  \{
 */
/**
 * \brief Retrieve data from 1553 sendlist containing one memory dump table
 * (MEMREG_DUMP_TAB1:16)
 * \param[in] rt_addr: the RT address identifying the DPU (1 B)
 * \param[in] subaddress: the SA identifying the table (1 B)
 * \param[in] length: the number of DW in the 1553 message (1 B)
 * \param[in] stream: data from 1553 sendlist, in DW (max size 32 DW)
 * \return void
 */
void asw_DPU_MEMREG_tab_interpreter(uint8_t rt_addr, uint8_t subaddress,
		uint8_t length, const uint16_t *stream);

/**
 * \}
 */

#endif /* PUS_6_DB_DEF_H_ */
