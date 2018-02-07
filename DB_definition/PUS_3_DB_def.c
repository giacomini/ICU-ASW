/**
 * \file	PUS_3_DB_def.h
 *
 * \brief	Definition of PUS 3 ancillary variables and functions.
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Apr 27, 2015
 */

#include "interface/PUS_3_DB_def.h"
#include "interface/PUS_8_DB_def.h"
#include "interface/db_type_def.h"
#include <public/basic_types.h>

HK_SID_t SID[APID_NUMBER - ICU_ASW_APID][TM_MAX_REPORT_NUMBER];
DIAG_SID_t diag_SID[APID_NUMBER - ICU_ASW_APID][TM_DIAG_MAX_REPORT_NUMBER];

/* all report are disabled*/
uint32_t TM_report_flag[APID_NUMBER - ICU_ASW_APID];
uint32_t TM_report_exists_flag[APID_NUMBER - ICU_ASW_APID];
uint32_t TM_DIAG_report_flag[APID_NUMBER - ICU_ASW_APID];
uint32_t TM_DIAG_report_exists_flag[APID_NUMBER - ICU_ASW_APID];
/* counter for the actual number of TM report for each report - hardcoded and on demand */
uint8_t TM_report_number[APID_NUMBER - ICU_ASW_APID];
uint8_t TM_DIAG_report_number[APID_NUMBER - ICU_ASW_APID];

uint8_t
		diag_sampling_space[TM_DIAG_MAX_SIMULT_REPORT_NUMB][TM_DIAG_STORED_DATA_HEAD_OFFSET
				+ TM_3_MAX_DATA_LENGTH_BYTE];
uint8_t diag_data_sampling_status = 0;
uint8_t diag_data_3_26_packet_status = 0;

void asw_PUS_3_initialize_HK_reports(void) {
	TM_report_exists_flag[0] = 0;
	TM_report_flag[0] = 0;
	/*
	 * SID 1: ICU HK + DPU status tabs
	 */
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_number = 1;
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].collection_interval = 2;
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].generation_cnt
			= SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].collection_interval;
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].param_number = DB_MONITORED_PARAM_NUMB
			+ DB_NOT_MONITORED_ICU_PARAM_NUMB
			+ (2 * (DB_ASW_DPU_DPU_STATUS_TAB_PARAM
					+ DB_ASW_DPU_DPU_MONIT_TAB_PARAM));

	asw_PUS_3_initialize_HK_SID_1();
	TM_report_exists_flag[0] = TM_report_exists_flag[0] | 1U;

	/*
	 * SID 2: DPU + DCU HK
	 */
	SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_number = 2;
	SID[0][TM_HK_SID_DPU_DCU_HK_IDX].collection_interval = 15;
	SID[0][TM_HK_SID_DPU_DCU_HK_IDX].generation_cnt
			= SID[0][TM_HK_SID_DPU_DCU_HK_IDX].collection_interval;
	SID[0][TM_HK_SID_DPU_DCU_HK_IDX].param_number = 1 + (2
			* (DB_ASW_DPU_DPU_HSK_PARAM_NUMBER + (8
					* DB_ASW_DPU_DCU_PARAM_NUMBER)));

	asw_PUS_3_initialize_HK_SID_2();
	TM_report_exists_flag[0] = TM_report_exists_flag[0] | (1U << 1);

	/*
	 * SID 3: SCE HK TAB_1
	 */
	SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].SID_number = 3;
	SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].collection_interval = 60;
	SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].generation_cnt
			= SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].collection_interval;
	SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].param_number = 1 + (2 * DCU_SCE_NUMBER
			* DB_ASW_DPU_SCE_HSK1_PAR_NUM);

	asw_PUS_3_initialize_HK_SID_3();
	TM_report_exists_flag[0] = TM_report_exists_flag[0] | (1U << 2);

	/*
	 * SID 4: SCE HK TAB_2
	 */
	SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].SID_number = 4;
	SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].collection_interval = 60;
	SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].generation_cnt
			= SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].collection_interval;
	SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].param_number = 1 + (2 * DCU_SCE_NUMBER
			* DB_ASW_DPU_SCE_HSK2_PAR_NUM);

	asw_PUS_3_initialize_HK_SID_4();
	TM_report_exists_flag[0] = TM_report_exists_flag[0] | (1U << 3);

	/*
	 * SID 5: SCE HK TAB_3
	 */
	SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].SID_number = 5;
	SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].collection_interval = 60;
	SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].generation_cnt
			= SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].collection_interval;
	SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].param_number = 1 + (2 * DCU_SCE_NUMBER
			* DB_ASW_DPU_SCE_HSK3_PAR_NUM);

	asw_PUS_3_initialize_HK_SID_5();
	TM_report_exists_flag[0] = TM_report_exists_flag[0] | (1U << 4);

	/*
	 * SID 6: DBB_DRB_STATUS_TAB
	 */
	SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].SID_number = 6;
	SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].collection_interval = 60;
	SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].generation_cnt
			= SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].collection_interval;
	SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].param_number = 1 + (2
			* DB_ASW_DPU_DBB_DRB_STAT_TAB_PAR);

	asw_PUS_3_initialize_HK_SID_6();
	TM_report_exists_flag[0] = TM_report_exists_flag[0] | (1U << 5U);

	/*
	 * SID 7: DPU BSW HK tables
	 */
	SID[0][TM_HK_SID_DPU_BSW_IDX].SID_number = 7;
	SID[0][TM_HK_SID_DPU_BSW_IDX].collection_interval = 15;
	SID[0][TM_HK_SID_DPU_BSW_IDX].generation_cnt
			= SID[0][TM_HK_SID_DPU_BSW_IDX].collection_interval;
	SID[0][TM_HK_SID_DPU_BSW_IDX].param_number = 1 + (2
			* DB_NOT_MONIT_BSW_DPU_PARAM_NUMB);

	asw_PUS_3_initialize_HK_SID_7();
	TM_report_exists_flag[0] = TM_report_exists_flag[0] | (1U << 6U);

	/*
	 * Update counters
	 */
	TM_report_number[0] = (uint8_t) TM_HK_SID_NUMBER;

	return;
}

void asw_PUS_3_enable_HK_report_flag(uint16_t sid) {
	if ((((sid - 1) >= 0) && ((uint32_t) (sid - 1) < (8U
			* sizeof(TM_report_flag[0])))) == TRUE) {
		TM_report_flag[0] = TM_report_flag[0] | ((uint32_t) 1U << (sid - 1));
	}
	return;
}

void asw_PUS_3_disable_HK_rep_flag(uint16_t sid) {
	if ((((sid - 1) >= 0) && ((uint32_t) (sid - 1) < (8U
			* sizeof(TM_report_flag[0])))) == TRUE) {
		TM_report_flag[0] = TM_report_flag[0] & ~((uint32_t) 1U << (sid - 1));
	}
	return;
}

void asw_PUS_3_init_DIAG_reports(void) {
	uint16_t param_id = 0;
	uint8_t i = 0;

	/*
	 * SID 1 contains the two motor phases of FWA, each one sampled 240 times
	 * Its frequency can be multiple of 2 minutes
	 */

	diag_SID[0][TM_DIAG_SID_FWA_IDX].SID_number = 1;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].collection_interval = 120;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].generation_cnt
			= diag_SID[0][TM_DIAG_SID_FWA_IDX].collection_interval;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].param_number = 0;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].fixed_array_number = 2;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].os_param_number[0] = 1;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].os_param_number[1] = 1;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].os_repetition_number[0] = 240;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].os_repetition_number[1] = 240;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].fa_param_system = 0; /* ICU */
	diag_SID[0][TM_DIAG_SID_FWA_IDX].fa_length[0] = 480; /* Bytes */
	diag_SID[0][TM_DIAG_SID_FWA_IDX].fa_length[1] = 480; /* Bytes */

	/* FWA phase A (DAS idx = FWA_MOT_PHA_I) */
	diag_SID[0][TM_DIAG_SID_FWA_IDX].DIAG_data[0][0].DB_table = PARAM_MONITORED;
	param_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_FWA
			<< PARAM_SUB_SYSTEM_OFFSET) | ((uint16_t) PARAM_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | 1U;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].DIAG_data[0][0].DB_index
			= asw_PUS_3_ID_to_mon_DB_idx(param_id);
	/* FWA phase B (DAS idx = FWA_MOT_PHB_I) */
	diag_SID[0][TM_DIAG_SID_FWA_IDX].DIAG_data[1][0].DB_table = PARAM_MONITORED;
	param_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_FWA
			<< PARAM_SUB_SYSTEM_OFFSET) | ((uint16_t) PARAM_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | 2U;
	diag_SID[0][TM_DIAG_SID_FWA_IDX].DIAG_data[0][0].DB_index
			= asw_PUS_3_ID_to_mon_DB_idx(param_id);

	TM_DIAG_report_exists_flag[0] = TM_DIAG_report_exists_flag[0] | 1U;

	/*
	 * SID 2 contains the two motor phases of GWA, each one sampled 240 times
	 * Its frequency can be multiple of 2 minutes
	 */
	diag_SID[0][TM_DIAG_SID_GWA_IDX].SID_number = 2;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].collection_interval = 120;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].generation_cnt
			= diag_SID[0][1].collection_interval;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].param_number = 0;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].fixed_array_number = 2;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].os_param_number[0] = 1;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].os_param_number[1] = 1;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].os_repetition_number[0] = 240;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].os_repetition_number[1] = 240;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].fa_param_system = 0; /* ICU */
	diag_SID[0][TM_DIAG_SID_GWA_IDX].fa_length[0] = 480; /* Bytes */
	diag_SID[0][TM_DIAG_SID_GWA_IDX].fa_length[1] = 480; /* Bytes */

	/* GWA phase A (DAS idx = GWA_MOT_PHA_I) */
	diag_SID[0][TM_DIAG_SID_GWA_IDX].DIAG_data[0][0].DB_table = PARAM_MONITORED;
	param_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_GWA
			<< PARAM_SUB_SYSTEM_OFFSET) | ((uint16_t) PARAM_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | 1U;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].DIAG_data[0][0].DB_index
			= asw_PUS_3_ID_to_mon_DB_idx(param_id);
	/* GWA phase B (DAS idx = GWA_MOT_PHB_I) */
	diag_SID[0][TM_DIAG_SID_GWA_IDX].DIAG_data[1][0].DB_table = PARAM_MONITORED;
	param_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_GWA
			<< PARAM_SUB_SYSTEM_OFFSET) | ((uint16_t) PARAM_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | 2U;
	diag_SID[0][TM_DIAG_SID_GWA_IDX].DIAG_data[0][0].DB_index
			= asw_PUS_3_ID_to_mon_DB_idx(param_id);

	TM_DIAG_report_exists_flag[0] = TM_report_exists_flag[0] | (1U << 1);

	/*
	 * SIDs 3 to 7 contain the voltage of a selected CU (CU1 to CU5)
	 * Its frequency can be multiple of 4 minutes
	 */
	for (i = 0; i < 5; i++) {
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].SID_number = 3  + i;
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].collection_interval = 240;
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].generation_cnt
				= diag_SID[0][TM_DIAG_SID_CU1_IDX + i].collection_interval;
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].param_number = 0;
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].fixed_array_number = 1;
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].os_param_number[0] = 1;
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].os_repetition_number[0] = 480;
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].fa_param_system = 0; /* ICU */
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].fa_length[0] = 960; /* Bytes */

		/* CU1 voltage (DAS idx = NI_CU_LED_V_1) */
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].DIAG_data[0][0].DB_table
				= PARAM_MONITORED;
		param_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU
				<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_CU
				<< PARAM_SUB_SYSTEM_OFFSET) | ((uint16_t) PARAM_MONITORED
				<< PARAM_MONIT_FLAG_OFFSET) | (1 + i);
		diag_SID[0][TM_DIAG_SID_CU1_IDX + i].DIAG_data[0][0].DB_index
				= asw_PUS_3_ID_to_mon_DB_idx(param_id);
		TM_DIAG_report_exists_flag[0] = TM_report_exists_flag[0] | ((uint32_t)1U << (2 + i));
	}
	/*
	 * update the number of existing DIAG packets and declare they are created
	 */
	TM_DIAG_report_number[0] = TM_DIAG_SID_NUMBER;

	return;
}

void asw_PUS_3_enable_DIAG_rep_flag(uint16_t sid) {
	if ((((sid - 1) >= 0) && ((uint32_t) (sid - 1) < (8U
			* sizeof(TM_report_flag[0])))) == TRUE) {
		TM_DIAG_report_flag[0] = TM_DIAG_report_flag[0] | ((uint32_t) 1U
				<< (sid - 1));
	}
	return;
}

void asw_PUS_3_disable_DIAG_rep_flag(uint16_t sid) {
	if ((((sid - 1) >= 0) && ((uint32_t) (sid - 1) < (8U
			* sizeof(TM_report_flag[0])))) == TRUE) {
		TM_DIAG_report_flag[0] = TM_DIAG_report_flag[0] & ~((uint32_t) 1U
				<< (sid - 1));
	}
	return;
}

/**
 * \brief Function used to check if a DIAG packet is enabled or not.
 * \param[in] sid The SID number of the DIAG packet to be checked
 * \return status (0: disabled, 1: enabled)
 */
uint8_t asw_PUS_3_check_DIAG_rep_stat(uint16_t sid) {
	uint8_t result = 0;
	uint32_t mask = 0;
	if ((((sid - 1) >= 0) && ((uint32_t) (sid - 1) < (8U
			* sizeof(TM_report_flag[0])))) == TRUE) {
		mask = (uint32_t) 1U << (sid - 1);
	}
	if ((TM_DIAG_report_flag[0] & mask) > 0) {
		result = 1;
	}
	return (result);
}

uint16_t asw_PUS_3_ID_to_not_mon_DB_idx(uint16_t param_ID) {
	uint16_t idx = 0, subsys_tmp = 0, sys_tmp = 0, tmp = 0;
	uint16_t asw_mask = 1;
	uint16_t sys_mask = 3;
	uint16_t subsys_mask = 31;
	uint16_t param_mask = 127;

	tmp = param_ID >> PARAM_SW_OFFSET;
	tmp &= asw_mask;
	sys_tmp = param_ID >> PARAM_SYSTEM_OFFSET;
	sys_tmp = sys_tmp & sys_mask;
	subsys_tmp = param_ID >> PARAM_SUB_SYSTEM_OFFSET;
	subsys_tmp &= subsys_mask;

	if (tmp == SW_ASW) {
		switch (sys_tmp) {
		case (SYS_ICU):
			switch (subsys_tmp) {
			case (SUBSYS_ICU):
				idx = 0;
				break;
			case (SUBSYS_GWA):
				idx = DB_PARAM_IDX_ASW_ICU_GWA_OFFSET;
				break;
			case (SUBSYS_FWA):
				idx = DB_PARAM_IDX_ASW_ICU_FWA_OFFSET;
				break;
			case (SUBSYS_CU):
				idx = DB_PARAM_IDX_ASW_ICU_CU_OFFSET;
				break;
			case (SUBSYS_TC):
				idx = DB_PARAM_IDX_ASW_ICU_TC_OFFSET;
				break;
			default:
				/*
				 * FIXME
				 */
				break;
			}

			break;
			//		} /* end case (SYS_ICU) */
		case (SYS_DPU1): {
			switch (subsys_tmp) {
			case (SUBSYS_DPU_STATUS):
				idx = DB_PARAM_IDX_ASW_DPU1_OFFSET;
				break;
			case (SUBSYS_DPU_HK):
				idx = DB_PAR_IDX_ASW_DPU1_DPU_OFFSET;
				break;
			case (SUBSYS_DCU1):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET;
				break;
			case (SUBSYS_DCU2):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET
						+ DB_ASW_DPU_DCU_PARAM_NUMBER;
				break;
			case (SUBSYS_DCU3):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET + (2
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU4):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET + (3
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU5):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET + (4
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU6):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET + (5
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU7):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET + (6
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU8):
				idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET + (7
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE1):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET;
				break;
			case (SUBSYS_SCE2):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET
						+ DB_ASW_DPU_SCE_PARAM_NUMBER;
				break;
			case (SUBSYS_SCE3):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (2
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE4):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (3
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE5):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (4
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE6):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (5
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE7):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (6
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE8):
				idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (7
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			default:
				/*
				 * error: par_ID not correct
				 */
				break;
			} /* end switch (tmp) */

			break;
		} /* end case (SYS_DPU1) */
		case (SYS_DPU2): {
			switch (subsys_tmp) {
			case (SUBSYS_DPU_STATUS):
				idx = DB_PAR_IDX_ASW_DPU2_OFFSET;
				break;
			case (SUBSYS_DPU_HK):
				idx = DB_PAR_IDX_ASW_DPU2_DPU_OFFSET;
				break;
			case (SUBSYS_DCU1):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET;
				break;
			case (SUBSYS_DCU2):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET
						+ DB_ASW_DPU_DCU_PARAM_NUMBER;
				break;
			case (SUBSYS_DCU3):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET + (2
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU4):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET + (3
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU5):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET + (4
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU6):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET + (5
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU7):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET + (6
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_DCU8):
				idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET + (7
						* DB_ASW_DPU_DCU_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE1):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET;
				break;
			case (SUBSYS_SCE2):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET
						+ DB_ASW_DPU_SCE_PARAM_NUMBER;
				break;
			case (SUBSYS_SCE3):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (2
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE4):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (3
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE5):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (4
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE6):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (5
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE7):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (6
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			case (SUBSYS_SCE8):
				idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (7
						* DB_ASW_DPU_SCE_PARAM_NUMBER);
				break;
			default:
				/*
				 * error: par_ID not correct
				 */
				break;
			} /* end switch (tmp) */

			break;
		} /* end case (SYS_DPU2) */
		default:
			/*
			 * par_ID not correct
			 */
			break;
		} /* end switch (sys_tmp) */

	} else { /* BSW - only for DPUs*/
		switch (sys_tmp) {
		case (SYS_ICU):
			/* this is an error... */
			break;
		case (SYS_DPU1):
			idx = DB_PARAM_INDEX_BSW_DPU1_OFFSET;
			break;
		case (SYS_DPU2):
			idx = DB_PARAM_INDEX_BSW_DPU2_OFFSET;
			break;
		default:
			/* par_ID not correct */
			break;
		}
	} /* end else(tmp == BSW) */

	/*
	 * parameters indexes start from 1!
	 */
	idx = idx + (uint16_t) ((param_ID & param_mask) - 1);

	return (idx);
}

uint16_t asw_PUS_3_ID_to_mon_DB_idx(uint16_t param_ID) {
	uint16_t idx = 0, subsys_tmp = 0, sys_tmp = 0, tmp = 0;
	uint16_t asw_mask = 1;
	uint16_t sys_mask = 3;
	uint16_t subsys_mask = 31;
	uint16_t param_mask = 127;

	tmp = param_ID >> PARAM_SW_OFFSET;
	tmp &= asw_mask;
	sys_tmp = param_ID >> PARAM_SYSTEM_OFFSET;
	sys_tmp = sys_tmp & sys_mask;
	subsys_tmp = param_ID >> PARAM_SUB_SYSTEM_OFFSET;
	subsys_tmp &= subsys_mask;

	if ((tmp == SW_ASW) && (sys_tmp == SYS_ICU)) {
		switch (subsys_tmp) {
		case (SUBSYS_ICU):
			idx = 0;
			break;
		case (SUBSYS_GWA):
			idx = DB_PAR_IDX_ICU_MON_GWA_OFFSET;
			break;
		case (SUBSYS_FWA):
			idx = DB_PAR_IDX_ICU_MON_FWA_OFFSET;
			break;
		case (SUBSYS_CU):
			idx = DB_PAR_IDX_ICU_MON_CU_OFFSET;
			break;
		case (SUBSYS_TC):
			idx = DB_PAR_IDX_ICU_MON_TC_OFFSET;
			break;
		default:
			/*
			 * FIXME
			 */
			break;
		}
		/*
		 * parameters indexes start from 1!
		 */
		idx = idx + (uint16_t) ((param_ID & param_mask) - 1);
	} else {
		/*
		 * FIXME
		 * ASW DPU1 and DPU2, BSW are not monitored!!
		 */
	}

	return (idx);
}

void asw_PUS_3_initialize_HK_SID_1(void) {
	uint16_t pos = 0;
	uint16_t i = 0;

	const uint16_t parID_asw_icu_icu_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET) | 0U;
	const uint16_t parID_asw_icu_gwa_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET) | 0U;
	const uint16_t parID_asw_icu_fwa_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET) | 0U;
	const uint16_t parID_asw_icu_cu_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_CU << PARAM_SUB_SYSTEM_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET) | 0U;
	const uint16_t parID_asw_icu_tc_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_TC << PARAM_SUB_SYSTEM_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET) | 0U;

	const uint16_t parID_asw_icu_icu_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) PARAM_NOT_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_ICU
			<< PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_gwa_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) PARAM_NOT_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_GWA
			<< PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_fwa_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) PARAM_NOT_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_FWA
			<< PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_cu_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) PARAM_NOT_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_CU
			<< PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_tc_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) PARAM_NOT_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | ((uint16_t) SYS_ICU
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_TC
			<< PARAM_SUB_SYSTEM_OFFSET) | 0U;

	const uint16_t parID_asw_dpu1_status_id = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) PARAM_NOT_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | ((uint16_t) SYS_DPU1
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_DPU_STATUS
			<< PARAM_SUB_SYSTEM_OFFSET) | 0U;

	const uint16_t parID_asw_dpu2_status_id = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET) | ((uint16_t) PARAM_NOT_MONITORED
			<< PARAM_MONIT_FLAG_OFFSET) | ((uint16_t) SYS_DPU2
			<< PARAM_SYSTEM_OFFSET) | ((uint16_t) SUBSYS_DPU_STATUS
			<< PARAM_SUB_SYSTEM_OFFSET) | 0U;

	/* planning ID */
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[0].DB_table
			= (uint16_t) PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[0].DB_index = 0;

	/*
	 * ICU parameters
	 * GWA
	 */
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[1].DB_table
			= (uint16_t) PARAM_MONITORED;
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[1].DB_index
			= asw_PUS_3_ID_to_mon_DB_idx(parID_asw_icu_gwa_monit
					| (uint16_t) 1U);
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[2].DB_table
			= (uint16_t) PARAM_MONITORED;
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[2].DB_index
			= asw_PUS_3_ID_to_mon_DB_idx(parID_asw_icu_gwa_monit
					| (uint16_t) 2U);

	pos = 3;

	for (i = 0; i < DB_ASW_ICU_GWA_NOT_MON_PAR_NUM; i++) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
				= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_icu_gwa_not_monit
						| (uint16_t) (i + 1));
	}
	pos += DB_ASW_ICU_GWA_NOT_MON_PAR_NUM;

	/*
	 * FWA
	 */
	if ((pos + (DB_ASW_ICU_FWA_MON_PARAM_NUMBER - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos].DB_table
				= (uint16_t) PARAM_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos].DB_index
				= asw_PUS_3_ID_to_mon_DB_idx(parID_asw_icu_fwa_monit
						| (uint16_t) 1U);
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + 1].DB_table
				= PARAM_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + 1].DB_index
				= asw_PUS_3_ID_to_mon_DB_idx(parID_asw_icu_fwa_monit
						| (uint16_t) 2U);

		pos += DB_ASW_ICU_FWA_MON_PARAM_NUMBER;
	}

	if ((pos + (DB_ASW_ICU_FWA_NOT_MON_PAR_NUM - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (i = 0; i < DB_ASW_ICU_FWA_NOT_MON_PAR_NUM; i++) {
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
					= asw_PUS_3_ID_to_not_mon_DB_idx(
							parID_asw_icu_fwa_not_monit | (uint16_t) (i + 1));
		}

		pos += DB_ASW_ICU_FWA_NOT_MON_PAR_NUM;

	}

	/*
	 * CU
	 */
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos].DB_table
			= (uint16_t) PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos].DB_index
			= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_icu_cu_not_monit
					| (uint16_t) 1U);

	pos++;

	if ((pos + (DB_ASW_ICU_CU_MON_PARAM_NUMBER - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (i = 0; i < DB_ASW_ICU_CU_MON_PARAM_NUMBER; i++) {
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
					= (uint16_t) PARAM_MONITORED;
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
					= asw_PUS_3_ID_to_mon_DB_idx(parID_asw_icu_cu_monit
							| (uint16_t) (i + 1));
		}

		pos += DB_ASW_ICU_CU_MON_PARAM_NUMBER;
	}

	/*
	 * TC
	 */
	if ((pos + (DB_ASW_ICU_TC_MON_PARAM_NUMBER - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (i = 0; i < DB_ASW_ICU_TC_MON_PARAM_NUMBER; i++) {
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
					= (uint16_t) PARAM_MONITORED;
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
					= asw_PUS_3_ID_to_mon_DB_idx(parID_asw_icu_tc_monit
							| (uint16_t) (i + 1));
		}
		pos += DB_ASW_ICU_TC_MON_PARAM_NUMBER;
	}

	if ((pos + (DB_ASW_ICU_TC_NOT_MON_PAR_NUM - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (i = 0; i < DB_ASW_ICU_TC_NOT_MON_PAR_NUM; i++) {
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
					= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_icu_tc_not_monit
							| (uint16_t) (i + 1));
		}
		pos += DB_ASW_ICU_TC_NOT_MON_PAR_NUM;
	}
	/*
	 * ICU generic parameters
	 * Exclude Planning ID parameter
	 */
	for (i = 1; i < DB_ASW_ICU_ICU_NOT_MON_PAR_NUM; i++) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[(pos + i) - 1].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[(pos + i) - 1].DB_index
				= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_icu_icu_not_monit
						| (uint16_t) (i + 1));
	}
	pos = pos + (DB_ASW_ICU_ICU_NOT_MON_PAR_NUM - 1);

	for (i = 0; i < DB_ASW_ICU_ICU_MON_PARAM_NUMBER; i++) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
				= (uint16_t) PARAM_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
				= asw_PUS_3_ID_to_mon_DB_idx(parID_asw_icu_icu_monit
						| (uint16_t) (i + 1));
	}
	pos += DB_ASW_ICU_ICU_MON_PARAM_NUMBER;

	/*
	 * DPU1 status tab
	 */
	/* DPU1 status tab */
	for (i = 0; i < DB_ASW_DPU_DPU_STATUS_TAB_PARAM; i++) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
				= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_dpu1_status_id
						| (uint16_t) (i + 1));
	}
	pos += DB_ASW_DPU_DPU_STATUS_TAB_PARAM;

	/* DPU1 monitor tab */
	for (i = 0; i < DB_ASW_DPU_DPU_MONIT_TAB_PARAM; i++) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
				= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_dpu1_status_id
						| (uint16_t) (DB_ASW_DPU_DPU_STATUS_TAB_PARAM + i + 1));
	}
	pos += DB_ASW_DPU_DPU_MONIT_TAB_PARAM;

	/*
	 * DPU2 status tab
	 */
	/* DPU2 status tab */
	for (i = 0; i < DB_ASW_DPU_DPU_STATUS_TAB_PARAM; i++) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
				= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_dpu2_status_id
						| (uint16_t) (i + 1));
	}
	pos += DB_ASW_DPU_DPU_STATUS_TAB_PARAM;

	/* DPU2 monitor tab */
	for (i = 0; i < DB_ASW_DPU_DPU_MONIT_TAB_PARAM; i++) {
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_ICU_DPU_STATUS_IDX].SID_data[pos + i].DB_index
				= asw_PUS_3_ID_to_not_mon_DB_idx(parID_asw_dpu2_status_id
						| (uint16_t) (DB_ASW_DPU_DPU_STATUS_TAB_PARAM + i + 1));
	}

	return;
}

void asw_PUS_3_initialize_HK_SID_2(void) {
	uint16_t pos = 0;
	uint16_t i = 0;
	uint8_t par_idx = 0;
	uint16_t start_index = 0;

	/* planning ID */
	SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[0].DB_table
			= (uint16_t) PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[0].DB_index = 0;

	/*
	 * DPUs HK
	 */
	for (i = 0; i < DB_ASW_DPU_DPU_HSK_PARAM_NUMBER; i++) {
		SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[1 + i].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[1 + i].DB_index
				= DB_PAR_IDX_ASW_DPU1_DPU_OFFSET + i;
	}
	pos = DB_ASW_DPU_DPU_HSK_PARAM_NUMBER + 1;

	for (i = 0; i < DB_ASW_DPU_DPU_HSK_PARAM_NUMBER; i++) {
		SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[pos + i].DB_table
				= (uint16_t) PARAM_NOT_MONITORED;
		SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[pos + i].DB_index
				= DB_PAR_IDX_ASW_DPU2_DPU_OFFSET + i;
	}
	pos += DB_ASW_DPU_DPU_HSK_PARAM_NUMBER;

	/*
	 * DPU1 DCUs HK
	 */
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET + (i
				* DB_ASW_DPU_DCU_PARAM_NUMBER);
		for (par_idx = 0; par_idx < DB_ASW_DPU_DCU_PARAM_NUMBER; par_idx++) {
			SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[pos + (uint16_t) par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[pos + (uint16_t) par_idx].DB_index
					= start_index + (uint16_t) par_idx;
		}
		pos += DB_ASW_DPU_DCU_PARAM_NUMBER;
	}

	/*
	 * DPU2 DCUs HK
	 */
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET + (i
				* DB_ASW_DPU_DCU_PARAM_NUMBER);
		for (par_idx = 0; par_idx < DB_ASW_DPU_DCU_PARAM_NUMBER; par_idx++) {
			SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[pos + (uint16_t) par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DPU_DCU_HK_IDX].SID_data[pos + (uint16_t) par_idx].DB_index
					= start_index + (uint16_t) par_idx;
		}
		pos += DB_ASW_DPU_DCU_PARAM_NUMBER;
	}

	return;
}

void asw_PUS_3_initialize_HK_SID_3(void) {
	uint16_t pos = 0;
	uint16_t i = 0;
	uint16_t par_idx = 0;
	uint16_t start_index = 0;

	/* planning ID */
	SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].SID_data[0].DB_table
			= (uint16_t) PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].SID_data[0].DB_index = 0;

	/*
	 * DPU1 SCEs HK TAB_1
	 */
	pos = 1;
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (i
				* DB_ASW_DPU_SCE_PARAM_NUMBER);
		for (par_idx = 0; par_idx < DB_ASW_DPU_SCE_HSK1_PAR_NUM; par_idx++) {
			SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].SID_data[pos + par_idx].DB_table
					= PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_ASW_DPU_SCE_HSK1_PAR_NUM;
	}

	/*
	 * DPU2 SCEs HK TAB_1
	 */
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (i
				* DB_ASW_DPU_SCE_PARAM_NUMBER);
		for (par_idx = 0; par_idx < DB_ASW_DPU_SCE_HSK1_PAR_NUM; par_idx++) {
			SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].SID_data[pos + par_idx].DB_table
					= PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_SCE_TAB1_HK_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_ASW_DPU_SCE_HSK1_PAR_NUM;
	}

	return;
}
void asw_PUS_3_initialize_HK_SID_4(void) {
	uint16_t pos = 0;
	uint16_t i = 0;
	uint16_t par_idx = 0;
	uint16_t start_index = 0;

	/* planning ID */
	SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].SID_data[0].DB_table
			= PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].SID_data[0].DB_index = 0;

	/*
	 * DPU1 SCEs HK TAB_2
	 */
	pos = 1;
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (i
				* DB_ASW_DPU_SCE_PARAM_NUMBER) + DB_ASW_DPU_SCE_HSK1_PAR_NUM;
		for (par_idx = 0; par_idx < DB_ASW_DPU_SCE_HSK2_PAR_NUM; par_idx++) {
			SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].SID_data[pos + par_idx].DB_table
					= PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_ASW_DPU_SCE_HSK2_PAR_NUM;
	}

	/*
	 * DPU2 SCEs HK TAB_2
	 */
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (i
				* DB_ASW_DPU_SCE_PARAM_NUMBER) + DB_ASW_DPU_SCE_HSK1_PAR_NUM;
		for (par_idx = 0; par_idx < DB_ASW_DPU_SCE_HSK2_PAR_NUM; par_idx++) {
			SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].SID_data[pos + par_idx].DB_table
					= PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_SCE_TAB2_HK_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_ASW_DPU_SCE_HSK2_PAR_NUM;
	}

	return;
}
void asw_PUS_3_initialize_HK_SID_5(void) {
	uint16_t pos = 0;
	uint16_t i = 0;
	uint16_t par_idx = 0;
	uint16_t start_index = 0;

	/* planning ID */
	SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].SID_data[0].DB_table
			= (uint16_t) PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].SID_data[0].DB_index = 0;

	/*
	 * DPU1 SCEs HK TAB_3
	 */
	pos = 1;
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET + (i
				* DB_ASW_DPU_SCE_PARAM_NUMBER) + DB_ASW_DPU_SCE_HSK1_PAR_NUM
				+ DB_ASW_DPU_SCE_HSK2_PAR_NUM;
		for (par_idx = 0; par_idx < DB_ASW_DPU_SCE_HSK3_PAR_NUM; par_idx++) {
			SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_ASW_DPU_SCE_HSK3_PAR_NUM;
	}

	/*
	 * DPU2 SCEs HK TAB_3
	 */
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		start_index = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET + (i
				* DB_ASW_DPU_SCE_PARAM_NUMBER) + DB_ASW_DPU_SCE_HSK1_PAR_NUM
				+ DB_ASW_DPU_SCE_HSK2_PAR_NUM;
		for (par_idx = 0; par_idx < DB_ASW_DPU_SCE_HSK3_PAR_NUM; par_idx++) {
			SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_SCE_TAB3_HK_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_ASW_DPU_SCE_HSK3_PAR_NUM;
	}

	return;
}
void asw_PUS_3_initialize_HK_SID_6(void) {
	uint16_t pos = 0;
	uint16_t par_idx = 0;
	uint16_t start_index = 0;

	/* planning ID */
	SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].SID_data[0].DB_table
			= (uint16_t) PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].SID_data[0].DB_index = 0;

	/*
	 * DPU1 DBB_DRB_STATUS_TAB
	 */
	start_index = DB_PARAM_IDX_ASW_DPU1_OFFSET
			+ DB_ASW_DPU_DPU_STATUS_TAB_PARAM + DB_ASW_DPU_DPU_MONIT_TAB_PARAM;
	pos = 1;

	if ((pos + (DB_ASW_DPU_DBB_DRB_STAT_TAB_PAR - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (par_idx = 0; par_idx < DB_ASW_DPU_DBB_DRB_STAT_TAB_PAR; par_idx++) {
			SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_ASW_DPU_DBB_DRB_STAT_TAB_PAR;
	}

	/*
	 * DPU2 DBB_DRB_STATUS_TAB
	 */
	start_index = DB_PAR_IDX_ASW_DPU2_OFFSET + DB_ASW_DPU_DPU_STATUS_TAB_PARAM
			+ DB_ASW_DPU_DPU_MONIT_TAB_PARAM;

	if ((pos + (DB_ASW_DPU_DBB_DRB_STAT_TAB_PAR - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (par_idx = 0; par_idx < DB_ASW_DPU_DBB_DRB_STAT_TAB_PAR; par_idx++) {
			SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DBB_DRB_STATUS_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
	}
	return;
}

void asw_PUS_3_initialize_HK_SID_7(void) {
	uint16_t pos = 0;
	uint16_t par_idx = 0;
	uint16_t start_index = 0;

	/* planning ID */
	SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[0].DB_table
			= (uint16_t) PARAM_NOT_MONITORED;
	SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[0].DB_index = 0;

	/*
	 * DPU1 DBB_DRB_STATUS_TAB
	 */
	start_index = DB_PARAM_INDEX_BSW_DPU1_OFFSET;
	pos = 1;

	/* DPU1 DPU_DRB BSW HK tab */
	if ((pos + (DB_BSW_DPU_DPU_DRB_PARAM_NUMBER - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (par_idx = 0; par_idx < DB_BSW_DPU_DPU_DRB_PARAM_NUMBER; par_idx++) {
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_BSW_DPU_DPU_DRB_PARAM_NUMBER;
	}

	/* DPU2 DPU_DRB BSW HK tab */
	start_index = DB_PARAM_INDEX_BSW_DPU1_OFFSET;
	if ((pos + (DB_BSW_DPU_DPU_DRB_PARAM_NUMBER - 1))
			< TM_REPORT_MAX_PARAM_NUMBER) {
		for (par_idx = 0; par_idx < DB_BSW_DPU_DPU_DRB_PARAM_NUMBER; par_idx++) {
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_BSW_DPU_DPU_DRB_PARAM_NUMBER;
	}

	/* DPU1 BSW DCU HK tab */
	start_index = DB_PARAM_INDEX_BSW_DPU1_OFFSET
			+ DB_BSW_DPU_DPU_DRB_PARAM_NUMBER;
	if ((pos + (DB_BSW_DPU_DCU_PARAM_NUMBER - 1)) < TM_REPORT_MAX_PARAM_NUMBER) {
		for (par_idx = 0; par_idx < DB_BSW_DPU_DCU_PARAM_NUMBER; par_idx++) {
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
		pos += DB_BSW_DPU_DCU_PARAM_NUMBER;
	}

	/* DPU2 BSW DCU HK tab */
	start_index = DB_PARAM_INDEX_BSW_DPU2_OFFSET
			+ DB_BSW_DPU_DPU_DRB_PARAM_NUMBER;
	if ((pos + (DB_BSW_DPU_DCU_PARAM_NUMBER - 1)) < TM_REPORT_MAX_PARAM_NUMBER) {
		for (par_idx = 0; par_idx < DB_BSW_DPU_DCU_PARAM_NUMBER; par_idx++) {
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_table
					= (uint16_t) PARAM_NOT_MONITORED;
			SID[0][TM_HK_SID_DPU_BSW_IDX].SID_data[pos + par_idx].DB_index
					= start_index + par_idx;
		}
	}

	return;
}
uint16_t asw_bitCount(uint32_t value) {
	uint16_t counter = 0;
	while (((value > 0) && (counter < 32)) == TRUE) {
		if ((value & 1U) == 1) {
			counter++;
		}
		value = value >> 1U;
	}
	return (counter);
}
