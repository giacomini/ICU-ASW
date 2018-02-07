/**
 * \file	db_type_def.c
 *
 * \brief	Definition of ICU ASW DataBase Parameters
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Mar 31, 2015
 */

#include "../rtems_configuration.h"

/*****************************************************
 * global variables
 *****************************************************/
db_bnd_param_t monitored_db_param[DB_MONITORED_PARAM_NUMB];
db_param_t db_param[DB_NOT_MONITORED_PARAM_NUMB];
uint32_t db_long_curr_value[DB_LONG_PARAM_NUMBER][2];

void asw_initialize_DB(void) {
	/*
	 * ICU monitored parameters
	 */
	asw_initialize_ICU_monit_param();
	/*
	 * ICU not-monitored parameters
	 */
	asw_initialize_ICU_param();

	/*
	 * DPU database: status tabs, DPUs, DCUs, SCEs and SCEs boot HK tab
	 */
	asw_initialize_DPU_status_tabs(1);
	asw_initialize_DPU_status_tabs(2);
	asw_initialize_DPU_hk_tabs(1);
	asw_initialize_DPU_hk_tabs(2);
	asw_initialize_DPU_DCU_tabs(1);
	asw_initialize_DPU_DCU_tabs(2);
	asw_initialize_DPU_SCE_tabs(1);
	asw_initialize_DPU_SCE_tabs(2);

	/*
	 * DPU BSW tabs - DPU-DRB and DCU HK
	 */
	asw_initialize_DPU_BSW_tabs(1);
	asw_initialize_DPU_BSW_tabs(2);
	return;

}

void asw_initialize_ICU_monit_param(void) {
	uint16_t table_idx = 0;
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

	/***********************************************************************************
	 * monitored parameters
	 ***********************************************************************************/
	icu_sem_lock(monit_db_sem_Id, 5);
	/*
	 * ICU monitored parameters
	 */
	monitored_db_param[0].parID = parID_asw_icu_icu_monit | ((uint16_t) 1U);
	monitored_db_param[0].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[0].DAS_idx = ICU_CDPU_TMP;
	monitored_db_param[0].hk_monitor_flag = 0;
	monitored_db_param[0].current_value = 0;

	monitored_db_param[1].parID = parID_asw_icu_icu_monit | ((uint16_t) 2U);
	monitored_db_param[1].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[1].DAS_idx = ICU_LVPS_TMP;
	monitored_db_param[1].hk_monitor_flag = 0;
	monitored_db_param[1].current_value = 0;

	monitored_db_param[2].parID = parID_asw_icu_icu_monit | ((uint16_t) 3U);
	monitored_db_param[2].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[2].DAS_idx = ICU_5V;
	monitored_db_param[2].hk_monitor_flag = 0;
	monitored_db_param[2].current_value = 0;

	monitored_db_param[3].parID = parID_asw_icu_icu_monit | ((uint16_t) 4U);
	monitored_db_param[3].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[3].DAS_idx = ICU_5V_CURRENT;
	monitored_db_param[3].hk_monitor_flag = 0;
	monitored_db_param[3].current_value = 0;

	monitored_db_param[4].parID = parID_asw_icu_icu_monit | ((uint16_t) 5U);
	monitored_db_param[4].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[4].DAS_idx = ASM_TSM1_GAIN;
	monitored_db_param[4].hk_monitor_flag = 0;
	monitored_db_param[4].current_value = 0;

	monitored_db_param[5].parID = parID_asw_icu_icu_monit | ((uint16_t) 6U);
	monitored_db_param[5].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[5].DAS_idx = ASM_TSM1_OFFSET;
	monitored_db_param[5].hk_monitor_flag = 0;
	monitored_db_param[5].current_value = 0;

	monitored_db_param[6].parID = parID_asw_icu_icu_monit | ((uint16_t) 7U);
	monitored_db_param[6].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[6].DAS_idx = SSS_CHAIN_CRYO_OFFSET;
	monitored_db_param[6].hk_monitor_flag = 0;

	monitored_db_param[7].parID = parID_asw_icu_icu_monit | ((uint16_t) 8U);
	monitored_db_param[7].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[7].DAS_idx = SSS_CRYO_CHAIN_GAIN;
	monitored_db_param[7].hk_monitor_flag = 0;

	monitored_db_param[8].parID = parID_asw_icu_icu_monit | ((uint16_t) 9U);
	monitored_db_param[8].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[8].DAS_idx = CSS_CRYO_CHAIN_OFFSET;
	monitored_db_param[8].hk_monitor_flag = 0;

	monitored_db_param[9].parID = parID_asw_icu_icu_monit | ((uint16_t) 10U);
	monitored_db_param[9].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[9].DAS_idx = CSS_CRYO_CHAIN_GAIN;
	monitored_db_param[9].hk_monitor_flag = 0;

	monitored_db_param[10].parID = parID_asw_icu_icu_monit | ((uint16_t) 11U);
	monitored_db_param[10].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[10].DAS_idx = CMD_DAS_ALARMS_READ_REQ;
	monitored_db_param[10].hk_monitor_flag = 0;

	monitored_db_param[11].parID = parID_asw_icu_icu_monit | ((uint16_t) 12U);
	monitored_db_param[11].size = UINT16_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[11].DAS_idx = CMD_MOT_HS_READ_REQ;
	monitored_db_param[11].hk_monitor_flag = 0;

	monitored_db_param[12].parID = parID_asw_icu_icu_monit | ((uint16_t) 13U);
	monitored_db_param[12].size = UINT32_TYPE;
	/* FIXME: FIX upper and lower limits */
	monitored_db_param[12].DAS_idx = OBT_COARSE_HIGH;
	monitored_db_param[12].hk_monitor_flag = 0;
	monitored_db_param[12].current_value = DB_DAS_TABLE_OBT_COARSE_IDX;

	monitored_db_param[13].parID = parID_asw_icu_icu_monit | ((uint16_t) 14U);
	monitored_db_param[13].size = UINT32_TYPE;
	monitored_db_param[13].DAS_idx = OBT_FINE_HIGH;
	monitored_db_param[13].hk_monitor_flag = 0;
	monitored_db_param[13].current_value = DB_DAS_TABLE_OBT_FINE_IDX;

	/*
	 * GWA monitored parameters
	 */
	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER].parID =
			parID_asw_icu_gwa_monit | ((uint16_t) 1U);
	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER].size = UINT16_TYPE;
	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER].DAS_idx = GWA_MOT_PHA_I;
	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER].hk_monitor_flag = 0;

	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER + 1].parID =
			parID_asw_icu_gwa_monit | ((uint16_t) 2U);
	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER + 1].size = UINT16_TYPE;
	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER + 1].DAS_idx =
			GWA_MOT_PHB_I;
	monitored_db_param[DB_ASW_ICU_ICU_MON_PARAM_NUMBER + 1].hk_monitor_flag = 0;

	if (table_idx < DB_MONITORED_PARAM_NUMB) {
		/*
		 * FWA monitored parameters
		 */
		table_idx = DB_ASW_ICU_ICU_MON_PARAM_NUMBER
				+ DB_ASW_ICU_GWA_MON_PARAM_NUMBER;
		monitored_db_param[table_idx].parID = parID_asw_icu_fwa_monit
				| ((uint16_t) 1U);
		monitored_db_param[table_idx].size = UINT16_TYPE;
		monitored_db_param[table_idx].DAS_idx = FWA_MOT_PHA_I;
		monitored_db_param[table_idx].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 1].parID = parID_asw_icu_fwa_monit
				| ((uint16_t) 2U);
		monitored_db_param[table_idx + 1].size = UINT16_TYPE;
		monitored_db_param[table_idx + 1].DAS_idx = FWA_MOT_PHB_I;
		monitored_db_param[table_idx + 1].hk_monitor_flag = 0;

		/*
		 * CU monitored parameters
		 */
		table_idx += DB_ASW_ICU_GWA_MON_PARAM_NUMBER;
		monitored_db_param[table_idx].parID = parID_asw_icu_cu_monit
				| ((uint16_t) 1U);
		monitored_db_param[table_idx].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx].DAS_idx = NI_CU_LED_V_1;
		monitored_db_param[table_idx].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 1].parID = parID_asw_icu_cu_monit
				| ((uint16_t) 2U);
		monitored_db_param[table_idx + 1].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 1].DAS_idx = NI_CU_LED_V_2;
		monitored_db_param[table_idx + 1].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 2].parID = parID_asw_icu_cu_monit
				| ((uint16_t) 3U);
		monitored_db_param[table_idx + 2].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 2].DAS_idx = NI_CU_LED_V_3;
		monitored_db_param[table_idx + 2].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 3].parID = parID_asw_icu_cu_monit
				| ((uint16_t) 4U);
		monitored_db_param[table_idx + 3].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 3].DAS_idx = NI_CU_LED_V_4;
		monitored_db_param[table_idx + 3].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 4].parID = parID_asw_icu_cu_monit
				| ((uint16_t) 5U);
		monitored_db_param[table_idx + 4].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 4].DAS_idx = NI_CU_LED_V_5;
		monitored_db_param[table_idx + 4].hk_monitor_flag = 0;

		/*
		 * TC monitored parameters
		 */
		table_idx += DB_ASW_ICU_CU_MON_PARAM_NUMBER;
		monitored_db_param[table_idx].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 1U);
		monitored_db_param[table_idx].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx].DAS_idx = NI_SA_TMP_1_CRYO;
		monitored_db_param[table_idx].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 1].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 2U);
		monitored_db_param[table_idx + 1].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 1].DAS_idx = NI_SA_TMP_2_CRYO;
		monitored_db_param[table_idx + 1].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 2].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 3U);
		monitored_db_param[table_idx + 2].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 2].DAS_idx = NI_SA_TMP_3_CRYO;
		monitored_db_param[table_idx + 2].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 3].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 4U);
		monitored_db_param[table_idx + 3].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 3].DAS_idx = NI_SA_TMP_4_CRYO;
		monitored_db_param[table_idx + 3].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 4].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 5U);
		monitored_db_param[table_idx + 4].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 4].DAS_idx = NI_SA_TMP_5_CRYO;
		monitored_db_param[table_idx + 4].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 5].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 6U);
		monitored_db_param[table_idx + 5].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 5].DAS_idx = NI_SA_TMP_6_CRYO;
		monitored_db_param[table_idx + 5].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 6].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 7U);
		monitored_db_param[table_idx + 6].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 6].DAS_idx = NI_CSS_TMP_1_CRYO;
		monitored_db_param[table_idx + 6].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 7].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 8U);
		monitored_db_param[table_idx + 7].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 7].DAS_idx = NI_CSS_TMP_2_CRYO;
		monitored_db_param[table_idx + 7].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 8].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 9U);
		monitored_db_param[table_idx + 8].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 8].DAS_idx = NI_SSS_TMP_1_CRYO;
		monitored_db_param[table_idx + 8].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 9].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 10U);
		monitored_db_param[table_idx + 9].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 9].DAS_idx = NI_SSS_TMP_2_CRYO;
		monitored_db_param[table_idx + 9].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 10].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 11U);
		monitored_db_param[table_idx + 10].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 10].DAS_idx = NI_FWA_TMP_CRYO;
		monitored_db_param[table_idx + 10].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 11].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 12U);
		monitored_db_param[table_idx + 11].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 11].DAS_idx = NI_GWA_TMP_CRYO;
		monitored_db_param[table_idx + 11].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 12].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 13U);
		monitored_db_param[table_idx + 12].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 12].DAS_idx = NI_SA_TMP_1_FR;
		monitored_db_param[table_idx + 12].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 13].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 14U);
		monitored_db_param[table_idx + 13].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 13].DAS_idx = NI_SA_TMP_2_FR;
		monitored_db_param[table_idx + 13].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 14].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 15U);
		monitored_db_param[table_idx + 14].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 14].DAS_idx = NI_SA_TMP_3_FR;
		monitored_db_param[table_idx + 14].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 15].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 16U);
		monitored_db_param[table_idx + 15].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 15].DAS_idx = NI_SA_TMP_4_FR;
		monitored_db_param[table_idx + 15].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 16].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 17U);
		monitored_db_param[table_idx + 16].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 16].DAS_idx = NI_SA_TMP_5_FR;
		monitored_db_param[table_idx + 16].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 17].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 18U);
		monitored_db_param[table_idx + 17].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 17].DAS_idx = NI_SA_TMP_6_FR;
		monitored_db_param[table_idx + 17].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 18].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 19U);
		monitored_db_param[table_idx + 18].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 18].DAS_idx = NI_CSS_TMP_1_FR;
		monitored_db_param[table_idx + 18].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 19].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 20U);
		monitored_db_param[table_idx + 19].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 19].DAS_idx = NI_CSS_TMP_2_FR;
		monitored_db_param[table_idx + 19].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 20].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 21U);
		monitored_db_param[table_idx + 20].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 20].DAS_idx = NI_SSS_TMP_1_FR;
		monitored_db_param[table_idx + 20].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 21].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 22U);
		monitored_db_param[table_idx + 21].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 21].DAS_idx = NI_SSS_TMP_2_FR;
		monitored_db_param[table_idx + 21].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 22].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 23U);
		monitored_db_param[table_idx + 22].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 22].DAS_idx = NI_FWA_TMP_FR;
		monitored_db_param[table_idx + 22].hk_monitor_flag = 0;

		monitored_db_param[table_idx + 23].parID = parID_asw_icu_tc_monit
				| ((uint16_t) 24U);
		monitored_db_param[table_idx + 23].size = UINT16_TYPE;
		/* FIXME: FIX upper and lower limits */
		monitored_db_param[table_idx + 23].DAS_idx = NI_GWA_TMP_FR;
		monitored_db_param[table_idx + 23].hk_monitor_flag = 0;
	}

	icu_sem_unlock(monit_db_sem_Id);

	return;
}

void asw_initialize_ICU_param(void) {
	const uint16_t parID_asw_icu_icu_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_gwa_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_fwa_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_cu_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_CU << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_tc_not_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_TC << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	uint16_t table_idx = 0, i = 0;

	/*
	 * Not monitored parameters
	 */
	icu_sem_lock(not_mon_db_sem_Id, 5);

	/* planning ID */
	db_param[0].parID = parID_asw_icu_icu_not_monit | ((uint16_t) 1U);
	db_param[0].size = UINT32_TYPE;
	db_param[0].current_value = DB_PLANNING_ID_IDX;

	/* RT addresses and NISP operation mode */
	for (i = 1; i < 4; i++) {
		db_param[i].parID = parID_asw_icu_icu_not_monit | ((uint16_t) (i + 1));
		db_param[i].size = UINT8_TYPE;
		db_param[i].current_value = 0;
	}

	/* ICU task status */
	for (i = 0; i < 12; i++) {
		db_param[4 + i].parID = parID_asw_icu_icu_not_monit
				| ((uint16_t) (5 + i));
		db_param[4 + i].size = UINT32_TYPE;
		db_param[4 + i].current_value = DB_RT_TASK_STATUS_IDX + i;
	}

	table_idx = 16;

	if ((table_idx + (uint16_t)(DB_ASW_ICU_TC_NOT_MON_PAR_NUM + 10)) < DB_NOT_MONITORED_PARAM_NUMB) {
		db_param[table_idx].parID = parID_asw_icu_icu_not_monit
				| ((uint16_t) 17U);
		db_param[table_idx].size = UINT16_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		db_param[table_idx].parID = parID_asw_icu_icu_not_monit
				| ((uint16_t) 18U);
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		db_param[table_idx].parID = parID_asw_icu_icu_not_monit
				| ((uint16_t) 19U);
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		db_param[table_idx].parID = parID_asw_icu_icu_not_monit
				| ((uint16_t) 20U);
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		db_param[table_idx].parID = parID_asw_icu_icu_not_monit
				| ((uint16_t) 21U);
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = DB_ICU_FDIR_IDX;
		table_idx++;

		/*
		 * GWA
		 */
		db_param[table_idx].parID = parID_asw_icu_gwa_not_monit
				| ((uint16_t) 1U);
		db_param[table_idx].size = UINT16_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		db_param[table_idx].parID = parID_asw_icu_gwa_not_monit
				| ((uint16_t) 2U);
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		/*
		 * FWA
		 */
		db_param[table_idx].parID = parID_asw_icu_fwa_not_monit
				| ((uint16_t) 1U);
		db_param[table_idx].size = UINT16_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		db_param[table_idx].parID = parID_asw_icu_fwa_not_monit
				| ((uint16_t) 2U);
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		/*
		 * CU
		 */
		db_param[table_idx].parID = parID_asw_icu_cu_not_monit
				| ((uint16_t) 1U);
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		/*
		 * TC
		 */
		for (i = 0; i < DB_ASW_ICU_TC_NOT_MON_PAR_NUM; i++) {
			db_param[table_idx + i].parID = parID_asw_icu_tc_not_monit
					| ((uint16_t) (i + 1));
			db_param[table_idx + i].size = UINT16_TYPE;
			db_param[table_idx + i].current_value = 0;
		}
	}

	icu_sem_unlock(not_mon_db_sem_Id);
	return;
}

void asw_initialize_DPU_status_tabs(uint16_t DPU) {
	uint16_t i = 0, table_idx = 0;
	uint16_t sys_id = 0;
	uint16_t dpu_status_obt = 0;
	uint16_t dpu_sce_words = 0;
	uint16_t dpu_dbb_status = 0;
	uint16_t dpu_monitor_obt = 0;
	uint16_t dpu_rej_cmd = 0;
	uint16_t dpu_err_cnt = 0;
	uint16_t dcu_data_header_index = 0;
	uint16_t dbb_drb_status_obt = 0;
	uint16_t dbb_drb_scrub_double_err = 0;
	uint16_t dbb_drb_scrub_single_err = 0;
	uint16_t dbb_drb_edac_double_err = 0;
	uint16_t dbb_drb_edac_single_err = 0;

	if (DPU == 1) {
		sys_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_DPU1 << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_DPU_STATUS << PARAM_SUB_SYSTEM_OFFSET)
				| 0U;
		table_idx = DB_PARAM_IDX_ASW_DPU1_OFFSET;
		dpu_status_obt = DB_DPU1_STATUS_TAB_OBT_IDX;
		dcu_data_header_index = DB_DPU1_DCU_DATA_IDX;
		dpu_sce_words = DB_DPU1_SCE_OBTAINED_WORDS_IDX;
		dpu_dbb_status = DB_DPU1_DBB_STATUS_REG_IDX;
		dpu_monitor_obt = DB_DPU1_MONITOR_TAB_OBT_IDX;
		dpu_rej_cmd = DB_DPU1_REJECTED_CMD_IDX;
		dpu_err_cnt = DB_DPU1_ERROR_CNT_IDX;
		dbb_drb_status_obt = DB_DPU1_DBB_DRB_STATUS_OBT_IDX;
		dbb_drb_scrub_double_err = DB_DPU1_DBB_SCRUB_2_ERR_IDX;
		dbb_drb_scrub_single_err = DB_DPU1_DBB_SCRUB_1_ERR_IDX;
		dbb_drb_edac_double_err = DB_DPU1_DRB_EDAC_DOUBLE_ERR_IDX;
		dbb_drb_edac_single_err = DB_DPU1_DRB_EDAC_SINGLE_ERR_IDX;
	} else {
		sys_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) SYS_DPU2 << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_DPU_STATUS << PARAM_SUB_SYSTEM_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| 0U;
		table_idx = DB_PAR_IDX_ASW_DPU2_OFFSET;
		dpu_status_obt = DB_DPU2_STATUS_TAB_OBT_IDX;
		dcu_data_header_index = DB_DPU2_DCU_DATA_IDX;
		dpu_sce_words = DB_DPU2_SCE_OBTAINED_WORDS_IDX;
		dpu_dbb_status = DB_DPU2_DBB_STATUS_REG_IDX;
		dpu_monitor_obt = DB_DPU2_MONITOR_TAB_OBT_IDX;
		dpu_rej_cmd = DB_DPU2_REJECTED_CMD_IDX;
		dpu_err_cnt = DB_DPU2_ERROR_CNT_IDX;
		dbb_drb_status_obt = DB_DPU2_DBB_DRB_STATUS_OBT_IDX;
		dbb_drb_scrub_double_err = DB_DPU2_DBB_SCRUB_2_ERR_IDX;
		dbb_drb_scrub_single_err = DB_DPU2_DBB_SCRUB_1_ERR_IDX;
		dbb_drb_edac_double_err = DB_DPU2_DRB_EDAC_DOUBLE_ERR_IDX;
		dbb_drb_edac_single_err = DB_DPU2_DRB_EDAC_SINGLE_ERR_IDX;
	}

	icu_sem_lock(not_mon_db_sem_Id, 5);

	if (table_idx < DB_NOT_MONITORED_PARAM_NUMB) {
		/* DPU_status_tab */
		db_param[table_idx].parID = sys_id | 1U;
		db_param[table_idx].size = UINT48_TYPE;
		db_param[table_idx].current_value = dpu_status_obt;
		table_idx++;

		for (i = 0; i < 5; i++) {
			db_param[table_idx].parID = sys_id | ((uint16_t) (i + 2));
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;
		}

		/* The DCU data header */
		db_param[table_idx].parID = sys_id | 7U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dcu_data_header_index;
		table_idx++;

		/* the SCE Obtained Words */
		db_param[table_idx].parID = sys_id | 8U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_sce_words;
		table_idx++;

		db_param[table_idx].parID = sys_id | 9U;
		db_param[table_idx].size = UINT16_TYPE;
		db_param[table_idx].current_value = 0;

		/* the DBB Status Register */
		table_idx++;
		db_param[table_idx].parID = sys_id | 10U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_dbb_status;

		table_idx++;
		for (i = 0; i < 7; i++) {
			db_param[table_idx + i].parID = sys_id | ((uint16_t) (11 + i));
			db_param[table_idx + i].size = UINT16_TYPE;
			db_param[table_idx + i].current_value = 0;
		}

		/* DPU_MONITOR_TAB */
		table_idx += 7;
		db_param[table_idx].parID = sys_id | 18U;
		db_param[table_idx].size = UINT48_TYPE;
		db_param[table_idx].current_value = dpu_monitor_obt;

		table_idx++;

		db_param[table_idx].parID = sys_id | 19U;
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;

		table_idx++;

		db_param[table_idx].parID = sys_id | 20U;
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;

		table_idx++;
		db_param[table_idx].parID = sys_id | 21U;
		db_param[table_idx].size = UINT16_TYPE;
		db_param[table_idx].current_value = 0;

		table_idx++;
		db_param[table_idx].parID = sys_id | 22U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_rej_cmd;

		table_idx++;
		db_param[table_idx].parID = sys_id | 23U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_err_cnt;

		table_idx++;
		for (i = 0; i < 5; i++) {
			db_param[table_idx].parID = sys_id | ((uint16_t) (24 + i));
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;
		}

		/* DBB_DRB_STATUS_TAB */
		db_param[table_idx].parID = sys_id | 29U;
		db_param[table_idx].size = UINT48_TYPE;
		db_param[table_idx].current_value = dbb_drb_status_obt;

		table_idx++;
		db_param[table_idx].parID = sys_id | 30U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dbb_drb_scrub_double_err;

		table_idx++;
		db_param[table_idx].parID = sys_id | 31U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dbb_drb_scrub_single_err;

		table_idx++;
		db_param[table_idx].parID = sys_id | 32U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dbb_drb_edac_double_err;

		table_idx++;
		db_param[table_idx].parID = sys_id | 33U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dbb_drb_edac_single_err;

		table_idx++;
		for (i = 0; i < 8; i++) {
			db_param[table_idx].parID = sys_id | ((uint16_t) (34 + i));
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;
		}

		db_param[table_idx].parID = sys_id | 42U;
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;

		table_idx++;

		db_param[table_idx].parID = sys_id | 43U;
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
	}

	icu_sem_unlock(not_mon_db_sem_Id);
	return;
}

void asw_initialize_DPU_hk_tabs(uint16_t DPU) {
	uint16_t i = 0, table_idx = 0;
	uint16_t sys_id = 0;
	uint16_t dpu_monitor_obt = 0;

	if (DPU == 1) {
		sys_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_DPU1 << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_DPU_HK << PARAM_SUB_SYSTEM_OFFSET) | 0U;
		table_idx = DB_PAR_IDX_ASW_DPU1_DPU_OFFSET;
		dpu_monitor_obt = DB_DPU1_HSK_OBT_IDX;
	} else {
		sys_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) SYS_DPU2 << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_DPU_HK << PARAM_SUB_SYSTEM_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| 0U;
		table_idx = DB_PAR_IDX_ASW_DPU2_DPU_OFFSET;
		dpu_monitor_obt = DB_DPU2_HSK_OBT_IDX;
	}

	icu_sem_lock(not_mon_db_sem_Id, 5);

	if (table_idx < DB_NOT_MONITORED_PARAM_NUMB) {
		db_param[table_idx].parID = sys_id | 1U;
		db_param[table_idx].size = UINT48_TYPE;
		db_param[table_idx].current_value = dpu_monitor_obt;

		for (i = 1; i < DB_ASW_DPU_DPU_HSK_PARAM_NUMBER; i++) {
			db_param[table_idx + i].parID = sys_id | ((uint16_t) (i + 1));
			db_param[table_idx + i].size = UINT16_TYPE;
			db_param[table_idx + i].current_value = 0;

		}
	}

	icu_sem_unlock(not_mon_db_sem_Id);
	return;
}

void asw_initialize_DPU_DCU_tabs(uint16_t DPU) {
	uint16_t i = 0, j = 0, table_idx = 0;
	uint16_t sys_id = 0, subsys_id = 0, par_id = 0;
	uint16_t dpu_monitor_obt = 0;
	uint16_t dcu_fifo_error = 0;

	if (DPU == 1) {
		sys_id = SYS_DPU1;
		dpu_monitor_obt = DB_DPU1_DCU1_HSK_OBT_IDX;
		dcu_fifo_error = DB_DPU1_DCU1_FIFO_ERR_CNT_IDX;
	} else {
		sys_id = SYS_DPU2;
		dpu_monitor_obt = DB_DPU2_DCU1_HSK_OBT_IDX;
		dcu_fifo_error = DB_DPU2_DCU1_FIFO_ERR_CNT_IDX;
	}

	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		subsys_id = SUBSYS_DCU1 + i;
		par_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| (sys_id << PARAM_SYSTEM_OFFSET)
				| (subsys_id << PARAM_SUB_SYSTEM_OFFSET) | 0U;

		if (DPU == 1) {
			table_idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET
					+ (i * DB_ASW_DPU_DCU_PARAM_NUMBER);
		} else {
			table_idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET
					+ (i * DB_ASW_DPU_DCU_PARAM_NUMBER);
		}

		icu_sem_lock(not_mon_db_sem_Id, 5);

		if (table_idx < DB_NOT_MONITORED_PARAM_NUMB) {
			db_param[table_idx].parID = par_id | 1U;
			db_param[table_idx].size = UINT48_TYPE;
			db_param[table_idx].current_value = dpu_monitor_obt;

			table_idx++;
			db_param[table_idx].parID = par_id | 2U;
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;

			table_idx++;
			db_param[table_idx].parID = par_id | 3U;
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;

			table_idx++;
			db_param[table_idx].parID = par_id | 4U;
			db_param[table_idx].size = UINT32_TYPE;
			db_param[table_idx].current_value = dcu_fifo_error;

			table_idx++;
			db_param[table_idx].parID = par_id | 5U;
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;

			table_idx++;
			db_param[table_idx].parID = par_id | 6U;
			db_param[table_idx].size = UINT8_TYPE;
			db_param[table_idx].current_value = 0;

			table_idx++;
			db_param[table_idx].parID = par_id | 7U;
			db_param[table_idx].size = UINT8_TYPE;
			db_param[table_idx].current_value = 0;

			table_idx++;
			for (j = 0; j < (DB_ASW_DPU_DCU_PARAM_NUMBER - 7); j++) {
				db_param[table_idx + j].parID = par_id | ((uint16_t) (8 + j));
				db_param[table_idx + j].size = UINT16_TYPE;
				db_param[table_idx + j].current_value = 0;

			}
		}

		icu_sem_unlock(not_mon_db_sem_Id);

		/* for next cycle */
		dpu_monitor_obt = dpu_monitor_obt + 2;
		dcu_fifo_error = dcu_fifo_error + 2;
	}
	return;
}

void asw_initialize_DPU_SCE_tabs(uint16_t DPU) {
	uint16_t i = 0, j = 0, table_idx = 0;
	uint16_t sys_id = 0, subsys_id = 0, par_id = 0, par_num = 0;
	uint16_t dpu_monitor_obt_1 = 0, dpu_monitor_obt_2 = 0,
			dpu_monitor_obt_3 = 0;

	if (DPU == 1) {
		sys_id = SYS_DPU1;
		table_idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET;
		dpu_monitor_obt_1 = DB_DPU1_SCE1_HSK1_OBT_IDX;
		dpu_monitor_obt_2 = DB_DPU1_SCE1_HSK2_OBT_IDX;
		dpu_monitor_obt_3 = DB_DPU1_SCE1_HSK3_OBT_IDX;
	} else {
		sys_id = SYS_DPU2;
		table_idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET;
		dpu_monitor_obt_1 = DB_DPU2_SCE1_HSK1_OBT_IDX;
		dpu_monitor_obt_2 = DB_DPU2_SCE1_HSK2_OBT_IDX;
		dpu_monitor_obt_3 = DB_DPU2_SCE1_HSK3_OBT_IDX;
	}

	for (j = 0; j < DCU_SCE_NUMBER; j++) {
		subsys_id = SUBSYS_SCE1 + j;
		par_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| (sys_id << PARAM_SYSTEM_OFFSET)
				| (subsys_id << PARAM_SUB_SYSTEM_OFFSET) | 0U;

		if (DPU == 1) {
			table_idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET
					+ (j * DB_ASW_DPU_SCE_PARAM_NUMBER);
		} else {
			table_idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET
					+ (j * DB_ASW_DPU_SCE_PARAM_NUMBER);
		}

		icu_sem_lock(not_mon_db_sem_Id, 5);

		if (table_idx < DB_NOT_MONITORED_PARAM_NUMB) {

			/* SCE HSK TAB 1 */
			db_param[table_idx].parID = par_id | 1U;
			db_param[table_idx].size = UINT48_TYPE;
			db_param[table_idx].current_value = dpu_monitor_obt_1;

			table_idx++;
			for (i = 1; i < DB_ASW_DPU_SCE_HSK1_PAR_NUM; i++) {
				db_param[table_idx].parID = par_id | ((uint16_t) (1 + i));
				db_param[table_idx].size = UINT16_TYPE;
				db_param[table_idx].current_value = 0;
				table_idx++;
			}

			/* SCE HSK TAB 2 */
			par_num = DB_ASW_DPU_SCE_HSK1_PAR_NUM + 1;
			db_param[table_idx].parID = par_id | par_num;
			db_param[table_idx].size = UINT48_TYPE;
			db_param[table_idx].current_value = dpu_monitor_obt_2;

			table_idx++;
			for (i = 1; i <= 5; i++) {
				db_param[table_idx].parID = par_id | (uint16_t) (par_num + i);
				db_param[table_idx].size = UINT16_TYPE;
				db_param[table_idx].current_value = 0;
				table_idx++;
			}

			db_param[table_idx].parID = par_id | (uint16_t) (par_num + 6U);
			db_param[table_idx].size = UINT8_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;

			db_param[table_idx].parID = par_id | (uint16_t) (par_num + 7U);
			db_param[table_idx].size = UINT8_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;

			for (i = 8; i < DB_ASW_DPU_SCE_HSK2_PAR_NUM; i++) {
				db_param[table_idx].parID = par_id | (uint16_t) (par_num + i);
				db_param[table_idx].size = UINT16_TYPE;
				db_param[table_idx].current_value = 0;
				table_idx++;
			}

			/* SCE HSK TAB 3 */
			par_num = DB_ASW_DPU_SCE_HSK1_PAR_NUM
					+ DB_ASW_DPU_SCE_HSK2_PAR_NUM + 1;
			db_param[table_idx].parID = par_id | par_num;
			db_param[table_idx].size = UINT48_TYPE;
			db_param[table_idx].current_value = dpu_monitor_obt_3;

			table_idx++;
			for (i = 1; i < DB_ASW_DPU_SCE_HSK3_PAR_NUM; i++) {
				db_param[table_idx].parID = par_id | (uint16_t) (par_num + i);
				db_param[table_idx].size = UINT16_TYPE;
				db_param[table_idx].current_value = 0;
				table_idx++;
			}
		}

		icu_sem_unlock(not_mon_db_sem_Id);

		/* for next cycle */
		dpu_monitor_obt_1 = dpu_monitor_obt_1 + 1;
		dpu_monitor_obt_2 = dpu_monitor_obt_2 + 1;
		dpu_monitor_obt_3 = dpu_monitor_obt_3 + 1;
	}
	return;
}

void asw_initialize_DPU_BSW_tabs(uint16_t DPU) {
	uint16_t i = 0, table_idx = 0, par_num = 0;
	uint16_t sys_id = 0, par_id = 0;
	uint16_t dpu_drb_obt = 0;
	uint16_t dpu_drb_sw_ver = 0, dpu_drb_sc_stat = 0, dpu_drb_eeprom_stat = 0,
			dpu_drb_eeprom_err = 0;
	uint16_t dcu_obt = 0;
	uint16_t dcu_sw_vers = 0;

	if (DPU == 1) {
		table_idx = DB_PARAM_INDEX_BSW_DPU1_OFFSET;
		sys_id = SYS_DPU1;
		dpu_drb_obt = DB_DPU1_BSW_CPU_DRB_OBT_IDX;
		dpu_drb_sw_ver = DB_DPU1_BSW_CPU_DRB_SW_VERS_IDX;
		dpu_drb_sc_stat = DB_DPU1_BSW_CPU_DRB_SC_STAT_IDX;
		dpu_drb_eeprom_stat = DB_DPU1_BSW_CPU_DRB_MEMSTAT_IDX;
		dpu_drb_eeprom_err = DB_DPU1_BSW_CPU_DRB_MEMERR_IDX;
		dcu_obt = DB_DPU1_BSW_DCU_OBT_IDX;
		dcu_sw_vers = DB_DPU1_BSW_DCU_SW_VERS_IDX;
	} else {
		table_idx = DB_PARAM_INDEX_BSW_DPU2_OFFSET;
		sys_id = SYS_DPU2;
		dpu_drb_obt = DB_DPU2_BSW_CPU_DRB_OBT_IDX;
		dpu_drb_sw_ver = DB_DPU2_BSW_CPU_DRB_SW_VERS_IDX;
		dpu_drb_sc_stat = DB_DPU2_BSW_CPU_DRB_SC_STAT_IDX;
		dpu_drb_eeprom_stat = DB_DPU2_BSW_CPU_DRB_MEMSTAT_IDX;
		dpu_drb_eeprom_err = DB_DPU2_BSW_CPU_DRB_MEMERR_IDX;
		dcu_obt = DB_DPU2_BSW_DCU_OBT_IDX;
		dcu_sw_vers = DB_DPU2_BSW_DCU_SW_VERS_IDX;
	}

	par_id = ((uint16_t) SW_BSW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| (sys_id << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_BSW << PARAM_SUB_SYSTEM_OFFSET) | 0U;

	icu_sem_lock(not_mon_db_sem_Id, 5);

	if (table_idx < DB_NOT_MONITORED_PARAM_NUMB) {

		db_param[table_idx].parID = par_id | 1U;
		db_param[table_idx].size = UINT64_TYPE;
		db_param[table_idx].current_value = dpu_drb_obt;
		table_idx++;

		db_param[table_idx].parID = par_id | 2U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_drb_sw_ver;
		table_idx++;

		for (i = 0; i < 7; i++) {
			db_param[table_idx].parID = par_id | ((uint16_t) (3U + i));
			db_param[table_idx].size = UINT8_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;
		}

		db_param[table_idx].parID = par_id | 10U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_drb_sc_stat;
		table_idx++;

		db_param[table_idx].parID = par_id | 11U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_drb_eeprom_stat;
		table_idx++;

		db_param[table_idx].parID = par_id | 12U;
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dpu_drb_eeprom_err;
		table_idx++;

		for (i = 0; i < 11; i++) {
			db_param[table_idx].parID = par_id | ((uint16_t) (13U + i));
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;
		}

		par_num = DB_BSW_DPU_DPU_DRB_PARAM_NUMBER;
		db_param[table_idx].parID = par_id | ((uint16_t) (par_num + 1U));
		db_param[table_idx].size = UINT64_TYPE;
		db_param[table_idx].current_value = dcu_obt;
		table_idx++;

		db_param[table_idx].parID = par_id | ((uint16_t) (par_num + 2U));
		db_param[table_idx].size = UINT32_TYPE;
		db_param[table_idx].current_value = dcu_sw_vers;
		table_idx++;

		db_param[table_idx].parID = par_id | ((uint16_t) (par_num + 3U));
		db_param[table_idx].size = UINT8_TYPE;
		db_param[table_idx].current_value = 0;
		table_idx++;

		for (i = 0; i < 16; i++) {
			db_param[table_idx].parID = par_id
					|  (uint16_t)(par_num + (uint16_t)(4U + i));
			db_param[table_idx].size = UINT16_TYPE;
			db_param[table_idx].current_value = 0;
			table_idx++;
		}
	}

	icu_sem_unlock(not_mon_db_sem_Id);

	return;
}
