/**
 * \file	PUS_Service_8_1_DPU_BSW.c
 *
 * \brief	PUS Service 8: commands toward DPU BSW
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 * \author  Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 19, 2017
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_8_1_11_1_load_ASW_img(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint8_t ASW_image = 0;
	uint16_t msg_index_in_sl = 0;

	/*
	 * check ICU status - should be MANUAL (?!) - TODO
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (6 Bytes)
	 * function ID (4 B), DPU (1 B), ASW image (1 B) */
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
		ASW_image = tc.TC_pkt_data[5];
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
			msg_data[0] = ((uint16_t) 14 << BSW_CMD_TYPE_OFFSET)
					| (uint16_t) DPU_BSW_COPY_IMG_2_RAM_CMD_ID;
			msg_data[2] = (uint16_t) 3 << 8 | 0x00FFU;
			msg_data[3] = (uint16_t) ASW_image;

			msg_length = 4;
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
				 * The BSW CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when command is verified
				 */
				status = asw_push_bsw_cmd_tag(tc.data_field.ack,
						tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
						tc.data_field.sourceId, (uint8_t) rt_addr,
						msg_data[0], msg_data[1], NULL );
			}
		}
	} /* end else (size in byte) */

	return (status);
}

icu_status_code asw_PUS_8_1_11_3_jump_to_ASW(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint32_t ASW_address = 0;
	uint16_t msg_index_in_sl = 0;
	/*
	 * check ICU status - should be MANUAL (?!) - TODO
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (6 Bytes)
	 * function ID (4 B), DPU (1 B), ASW address (4 B) */
	if (size_in_bytes != 9) {
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
		memcpy(&ASW_address, &tc.TC_pkt_data[5], sizeof(uint32_t));

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
			msg_data[0] = ((uint16_t) 14 << BSW_CMD_TYPE_OFFSET)
					| (uint16_t) DPU_BSW_JUMP_TO_ASW_CMD_ID;
			msg_data[2] = (uint16_t) 5 << 8 | 0x00FFU;
			msg_data[3] = (uint16_t) ((ASW_address & 0xFFFF0000U) >> 16U);
			msg_data[4] = (uint16_t) (ASW_address & 0x0000FFFFU);

			msg_length = 5;
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
				 * The BSW CMD_VER_TAB interpreter is in charge of generating the TM(1,8)
				 * only if the command is not verified
				 */
				status = asw_push_bsw_cmd_tag(tc.data_field.ack,
						tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
						tc.data_field.sourceId, (uint8_t) rt_addr,
						msg_data[0], msg_data[1], NULL );
			}
		}
	} /* end else (size in byte) */

	return (status);
}

/**
 * \brief This function implements the DPU BSW command that manages the FDIR of the BSW.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_11_7_BSW_FDIR(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t j = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t msg_length = 0;
	uint16_t rt_addr = 0;
	uint16_t size_in_bytes = 0;
	uint8_t DPU = 0;
	uint8_t op_type = 0;
	uint8_t cmd_params = 0;
	uint16_t synch = 0;
	uint16_t msg_index_in_sl = 0;
	/*
	 * check ICU status - should be MANUAL (?!) - TODO
	 * ICU_ILLEGAL_STATE
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* expected size is fixed (9 Bytes)
	 * function ID (4 B), DPU (1 B), operation type (1 B),
	 * command params (1 B), synch/refresh rate (2 B)  */
	if (size_in_bytes != 9) {
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
		op_type = tc.TC_pkt_data[5];
		cmd_params = tc.TC_pkt_data[6];
		memcpy(&synch, &tc.TC_pkt_data[7], sizeof(uint16_t));

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
			msg_data[0] = ((uint16_t) 14 << BSW_CMD_TYPE_OFFSET)
					| (uint16_t) DPU_BSW_CPU_FDIR_CMD_ID;
			msg_data[2] = (uint16_t) 7 << 8 | 0x00FFU;
			msg_data[3] = (uint16_t) op_type;
			msg_data[4] = (uint16_t) cmd_params;
			msg_data[5] = synch;

			msg_length = 6;
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
			}

			else {
				/*
				 * push the commands info in the command history arrays.
				 * The BSW CMD_VER_TAB interpreter is in charge of generating the TM(1,7)/(1,8)
				 * when command is verified
				 */
				status = asw_push_bsw_cmd_tag(tc.data_field.ack,
						tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
						tc.data_field.sourceId, (uint8_t) rt_addr,
						msg_data[0], msg_data[1], NULL );
			}
		}
	} /* end else (size in byte) */

	return (status);
}
