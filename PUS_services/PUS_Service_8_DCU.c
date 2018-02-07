/**
 * \file	PUS_Service_8_DCU.c
 *
 * \brief	PUS Service 8: commands toward DCUs
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 * \author  Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Jun 22, 2017
 */

#include "interface/PUS_Service_8_DCU.h"

icu_status_code asw_PUS_8_1_6_1_DCU_PWONOFF(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint8_t DCU = 0;
	uint8_t sce_onoff = 0;
	uint8_t sce_cmd_id = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * check ICU status - should be MANUAL (?!) - TODO
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
		DCU = tc.TC_pkt_data[5];
		sce_onoff = tc.TC_pkt_data[6];

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
			if (DCU == 0) {
				sce_cmd_id = DCUSCE_CMD_BRD_TAB_ID;
			} else {
				sce_cmd_id = DCUSCE_CMD_TAB1_ID + DCU;
			}

			/*
			 * prepare the command packet.
			 */
			msg_data[0] =
					((uint16_t) DCU_PWONFF_COMMAND_ID << COMMAND_ID_OFFSET)
							| sce_cmd_id;
			msg_data[2] = sce_onoff;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

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
						(uint8_t) rt_addr, msg_data[0], msg_data[1],
						0, 0, NULL );
			}
		} /* end else (check on parameters) */
	} /* end else (size in byte) */
	return (status);
}

icu_status_code asw_PUS_8_1_6_7_DCU_SYNC(telecommand_t tc) {
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
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) DCU_SYNC_COMMAND_ID << COMMAND_ID_OFFSET)
					| DCUSCE_CMD_BRD_TAB_ID;

			msg_length = 2;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

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
		} /* end else (check on parameters) */
	} /* end else (size in byte) */
	return (status);

}

icu_status_code asw_PUS_8_1_6_8_DCU_RST(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint8_t DCU = 0;
	uint8_t dcu_mode = 0;
	uint8_t sce_cmd_id = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * check ICU status - should be MANUAL (?!) - TODO
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
		DCU = tc.TC_pkt_data[5];
		dcu_mode = tc.TC_pkt_data[6];

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
			sce_cmd_id = DCUSCE_CMD_TAB1_ID + (DCU - 1);

			/*
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) DCU_RST_COMMAND_ID << COMMAND_ID_OFFSET)
					| sce_cmd_id;
			msg_data[2] = dcu_mode;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

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
		} /* end else (check on parameters) */
	} /* end else (size in byte) */
	return (status);
}

icu_status_code asw_PUS_8_1_6_11_DCU_SHSK(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint16_t scanner_period = 0;
	uint8_t DPU = 0;
	uint16_t msg_index_in_sl = 0;
	/*
	 * check ICU status - should be MANUAL (?!) - TODO
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
		scanner_period = (uint16_t) ((uint16_t) tc.TC_pkt_data[5] << 8U)
				| (uint16_t) tc.TC_pkt_data[6];

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
			 * prepare the command packet.
			 */
			msg_data[0] = ((uint16_t) DCU_RST_COMMAND_ID << COMMAND_ID_OFFSET)
					| (uint16_t) DCUSCE_CMD_BRD_TAB_ID;
			msg_data[2] = scanner_period;

			msg_length = 3;
			for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
				msg_data[j] = 0;
			}

			if (DPU == 1) {
				rt_addr = dpu1_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
			} else {
				rt_addr = dpu2_active_rt;
				msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
			}

			/*
			 * update here the ICU command counter
			 */
			msg_data[1] = ++ICU_command_counter;

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
		} /* end else (check on parameters) */
	} /* end else (size in byte) */

	return (status);
}
