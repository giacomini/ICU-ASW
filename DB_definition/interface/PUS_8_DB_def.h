/**
 * \file	PUS_8_DB_def.h
 *
 * \brief	PUS Service 8: auxiliary variables and functions for DPUs management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 1, 2015
 */

#ifndef PUS_8_DB_DEF_H_
#define PUS_8_DB_DEF_H_

#include "db_type_def.h"
#include "ICU_config.h"

/**
 *  \ingroup PUS_8_SUPPORT_CONSTANTS
 *  \{
 */

#define DCU_SCE_NUMBER 8U
#define MAX_EXPOSURES_IN_DITHER 7U
/* EUCL_OPD_TN_7-004 2.1 20/01/16 */
#define COMMAND_ID_OFFSET 8U
#define BSW_CMD_TYPE_OFFSET 6U
#define MAX_CMD_HISTORY_NUMBER 30U   // FIXME
#define MAX_BSW_CMD_HIST_NUMB 10U   // FIXME
/* timeouts for DPU commands */
#define CPU_MDUMP_TIMEOUT 10U
#define CPU_MLOAD_TIMEOUT 10U
/* number of DPU BSW different tables
 * 2 CMD_VER_TAB, 2 HK, 5 CHECK_REP*/

/**
 * \brief DPU software status. It is deduced from BSW_HK tables and DPU_MONITOR_TAB.
 */
enum DPU_SW_STATUS {
	CPU_OFF = 0U,
	CPU_ASW_INIT = 1U,
	CPU_SAFE = 1U << 1U,
	CPU_PARKED = 1U << 2U,
	CPU_SCE_INIT = 1U << 3U,
	CPU_OBSERVATION_WAITING = 1U << 4U,
	CPU_OBSERVATION_PROCESSING = 1U << 5U,
	CPU_MANUAL = 1U << 6U,
	CPU_BOOT = 1U << 7U,
	CPU_UNDEFINED = 0xFFU,
};

/**
 * \brief DPUs SCE global status. It is deduced from DPU_MONITOR_TAB.
 */
enum SCE_STATUS {
	SCE_IDLE = 1U,
	SCE_EXPOSING = 2U,
};


/**
 * \brief Command error codes - as read from CMD_VER_TAB
 */
enum DPU_CMD_ERROR_CODE {
	DPU_CMD_CRC_ERROR = 1U,
	DPU_CMD_MODE_ERROR,
	DPU_CMD_TYPE_ERROR,
	DPU_CMD_SERVICE_ERROR,
	DPU_CMD_COUNTER_ERROR,
	DPU_CMD_LENGTH_ERROR,
	DPU_CMD_PARAM_RANGE_ERROR,
	DPU_CMD_TYPE_RANGE_ERROR,
	CMD_STATUS_NON_FOUND,
};

/***************************************************************************************
 * TABLE IDs  - EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.4
 ***************************************************************************************/
/**
 * \enum ICU_DPU_table_ID
 * \brief DPU tables ID
 *  EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.4
 */
typedef enum {
	DPU_CMD_ID = 1U, /* from ICU to DPU */
	DITH_CONFIG_TAB_ID = 2U, /* from ICU to DPU */
	SYS_CONFIG_TAB_ID = 3U, /* from ICU to DPU */
	PROC_PARAM_TAB_ID = 4U, /* from ICU to DPU */
	DPU_STATUS_TAB_ID = 10U,
	ASW_BOOT_REP_TAB_ID = 11U,
	CMD_VER_TAB_ID = 12U,
	ASW_DEATH_REP_TAB_ID = 13U,
	ASW_ERROR_TAB_ID = 14U,
	DPU_HSK_TAB_ID = 15,
	DBB_DRB_STATUS_TAB_ID = 16U,
	DPU_MONITOR_TAB_ID = 17U,
	DCU1_HSK_TAB_ID = 20U,
	DCU2_HSK_TAB_ID = 21U,
	DCU3_HSK_TAB_ID = 22U,
	DCU4_HSK_TAB_ID = 23U,
	DCU5_HSK_TAB_ID = 24U,
	DCU6_HSK_TAB_ID = 25U,
	DCU7_HSK_TAB_ID = 26U,
	DCU8_HSK_TAB_ID = 27U,
	DCUSCE_CMD_TAB1_ID = 50U, /* from ICU to DPU */
	DCUSCE_CMD_TAB2_ID = 51U,
	DCUSCE_CMD_TAB3_ID = 52U,
	DCUSCE_CMD_TAB4_ID = 53U,
	DCUSCE_CMD_TAB5_ID = 54U,
	DCUSCE_CMD_TAB6_ID = 55U,
	DCUSCE_CMD_TAB7_ID = 56U,
	DCUSCE_CMD_TAB8_ID = 57U,
	DCUSCE_CMD_BRD_TAB_ID = 58U,
	SCE1_HSK_TAB1_ID = 65U,
	SCE1_HSK_TAB2_ID = 66U,
	SCE1_HSK_TAB3_ID = 67U,
	SCE2_HSK_TAB1_ID = 68U,
	SCE2_HSK_TAB2_ID = 69U,
	SCE2_HSK_TAB3_ID = 70U,
	SCE3_HSK_TAB1_ID = 71U,
	SCE3_HSK_TAB2_ID = 72U,
	SCE3_HSK_TAB3_ID = 73U,
	SCE4_HSK_TAB1_ID = 74U,
	SCE4_HSK_TAB2_ID = 75U,
	SCE4_HSK_TAB3_ID = 76U,
	SCE5_HSK_TAB1_ID = 77U,
	SCE5_HSK_TAB2_ID = 78U,
	SCE5_HSK_TAB3_ID = 79U,
	SCE6_HSK_TAB1_ID = 80U,
	SCE6_HSK_TAB2_ID = 81U,
	SCE6_HSK_TAB3_ID = 82U,
	SCE7_HSK_TAB1_ID = 83U,
	SCE7_HSK_TAB2_ID = 84U,
	SCE7_HSK_TAB3_ID = 85U,
	SCE8_HSK_TAB1_ID = 86U,
	SCE8_HSK_TAB2_ID = 87U,
	SCE8_HSK_TAB3_ID = 88U,
	MEMREG_DUMP_TAB1_ID = 100U,
	MEMREG_DUMP_TAB2_ID = 101U,
	MEMREG_DUMP_TAB3_ID = 102U,
	MEMREG_DUMP_TAB4_ID = 103U,
	MEMREG_DUMP_TAB5_ID = 104U,
	MEMREG_DUMP_TAB6_ID = 105U,
	MEMREG_DUMP_TAB7_ID = 106U,
	MEMREG_DUMP_TAB8_ID = 107U,
	MEMREG_DUMP_TAB9_ID = 108U,
	MEMREG_DUMP_TAB10_ID = 109U,
	MEMREG_DUMP_TAB11_ID = 110U,
	MEMREG_DUMP_TAB12_ID = 111U,
	MEMREG_DUMP_TAB13_ID = 112U,
	MEMREG_DUMP_TAB14_ID = 113U,
	MEMREG_DUMP_TAB15_ID = 114U,
	MEMREG_DUMP_TAB16_ID = 115U,
	MEMREG_LOAD_TAB1 = 116U, /* from ICU to DPU */
	MEMREG_LOAD_TAB2 = 117U, /* from ICU to DPU */
	MEMREG_LOAD_TAB3 = 118U, /* from ICU to DPU */
	MEMREG_LOAD_TAB4 = 119U, /* from ICU to DPU */
	MEMREG_LOAD_TAB5 = 120U, /* from ICU to DPU */
	MEMREG_LOAD_TAB6 = 121U, /* from ICU to DPU */
	MEMREG_LOAD_TAB7 = 122U, /* from ICU to DPU */
	MEMREG_LOAD_TAB8 = 123U, /* from ICU to DPU */
	MEMREG_LOAD_TAB9 = 124U, /* from ICU to DPU */
	MEMREG_LOAD_TAB10 = 125U, /* from ICU to DPU */
	MEMREG_LOAD_TAB11 = 126U, /* from ICU to DPU */
	MEMREG_LOAD_TAB12 = 127U, /* from ICU to DPU */
	MEMREG_LOAD_TAB13 = 128U, /* from ICU to DPU */
	MEMREG_LOAD_TAB14 = 129U, /* from ICU to DPU */
	MEMREG_LOAD_TAB15 = 130U, /* from ICU to DPU */
	MEMREG_LOAD_TAB16 = 131U, /* from ICU to DPU */
	ICU_DPU_TABLES_NUMBER = 132U,
} ICU_DPU_table_ID;

/**
 * \enum BSW_DPU_table_ID BSW_table_ID
 * \brief DPU tables ID
 */
typedef enum {
	BSW_DPU_HK_TAB = 0x1FFU, //!< BSW_DPU_HK_TABS
	BSW_CMD_VER_TAB = 0x2FFU,   //!< BSW_CMD_VER_TAB
	BSW_BIT_CHECK_TAB = 0x3FFU,   //!< BSW_BIT_CHECK_TAB
} BSW_DPU_table_ID;

/**
 * \enum BSW_DPU_table_list
 * \brief List of DPU BSW tables
 */
typedef enum {
	BSW_CPU_DRB_HK_TAB = 1U,
	BSW_DCU_HK_TAB = 2U,
	BSW_CMD_VER_TAB_1 = 3U,
	BSW_CMD_VER_TAB_2 = 4U,
	BSW_BIT_EVENT_CHECK_TAB = 5U,
	BSW_BIT_EEPROM_CHECK_TAB = 6U,
	BSW_BIT_RAM_CHECK_TAB = 7U,
	BSW_BIT_SUROM_CHECK_TAB = 8U,
	BSW_BIT_EEPROM_DATA_CHECK_TAB = 9U,
	BSW_MAX_TABLE_NUMBER = 10U,
} BSW_DPU_table_list;

/****************************************************************************************
 * COMMANDS ENUMs
 ****************************************************************************************/
/**
 * \enum CPU_COMMAND_ID
 * \brief IDs for command to DPUs
 * EUCL-OPD-TN-7-004 Commanding Table 1.2, 06/08/2015
 * EUCL-OPD-TN-7-004 Commanding Table 2.1, 20/01/2016
 * EUCL-OPD-TN-7-004 Commanding Table 2.2, 15/02/2016
 * EUCL-OPD-TN-7-004 Commanding Table 2.5, 12/01/2017
 * EUCL-OPD-TN-7-004 Commanding Table 2.5, 05/06/2017
 */

typedef enum {
	CPU_RBT_COMMAND_ID = 1U,        // DPU_RST_COMMAND_ID,
	CPU_SST_COMMAND_ID = 2U,        // DPU_STATUS_COMMAND_ID,
	CPU_STM_COMMAND_ID = 3U,       // DPU_MASTER_COMMAND_ID,
	CPU_STDM_COMMAND_ID = 4U,
	CPU_MTST_COMMAND_ID = 6U,        // DPU_WDG_COMMAND_ID,
	CPU_MBE_COMMAND_ID = 7U,      // DPU_MFILL_COMMAND_ID,
	CPU_DABT_COMMAND_ID = 10U,       // DPU_SET_DPMEM_COMMAND_ID,
	CPU_GTAB_COMMAND_ID = 11U, // DPU_MTAB_COMMAND_ID,
	CPU_WDG_COMMAND_ID = 13U,       // DPU_EXPOSE_COMMAND_ID,
	DRB_SHSK_COMMAND_ID = 16U,
//	CPU_CSPW_COMMAND_ID = 19U,
	CPU_MDUMP_COMMAND_ID = 20U,
	CPU_MLOAD_COMMAND_ID = 21U,
//	CPU_TSYN_COMMAND_ID = 23U, /* fixme TBD */
	CPU_NOP_COMMAND_ID = 24U,
	CPU_RSTC_COMMAND_ID = 25U,
	CPU_DRB_CSPW_COMMAND_ID = 27U,
} CPU_COMMAND_ID;

typedef enum {
	DBB_STS_COMMAND_ID = 5U,
	DRB_STS_COMMAND_ID = 12U,
	DBB_SCRUB_COMMAND_ID = 17U,
	DRB_EDAC_COMMAND_ID = 18U,
} CPU_INTERNAL_COMMAND_ID;

/**
 * \enum DCU_COMMAND_ID
 * \brief IDs for command to DCUs
 * EUCL-OPD-TN-7-004 Commanding Table 2.1, 20/01/2016
 * EUCL-OPD-TN-7-004 Commanding Table 2.2, 15/02/2016
 * EUCL-OPD-TN-7-004 Commanding Table 2.5, 12/01/2017
 * EUCL-OPD-TN-7-004 Commanding Table 2.5, 05/06/2017
 */
typedef enum {
	DCU_PWONFF_COMMAND_ID = 50U,
	DCU_THSET_COMMAND_ID = 51U,
	DCU_RST_COMMAND_ID = 52U,
	DCU_SRLO_COMMAND_ID = 53U,
	DCU_SHSK_COMMAND_ID = 54U,
	DCU_SYNC_COMMAND_ID = 56U,
//	DCU_CSPW_COMMAND_ID,
} DCU_COMMAND_ID;

typedef enum {
	DCU_DABT_COMMAND_ID = 58,
} DCU_INTERNAL_COMMAND_ID;
/**
 * \enum SCE_EEF_COMMAND_ID
 * \brief IDs for command to SCEs/EEFs
 * EUCL-OPD-TN-7-004 Commanding Table 1.2, 06/08/2015
 * EUCL-OPD-TN-7-004 Commanding Table 2.1, 20/01/2016
 * EUCL-OPD-TN-7-004 Commanding Table 2.2, 15/02/2016
 * EUCL-OPD-TN-7-004 Commanding Table 2.5, 05/06/2017
 */

typedef enum {
	SCE_IPCEXP_COMMAND_ID = 100U,
	SCE_EXP_COMMAND_ID = 101U,
	SCE_KTCEXP_COMMAND_ID = 102U,
	SCE_FMODE_COMMAND_ID = 104U,
	SCE_CONFIG_COMMAND_ID = 105U,
	SCE_BOOT_COMMAND_ID = 108U,
	SCE_SHSK_COMMAND_ID = 109U,
	SCE_ALIVE_COMMAND_ID = 110U,
	SCE_BTEST_COMMAND_ID = 112U,
} SCE_EEF_COMMAND_ID;

typedef enum {
	SCE_TESTEXP_COMMAND_ID = 113,
} SCE_EEF_INTERNAL_COMMAND_ID;

/**
 * \enum DPU_HIGH_LEVEL_COMMAND_ID
 * \brief High level commands IDs
 * EUCL-OPD-TN-7-004 Commanding Table 2.2, 15/02/2016
 */
typedef enum {
	DPU_HL_SEND_DITHER_TAB_CMD_ID = 2U,
	DPU_HL_SEND_SYS_CFG_TAB_CMD_ID = 3U,
	DPU_HL_SEND_PROC_PAR_TAB_CMD_ID = 4U,
} DPU_HIGH_LEVEL_COMMAND_ID;

/**
 * \enum DPU_BSW_COMMAND_ID
 * \brief DPU BSW commands IDs
 * EUC-IC-ATI-006 Issue 3, draft 7
 */
typedef enum {
	DPU_BSW_COPY_IMG_2_RAM_CMD_ID = 0U,
	DPU_BSW_JUMP_TO_ASW_CMD_ID = 1U,
	DPU_BSW_CHECK_MEM_CMD_ID = 2U,
	DPU_BSW_CHK_EEPROM_SEGM_CMD_ID = 3U,
	DPU_BSW_LOAD_MEMORY_CMD_ID = 4U,
	DPU_BSW_DUMP_MEMORY_CMD_ID = 5U,
	DPU_BSW_CPU_FDIR_CMD_ID = 6U,
	DPU_BSW_RESYNCH_CNTS_CMD_ID = 7U,
} DPU_BSW_COMMAND_ID;

/**
 * \enum COMMAND_MODE
 * \brief Command Mode, to be used in the command_tab
 * EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.4
 */
typedef enum {
	DPU_COMMAND_MODE = 0U,
	DCU_1_COMMAND_MODE = 1U,
	DCU_2_COMMAND_MODE = 2U,
	DCU_3_COMMAND_MODE = 3U,
	DCU_4_COMMAND_MODE = 4U,
	DCU_5_COMMAND_MODE = 5U,
	DCU_6_COMMAND_MODE = 6U,
	DCU_7_COMMAND_MODE = 7U,
	DCU_8_COMMAND_MODE = 8U,
	DCU_BROADCAST_COMMAND_MODE = 9U,
	SCE_1_COMMAND_MODE = 11U,
	SCE_2_COMMAND_MODE = 12U,
	SCE_3_COMMAND_MODE = 13U,
	SCE_4_COMMAND_MODE = 14U,
	SCE_5_COMMAND_MODE = 15U,
	SCE_6_COMMAND_MODE = 16U,
	SCE_7_COMMAND_MODE = 17U,
	SCE_8_COMMAND_MODE = 18U,
	SCE_BROADCAST_COMMAND_MODE = 19U,
	DPMEM_COMMAND_MODE = 20U,
	ROUTER_COMMAND_MODE = 21U,
} COMMAND_MODE;

/**
 * \}
 */

/***************************************************************************************
 * PARAMETER POSITION IN TC DATA PACKET
 * not in doxygen
 ***************************************************************************************/
/*
 * \enum dither_conf_TC_parameters
 * \brief Position of parameters of DITH_CONFIG_TAB in the TC data structure
 * EUCL-OTO-LI-7-001, v. 1.2, 22/09/2016
 */
enum dither_conf_TC_parameters {
	DITH_CONF_TC_EXP_NUMBER = 4U, /* 1 Byte */
	DITH_CONF_TC_DITHER_MAX_TIME = 5U, /* 2 Bytes */
	DITH_CONF_TC_EXP_1_FRAMES_NUMB = 7U,/* 1 Byte */
	DITH_CONF_TC_EXP_1_GROUPS_NUMB = 8U, /* 1 Byte */
	DITH_CONF_TC_EXP_1_DL_1_MSB = 9U, /* 2 Bytes */
	DITH_CONF_TC_EXP_1_DL_2_LSB = 11U, /* 1 Byte */
	DITH_CONF_TC_EXP_1_INS_MODE_MSB = 12U, /* Instrument Mode 2 Bytes */
};

/*
 * \enum proc_param_TC_parameters
 * \brief Position of parameters of PROC_PARAM_TAB in the TC data structure
 * EUCL-OTO-LI-7-001, v. 1.2, 22/09/2016
 */
enum proc_param_TC_parameters {
	PROC_PAR_TC_DPU_ID = 4U,
	PROC_PAR_TC_PROC_MODE_ID = 5U, /* 1 Byte */
	PROC_PAR_TC_CHI2_TSH_FIRST_ID = 6U,
	PROC_PAR_TC_RON_FIRST_ID = 14U,
	PROC_PAR_TC_BOX_SIZE_MSB_ID = 22U,
	PROC_PAR_TC_BOX_SIZE_LSB_ID = 23U,
	PROC_PAR_TC_BOX_START_X_MSB_ID = 24U,
	PROC_PAR_TC_BOX_START_X_LSB_ID = 25U,
	PROC_PAR_TC_BOX_START_Y_MSB_ID = 26U,
	PROC_PAR_TC_BOX_START_Y_LSB_ID = 27U,
	PROC_PAR_TC_CPR_BCK_SIZE_MSB_ID = 28U,
	PROC_PAR_TC_CPR_BCK_SIZE_LSB_ID = 29U,
	PROC_PAR_TC_CPR_FSBITS_MSB_ID = 30U,
	PROC_PAR_TC_CPR_FSBITS_LSB_ID = 31U,
	PROC_PAR_TC_CPR_FSMAX_MSB_ID = 32U,
	PROC_PAR_TC_CPR_FSMAX_LSB_ID = 33U,
	PROC_PAR_TC_CPR_PRESCALING_ID = 34U,
	PROC_PAR_TC_IMG_OFFSET_MSB_ID = 35U,
	PROC_PAR_TC_IMG_OFFSET_LSB_ID = 36U,
	PROC_PAR_TC_RAW_LINES_ID = 37U,
	PROC_PAR_TC_BYTE_NUMBER = 38U
};

/*
 * \enum sys_param_TC_parameters
 * \brief Position of parameters of SYS_PARAM_TAB in the TC data structure
 * EUCL-OTO-LI-7-001, v. 1.2, 22/09/2016
 */
enum sys_config_TC_parameters {
	SYS_PARAM_EXP_TERMIN_WD = 4U,
	SYS_PARAM_DITH_TERMIN_WD,
	SYS_PARAM_DET_TRANSM_TERMIN_WD,
	SYS_PARAM_1553_WD_ID,
	SYS_PARAM_TRP_ERROR_WD_MSB,
	SYS_PARAM_TRP_ERROR_WD_LSB,
	SYS_PARAM_DRB_EDAC_ON_OFF_MSB,
	SYS_PARAM_DRB_EDAC_ON_OFF_LSB,
	SYS_PARAM_DBB_EDAC_ON_OFF_MSB,
	SYS_PARAM_DBB_EDAC_ON_OFF_LSB,
	SYS_PARAM_DC_DC_TEMP_MSB,
	SYS_PARAM_DC_DC_TEMP_LSB,
	SYS_PARAM_DCU_TEMP_MSB,
	SYS_PARAM_DCU_TEMP_LSB,
	SYS_PARAM_DBB_TEMP_MSB,
	SYS_PARAM_DBB_TEMP_LSB,
	SYS_PARAM_CPU_TEMP_MSB,
	SYS_PARAM_CPU_TEMP_LSB,
	SYS_PARAM_TEMP_COUNTER_MSB,
	SYS_PARAM_TEMP_COUNTER_LSB,
	SYS_PARAM_TC_BYTE_NUMBER,
};

/*
 * \enum sce_IPCexp_TC_parameters
 * \brief Position of parameters of SCE_IPCEXP in the TC data structure
 * first four bytes are the ID of the TC!
 * EUCL-OTO-LI-7-001, v. 1.2, 22/09/2016
 */
enum sce_IPCexp_TC_parameters {
	SCE_IPCEXP_TC_DPU_NUMB = 4U, /* 1 Byte */
	SCE_IPCEXP_TC_DCU_NUMB = 5U, /* 1 Byte */
	SCE_IPCEXP_TC_RST_NFRAME_MSB = 6U,/* 2 Byte */
	SCE_IPCEXP_TC_DROPLINES_1_MSB = 8U, /* 2 Byte */
	SCE_IPCEXP_TC_GROUPS_NUMB = 10U, /* 1 Byte */
	SCE_IPCEXP_TC_READ_GROUP_NUMB = 11U, /* 1 Byte */
	SCE_IPCEXP_TC_DROPLINES_2_MSB = 12U, /* 2 Byte */
	SCE_IPCEXP_TC_DROPLINES_3 = 14U, /* 1 Byte */
	SCE_IPCEXP_TC_EXP_CONF_MSB = 15U, /* 2 Byte */
	SCE_IPCEXP_TC_IPC_START_Y_MSB = 17U, /* 2 Byte */
	SCE_IPCEXP_TC_IPC_STEP_Y_MSB = 19U, /* 2 Byte */
	SCE_IPCEXP_TC_IPC_START_X_MSB = 21U, /* 2 Byte */
	SCE_IPCEXP_TC_IPC_STEP_X_MSB = 23U, /* 2 Byte */
	SCE_IPCEXP_TC_VRESET_MSB = 25U, /* 2 Byte */
	SCE_IPCEXP_TC_VRESET_LSB = 26U, SCE_IPCEXP_TC_PARAM_SIZE = 27U,
};

/*
 * \enum sce_KTCexp_TC_parameters
 * \brief Position of parameters of SCE_KTCEXP in the TC data structure
 * first four bytes are the ID of the TC!
 * EUCL-OTO-LI-7-001, v. 1.2, 22/09/2016
 */
enum sce_KTCexp_TC_parameters {
	SCE_KTCEXP_TC_DPU_NUMB = 4U,
	SCE_KTCEXP_TC_DCU_NUMB = 5U,
	SCE_KTCEXP_TC_RESET_NUMB_MSB = 6U,
	SCE_KTCEXP_TC_DROPLINES_1_MSB = 8U,
	SCE_KTCEXP_TC_KTC_ITERATIONS = 10U,
	SCE_KTCEXP_TC_EXP_CONF_MSB = 11U,
	SCE_KTCEXP_TC_EXP_CONF_LSB = 12U,
	SCE_KTCEXP_TC_PARAM_SIZE = 13U,
};

/*
 * \enum sce_exp_TC_parameters
 * \brief Position of parameters of SCE_EXP in the TC data structure
 * EUCL-OTO-LI-7-001, v. 1.2, 22/09/2016
 */
enum sce_exp_TC_parameters {
	SCE_EXP_TC_DPU_NUMB = 4U, /* 1 Byte */
	SCE_EXP_TC_DCU_NUMB = 5U, /* 1 Byte */
	SCE_EXP_TC_RESET_FRAME_NUMB_MSB = 6U, /* 2 Byte */
	SCE_EXP_TC_DROPLINES_1_MSB = 8U, /* 2 Byte */
	SCE_EXP_TC_GROUPS_NUMB = 10U, /* 1 Byte */
	SCE_EXP_TC_READ_FRAMES_NUMB = 11U, /* 1 Byte */
	SCE_EXP_TC_DROPLINES_2_MSB = 12U, /* 2 Byte */
	SCE_EXP_TC_DROPLINES_3 = 14U, /* 1 Byte */
	SCE_EXP_TC_EXP_CONF_MSB = 15U, /* 2 Byte */
	SCE_EXP_TC_EXP_CONF_LSB = 16U, SCE_EXP_TC_PARAM_SIZE = 17U,
};

/**
 *  \ingroup PUS_8_SUPPORT_DPU_TO_ICU
 *  \{
 */
/**
 * NB: MEMREG_Dump_Tab(1,2,...,16) defined in PUS_6_DB_def.h
 */

/*
 * DPU to ICU
 * internally refreshed
 * ASW_Status_Tab / CMD_Ver_Tab / ASW_ERROR_Tab / DPU_SYS_TAB
 *
 * on demand
 * DPU_BOOT_REP_Tab / ASW_Death_Rep_Tab / SCE_BOOT_REP_TAB / DPU_HSK_TAB
 *
 * ***********************************************************
 * queste spostate PROBABILMENTE (!) in PUS_8_TM_HSK_def.h
 *
 * DCU#_HSK_Tab(1,2,3) / SCE#_TLM_Tab(1,2,3)
 * ***********************************************************
 */

/**
 * \struct CMD_verification_t
 * \brief Command verification table - as received from DPU
 */

typedef struct {
	uint16_t table_ID;
	uint16_t counter;
	uint32_t OBT;
	uint16_t cmd_id;
	uint16_t cmd_ICU_counter;
	uint16_t verification_code;
	uint16_t error_code_data;
} CMD_verification_t;

/**
 * \struct command_tag_t
 * \brief Structure to hold the features of a single command sent to DPU.
 * Used to record locally the command history.
 */
typedef struct {
	uint8_t useFlag; /* 0: command_tag is free; 1: command_tag not free */
	uint8_t is_cmd_verified; /* 0: cmd not verified; 1: cmd verified -- used by MLOAD/MDUMP */
	uint8_t ack_flag; /* TC ack_flag -- used to generate 1,7 packet */
	uint16_t cmd_id;
	uint16_t cmd_ICU_counter;
	uint16_t cmd_verification_code; /* from CMD_VER_TAB */
	uint16_t cmd_error_data; /* from CMD_VER_TAB */
	uint16_t TC_packet_ID;
	uint16_t TC_seq_control;
	uint8_t TC_dest_ID;
	uint16_t twin_cmd_id; /* 0: command sent to one DPU; != 0: id of the twin cmd (cmd sent to both DPUs) */
	uint16_t twin_cmd_ICU_count;
} command_tag_t;

/**
 * \struct ASW_status_t
 * \brief ASW_STATUS_TAB - as recevied from DPU.
 *  EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.x, 31/03/2017
 *  EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.5 pag. 37, 30/05/2016
 *  EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.4 pag. 30
 *  EUCL-OPD-TN-7-004 v. 2.1, 20/01/2016
 */
typedef struct {
	//	uint8_t mode;
	uint16_t table_ID;
	uint16_t counter;
	uint32_t OBT_coarse;
	uint16_t OBT_fine;
	uint16_t ASW_processing_mode;
	uint16_t LSQfit_Delta_t;
	uint16_t compression_Delta_t;
	uint16_t data_compression_factor;
	uint16_t chi2_compression_factor;
	uint16_t detector_ID;
	uint16_t process_det_pedestal;
	uint16_t SCE_data_word;
	uint16_t MSW_data_word;
	uint16_t DRB_to_MMU_SpW_Channel;
	uint16_t DBB_status_reg_1;
	uint16_t DBB_status_reg_2;
	uint16_t DRB_status_reg;
	uint16_t CPU_mode;
	uint16_t DCU_status_reg[8];
	uint16_t SCE_bootable_source;
	uint16_t DCU_SCE_active_mask;
} DPU_status_t;

/**
 * \struct DPU_sys_t
 * \brief DPU_SYS_TAB - as recevied from DPU.
 *  EUCL-OPD-TN-7-004 v. 2.1, 20/01/2016
 *  EUCL-OPD-TN-7-004 v. xxx, September 2016
 */
typedef struct {
	uint16_t table_ID;
	uint16_t counter;
	uint32_t OBT_coarse;
	uint16_t OBT_fine;
	uint8_t SCE_boot_status_reg;
	uint8_t DCU_SCE_onoff_status_reg;
	uint16_t DCU_SCE_supply_alarm_reg;
	uint16_t last_dispatched_ICU_cmd_cnt;
	uint16_t rejected_cmd_counter_LSW;
	uint16_t rejected_cmd_counter_MSW;
	uint16_t error_counter_LSW;
	uint16_t error_counter_MSW;
	uint16_t ASW_status;
	uint16_t dither_status;
	uint16_t exposure_counter;
	uint16_t last_det_ID_transm_MMU;
	uint16_t DPU_load;
} DPU_monit_t;

/*
 * DCU#_HSK_TAB: subaddress 11, HSK mode: TBD, table_ID: 21-28 one each DCU
 */

/**
 * \}
 */

/**
 *  \ingroup PUS_8_SUPPORT_ICU_TO_DPU
 *  \{
 */
/**
 * NB: MEMREG_Load_Tab(1,..,16) defined in PUS_6_DB_def.h
 */

/**
 * \struct command_packet_t
 * \brief Command packet tab - ICU to DPU
 */

typedef struct {
	uint8_t command_dest_table_ID;
	uint8_t command_ID;
	uint16_t ICU_request_counter;
	uint16_t parameters[30];
} command_packet_t;

/**
 * \struct exp_param_t
 * \brief Description of a single exposure in the DITH_CONFIG_TAB
 */
typedef struct {
	uint8_t group_number;
	uint8_t framesPerGroup;
	uint16_t dropLines1;
	uint16_t dropLines2;
	uint16_t instrument_mode;
} exp_param_t;

/**
 * \struct dither_config_table_t
 * \brief The DITH_CONFIG_TAB - as sent to DPU from ICU
 * EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.4 pag. 51
 * EUCL-OPD-TN-7-004 Commanding Table 2.1, 20/01/2016
 * EUCL-OPD-TN-7-004 Commanding Table xxxx, 09/09/2016 - DPU simulator version
 */
typedef struct {
	uint8_t table_ID;
	uint8_t command_ID;
	uint16_t counter;
	uint8_t number_of_exposures;
	uint8_t DPMEM;
	uint16_t dither_max_time;
	exp_param_t exposure_param[MAX_EXPOSURES_IN_DITHER];
} dither_config_table_t;

/**
 * \struct proc_param_table_t
 * \brief The PROC_PARAM_TAB - as sent to DPU from ICU
 * EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.4 pag. 51
 * EUCL-OPD-TN-7-004 Commanding Table 2.1, 20/01/2016
 * EUCL-OPD-TN-7-004 Commanding Table xxxx, 09/09/2016 - DPU simulator version
 */
typedef struct {
	uint8_t command_ID;
	uint8_t table_ID;
	uint16_t counter;
	uint16_t processing_mode;
	uint8_t Chi2_pixel_threshold[8];
	uint8_t RON[8];
	uint16_t final_frame_stat_box_size;
	uint16_t final_frame_stat_box_xStart;
	uint16_t final_frame_stat_box_yStart;
	uint16_t compression_block_size;
	uint16_t compression_FSbits;
	uint16_t compression_FSmax;
	uint16_t compression_preScaling_factor;
	int16_t det_chain_final_image_offset; //[8];
	uint8_t raw_lines_number;
} proc_param_table_t;

/**
 * \struct sys_config_table_t
 * \brief The SYS_CONFIG_TAB - as sent to DPU from ICU
 * EUCL-OPD-ICD-7-003 NI-DPU ASW ICD V2.4 pag. 51
 * EUCL-OPD-TN-7-004 Commanding Table 2.1, 20/01/2016
 */
typedef struct {
	uint8_t command_ID;
	uint8_t table_ID;
	uint16_t counter;
	int8_t exp_WD_time;
	int8_t dither_WD_time;
	int8_t det_transm_WD_time;
	int8_t bus1553_WD_time;
	uint16_t TRP_error_WD_time;
	uint16_t DRB_EDAC_ON_OFF;
	uint16_t DBB_EDAC_ON_OFF;
	uint16_t DC_DC_temp_thresh;
	uint16_t DCU_temp_thresh;
	uint16_t DBB_temp_thresh;
	uint16_t CPU_temp_thresh;
	uint16_t temp_thresh_cnt;
} sys_config_table_t;

/**
 * \}
 */

/**
 *  \ingroup PUS_8_SUPPORT_VARIABLES
 *  \{
 */
extern uint8_t DPU_in_use_DPMEM;
extern uint16_t ICU_command_counter;
/* asynchronous tabs */
extern CMD_verification_t CMD_ver_tab[2];
extern uint16_t tab_counter[2][ICU_DPU_TABLES_NUMBER];
/* DPU BSW asynchronous tabs */
//extern CMD_verification_t BSW_CMD_ver_tab[2];
extern uint16_t bsw_tab_counter[2][BSW_MAX_TABLE_NUMBER - 1];

/* configuration tables */
extern proc_param_table_t proc_param_tab[2];
extern dither_config_table_t dith_config_tab;
extern sys_config_table_t sys_config_tab;

/**
 * \brief arrays of structures for the commands history - one for each DPU
 */
extern command_tag_t DPU_cmd_history[2][MAX_CMD_HISTORY_NUMBER];
extern command_tag_t DPU_BSW_cmd_hist[2][MAX_BSW_CMD_HIST_NUMB];

/**
 * \}
 */

/**
 *  \ingroup PUS_8_SUPPORT_FUNCTIONS
 *  \{
 */
/*
 * ********************************************************************************************
 * Functions for the handling of tables asynchronously refreshed by DPU:
 * ASW_Status_Tab / CMD_Ver_Tab / ASW_ERROR_Tab / DPU_HSK_TAB (?)
 * ********************************************************************************************
 */

/**
 * \brief This function interprets the ASW_STATUS_TAB received from DPU through the 1553 channel.
 * It fills the ASW_status_tab structure and writes data in the ICU-ASW database.
 * \param[in] stream: a Byte stream coming from DPU RT SA 18
 * \param[in] rt_address: DPU Remote Terminal address
 * \return none
 */
void asw_DPU_status_tab_interpreter(const uint16_t *stream,
		uint16_t rt_address);

/**
 * \brief This function interprets the DPU_SYS_TAB received from DPU through the 1553 channel.
 * It fills the dpu_sys_tab structure and writes data in the ICU-ASW database.
 * \param[in] stream: a Byte stream coming from DPU RT SA 21
 * \param[in] rt_addr: DPU Remote Terminal address
 * \return none
 */
void asw_DPU_monit_tab_interpreter(const uint16_t *stream, uint16_t rt_addr);

/**
 * \brief This function interprets the ASW_ERROR_TAB received from DPU through the 1553 channel.
 * \param[in] stream: a Byte stream coming from DPU RT SA 21
 * \param[in] rt_addr: DPU Remote Terminal address
 * \return none
 */
void asw_DPU_error_tab_interpreter(const uint16_t *stream, uint16_t rt_addr);

/**
 * \brief This function interprets the DBB_DRB_STATUS_TAB received from DPU through the 1553 channel.
 * It writes data in the ICU-ASW database.
 * \param[in] stream: a Byte stream coming from DPU RT SA 22
 * \param[in] rt_address: DPU Remote Terminal address
 * \return status
 */
void asw_DBB_DRB_tab_interpreter(const uint16_t *stream,
		uint16_t rt_address);

/**
 * \brief This function interprets the CMD_VER_TAB received from DPU through the 1553 channel.
 * It fills the cmd_ver_tab structure and search the command in the local command history.
 * If the command has finished the execution, the TM(1,7)/(1,8) is prepared for transmission to S/C.
 * If the generating TC was sent to both DPUs, the TM(1,7)/(1,8) are prepared when both commands have
 * been checked for execution.
 * Exception: MDUMP does not produce the PUS 1 telemetry, but awakes the task waiting for collecting
 * memory data.
 * \param[in] rt_addr DPU Remote Terminal address
 * \param[in] stream a Byte stream coming from DPU RT SA 19
 * \return status
 */
icu_status_code asw_CMD_ver_table_interpreter(uint8_t rt_addr,
		const uint16_t *stream);

/**
 * \brief This function interprets the CMD_VER_TABs (1/2) received from the BSW of the DPUs.
 * It searches the command in the local command history.
 * If the command has not been verified (CMD_VER_TAB_1) the final TM(1,8) is prepared for transmission to S/C.
 * If the command has finished the execution (CMD_VER_TAB_2), the final TM(1,7)/(1,8) are prepared.
 * If the generating TC was sent to both DPUs, the TM(1,7)/(1,8) are prepared when both commands have
 * been checked for execution.
 * Exception: MDUMP does not produce the PUS 1 telemetry, but awakes the task waiting for collecting
 * memory data.
 * Exception: Jump to ASW command produces just a TM(1,8) in case it has not been verified.
 * \param[in] rt_addr DPU Remote Terminal address
 * \param[in] stream a Byte stream coming from DPU RT SA 17/19
 * \return status
 */
icu_status_code asw_BSW_CMD_ver_tab_interpreter(uint8_t rt_addr,
		const uint16_t *stream);


/*******************************************************************************
 * Functions managing the check of the status of commands sent to DPU:
 * icu_status_code asw_push_cmd_tag(uint16_t cmd_ID, uint16_t counter)
 * int32_t check_cmd_status(uint16_t cmd_ID, uint16_t counter)
 * *****************************************************************************/

/**
 * \brief This function pushes information about the current processed DPU command into
 * the command history array.
 * \param[in] acknowledge_flag: as from the generating S/C TC
 * \param[in] TC_pkt_ID (2 B): as from the generating S/C TC
 * \param[in] TC_seq_cntr (2 B): as from the generating S/C TC
 * \param[in] dest_ID (1 B): the TC source ID
 * \param[in] rt_addr (1 B): the RT address
 * \param[in] cmd_ID (2 B): the command ID for DPU ASW
 * \param[in] cmd_counter (2 B): command counter managed by ICU
 * \param[in] twin_command_id (2 B): the command ID for DPU ASW for the twin command, if it applies
 * \param[in] twin_ICU_cnt (2 B): ICU command counter for the twin command, if it applies
 * \param[out] cmd_idx: pointer to a Byte containing the position of the associated command_tag
 * in the command history array
 * \return status
 */
icu_status_code asw_push_cmd_tag(uint8_t acknowledge_flag, uint16_t TC_pkt_ID,
		uint16_t TC_seq_cntr, uint8_t dest_ID, uint8_t rt_addr, uint16_t cmd_ID,
		uint16_t cmd_counter, uint16_t twin_command_id, uint16_t twin_ICU_cnt,
		uint8_t* cmd_idx);

/**
 * \brief This function manages the final TM(1,7/8) in case of TCs that have been split into twin commands for
 * the DPUs. The final TM is produced when both commands have been verified.
 * \param[in] tab1 (1 B): index to identify the command history of first command
 * \param[in] cmd_idx (2 B): the index in the cmd history array of first command
 * \param[in] tab2 (1 B): index to identify the command history of twin command
 * \param[in] twin_cmd_idx (2 B): the index in the cmd history array of second command
 * \return status
 */
icu_status_code asw_DPU_twin_cmd_final_TM_1(uint8_t tab1, uint16_t cmd_idx,
		uint8_t tab2, uint16_t twin_cmd_idx);

/**
 * \brief This function provides the final TM(1,7/8) in case of TC that required a single DPU command.
 * \param[in] tab (1 B): index to identify the command history
 * \param[in] cmd_idx (2 B): the index in the cmd history array of the command
 * \return status
 */
icu_status_code asw_DPU_single_cmd_final_TM_1(uint8_t tab, uint16_t cmd_idx);

/**
 * \brief This function pushes information about the current processed DPU command into
 * the command history array.
 * \param[in] acknowledge_flag: as from the generating S/C TC
 * \param[in] TC_pkt_ID (2 B): as from the generating S/C TC
 * \param[in] TC_seq_cntr (2 B): as from the generating S/C TC
 * \param[in] dest_ID (1 B): the TC source ID
 * \param[in] rt_addr (1 B): the RT address
 * \param[in] cmd_ID (2 B): the command ID for DPU ASW
 * \param[in] cmd_counter (2 B): command counter managed by ICU
 * \param[out] cmd_idx: pointer to a Byte containing the position of the associated command_tag
 * in the command history array
 * \return status
 */
icu_status_code asw_push_bsw_cmd_tag(uint8_t acknowledge_flag, uint16_t TC_pkt_ID,
		uint16_t TC_seq_cntr, uint8_t dest_ID, uint8_t rt_addr, uint16_t cmd_ID,
		uint16_t cmd_counter, uint8_t* cmd_idx);
/**
 * \}
 */

#endif /* PUS_8_DB_DEF_H_ */
