/**
 * \file	PUS_Service_8_CPU.c
 *
 * \brief	PUS Service 8: commands toward DCUs
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: May 17, 2017
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_8_1_5_1_CPU_RBT(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * check ICU status - should be MANUAL (?!) - TODO
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (5 Bytes) */
	if (size_in_bytes != 5) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract parameters from the param array
		 * the first four bytes are the function ID
		 */
		DPU = tc.TC_pkt_data[4];
		if ((DPU != 1) && (DPU != 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) CPU_RBT_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DPU_CMD_ID;

			msg_length = 2;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}
			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, msg_length,
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
				 * push the commands info in the command history arrays.
				 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when command is verified
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) rt_addr, msg_data[0], msg_data[1], 0,
						0, NULL );
			}
		}
	} /* end else (size in byte) */
	return (status);
}

icu_status_code asw_PUS_8_1_5_2_CPU_SST(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint8_t ASW_status = 0;
	uint16_t msg_index_in_sl = 0;
	/*
	 * check ICU status? - any state!
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (6 Bytes) */
	if (size_in_bytes != 6) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract parameters from the param array
		 * the first four bytes are the function ID
		 */
		DPU = tc.TC_pkt_data[4];
		ASW_status = tc.TC_pkt_data[5];
		if ((DPU != 1) && (DPU != 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) CPU_SST_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DPU_CMD_ID;
			msg_data[2] = ASW_status;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}
			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, msg_length,
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
				 * push the commands info in the command history arrays.
				 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when command is verified
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) rt_addr, msg_data[0], msg_data[1], 0,
						0, NULL );
			}
		}
	} /* end else (size in byte) */
	return (status);
}

icu_status_code asw_PUS_8_1_5_3_CPU_STM(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t size_in_bytes = 0;
	uint8_t master_RT = 0;

	/*
	 * check ICU status? - any state!
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (5 Bytes) */
	if (size_in_bytes != 5) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract parameters from the param array
		 * the first four bytes are the function ID
		 */
		master_RT = tc.TC_pkt_data[4];
		if ((master_RT == 0) || (master_RT > 4)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) CPU_STM_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DPU_CMD_ID;
			msg_data[2] = master_RT;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
					RT_BROADCAST_1553, RECEIVE_BIT_1553, COMMAND_DPU_SA,
					msg_length, CMD_MSG_INDEX_DPU_1);

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
				 * push the commands info in the command history arrays.
				 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when both commands are verified
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu1_active_rt, msg_data[0],
						msg_data[1], msg_data[0], msg_data[1], NULL );

				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu2_active_rt, msg_data[0],
						msg_data[1], msg_data[0], msg_data[1], NULL );
			}
		}
	} /* end else (size in byte) */
	return (status);
}

icu_status_code asw_PUS_8_1_5_4_CPU_STDM(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint8_t DCU_mask = 0;
	uint16_t msg_index_in_sl = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (6 Bytes) */
	if (size_in_bytes != 6) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract parameters from the param array
		 * the first four bytes are the function ID
		 */
		DPU = tc.TC_pkt_data[4];
		DCU_mask = tc.TC_pkt_data[5];
		if ((DPU != 1) && (DPU != 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) CPU_STDM_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DPU_CMD_ID;
			msg_data[2] = DCU_mask;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}
			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, msg_length,
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
				 * push the commands info in the command history arrays.
				 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when command is verified
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) rt_addr, msg_data[0], msg_data[1], 0,
						0, NULL );
			}
		}
	} /* end else (size in byte) */

	return (status);
}

icu_status_code asw_PUS_8_1_5_5_DRB_SHSK(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint16_t mode = 0;
	uint16_t msg_index_in_sl = 0;
	/*
	 * check ICU status - should be MANUAL - TODO
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (7 Bytes) */
	if (size_in_bytes != 7) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract parameters from the param array
		 * the first four bytes are the function ID
		 */
		DPU = tc.TC_pkt_data[4];
		mode = (uint16_t) ((uint16_t) tc.TC_pkt_data[5] << 8U)
				| (uint16_t) tc.TC_pkt_data[6];
		if ((DPU != 1) && (DPU != 2)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) DRB_SHSK_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DPU_CMD_ID;
			msg_data[2] = mode;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}
			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) rt_addr), rt_addr,
					RECEIVE_BIT_1553, COMMAND_DPU_SA, msg_length,
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
				 * push the commands info in the command history arrays.
				 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when command is verified
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) rt_addr, msg_data[0], msg_data[1], 0,
						0, NULL );
			}
		}
	} /* end else (size in byte) */
	return (status);
}

icu_status_code asw_PUS_8_1_5_11_CPU_NOP(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_8_1_5_12_CPU_RSTC(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t size_in_bytes = 0;
	uint8_t dest_param = 0;

	/*
	 * check ICU status? - any state!
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (5 Bytes) */
	if (size_in_bytes != 5) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract parameters from the param array
		 * the first four bytes are the function ID
		 */
		dest_param = tc.TC_pkt_data[4];
		if ((dest_param == 0) || (dest_param > 3)) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) CPU_RSTC_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DPU_CMD_ID;
			msg_data[2] = dest_param;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
					RT_BROADCAST_1553, RECEIVE_BIT_1553, COMMAND_DPU_SA,
					msg_length, CMD_MSG_INDEX_DPU_1);

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
				 * push the commands info in the command history arrays.
				 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when both commands are verified
				 */
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu1_active_rt, msg_data[0],
						msg_data[1], msg_data[0], msg_data[1], NULL );

				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu2_active_rt, msg_data[0],
						msg_data[1], msg_data[0], msg_data[1], NULL );
			}
		}
	} /* end else (size in byte) */

	return (status);
}

icu_status_code asw_PUS_8_1_5_14_CPU_WDG(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_8_1_5_15_CPU_MTST(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_8_1_5_16_CPU_MBE(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint16_t msg_index_in_sl = 0;
	/*
	 * check ICU status? - must be CPU SAFE
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (5 Bytes) */
	if (size_in_bytes != 5) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract parameters from the param array
		 * the first four bytes are the function ID
		 */
		DPU = tc.TC_pkt_data[4];
		if (DPU > 2) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) CPU_MBE_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DPU_CMD_ID;

			msg_length = 2;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

			if (DPU == 0) {
				/* broadcast */
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
						RT_BROADCAST_1553, RECEIVE_BIT_1553, COMMAND_DPU_SA,
						msg_length, CMD_MSG_INDEX_DPU_1);

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
					 * push the commands info in the command history arrays.
					 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
					 * when both commands are verified
					 */
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu1_active_rt,
							msg_data[0], msg_data[1], msg_data[0],
							msg_data[1], NULL );

					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) dpu2_active_rt,
							msg_data[0], msg_data[1], msg_data[0],
							msg_data[1], NULL );
				}
			} else {
				if (DPU == 1) {
					rt_addr = dpu1_active_rt;
					msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
				} else {
					rt_addr = dpu2_active_rt;
					msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
				}
				status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
						(BASE_COMMAND_CF + (uint8_t) rt_addr), rt_addr,
						RECEIVE_BIT_1553, COMMAND_DPU_SA, msg_length,
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
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
				} else {
					/*
					 * push the commands info in the command history arrays.
					 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
					 * when the command is verified
					 */
					status = asw_push_cmd_tag(tc.data_field.ack,
							tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
							tc.data_field.sourceId, (uint8_t) rt_addr,
							msg_data[0], msg_data[1], 0, 0, NULL );
				}
			}
		} /* end else (check on parameters) */
	} /* end else (size in byte) */

	return (status);
}

icu_status_code asw_PUS_8_1_5_18_CPU_DABT(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t size_in_bytes = 0;

	/*
	 * check ICU status - should be MANUAL (?!) - TODO
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (4 Bytes) */
	if (size_in_bytes != 4) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * prepare the command packet.
		 */
		msg_data[0] = ((uint16_t) CPU_DABT_COMMAND_ID << COMMAND_ID_OFFSET)
				| (uint16_t) DPU_CMD_ID;
		msg_length = 2;
		for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
			msg_data[j] = 0;
		}

		/*
		 * update here the ICU command counter
		 * Broadcast commands are sent in the first command CF
		 */
		msg_data[1] = ++ICU_command_counter;

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
				(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt), RT_BROADCAST_1553,
				RECEIVE_BIT_1553, COMMAND_DPU_SA, msg_length,
				CMD_MSG_INDEX_DPU_1);
		if (status != ICU_SUCCESSFUL) {
			error_param_length = sizeof(icu_status_code);
			/*
			 * status is now containing the error code from
			 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
			 */
			memcpy(error_param, &status, sizeof(icu_status_code));
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_BC_1553_RW_ERROR, error_param_length, error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
		} else {

			/*
			 * push the commands info in the command history arrays.
			 * The CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
			 * when both commands are verified
			 */
			status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					(uint8_t) dpu1_active_rt, msg_data[0], msg_data[1],
					msg_data[0], msg_data[1], NULL );

			status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					(uint8_t) dpu2_active_rt, msg_data[0], msg_data[1],
					msg_data[0], msg_data[1], NULL );
		}
	} /* end else (size in byte) */

	return (status);
}

icu_status_code asw_PUS_8_1_5_20_DRB_CSPW(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_CPU_GTAB(uint16_t rt_address, uint8_t sa9_16_table,
		uint8_t sa22_table) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint8_t msg_length = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * TODO: check the DPU status: any status?!
	 */

	/*
	 * prepare the command packet message
	 */
	msg_data[0] = ((uint16_t) CPU_GTAB_COMMAND_ID << COMMAND_ID_OFFSET)
			| (uint16_t) DPU_CMD_ID;
	msg_data[2] = sa9_16_table;
	msg_data[3] = sa22_table;
	msg_length = 4;

	for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
		msg_data[j] = 0;
	}

	msg_data[1] = ++ICU_command_counter;

	/* Select message index */
	if (rt_address < RT_DPU_2_NOM) {
		msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
	} else {
		msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
			(BASE_COMMAND_CF + (uint8_t) rt_address), rt_address,
			RECEIVE_BIT_1553, COMMAND_DPU_SA, msg_length, msg_index_in_sl);

	if (status != ICU_SUCCESSFUL) {
		status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
	} else {
		/*
		 * push the command info in the command history arrays.
		 */

		status = asw_push_cmd_tag(0, 0, 0, 0, (uint8_t) rt_address,
				msg_data[0], msg_data[1], 0, 0, NULL );
	}
	return (status);
}
