/**
 * \file	PUS_Service_8_SCE.c
 *
 * \brief	PUS Service 8: commands toward SCE-EEF
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 22, 2015
 */

#include "../rtems_configuration.h"

/*
 * SCE-EEF control commands - function ID 4,*
 */
icu_status_code asw_PUS_8_1_4_1_SCE_IPCEXP(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint8_t mode = 0;
	uint8_t DPU = 0;
	uint8_t DCU = 0;
	uint8_t dropLines3 = 0;
	uint8_t param_length = 0;
	uint16_t reset_num = 0, dropLines1 = 0, groups_num = 0, read_per_group = 0,
			dropLine2A = 0, exp_conf = 0, IPC_start_col = 0, IPC_start_row = 0,
			IPC_step_X = 0, IPC_step_Y = 0, IPC_Vreset_value = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;
	uint16_t rt_addr = 0;
	uint32_t planning_ID = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * check ICU status - should be SCIENCE (??)
	 */

	/*
	 * extract parameters from input array
	 * the first four bytes are the function ID
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != SCE_IPCEXP_TC_PARAM_SIZE) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		DPU = tc.TC_pkt_data[SCE_IPCEXP_TC_DPU_NUMB];
		DCU = tc.TC_pkt_data[SCE_IPCEXP_TC_DCU_NUMB];

		if ((DPU == 0) || (DPU > 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (DCU > 8) {
			error_param_length = 1;
			error_param[0] = 2;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {

			memcpy(&reset_num, &(tc.TC_pkt_data[SCE_IPCEXP_TC_RST_NFRAME_MSB]),
					sizeof(uint16_t));
			memcpy(&dropLines1,
					&(tc.TC_pkt_data[SCE_IPCEXP_TC_DROPLINES_1_MSB]),
					sizeof(uint16_t));
			groups_num = tc.TC_pkt_data[SCE_IPCEXP_TC_GROUPS_NUMB];
			read_per_group = tc.TC_pkt_data[SCE_IPCEXP_TC_GROUPS_NUMB];
			memcpy(&dropLine2A,
					&(tc.TC_pkt_data[SCE_IPCEXP_TC_DROPLINES_2_MSB]),
					sizeof(uint16_t));
			dropLines3 = tc.TC_pkt_data[SCE_IPCEXP_TC_DROPLINES_3];
			memcpy(&exp_conf, &(tc.TC_pkt_data[SCE_IPCEXP_TC_EXP_CONF_MSB]),
					sizeof(uint16_t));
			memcpy(&IPC_start_col,
					&(tc.TC_pkt_data[SCE_IPCEXP_TC_IPC_START_Y_MSB]),
					sizeof(uint16_t));
			memcpy(&IPC_step_Y, &(tc.TC_pkt_data[SCE_IPCEXP_TC_IPC_STEP_Y_MSB]),
					sizeof(uint16_t));
			memcpy(&IPC_start_row,
					&(tc.TC_pkt_data[SCE_IPCEXP_TC_IPC_START_X_MSB]),
					sizeof(uint16_t));
			memcpy(&IPC_step_X, &(tc.TC_pkt_data[SCE_IPCEXP_TC_IPC_STEP_X_MSB]),
					sizeof(uint16_t));
			memcpy(&IPC_Vreset_value,
					&(tc.TC_pkt_data[SCE_IPCEXP_TC_VRESET_MSB]),
					sizeof(uint16_t));

			/*
			 * retrieve planning_ID from local DB. Its index in the db_array is 0
			 */
			icu_sem_lock(not_mon_db_sem_Id, 5);
			planning_ID = db_long_curr_value[DB_PLANNING_ID_IDX][1];
			icu_sem_unlock(not_mon_db_sem_Id);

			/*
			 * define command mode depending on DCU.
			 */
			if (DCU == 0) {
				mode = DCUSCE_CMD_BRD_TAB_ID;
			} else {
				mode = (uint8_t) (DCUSCE_CMD_TAB1_ID + (DCU - 1));
			}

			msg_data[0] = (uint16_t) (((uint16_t) SCE_IPCEXP_COMMAND_ID
					<< COMMAND_ID_OFFSET) | mode);
			msg_data[2] = reset_num;
			msg_data[3] = dropLines1;
			msg_data[4] = groups_num;
			msg_data[5] = read_per_group;
			msg_data[6] = dropLine2A;
			msg_data[7] = (uint16_t) dropLines3;
			msg_data[8] = exp_conf;
			msg_data[9] = (uint16_t) ((planning_ID & 0xFFFF0000U) >> 16); // PID1
			msg_data[10] = (uint16_t) (planning_ID & 0x0000FFFFU); // PID2
			msg_data[11] = IPC_start_col;
			msg_data[12] = IPC_step_Y;
			msg_data[13] = IPC_start_row;
			msg_data[14] = IPC_step_X;
			msg_data[15] = IPC_Vreset_value;
			param_length = 16;

			msg_data[1] = ++ICU_command_counter;

			// Create 1553 message - switch on DPU
			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, param_length,
					msg_index_in_sl);
			if (status != ICU_SUCCESSFUL) {
				error_param_length = sizeof(icu_status_code);
				/*
				 * status is now containing the error code from
				 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
				 */
				memcpy(error_param, &status, sizeof(icu_status_code));
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_BC_1553_RW_ERROR, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			} else {

				/*
				 * push the command info in the command history array for the TM(1,*)
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) rt_addr, msg_data[0], msg_data[1], 0, 0,
						NULL );
			}
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_4_2_SCE_KTCEXP(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t mode = 0, param_length = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint8_t DPU = 0, DCU = 0;
	uint8_t rt_addr = 0;
	uint8_t KTCiterations = 0;
	uint16_t reset_num = 0, dropLines1 = 0, exp_conf = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;
	uint32_t planning_ID = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * extract parameters from input array
	 * the first four bytes are the function ID
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != SCE_KTCEXP_TC_PARAM_SIZE) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		DPU = tc.TC_pkt_data[SCE_KTCEXP_TC_DPU_NUMB];
		DCU = tc.TC_pkt_data[SCE_KTCEXP_TC_DCU_NUMB];
		if ((DPU == 0) || (DPU > 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (DCU > 8) {
			error_param_length = 1;
			error_param[0] = 2;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			memcpy(&reset_num, &(tc.TC_pkt_data[SCE_KTCEXP_TC_RESET_NUMB_MSB]),
					sizeof(uint16_t));
			memcpy(&dropLines1,
					&(tc.TC_pkt_data[SCE_KTCEXP_TC_DROPLINES_1_MSB]),
					sizeof(uint16_t));
			KTCiterations = tc.TC_pkt_data[SCE_KTCEXP_TC_KTC_ITERATIONS];
			memcpy(&exp_conf, &(tc.TC_pkt_data[SCE_KTCEXP_TC_EXP_CONF_MSB]),
					sizeof(uint16_t));
			/*
			 * retrieve planning_ID from local DB. Its index in the db_array is 0
			 */
			icu_sem_lock(not_mon_db_sem_Id, 5);
			planning_ID = db_long_curr_value[DB_PLANNING_ID_IDX][1];
			icu_sem_unlock(not_mon_db_sem_Id);

			/*
			 * define command mode depending on DCU.
			 */
			if (DCU == 0) {
				mode = DCUSCE_CMD_BRD_TAB_ID;
			} else {
				mode = (uint8_t) (DCUSCE_CMD_TAB1_ID + (DCU - 1));
			}

			msg_data[0] = (uint16_t) (((uint16_t) SCE_KTCEXP_COMMAND_ID
					<< COMMAND_ID_OFFSET) | mode);
			msg_data[2] = reset_num;
			msg_data[3] = dropLines1;
			msg_data[4] = KTCiterations;
			msg_data[5] = exp_conf;
			msg_data[6] = (uint16_t) ((planning_ID & 0xFFFF0000U) >> 16U); // PID1
			msg_data[7] = (uint16_t) (planning_ID & 0x0000FFFFU); // PID2
			param_length = 8;

			msg_data[1] = ++ICU_command_counter;
			// Create 1553 message - switch on DPU
			if (DPU == 1) {
				rt_addr = (uint8_t) dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = (uint8_t) dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					((uint8_t) BASE_COMMAND_CF + rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, param_length,
					msg_index_in_sl);
			if (status != ICU_SUCCESSFUL) {
				error_param_length = sizeof(icu_status_code);
				/*
				 * status is now containing the error code from
				 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
				 */
				memcpy(error_param, &status, sizeof(icu_status_code));
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_BC_1553_RW_ERROR, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			} else {
				/*
				 * push the command info in the command history array for the TM(1,*)
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId, rt_addr,
						msg_data[0], msg_data[1], 0, 0, NULL );
			}
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_4_3_SCE_EXP(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint8_t mode = 0, param_length = 0;
	uint8_t DPU = 0, DCU = 0, dropLine3 = 0;
	uint16_t reset_num = 0, dropLines1 = 0, groups_num = 0, read_frames_num = 0,
			dropLine2A = 0, exp_conf = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;
	uint32_t planning_ID = 0;

	/*
	 * extract parameters from input array
	 * the first four bytes are the function ID
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != SCE_EXP_TC_PARAM_SIZE) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		DPU = tc.TC_pkt_data[SCE_EXP_TC_DPU_NUMB];
		DCU = tc.TC_pkt_data[SCE_EXP_TC_DCU_NUMB];
		if (DPU > 2) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (DCU > 8) {
			error_param_length = 1;
			error_param[0] = 2;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {

			memcpy(&reset_num,
					&(tc.TC_pkt_data[SCE_EXP_TC_RESET_FRAME_NUMB_MSB]),
					sizeof(uint16_t));
			memcpy(&dropLines1, &(tc.TC_pkt_data[SCE_EXP_TC_DROPLINES_1_MSB]),
					sizeof(uint16_t));
			groups_num = tc.TC_pkt_data[SCE_EXP_TC_GROUPS_NUMB];
			read_frames_num = tc.TC_pkt_data[SCE_EXP_TC_READ_FRAMES_NUMB];
			memcpy(&dropLine2A, &(tc.TC_pkt_data[SCE_EXP_TC_DROPLINES_2_MSB]),
					sizeof(uint16_t));
			dropLine3 = tc.TC_pkt_data[SCE_EXP_TC_DROPLINES_3];
			memcpy(&exp_conf, &(tc.TC_pkt_data[SCE_EXP_TC_EXP_CONF_MSB]),
					sizeof(uint16_t));
			/*
			 * retrieve planning_ID from local DB. Its index in the db_array is 0
			 */
			icu_sem_lock(not_mon_db_sem_Id, 5);
			planning_ID = db_long_curr_value[DB_PLANNING_ID_IDX][1];
			icu_sem_unlock(not_mon_db_sem_Id);

			/*
			 * define command mode depending on DCU.
			 * If DCU = 0 --> broadcast, mode = 58
			 * Otherwise, mode = 50 + DCU_number - 1
			 */
			if (DCU == 0) {
				mode = DCUSCE_CMD_BRD_TAB_ID;
			} else {
				mode = (uint8_t) (DCUSCE_CMD_TAB1_ID + (DCU - 1));
			}

			msg_data[0] = (uint16_t) (((uint16_t) SCE_EXP_COMMAND_ID
					<< COMMAND_ID_OFFSET) | mode);
			msg_data[2] = reset_num;
			msg_data[3] = dropLines1;
			msg_data[4] = groups_num;
			msg_data[5] = read_frames_num;
			msg_data[6] = dropLine2A;
			msg_data[7] = (uint16_t) dropLine3;
			msg_data[8] = exp_conf;
			msg_data[9] = (uint16_t) ((planning_ID & 0xFFFF0000U) >> 16U); // PID1
			msg_data[10] = (uint16_t) (planning_ID & 0x0000FFFFU); // PID2
			param_length = 11;

			msg_data[1] = ++ICU_command_counter;

			// Create 1553 message - switch on DPU
			if (DPU == 0) {
				/* broadcast */
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
						RT_BROADCAST_1553, RECEIVE_BIT_1553, COMMAND_DPU_SA,
						param_length, CMD_MSG_INDEX_DPU_1);
				if (status != ICU_SUCCESSFUL) {
					error_param_length = sizeof(icu_status_code);
					/*
					 * status is now containing the error code from
					 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
					 */
					memcpy(error_param, &status, sizeof(icu_status_code));
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_BC_1553_RW_ERROR, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
				} else {
					/*
					 * push the command info in the command history array for the TM(1,*)
					 */
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu1_active_rt,
							msg_data[0], msg_data[1], msg_data[0], msg_data[1], NULL );
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu2_active_rt,
							msg_data[0], msg_data[1], msg_data[0], msg_data[1], NULL );
				}
			} else if (DPU == 1) {
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
						dpu1_active_rt, RECEIVE_BIT_1553, COMMAND_DPU_SA,
						param_length, CMD_MSG_INDEX_DPU_1);
				if (status != ICU_SUCCESSFUL) {
					error_param_length = sizeof(icu_status_code);
					/*
					 * status is now containing the error code from
					 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
					 */
					memcpy(error_param, &status, sizeof(icu_status_code));
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_BC_1553_RW_ERROR, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
				} else {
					/*
					 * push the command info in the command history array for the TM(1,*)
					 */
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu1_active_rt,
							msg_data[0], msg_data[1], 0, 0, NULL );
				}
			} else {
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) dpu2_active_rt),
						dpu2_active_rt, RECEIVE_BIT_1553, COMMAND_DPU_SA,
						param_length, CMD_MSG_INDEX_DPU_2);
				if (status != ICU_SUCCESSFUL) {
					error_param_length = sizeof(icu_status_code);
					/*
					 * status is now containing the error code from
					 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
					 */
					memcpy(error_param, &status, sizeof(icu_status_code));
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_BC_1553_RW_ERROR, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
				} else {
					/*
					 * push the command info in the command history array for the TM(1,*)
					 */
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu2_active_rt,
							msg_data[0], msg_data[1], 0, 0, NULL );
				}
			}
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_4_4_SCE_FMODE(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint8_t DPU = 0, DCU = 0, mode = 0;
	uint16_t size_in_bytes = 0;
	uint16_t SCA_mode = 0;
	uint8_t sim_pattern_mode = 0;
	uint16_t frame2frame = 0;
	uint16_t pixel_start = 0;
	uint16_t gain = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint8_t param_length = 0;

	/*
	 * extract parameters from input array
	 * the first four bytes are the function ID
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* size is fixed: 4 (fz ID) + 11 */
	if (size_in_bytes != 15) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		DPU = tc.TC_pkt_data[4];
		DCU = tc.TC_pkt_data[5];
		if (DPU > 2) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (DCU > 8) {
			error_param_length = 1;
			error_param[0] = 2;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			memcpy(&SCA_mode, &(tc.TC_pkt_data[6]), sizeof(uint16_t));
			sim_pattern_mode = tc.TC_pkt_data[8];
			memcpy(&frame2frame, &(tc.TC_pkt_data[9]), sizeof(uint16_t));
			memcpy(&pixel_start, &(tc.TC_pkt_data[11]), sizeof(uint16_t));
			memcpy(&gain, &(tc.TC_pkt_data[13]), sizeof(uint16_t));

			/*
			 * define command mode depending on DCU.
			 * If DCU = 0 --> broadcast, mode = 58
			 * Otherwise, mode = 50 + DCU_number - 1
			 */
			if (DCU == 0) {
				mode = DCUSCE_CMD_BRD_TAB_ID;
			} else {
				mode = (uint8_t) (DCUSCE_CMD_TAB1_ID + (DCU - 1));
			}

			msg_data[0] = (uint16_t) (((uint16_t) SCE_FMODE_COMMAND_ID
					<< COMMAND_ID_OFFSET) | mode);
			msg_data[2] = SCA_mode;
			msg_data[3] = sim_pattern_mode;
			msg_data[4] = frame2frame;
			msg_data[5] = pixel_start;
			msg_data[6] = gain;
			param_length = 7;

			msg_data[1] = ++ICU_command_counter;

			// Create 1553 message - switch on DPU
			if (DPU == 0) {
				/* broadcast */
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
						RT_BROADCAST_1553, RECEIVE_BIT_1553, COMMAND_DPU_SA,
						param_length, CMD_MSG_INDEX_DPU_1);
			} else if (DPU == 1) {
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
						dpu1_active_rt, RECEIVE_BIT_1553, COMMAND_DPU_SA,
						param_length, CMD_MSG_INDEX_DPU_1);
			} else {
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) dpu2_active_rt),
						dpu2_active_rt, RECEIVE_BIT_1553, COMMAND_DPU_SA,
						param_length, CMD_MSG_INDEX_DPU_2);
			}

			if (status != ICU_SUCCESSFUL) {
				error_param_length = sizeof(icu_status_code);
				/*
				 * status is now containing the error code from
				 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
				 */
				memcpy(error_param, &status, sizeof(icu_status_code));
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_BC_1553_RW_ERROR, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			} else {
				/*
				 * push the command info in the command history array for the TM(1,*)
				 */
				if (DPU == 0) {
					/* broadcast */
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu1_active_rt,
							msg_data[0], msg_data[1], msg_data[0], msg_data[1],
							NULL );
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu2_active_rt,
							msg_data[0], msg_data[1], msg_data[0], msg_data[1],
							NULL );
				} else if (DPU == 1) {
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu1_active_rt,
							msg_data[0], msg_data[1], 0, 0, NULL );
				} else {
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu2_active_rt,
							msg_data[0], msg_data[1], 0, 0, NULL );
				}
			}

		} /* end else on DCU - DPU check */
	}/* end else on TC data size */
	return (status);
}

icu_status_code asw_PUS_8_1_4_7_SCE_BOOT(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t param_length = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint8_t DPU = 0, DCU = 0;
	uint8_t rt_addr = 0;
	uint8_t bootMode = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * extract parameters from input array
	 * the first four bytes are the function ID, then DPU, DCU, bootMode (3 B)
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != 7) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		DPU = tc.TC_pkt_data[4];
		DCU = tc.TC_pkt_data[5];
		bootMode = tc.TC_pkt_data[6];
		if ((DPU == 0) || (DPU > 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (((DCU == 0) || (DCU > 8)) == TRUE) {
			error_param_length = 1;
			error_param[0] = 2;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (((bootMode == 0) || (bootMode > 3)) == TRUE) {
			error_param_length = 1;
			error_param[0] = 3;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * define command mode depending on DCU.
			 */
			msg_data[0] = (uint16_t) (((uint16_t) SCE_BOOT_COMMAND_ID
					<< COMMAND_ID_OFFSET)
					| ((uint16_t) DCUSCE_CMD_TAB1_ID + (DCU - 1)));
			msg_data[1] = ++ICU_command_counter;
			msg_data[2] = bootMode;
			param_length = 3;

			// Create 1553 message - switch on DPU
			if (DPU == 1) {
				rt_addr = (uint8_t) dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = (uint8_t) dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					((uint8_t) BASE_COMMAND_CF + rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, param_length,
					msg_index_in_sl);
			if (status != ICU_SUCCESSFUL) {
				error_param_length = sizeof(icu_status_code);
				/*
				 * status is now containing the error code from
				 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
				 */
				memcpy(error_param, &status, sizeof(icu_status_code));
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_BC_1553_RW_ERROR, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			} else {
				/*
				 * push the command info in the command history array for the TM(1,*)
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId, rt_addr,
						msg_data[0], msg_data[1], 0, 0, NULL );
			}
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_4_8_SCE_ALIVE(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t param_length = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint8_t DPU = 0, DCU = 0;
	uint8_t rt_addr = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * extract parameters from input array
	 * the first four bytes are the function ID, then DPU, DCU (1 B each)
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != 6) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		DPU = tc.TC_pkt_data[4];
		DCU = tc.TC_pkt_data[5];
		if ((DPU == 0) || (DPU > 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if ((DCU == 0) || (DCU > 8)) {
			error_param_length = 1;
			error_param[0] = 2;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * define command mode depending on DCU.
			 */
			msg_data[0] = (uint16_t) (((uint16_t) SCE_ALIVE_COMMAND_ID
					<< COMMAND_ID_OFFSET)
					| ((uint16_t) DCUSCE_CMD_TAB1_ID + (DCU - 1)));
			msg_data[1] = ++ICU_command_counter;
			param_length = 2;

			// Create 1553 message - switch on DPU
			if (DPU == 1) {
				rt_addr = (uint8_t) dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = (uint8_t) dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					((uint8_t) BASE_COMMAND_CF + rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, param_length,
					msg_index_in_sl);
			if (status != ICU_SUCCESSFUL) {
				error_param_length = sizeof(icu_status_code);
				/*
				 * status is now containing the error code from
				 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
				 */
				memcpy(error_param, &status, sizeof(icu_status_code));
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_BC_1553_RW_ERROR, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			} else {
				/*
				 * push the command info in the command history array for the TM(1,*)
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId, rt_addr,
						msg_data[0], msg_data[1], 0, 0, NULL );
			}
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_4_9_SCE_SHSK(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t param_length = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint8_t DPU = 0;
	uint8_t rt_addr = 0;
	uint16_t scanner_period = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * extract parameters from input array
	 * the first four bytes are the function ID, then DPU, scanning period (3 B)
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != 7) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		DPU = tc.TC_pkt_data[4];
		memcpy(&scanner_period, &(tc.TC_pkt_data[5]), sizeof(uint16_t));
		if ((DPU == 0) || (DPU > 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * only in SCE broadcast mode
			 */
			msg_data[0] = (uint16_t) (((uint16_t) SCE_SHSK_COMMAND_ID
					<< COMMAND_ID_OFFSET) | (uint16_t) DCUSCE_CMD_BRD_TAB_ID);
			msg_data[1] = ++ICU_command_counter;
			msg_data[2] = scanner_period;
			param_length = 3;

			// Create 1553 message - switch on DPU
			if (DPU == 1) {
				rt_addr = (uint8_t) dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = (uint8_t) dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					((uint8_t) BASE_COMMAND_CF + rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, param_length,
					msg_index_in_sl);
			if (status != ICU_SUCCESSFUL) {
				error_param_length = sizeof(icu_status_code);
				/*
				 * status is now containing the error code from
				 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
				 */
				memcpy(error_param, &status, sizeof(icu_status_code));
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_BC_1553_RW_ERROR, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			} else {
				/*
				 * push the command info in the command history array for the TM(1,*)
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId, rt_addr,
						msg_data[0], msg_data[1], 0, 0, NULL );
			}
		}
	}
	return (status);
}
