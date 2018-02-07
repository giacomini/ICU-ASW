/**
 * \file	PUS_8_DB_def.c
 *
 * \brief	PUS Service 8: auxiliary variables and functions for DPUs management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 1, 2015
 */

#include "../rtems_configuration.h"

/*
 * global variables initialization
 */

uint8_t DPU_in_use_DPMEM = 0;
uint16_t ICU_command_counter = 0;
CMD_verification_t CMD_ver_tab[2] = { { .table_ID = CMD_VER_TAB_ID, 0, 0, 0, 0,
		0, 0 }, { .table_ID = CMD_VER_TAB_ID, 0, 0, 0, 0, 0, 0 } };
uint16_t tab_counter[2][ICU_DPU_TABLES_NUMBER];

//extern CMD_verification_t BSW_CMD_ver_tab[2];
uint16_t bsw_tab_counter[2][BSW_MAX_TABLE_NUMBER - 1] = { { 0xFFFF, 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF }, { 0xFFFF,
		0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF } };

proc_param_table_t proc_param_tab[2] = { { .command_ID = 0, .table_ID =
		PROC_PARAM_TAB_ID, 0, 0, .Chi2_pixel_threshold = { 0, 0, 0, 0, 0, 0, 0,
		0 }, .RON = { 0, 0, 0, 0, 0, 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, {
		.command_ID = 0, .table_ID = PROC_PARAM_TAB_ID, 0, 0,
		.Chi2_pixel_threshold = { 0, 0, 0, 0, 0, 0, 0, 0 }, .RON = { 0, 0, 0, 0,
				0, 0, 0, 0 }, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };
dither_config_table_t dith_config_tab = { DITH_CONFIG_TAB_ID, 0, 0, 0, 0, 0, { {
		0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0 } } };
sys_config_table_t sys_config_tab = { .command_ID = 0, .table_ID =
		SYS_CONFIG_TAB_ID, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/* array of structures for the commands history */
command_tag_t DPU_cmd_history[2][MAX_CMD_HISTORY_NUMBER];
command_tag_t DPU_BSW_cmd_hist[2][MAX_BSW_CMD_HIST_NUMB];

/*
 * ********************************************************************************************
 * Functions for the handling of tables asynchronously refreshed by DPU:
 * ASW_Status_Tab / CMD_Ver_Tab / ASW_ERROR_Tab / DPU_HSK_TAB (?)
 * DPU_SYS_TAB  -- new from EUCL-OPD-TN-7-004 v. 2.1, 20/01/2016
 * ********************************************************************************************
 */

/*
 * old version functions with uint8_t *stream are in git sw versions before
 * September 2016
 */

void asw_DPU_status_tab_interpreter(const uint16_t *stream, uint16_t rt_address) {
	uint8_t i = 0, tab_idx = 0;
	uint16_t db_index = 0;
	uint16_t obt_index = 0, dcu_data_header_index = 0, dpu_words_index = 0,
			DBB_status_reg_index = 0;

	if (rt_address == dpu1_active_rt) {
		tab_idx = 0;
		db_index = DB_PARAM_IDX_ASW_DPU1_OFFSET;
		obt_index = DB_DPU1_STATUS_TAB_OBT_IDX;
		dcu_data_header_index = DB_DPU1_DCU_DATA_IDX;
		dpu_words_index = DB_DPU1_SCE_OBTAINED_WORDS_IDX;
		DBB_status_reg_index = DB_DPU1_DBB_STATUS_REG_IDX;

	} else {
		tab_idx = 1;
		db_index = DB_PAR_IDX_ASW_DPU2_OFFSET;
		obt_index = DB_DPU2_STATUS_TAB_OBT_IDX;
		dcu_data_header_index = DB_DPU2_DCU_DATA_IDX;
		dpu_words_index = DB_DPU2_SCE_OBTAINED_WORDS_IDX;
		DBB_status_reg_index = DB_DPU2_DBB_STATUS_REG_IDX;

	}

	if (stream[1] == tab_counter[tab_idx][DPU_STATUS_TAB_ID]) {
		/*
		 * tab not updated - exit
		 */
	} else {
		tab_counter[tab_idx][DPU_STATUS_TAB_ID] = stream[1];

		/*
		 * update the DB values
		 */
		if (((obt_index < DB_LONG_PARAM_NUMBER)
				&& (db_index < DB_NOT_MONITORED_PARAM_NUMB)&& (dcu_data_header_index
						< DB_LONG_PARAM_NUMBER) && (dpu_words_index
						< DB_LONG_PARAM_NUMBER) && (DBB_status_reg_index
						< DB_LONG_PARAM_NUMBER))== TRUE) {
					icu_sem_lock(not_mon_db_sem_Id, 5);
					db_long_curr_value[obt_index][0] = stream[2]; /* 2^31 - 2^16 */
					db_long_curr_value[obt_index][1] = ((uint32_t) stream[3] << 16U)
					| (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */
					db_index++;

					if ((db_index + 4) < DB_NOT_MONITORED_PARAM_NUMB) {
						for (i = 0; i < 5; i++) {
							db_param[db_index].current_value = stream[5 + i];
							db_index++;
						}
					}

					db_long_curr_value[dcu_data_header_index][0] = 0;
					db_long_curr_value[dcu_data_header_index][1]
					= ((uint32_t) stream[10] << 16U) | (uint32_t) stream[11]; /* MSW is stream[13] */
					db_index++;

					db_long_curr_value[dpu_words_index][0] = 0;
					db_long_curr_value[dpu_words_index][1] = ((uint32_t) stream[13]
					<< 16U) | (uint32_t) stream[12]; /* MSW is stream[13] */
					db_index++;

					db_param[db_index].current_value = stream[14];
					db_index++;

					db_long_curr_value[DBB_status_reg_index][0] = 0;
					db_long_curr_value[DBB_status_reg_index][1]
					= ((uint32_t) stream[15] << 16U) | (uint32_t) stream[16]; // FIXME - which is MSW??!
					db_index++;

					if((db_index + (DB_ASW_DPU_DPU_STATUS_TAB_PARAM - 11)) < DB_NOT_MONITORED_PARAM_NUMB) {
						for (i = 0; i < (DB_ASW_DPU_DPU_STATUS_TAB_PARAM - 10); i++) {
							db_param[db_index].current_value = stream[17 + i];
							db_index++;
						}
					}
					icu_sem_unlock(not_mon_db_sem_Id);
				}
			}
	return;
}

void asw_DPU_monit_tab_interpreter(const uint16_t *stream, uint16_t rt_addr) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t i = 0, tab_idx = 0;
	uint16_t DPU_mode_ID = 0;
	uint16_t DPU_mode_index = 0;
	uint16_t db_index = 0;
	uint16_t dpu_monitor_obt = 0;
	uint16_t dpu_rej_cmd = 0;
	uint16_t dpu_err_cnt = 0;
	uint8_t tmp_sce_status = 1;
	uint8_t tmp_mmu_transm_status = 1;
	uint8_t event_param[2];

	if (rt_addr == dpu1_active_rt) {
		tab_idx = 0;
		db_index = DB_PARAM_IDX_ASW_DPU1_OFFSET
				+ DB_ASW_DPU_DPU_STATUS_TAB_PARAM;
		dpu_monitor_obt = DB_DPU1_MONITOR_TAB_OBT_IDX;
		dpu_rej_cmd = DB_DPU1_REJECTED_CMD_IDX;
		dpu_err_cnt = DB_DPU1_ERROR_CNT_IDX;
	} else {
		tab_idx = 1;
		db_index = DB_PAR_IDX_ASW_DPU2_OFFSET + DB_ASW_DPU_DPU_STATUS_TAB_PARAM;
		dpu_monitor_obt = DB_DPU2_MONITOR_TAB_OBT_IDX;
		dpu_rej_cmd = DB_DPU2_REJECTED_CMD_IDX;
		dpu_err_cnt = DB_DPU2_ERROR_CNT_IDX;
	}

	if (stream[1] == tab_counter[tab_idx][DPU_MONITOR_TAB_ID]) {
		/*
		 * tab not updated - exit
		 */
	} else {
		tab_counter[tab_idx][DPU_MONITOR_TAB_ID] = stream[1];

		if (((dpu_monitor_obt < DB_LONG_PARAM_NUMBER)
				&& (db_index < DB_NOT_MONITORED_PARAM_NUMB)&& (dpu_rej_cmd
						< DB_LONG_PARAM_NUMBER) && (dpu_err_cnt < DB_LONG_PARAM_NUMBER))== TRUE) {
					icu_sem_lock(not_mon_db_sem_Id, 5);
					db_long_curr_value[dpu_monitor_obt][0] = stream[2]; /* 2^31 - 2^16 */
					db_long_curr_value[dpu_monitor_obt][1] = ((uint32_t) stream[3]
					<< 16U) | (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */
					db_index++;

					db_param[db_index].current_value = (uint8_t) (stream[5] & 0x00FFU);
					db_index++;

					db_param[db_index].current_value = (uint8_t) ((stream[5] & 0xFF00U)
					>> 8U);
					db_index++;

					db_param[db_index].current_value = stream[6];
					db_index++;

					db_long_curr_value[dpu_rej_cmd][0] = 0;
					db_long_curr_value[dpu_rej_cmd][1] = ((uint32_t) stream[8] << 16U)
					| (uint32_t) stream[7];
					db_index++;

					db_long_curr_value[dpu_err_cnt][0] = 0;
					db_long_curr_value[dpu_err_cnt][1] = ((uint32_t) stream[10] << 16U)
					| (uint32_t) stream[9];
					db_index++;

					if ((db_index + 4) < DB_NOT_MONITORED_PARAM_NUMB) {
						for (i = 0; i < 5; i++) {
							db_param[db_index].current_value = stream[11 + i];
							db_index++;
						}
					}
					icu_sem_unlock(not_mon_db_sem_Id);

					/*
					 * Update DPU and SCE status: FIXME TBD if here
					 */
					DPU_mode_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
					| ((uint16_t) PARAM_NOT_MONITORED
					<< PARAM_MONIT_FLAG_OFFSET)
					| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
					| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
					| 17U;

					DPU_mode_index = asw_PUS_3_ID_to_not_mon_DB_idx(DPU_mode_ID);

					if (rt_addr == dpu1_active_rt) {
						/* Workaround to solve problem during jump */
						//dpu1_status = (uint8_t) (stream[11] & 0x00FFU);
						if ((stream[11] & 0x00FFU) == 0) {
							dpu1_status = CPU_UNDEFINED;
						} else {
							dpu1_status = (uint8_t) (stream[11] & 0x00FFU);
						}
						/* copy in the MSB of DPU MODE */
						icu_sem_lock(not_mon_db_sem_Id, 5);
						db_param[DPU_mode_index].current_value = (uint16_t)(dpu1_status << 8) | (db_param[DPU_mode_index].current_value & 0x00FFU);
						icu_sem_unlock(not_mon_db_sem_Id);

						tmp_sce_status = dpu1_sce_status;
						dpu1_sce_status = (uint8_t) ((stream[11] & 0xFF00U) >> 8U);
						if ((tmp_sce_status == SCE_EXPOSING) && (dpu1_sce_status
								== SCE_IDLE)) {
							memcpy(&event_param[0], &dpu1_active_rt, sizeof(uint16_t));
							/* current exposure is finished - produce an event*/
							status = asw_PUS_5_TM_report(EVENT_END_OF_EXPOSURE, 2,
							event_param);
						}
					} else {
						/* Workaround to solve problem during jump */
						//dpu2_status = (uint8_t) (stream[11] & 0x00FFU);
						if ((stream[11] & 0x00FFU) == 0) {
							dpu2_status = CPU_UNDEFINED;
						} else {
							dpu2_status = (uint8_t) (stream[11] & 0x00FFU);
						}
						/* copy in the LSB of DPU MODE */
						icu_sem_lock(not_mon_db_sem_Id, 5);
						db_param[DPU_mode_index].current_value = (db_param[DPU_mode_index].current_value & 0xFF00U) | (uint16_t)dpu2_status;
						icu_sem_unlock(not_mon_db_sem_Id);

						tmp_sce_status = dpu2_sce_status;
						dpu2_sce_status = (uint8_t) ((stream[11] & 0xFF00U) >> 8U);
						if ((tmp_sce_status == SCE_EXPOSING) && (dpu2_sce_status
								== SCE_IDLE)) {
							memcpy(&event_param[0], &dpu2_active_rt, sizeof(uint16_t));
							/* current exposure is finished - produce an event*/
							status = asw_PUS_5_TM_report(EVENT_END_OF_EXPOSURE, 2,
							event_param);
						}

					}

					/*
					 * Update MMU transmission status: FIXME
					 */
					if (rt_addr == dpu1_active_rt) {
						tmp_mmu_transm_status = dpu1_mmu_transm_status;
						dpu1_mmu_transm_status = (uint8_t) (stream[14] & 0x00FFU);
						if ((tmp_mmu_transm_status == 1) && (dpu1_mmu_transm_status
								== 0)) {
							memcpy(&event_param[0], &dpu1_active_rt, sizeof(uint16_t));
							/* data transmission to MMU is finished - produce an event*/
							status = asw_PUS_5_TM_report(EVENT_END_OF_MMU_TRANSMISSION,
							2, event_param);
						}
					} else {
						tmp_mmu_transm_status = dpu2_mmu_transm_status;
						dpu2_mmu_transm_status = (uint8_t) (stream[14] & 0x00FFU);
						if ((tmp_mmu_transm_status == 1) && (dpu2_mmu_transm_status
								== 0)) {
							memcpy(&event_param[0], &dpu2_active_rt, sizeof(uint16_t));
							/* data transmission to MMU is finished - produce an event*/
							status = asw_PUS_5_TM_report(EVENT_END_OF_MMU_TRANSMISSION,
							2, event_param);
						}
					}

				}
			}

	return;
}

void asw_DPU_error_tab_interpreter(const uint16_t *stream, uint16_t rt_addr) {
	/* ASW_ERROR_TAB Interpreter */
	return;
}

void asw_DBB_DRB_tab_interpreter(const uint16_t *stream, uint16_t rt_address) {
	uint16_t db_index = 0;
	uint16_t dbb_drb_status_obt = 0;
	uint16_t dbb_drb_scrub_double_err = 0;
	uint16_t dbb_drb_scrub_single_err = 0;
	uint16_t dbb_drb_edac_double_err = 0;
	uint16_t dbb_drb_edac_single_err = 0;
	uint8_t i = 0, tab_idx = 0;

	if (rt_address == dpu1_active_rt) {
		tab_idx = 0;
		db_index = DB_PARAM_IDX_ASW_DPU1_OFFSET
				+ DB_ASW_DPU_DPU_STATUS_TAB_PARAM
				+ DB_ASW_DPU_DPU_MONIT_TAB_PARAM;
		dbb_drb_status_obt = DB_DPU1_DBB_DRB_STATUS_OBT_IDX;
		dbb_drb_scrub_double_err = DB_DPU1_DBB_SCRUB_2_ERR_IDX;
		dbb_drb_scrub_single_err = DB_DPU1_DBB_SCRUB_1_ERR_IDX;
		dbb_drb_edac_double_err = DB_DPU1_DRB_EDAC_DOUBLE_ERR_IDX;
		dbb_drb_edac_single_err = DB_DPU1_DRB_EDAC_SINGLE_ERR_IDX;
	} else {
		tab_idx = 1;
		db_index = DB_PAR_IDX_ASW_DPU2_OFFSET + DB_ASW_DPU_DPU_STATUS_TAB_PARAM
				+ DB_ASW_DPU_DPU_MONIT_TAB_PARAM;
		dbb_drb_status_obt = DB_DPU2_DBB_DRB_STATUS_OBT_IDX;
		dbb_drb_scrub_double_err = DB_DPU2_DBB_SCRUB_2_ERR_IDX;
		dbb_drb_scrub_single_err = DB_DPU2_DBB_SCRUB_1_ERR_IDX;
		dbb_drb_edac_double_err = DB_DPU2_DRB_EDAC_DOUBLE_ERR_IDX;
		dbb_drb_edac_single_err = DB_DPU2_DRB_EDAC_SINGLE_ERR_IDX;
	}

	if (stream[1] == tab_counter[tab_idx][DBB_DRB_STATUS_TAB_ID]) {
		/*
		 * tab not updated - exit
		 */
	} else {
		tab_counter[tab_idx][DBB_DRB_STATUS_TAB_ID] = stream[1];
		if (((dbb_drb_status_obt < DB_LONG_PARAM_NUMBER)
				&& (dbb_drb_scrub_double_err < DB_LONG_PARAM_NUMBER)
				&& (dbb_drb_scrub_single_err < DB_LONG_PARAM_NUMBER)
				&& (dbb_drb_edac_double_err < DB_LONG_PARAM_NUMBER)
				&& (dbb_drb_edac_single_err < DB_LONG_PARAM_NUMBER)) == TRUE) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_long_curr_value[dbb_drb_status_obt][0] = stream[2]; /* 2^31 - 2^16 */
			db_long_curr_value[dbb_drb_status_obt][1] = ((uint32_t) stream[3]
					<< 16U) | (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */

			db_long_curr_value[dbb_drb_scrub_double_err][0] = 0;
			db_long_curr_value[dbb_drb_scrub_double_err][1] =
					((uint32_t) stream[6] << 16U) | (uint32_t) stream[5];

			db_long_curr_value[dbb_drb_scrub_single_err][0] = 0;
			db_long_curr_value[dbb_drb_scrub_single_err][1] =
					((uint32_t) stream[8] << 16U) | (uint32_t) stream[7];

			db_long_curr_value[dbb_drb_edac_double_err][0] = 0;
			db_long_curr_value[dbb_drb_edac_double_err][1] =
					((uint32_t) stream[10] << 16U) | (uint32_t) stream[9];

			db_long_curr_value[dbb_drb_edac_single_err][0] = 0;
			db_long_curr_value[dbb_drb_edac_single_err][1] =
					((uint32_t) stream[12] << 16U) | (uint32_t) stream[11];
			db_index += 5;

			if ((db_index + 8) < DB_NOT_MONITORED_PARAM_NUMB) {
				for (i = 0; i < 8; i++) {
					db_param[db_index].current_value = stream[13 + i];
					db_index++;
				}

				/* DBB scrub status */
				db_param[db_index].current_value = (uint8_t) (stream[21]
						& 0x00FFU);
				db_index++;

				/* DRB EDAC status */
				db_param[db_index].current_value = (uint8_t) ((stream[21]
						& 0xFF00U) >> 8U);
			}

			icu_sem_unlock(not_mon_db_sem_Id);
		}
	}

	return;
}

/*
 * This function fills the CMD_Verification_tab.
 * TODO: OPD ICD v.2.4: no more table length
 * 		 how to handle an incomplete table - check if received the full
 * table
 * 		 even if partially empty
 * OPD ICD v.2.5: one command at time
 */
icu_status_code asw_CMD_ver_table_interpreter(uint8_t rt_addr,
		const uint16_t *stream) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t i = 0, j = 0, cmd_idx = 0, twin_cmd_idx = 0;
	uint8_t cmd_found_flag = 0, twin_cmd_found_flag = 0;
	uint16_t DPU_error_code = 0;
	uint16_t TC_pkt_id = 0;
	uint16_t TC_seq_cnt = 0;
	uint8_t dest_ID = 0;
	uint8_t cmd_identifier = 0;
	uint8_t tab_idx = 0, tab2_idx = 0;
	uint8_t error_param_length = 0;
	uint8_t error_list[PUS_1_MAX_ERROR_PARAM_LENGTH];

	if (rt_addr == (uint8_t) dpu1_active_rt) {
		tab_idx = 0;
		tab2_idx = 1;
	} else {
		tab_idx = 1;
		tab2_idx = 0;
	}

	if (tab_counter[tab_idx][CMD_VER_TAB_ID] == stream[1]) {
		/*
		 * tab not updated. Exit
		 */
	} else {
		/*
		 * update internal counter. It could be not consecutive
		 */
		tab_counter[tab_idx][CMD_VER_TAB_ID] = stream[1];
		CMD_ver_tab[tab_idx].counter = stream[1];
		CMD_ver_tab[tab_idx].OBT = (((uint32_t) 0U | stream[2]) << 16U)
				| (uint32_t) stream[3];
		CMD_ver_tab[tab_idx].cmd_id = stream[6];
		CMD_ver_tab[tab_idx].cmd_ICU_counter = stream[7];
		CMD_ver_tab[tab_idx].verification_code = stream[8];
		CMD_ver_tab[tab_idx].error_code_data = stream[9];

		/*
		 * find this command in the ICU command history
		 */
		i = 0;
		icu_sem_lock(cmd_ver_tab_sem_Id, 5);
		while ((cmd_found_flag == 0) && (i < MAX_CMD_HISTORY_NUMBER)) {
			if (((DPU_cmd_history[tab_idx][i].useFlag == 1)
					&& (DPU_cmd_history[tab_idx][i].cmd_ICU_counter
							== CMD_ver_tab[tab_idx].cmd_ICU_counter)
					&& (DPU_cmd_history[tab_idx][i].cmd_id
							== CMD_ver_tab[tab_idx].cmd_id)) == TRUE) {
				cmd_idx = i;
				cmd_found_flag = 1;
			}
			i++;
		}
		icu_sem_unlock(cmd_ver_tab_sem_Id);

		if (cmd_found_flag == 0) {
			// TODO: do something, like an event or a FDIR action
			status = CMD_STATUS_NON_FOUND;
		} else {
			/*
			 * report the status of this command
			 */
			/*
			 * verification type:  bit 0-1 --> mask = 3
			 * verification result: bit  2-3 --> mask = 12
			 * error code: bit 4-15 --> mask = 0xFFF0
			 */
			cmd_identifier = (uint8_t) ((CMD_ver_tab[tab_idx].cmd_id & 0xFF00U)
					>> 8U);

			if (CMD_ver_tab[tab_idx].cmd_id == CPU_MDUMP_COMMAND_ID) {
				/*
				 * for MDUMP there is no need of TM(1,7/8), but to let know to the task
				 * handling the PUS(6,*) that the command has been verified
				 *
				 * verification type:  bit 0-1 --> mask = 3
				 * verification result: bit  2-3 --> mask = 12
				 * error code: bit 4-15 --> mask = 0xFFF0
				 *
				 * command ok is bit 2 (accepted or executed)
				 */
				if (((CMD_ver_tab[tab_idx].verification_code & 0x000cU) >> 2U)
						== 1) {

					/* Activation of mdump flag and enable messages */
					if (rt_addr == (uint8_t) dpu1_active_rt) {
						mem_dump_active.dpu1_asw = 1;
					} else {
						mem_dump_active.dpu2_asw = 1;
					}

					bus_profile_msg_counter[BASE_MEM_DUMP_CF + rt_addr] += 16;
				} else if (((CMD_ver_tab[tab_idx].verification_code & 0x000cU)
						>> 2U) == 2) {
					/*
					 * MDUMP has been refused - Prepare 1,8 and 5,2(CMD_NOT_EXECUTED)
					 */
					error_param_length = 4;
					DPU_error_code = ((CMD_ver_tab[tab_idx].verification_code)
							& 0xFFF0U) >> 4U;

					error_list[0] =
							(uint8_t) ((DPU_error_code & 0xFF00U) >> 8U);
					error_list[1] = (uint8_t) (DPU_error_code & 0x00FFU);
					error_list[2] =
							(uint8_t) ((CMD_ver_tab[tab_idx].error_code_data
									& 0xFF00U) >> 8U);
					error_list[3] =
							(uint8_t) (CMD_ver_tab[tab_idx].error_code_data
									& 0x00FFU);

					if (cmd_idx < MAX_CMD_HISTORY_NUMBER) {
						icu_sem_lock(cmd_ver_tab_sem_Id, 5);
						TC_pkt_id =
								DPU_cmd_history[tab_idx][cmd_idx].TC_packet_ID;
						TC_seq_cnt =
								DPU_cmd_history[tab_idx][cmd_idx].TC_seq_control;
						dest_ID = DPU_cmd_history[tab_idx][cmd_idx].TC_dest_ID;
						icu_sem_unlock(cmd_ver_tab_sem_Id);
					}

					status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id, TC_seq_cnt,
							dest_ID, TM_1_8_DPU_CMD_ERROR, error_param_length,
							error_list);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				} else {
					status = ICU_DPU_ILLEGAL_STATE; // TODO: change this error
				}
				/*
				 * in any case, mark this command_tag as reusable
				 * all other fields are reset by asw_push_cmd_tag function
				 */
				if (cmd_idx < MAX_CMD_HISTORY_NUMBER) {
					icu_sem_lock(cmd_ver_tab_sem_Id, 5);
					DPU_cmd_history[tab_idx][cmd_idx].useFlag = 0;
					icu_sem_unlock(cmd_ver_tab_sem_Id);
				}
			} else if (cmd_identifier == CPU_GTAB_COMMAND_ID) {
				/*
				 * GTAB is not a TC, so there is no need of TM(1,7/8), but to let know to the HSK task that the command has
				 * been verified
				 */
				if (cmd_idx < MAX_CMD_HISTORY_NUMBER) {
					icu_sem_lock(cmd_ver_tab_sem_Id, 5);
					DPU_cmd_history[tab_idx][cmd_idx].cmd_verification_code =
							CMD_ver_tab[tab_idx].verification_code;
					DPU_cmd_history[tab_idx][cmd_idx].cmd_error_data =
							CMD_ver_tab[tab_idx].error_code_data;
					DPU_cmd_history[tab_idx][cmd_idx].is_cmd_verified = 1;
					icu_sem_unlock(cmd_ver_tab_sem_Id);
				}
				/*
				 * command ok is bit 2 (accepted or executed)
				 */
				if (((CMD_ver_tab[tab_idx].verification_code & 0x000cU) >> 2U)
						== 1) {
					if (rt_addr == (uint8_t) dpu1_active_rt) {
						asw_SCE_and_DBB_DRB_msgs_in_sl(dpu1_next_gtab,
								dpu1_active_rt);
					} else {
						asw_SCE_and_DBB_DRB_msgs_in_sl(dpu2_next_gtab,
								dpu2_active_rt);
					}

				} else {
					/* GTAB command not OK FIXME*/
				}
				/*
				 * the command_tag is not useful anymore
				 * all other fields are reset by asw_push_cmd_tag function
				 */
				if (cmd_idx < MAX_CMD_HISTORY_NUMBER) {
					icu_sem_lock(cmd_ver_tab_sem_Id, 5);
					DPU_cmd_history[tab_idx][cmd_idx].useFlag = 0;
					icu_sem_unlock(cmd_ver_tab_sem_Id);
				}
			} else {
				/*
				 * for all other DPU commands, this function takes care of producing the TM(1,7/8). Need to check whether the TC was
				 * splitted into commands to both DPUs. In this case, only a TM(1,*) has to be produced, with the combination of the
				 * two results.
				 */

				/*
				 * fill the command tag
				 */
				if (cmd_idx < MAX_CMD_HISTORY_NUMBER) {
					icu_sem_lock(cmd_ver_tab_sem_Id, 5);
					DPU_cmd_history[tab_idx][cmd_idx].cmd_verification_code =
							CMD_ver_tab[tab_idx].verification_code;
					DPU_cmd_history[tab_idx][cmd_idx].cmd_error_data =
							CMD_ver_tab[tab_idx].error_code_data;
					DPU_cmd_history[tab_idx][cmd_idx].is_cmd_verified = 1;
					icu_sem_unlock(cmd_ver_tab_sem_Id);
				}

				if (DPU_cmd_history[tab_idx][cmd_idx].twin_cmd_id == 0) {
					/* if it is a lonely command, close it! */
					status = asw_DPU_single_cmd_final_TM_1(tab_idx, cmd_idx);
					if (status == ICU_DPU_ILLEGAL_STATE) {
						/* FIXME: do something!! change this error
						 * Note that we don't free the command_tag
						 */
					} else {
						/* mark the command_tag as reusable
						 * all other fields are reset by asw_push_cmd_tag function */
						if (cmd_idx < MAX_CMD_HISTORY_NUMBER) {
							DPU_cmd_history[tab_idx][cmd_idx].useFlag = 0;
						}
					}
				} /* end  if (DPU_cmd_history[cmd_idx].is_twin_flag == 0) */

				else {
					/*
					 * check the twin command into the history of the other DPU
					 */
					j = 0;
					while ((twin_cmd_found_flag == 0)
							&& (j < MAX_CMD_HISTORY_NUMBER)) {
						if ((DPU_cmd_history[tab2_idx][j].cmd_ICU_counter
								== DPU_cmd_history[tab_idx][cmd_idx].twin_cmd_ICU_count)
								&& (DPU_cmd_history[tab2_idx][j].cmd_id
										== DPU_cmd_history[tab_idx][cmd_idx].twin_cmd_id)) {
							twin_cmd_idx = j;
							twin_cmd_found_flag = 1;
						}
						j++;
					}
					if (twin_cmd_found_flag == 0) {
						// TODO: do something, like an event or a FDIR action
						status = CMD_STATUS_NON_FOUND;
					} else {
						if (DPU_cmd_history[tab2_idx][twin_cmd_idx].is_cmd_verified
								== 1) {
							/*
							 * produce the final TM(1,*) combining the results of the two commands
							 */
							status = asw_DPU_twin_cmd_final_TM_1(tab_idx,
									cmd_idx, tab2_idx, twin_cmd_idx);
							if (status == ICU_DPU_ILLEGAL_STATE) {
								/* TODO: do something - words in CMD_VER not acceptable
								 * Note that we don't free the command_tags */
							} else {
								/*
								 * set both command_tags as reusable
								 * all other fields are reset by asw_push_cmd_tag function
								 */
								icu_sem_lock(cmd_ver_tab_sem_Id, 5);
								DPU_cmd_history[tab_idx][cmd_idx].useFlag = 0;
								DPU_cmd_history[tab2_idx][twin_cmd_idx].useFlag =
										0;
								icu_sem_unlock(cmd_ver_tab_sem_Id);
							}
						}
						/*
						 * else: nothing to do
						 */
					} /* end else (twin command not found) */

				} /* end else if (DPU_cmd_history[cmd_idx].is_twin_flag == 0) */

			} /* end of else(cmd != MLOAD-MDUMP) */
		} /* End else CMD_NOT_FOUND */
	} /* End else counter changed */

	return (status);
}

/*******************************************************************************
 * Functions managing the check of the status of commands sent to DPU:
 * icu_status_code asw_push_cmd_tag(uint16_t cmd_ID, uint16_t counter)
 * int32_t check_cmd_status(uint16_t cmd_ID, uint16_t counter)
 * *****************************************************************************/

icu_status_code asw_push_cmd_tag(uint8_t acknowledge_flag, uint16_t TC_pkt_ID,
		uint16_t TC_seq_cntr, uint8_t dest_ID, uint8_t rt_addr, uint16_t cmd_ID,
		uint16_t cmd_counter, uint16_t twin_command_id, uint16_t twin_ICU_cnt,
		uint8_t* cmd_idx) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t i = 0, pos_found = 0;
	uint8_t tab_idx = 0;

	if (rt_addr == (uint8_t) dpu1_active_rt) {
		tab_idx = 0;
	} else {
		tab_idx = 1;
	}

	/*
	 * search in the DPU_cmd_history array the first void position -
	 * the array is not ordered!!
	 */
	while ((pos_found == 0) && (i < MAX_CMD_HISTORY_NUMBER)) {
		if (DPU_cmd_history[tab_idx][i].useFlag != 1) {
			// found a position for current command!!
			icu_sem_lock(cmd_ver_tab_sem_Id, 5);
			DPU_cmd_history[tab_idx][i].ack_flag = acknowledge_flag;
			DPU_cmd_history[tab_idx][i].useFlag = 1;
			DPU_cmd_history[tab_idx][i].is_cmd_verified = 0;
			DPU_cmd_history[tab_idx][i].cmd_id = cmd_ID;
			DPU_cmd_history[tab_idx][i].cmd_ICU_counter = cmd_counter;
			DPU_cmd_history[tab_idx][i].TC_packet_ID = TC_pkt_ID;
			DPU_cmd_history[tab_idx][i].TC_seq_control = TC_seq_cntr;
			DPU_cmd_history[tab_idx][i].TC_dest_ID = dest_ID;
			/*
			 * set to zero the fields for the results of verification
			 */
			DPU_cmd_history[tab_idx][i].cmd_verification_code = 0;
			DPU_cmd_history[tab_idx][i].cmd_error_data = 0;
			/*
			 * set fields for the twin command, if applicable
			 * FIXME: be sure that there is no DPU cmd with id==0...
			 * if twin_command_id = 0, then the command was alone
			 * if twin_command_id != 0, there is a twin command with that command_id
			 */
			if (twin_command_id == 0) {
				DPU_cmd_history[tab_idx][i].twin_cmd_id = 0;
				DPU_cmd_history[tab_idx][i].twin_cmd_ICU_count = 0;
			} else {
				DPU_cmd_history[tab_idx][i].twin_cmd_id = twin_command_id;
				DPU_cmd_history[tab_idx][i].twin_cmd_ICU_count = twin_ICU_cnt;
			}
			icu_sem_unlock(cmd_ver_tab_sem_Id);
			pos_found = 1;
			if (cmd_idx != NULL ) {
				*cmd_idx = i;
			}
		}
		i++;
	}
	//icu_sem_unlock(cmd_ver_tab_sem_Id);
	/*
	 * ATT!! The only recovery for this error is to change the size of the
	 * DPU_cmd_history array!!!!
	 */
	if (pos_found == 0) {
		status = ICU_UNSATISFIED;
	} else {
		/* Set DPU state as busy */
		if (((rt_addr == (uint8_t) dpu1_active_rt) && (TC_pkt_ID != 0)) == TRUE) {
			dpu1_tc_running = 1;
		}

		if (((rt_addr == (uint8_t) dpu2_active_rt) && (TC_pkt_ID != 0)) == TRUE) {
			dpu2_tc_running = 1;
		}
	}

	return (status);
}

icu_status_code asw_DPU_twin_cmd_final_TM_1(uint8_t tab1, uint16_t cmd_idx,
		uint8_t tab2, uint16_t twin_cmd_idx) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t TC_pkt_id = 0, TC_seq_cnt = 0;
	uint16_t ver_type_1 = 0, ver_result_1 = 0;
	uint16_t ver_type_2 = 0, ver_result_2 = 0;
	uint16_t DPU_error_code_1 = 0, DPU_error_code_2 = 0;
	uint16_t DPU_error_data_1 = 0, DPU_error_data_2 = 0;
	uint8_t TM_par_length = 10, TM_par_list[10]; // size fixed (data from DPU)
	uint8_t dest_ID = 0;
	uint8_t ack_flag = 0;

	if (((cmd_idx < MAX_CMD_HISTORY_NUMBER) && (tab1 < 2) && (tab2 < 2)
			&& (twin_cmd_idx < MAX_CMD_HISTORY_NUMBER)) == TRUE) {
		icu_sem_lock(cmd_ver_tab_sem_Id, 5);
		TC_pkt_id = DPU_cmd_history[tab1][cmd_idx].TC_packet_ID;
		TC_seq_cnt = DPU_cmd_history[tab1][cmd_idx].TC_seq_control;
		dest_ID = DPU_cmd_history[tab1][cmd_idx].TC_dest_ID;
		ack_flag = DPU_cmd_history[tab1][cmd_idx].ack_flag;

		ver_type_1 = DPU_cmd_history[tab1][cmd_idx].cmd_verification_code
				& 0x0003U;
		ver_result_1 = (DPU_cmd_history[tab1][cmd_idx].cmd_verification_code
				& 0x000cU) >> 2U;
		DPU_error_code_1 = (DPU_cmd_history[tab1][cmd_idx].cmd_verification_code
				& 0xFFF0U) >> 4U;

		ver_type_2 = DPU_cmd_history[tab2][twin_cmd_idx].cmd_verification_code
				& 0x0003U;
		ver_result_2 =
				(DPU_cmd_history[tab2][twin_cmd_idx].cmd_verification_code
						& 0x000cU) >> 2U;
		DPU_error_code_2 =
				(DPU_cmd_history[tab2][twin_cmd_idx].cmd_verification_code
						& 0xFFF0U) >> 4U;
		icu_sem_unlock(cmd_ver_tab_sem_Id);
	}

	if ((ver_result_1 == 1) && (ver_result_2 == 1)) {
		if ((ack_flag & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(TC_pkt_id, TC_seq_cnt, dest_ID);
		}
	} else if ((ver_result_1 == 2) || (ver_result_2 == 2)) {

		if (tab1 == 0) {
			TM_par_list[0] = 1;
			TM_par_list[5] = 2;
		} else {
			TM_par_list[0] = 2;
			TM_par_list[5] = 1;
		}

		if (((cmd_idx < MAX_CMD_HISTORY_NUMBER) && (tab1 < 2) && (tab2 < 2)
				&& (twin_cmd_idx < MAX_CMD_HISTORY_NUMBER)) == TRUE) {
			icu_sem_lock(cmd_ver_tab_sem_Id, 5);
			DPU_error_data_1 = DPU_cmd_history[tab1][cmd_idx].cmd_error_data;
			DPU_error_data_2 =
					DPU_cmd_history[tab2][twin_cmd_idx].cmd_error_data;
			icu_sem_unlock(cmd_ver_tab_sem_Id);

			TM_par_list[1] = (uint8_t) ((DPU_error_code_1 & 0xFF00U) >> 8U);
			TM_par_list[2] = (uint8_t) (DPU_error_code_1 & 0x00FFU);
			TM_par_list[3] = (uint8_t) ((DPU_error_data_1 & 0xFF00U) >> 8U);
			TM_par_list[4] = (uint8_t) (DPU_error_data_1 & 0x00FFU);
			TM_par_list[6] = (uint8_t) ((DPU_error_code_2 & 0xFF00U) >> 8U);
			TM_par_list[7] = (uint8_t) (DPU_error_code_2 & 0x00FFU);
			TM_par_list[8] = (uint8_t) ((DPU_error_data_2 & 0xFF00U) >> 8U);
			TM_par_list[9] = (uint8_t) (DPU_error_data_2 & 0x00FFU);

			status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id, TC_seq_cnt, dest_ID,
					TM_1_8_DPU_CMD_ERROR, TM_par_length, TM_par_list);
		}
	} else { // boh?? stato indefinibile
		status = ICU_DPU_ILLEGAL_STATE; // TODO: change this error
	}

	return (status);
}

icu_status_code asw_DPU_single_cmd_final_TM_1(uint8_t tab, uint16_t cmd_idx) {

	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t TC_pkt_id = 0, TC_seq_cnt = 0;
	uint16_t ver_type = 0, ver_result = 0;
	uint16_t DPU_error_code = 0;
	uint16_t DPU_error_data = 0;
	uint8_t TM_par_length = 4, TM_par_list[4]; // size fixed (data from DPU)
	uint8_t dest_ID = 0;
	uint8_t ack_flag = 0;

	if (((tab < 2) && (cmd_idx < MAX_CMD_HISTORY_NUMBER)) == TRUE) {
		icu_sem_lock(cmd_ver_tab_sem_Id, 5);
		TC_pkt_id = DPU_cmd_history[tab][cmd_idx].TC_packet_ID;
		TC_seq_cnt = DPU_cmd_history[tab][cmd_idx].TC_seq_control;
		dest_ID = DPU_cmd_history[tab][cmd_idx].TC_dest_ID;
		ack_flag = DPU_cmd_history[tab][cmd_idx].ack_flag;

		/*
		 * FIXME: the meaning and usefulness of verification type
		 *  How many table will be transmitted? 1 or 2?
		 *
		 * verification type:  bit 0-1 --> mask = 3
		 * verification result: bit  2-3 --> mask = 12
		 * error code: bit 4-15 --> mask = 0xFFF0
		 */
		ver_type = DPU_cmd_history[tab][cmd_idx].cmd_verification_code
				& 0x0003U;
		ver_result = (DPU_cmd_history[tab][cmd_idx].cmd_verification_code
				& 0x000cU) >> 2U;
		DPU_error_code = (DPU_cmd_history[tab][cmd_idx].cmd_verification_code
				& 0xFFF0U) >> 4U;
		DPU_error_data = DPU_cmd_history[tab][cmd_idx].cmd_error_data;
		icu_sem_unlock(cmd_ver_tab_sem_Id);
	}

	if (ver_result == 1) {
		if ((ack_flag & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(TC_pkt_id, TC_seq_cnt, dest_ID);
		}
	} else if (ver_result == 2) {
		TM_par_list[0] = (uint8_t) ((DPU_error_code & 0xFF00U) >> 8U);
		TM_par_list[1] = (uint8_t) (DPU_error_code & 0x00FFU);
		TM_par_list[2] = (uint8_t) ((DPU_error_data & 0xFF00U) >> 8U);
		TM_par_list[3] = (uint8_t) (DPU_error_data & 0x00FFU);
		status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id, TC_seq_cnt, dest_ID,
				TM_1_8_DPU_CMD_ERROR, TM_par_length, TM_par_list);
	} else {
		status = ICU_DPU_ILLEGAL_STATE; // TODO: change this error
	}

	return (status);
}

/* BSW */
icu_status_code asw_push_bsw_cmd_tag(uint8_t acknowledge_flag,
		uint16_t TC_pkt_ID, uint16_t TC_seq_cntr, uint8_t dest_ID,
		uint8_t rt_addr, uint16_t cmd_ID, uint16_t cmd_counter,
		uint8_t* cmd_idx) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t i = 0;
	uint8_t pos_found = 0;
	uint8_t tab_idx = 0;

	if (rt_addr == (uint8_t) dpu2_active_rt) {
		tab_idx = 1;
	}

	/*
	 * search in the DPU_BSW_cmd_history array the first void position -
	 * the array is not ordered!!
	 */
	while ((pos_found == 0) && (i < MAX_BSW_CMD_HIST_NUMB)) {
		icu_sem_lock(cmd_ver_tab_sem_Id, 5);
		if (DPU_BSW_cmd_hist[tab_idx][i].useFlag != 1) {
			// found a position for current command!!
			DPU_BSW_cmd_hist[tab_idx][i].ack_flag = acknowledge_flag;
			DPU_BSW_cmd_hist[tab_idx][i].useFlag = 1;
			DPU_BSW_cmd_hist[tab_idx][i].is_cmd_verified = 0;
			DPU_BSW_cmd_hist[tab_idx][i].cmd_id = cmd_ID;
			DPU_BSW_cmd_hist[tab_idx][i].cmd_ICU_counter = cmd_counter;
			DPU_BSW_cmd_hist[tab_idx][i].TC_packet_ID = TC_pkt_ID;
			DPU_BSW_cmd_hist[tab_idx][i].TC_seq_control = TC_seq_cntr;
			DPU_BSW_cmd_hist[tab_idx][i].TC_dest_ID = dest_ID;
			/*
			 * set to zero the fields for the results of verification - not useful for BSW
			 */
			DPU_BSW_cmd_hist[tab_idx][i].cmd_verification_code = 0;
			DPU_BSW_cmd_hist[tab_idx][i].cmd_error_data = 0;
			/*
			 * set fields for the twin command to zero - not applicable for BSW
			 */
			DPU_BSW_cmd_hist[tab_idx][i].twin_cmd_id = 0;
			DPU_BSW_cmd_hist[tab_idx][i].twin_cmd_ICU_count = 0;
			pos_found = 1;
			*cmd_idx = i;
		}
		icu_sem_unlock(cmd_ver_tab_sem_Id);
		i++;
	}

	/*
	 * ATT!! The only recovery for this error is to change the size of the
	 * DPU_cmd_history array!!!!
	 */
	if (pos_found == 0) {
		status = ICU_UNSATISFIED;
	}

	return (status);
}

icu_status_code asw_BSW_CMD_ver_tab_interpreter(uint8_t rt_addr,
		const uint16_t *stream) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t cmd_id = 0;
	uint16_t cmd_cnt = 0;
	uint8_t cmd_ver_tab_id = 0;
	uint8_t tab_idx = 0, i = 0, cmd_found_flag = 0, cmd_idx = 0;

	uint16_t TC_pkt_id = 0;
	uint16_t TC_seq_cnt = 0;
	uint16_t dest_ID = 0;
	uint8_t cmd_service = 0;
	uint8_t ackn_flag = 0;
	uint8_t error_param_length = 0;
	uint8_t error_list[PUS_1_MAX_ERROR_PARAM_LENGTH];

	if (rt_addr == (uint8_t) dpu2_active_rt) {
		tab_idx = 1;
	}

	cmd_ver_tab_id = (uint8_t) (stream[8] >> 14);
	/*
	 * check if table has been updated
	 */
	if (((cmd_ver_tab_id == 1)
			&& (bsw_tab_counter[tab_idx][BSW_CMD_VER_TAB_1] == stream[1]))
			== TRUE) {
		/* CMD_VER_1 tab not updated. Exit */
	} else if (((cmd_ver_tab_id == 2)
			&& (bsw_tab_counter[tab_idx][BSW_CMD_VER_TAB_2] == stream[1]))
			== TRUE) {
		/* CMD_VER_2 tab not updated. Exit */
	} else if (((cmd_ver_tab_id != 1) && (cmd_ver_tab_id != 2)) == TRUE) {
		/* wrong tab id FIXME */
	} else {
		cmd_id = stream[6];
		cmd_cnt = stream[7];

		i = 0;
		icu_sem_lock(cmd_ver_tab_sem_Id, 5);
		while ((cmd_found_flag == 0) && (i < MAX_BSW_CMD_HIST_NUMB)) {
			if (((DPU_BSW_cmd_hist[tab_idx][i].useFlag == 1)
					&& (DPU_BSW_cmd_hist[tab_idx][i].cmd_ICU_counter == cmd_cnt)
					&& (DPU_BSW_cmd_hist[tab_idx][i].cmd_id == cmd_id)) == TRUE) {
				cmd_idx = i;
				cmd_found_flag = 1;
			}
			i++;
		}
		icu_sem_unlock(cmd_ver_tab_sem_Id);

		if (cmd_found_flag == 0) {
			// TODO: do something, like an event or a FDIR action
			status = CMD_STATUS_NON_FOUND;
		} else {
			/*
			 * switch CMD_tab.
			 * If 2, produce TM(1,7) or TM(1,8)
			 */
			if (cmd_idx < MAX_BSW_CMD_HIST_NUMB) {
				icu_sem_lock(cmd_ver_tab_sem_Id, 5);
				TC_pkt_id = DPU_BSW_cmd_hist[tab_idx][cmd_idx].TC_packet_ID;
				TC_seq_cnt = DPU_BSW_cmd_hist[tab_idx][cmd_idx].TC_seq_control;
				dest_ID = DPU_BSW_cmd_hist[tab_idx][cmd_idx].TC_dest_ID;
				ackn_flag = DPU_BSW_cmd_hist[tab_idx][cmd_idx].ack_flag;
				icu_sem_unlock(cmd_ver_tab_sem_Id);
			}

			if (cmd_ver_tab_id == 1) {
				/* If 1, produce TM(1,8) only if the command has not been accepted. */
				if ((uint8_t) stream[9] != 0xF0U) {
					error_param_length = 15;
					error_list[0] = cmd_ver_tab_id;
					memcpy(&error_list[1], &stream[9], sizeof(uint16_t));
					for (i = 3; i < 15; i++) {
						error_list[i] = 0;
					}
					status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id, TC_seq_cnt,
							dest_ID, TM_1_8_DPU_BSW_CMD_ERROR,
							error_param_length, error_list);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				}
			} else {
				/* cmd_ver_tab_id = 2
				 * produce TM(1,7/8) */
				if ((uint8_t) stream[9] == 0xF0U) {
					/* produce TM(1,7) */
					if ((ackn_flag & PUS_1_7_GEN_FLAG) > 0) {
						status = asw_PUS_1_7_TM_TC_exec_ok(TC_pkt_id,
								TC_seq_cnt, dest_ID);
					}
				} else {
					cmd_service = (uint8_t) (stream[6] & 0x3FU);
					switch (cmd_service) {
					case (0):
						/* Copy EEPROM to RAM */
						error_param_length = 15;
						error_list[0] = cmd_ver_tab_id;
						memcpy(&error_list[1], &stream[9],
								7 * sizeof(uint16_t));
						status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id,
								TC_seq_cnt, dest_ID, TM_1_8_DPU_BSW_CMD_ERROR,
								error_param_length, error_list);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						break;
					case (2):
						/* check memory */
						error_param_length = 15;
						error_list[0] = cmd_ver_tab_id;
						for (i = 1; i < 15; i++) {
							error_list[i] = 0;
						}
						status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id,
								TC_seq_cnt, dest_ID, TM_1_8_DPU_BSW_CMD_ERROR,
								error_param_length, error_list);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						break;
					case (3):
						/* check EEPROM segment */
						error_param_length = 15;
						error_list[0] = cmd_ver_tab_id;
						memcpy(&error_list[1], &stream[9],
								6 * sizeof(uint16_t));
						for (i = 13; i < 15; i++) {
							error_list[i] = 0;
						}
						status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id,
								TC_seq_cnt, dest_ID, TM_1_8_DPU_BSW_CMD_ERROR,
								error_param_length, error_list);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						break;
					case (4):
						/* load memory */
					case (5):
						/* dump memory */
					case (6):
						/* FDIR */
						error_param_length = 15;
						error_list[0] = cmd_ver_tab_id;
						for (i = 1; i < 15; i++) {
							error_list[i] = 0;
						}
						status = asw_PUS_1_8_TM_TC_exec_fail(TC_pkt_id,
								TC_seq_cnt, dest_ID, TM_1_8_DPU_BSW_CMD_ERROR,
								error_param_length, error_list);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );

						break;
					default:
						break;
					}
				}
				/* mark this command_tag as reusable
				 * all other fields are reset by asw_push_bsw_cmd_tag function
				 */
				if (cmd_idx < MAX_BSW_CMD_HIST_NUMB) {
					icu_sem_lock(cmd_ver_tab_sem_Id, 5);
					DPU_BSW_cmd_hist[tab_idx][cmd_idx].useFlag = 0;
					icu_sem_unlock(cmd_ver_tab_sem_Id);
				}

			} /* end else on CMD_VER_TAB type (1/2) */
		} /* end else cmd_not_found */
	} /* end else table not updated */

	return (status);
}
