/**
 * \file	PUS_Service_8_DPU_config.c
 *
 * \brief	PUS Service 8: commands toward DPUs
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 * \author  Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 2, 2015
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_8_1_3_14_DPU_tx_dither(telecommand_t tc) {

	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t msg_length = 0, command_id = 0;
	uint8_t exp_group_number[MAX_EXPOSURES_IN_DITHER],
			exp_framesPerGroup[MAX_EXPOSURES_IN_DITHER];
	uint16_t droplines1[MAX_EXPOSURES_IN_DITHER],
			droplines2[MAX_EXPOSURES_IN_DITHER],
			instr_mode[MAX_EXPOSURES_IN_DITHER];
	//	uint16_t DPU;
	uint16_t dith_time = 0, j = 0;
	uint8_t DPMEM = 0, exp_number = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0, expect_TC_length = 0;

	/*
	 * check ICU status - should be MANUAL (?!) - TODO
	 * ICU_ILLEGAL_STATE
	 */

	/*
	 * extract parameters from the param array
	 * the first four bytes are the function ID
	 */
	exp_number = tc.TC_pkt_data[DITH_CONF_TC_EXP_NUMBER];
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	expect_TC_length = 3 + (7 * MAX_EXPOSURES_IN_DITHER) + 4;

	if (size_in_bytes != expect_TC_length) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (exp_number > MAX_EXPOSURES_IN_DITHER) {
		error_param_length = 1;
		error_param[0] = 1; // exposure number is the 1-st parameter of TC
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		dith_time = ((uint16_t) tc.TC_pkt_data[DITH_CONF_TC_DITHER_MAX_TIME]
				<< 8U)
				| (uint16_t) tc.TC_pkt_data[DITH_CONF_TC_DITHER_MAX_TIME + 1];

		for (j = 0; j < MAX_EXPOSURES_IN_DITHER; j++) {
			exp_framesPerGroup[j] =
					tc.TC_pkt_data[DITH_CONF_TC_EXP_1_FRAMES_NUMB + (7 * j)];
			exp_group_number[j] = tc.TC_pkt_data[DITH_CONF_TC_EXP_1_GROUPS_NUMB
					+ (7 * j)];
			droplines1[j] =
					((uint16_t) tc.TC_pkt_data[DITH_CONF_TC_EXP_1_DL_1_MSB
							+ (7 * j)] << 8U)
							| (uint16_t) tc.TC_pkt_data[DITH_CONF_TC_EXP_1_DL_1_MSB
									+ (7 * j) + 1];
			droplines2[j] = (uint16_t) (0U
					| tc.TC_pkt_data[(DITH_CONF_TC_EXP_1_DL_2_LSB + (7 * j))]);
			instr_mode[j] =
					((uint16_t) tc.TC_pkt_data[DITH_CONF_TC_EXP_1_INS_MODE_MSB
							+ (7 * j)] << 8U)
							| (uint16_t) tc.TC_pkt_data[DITH_CONF_TC_EXP_1_INS_MODE_MSB
									+ (7 * j) + 1];
		}

		/*
		 * fill the global structure.
		 */
		/* choose DPMEM */
		if (DPU_in_use_DPMEM == 0) {
			DPMEM = 1;
			DPU_in_use_DPMEM = 1;
		} else {
			DPMEM = 0;
			DPU_in_use_DPMEM = 0;
		}

		asw_set_dither_table_values(&dith_config_tab, exp_number, DPMEM,
				dith_time, exp_group_number, exp_framesPerGroup, droplines1,
				droplines2, instr_mode);

		/*
		 * prepare the command packet. The table is broadcasted to both DPUs
		 */
		command_id = 0; // not meaningful, EUCL_OPD_TN_7-004 2.1 20/01/16
		msg_data[0] = (command_id << COMMAND_ID_OFFSET)
				| (uint16_t) DPU_HL_SEND_DITHER_TAB_CMD_ID;
		msg_data[2] = ((uint16_t) dith_config_tab.number_of_exposures << 8U)
				| (uint16_t) dith_config_tab.DPMEM;
		msg_data[3] = dith_config_tab.dither_max_time;
		for (j = 0; j < MAX_EXPOSURES_IN_DITHER; j++) {
			msg_data[4 + (4 * j)] =
					((uint16_t) dith_config_tab.exposure_param[j].group_number
							<< 8U)
							| (uint16_t) dith_config_tab.exposure_param[j].framesPerGroup;
			msg_data[5 + (4 * j)] =
					dith_config_tab.exposure_param[j].dropLines1;
			msg_data[6 + (4 * j)] =
					dith_config_tab.exposure_param[j].dropLines2;
			msg_data[7 + (4 * j)] =
					dith_config_tab.exposure_param[j].instrument_mode;
		}

		msg_length = 4 + (4 * MAX_EXPOSURES_IN_DITHER);
		for (j = msg_length; j < BLOCK_SIZE_IN_WORDS16; j++) {
			msg_data[j] = 0;
		}

		/*
		 * update here the ICU command counter - fill also the dither_tab structure
		 */
		dith_config_tab.counter = ++ICU_command_counter;
		msg_data[1] = dith_config_tab.counter;

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
				(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt), RT_BROADCAST_1553,
				RECEIVE_BIT_1553, DITHER_TAB_SA, msg_length,
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
					(uint8_t) dpu1_active_rt, msg_data[0],
					dith_config_tab.counter, msg_data[0],
					dith_config_tab.counter, NULL );

			status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					(uint8_t) dpu2_active_rt, msg_data[0],
					dith_config_tab.counter, msg_data[0],
					dith_config_tab.counter, NULL );
		}
	}

	return (status);
}

icu_status_code asw_PUS_8_1_3_15_DPU_tx_procPar(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t pos = 0;
	uint8_t raw_lines_number = 0;
	uint16_t DPU = 0, proc_mode = 0, ffstat_box_size = 0,
			ffstat_box_Start_coord[2] = { 0, 0 };
	uint16_t compr_bck_size = 0, compr_FSbits = 0, compr_FSmax = 0,
			compr_prescaling = 0;
	int16_t det_final_ima_offset = 0;
	uint8_t Chi2_pix_thrhold[8], RON[8];
	uint16_t length = 0, command_id = 0, i = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* the first four bytes are the function ID */
	if (size_in_bytes != PROC_PAR_TC_BYTE_NUMBER) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {

		/*
		 * extract parameters from the input array
		 */
		DPU = tc.TC_pkt_data[PROC_PAR_TC_DPU_ID];
		proc_mode = tc.TC_pkt_data[PROC_PAR_TC_PROC_MODE_ID];
		for (i = 0; i < 8; i++) {
			Chi2_pix_thrhold[i] = tc.TC_pkt_data[PROC_PAR_TC_CHI2_TSH_FIRST_ID
					+ i];
		}
		for (i = 0; i < 8; i++) {
			RON[i] = tc.TC_pkt_data[PROC_PAR_TC_RON_FIRST_ID + i];
		}
		ffstat_box_size =
				((uint16_t) tc.TC_pkt_data[PROC_PAR_TC_BOX_SIZE_MSB_ID] << 8U)
						| (uint16_t) tc.TC_pkt_data[PROC_PAR_TC_BOX_SIZE_LSB_ID];
		ffstat_box_Start_coord[0] =
				((uint16_t) tc.TC_pkt_data[PROC_PAR_TC_BOX_START_X_MSB_ID] << 8U)
						| (uint16_t) tc.TC_pkt_data[PROC_PAR_TC_BOX_START_X_LSB_ID];
		ffstat_box_Start_coord[1] =
				((uint16_t) tc.TC_pkt_data[PROC_PAR_TC_BOX_START_Y_MSB_ID] << 8U)
						| (uint16_t) tc.TC_pkt_data[PROC_PAR_TC_BOX_START_Y_LSB_ID];
		compr_bck_size =
				((uint16_t) tc.TC_pkt_data[PROC_PAR_TC_CPR_BCK_SIZE_MSB_ID]
						<< 8U)
						| (uint16_t) tc.TC_pkt_data[PROC_PAR_TC_CPR_BCK_SIZE_LSB_ID];
		compr_FSbits = ((uint16_t) tc.TC_pkt_data[PROC_PAR_TC_CPR_FSBITS_MSB_ID]
				<< 8U)
				| (uint16_t) tc.TC_pkt_data[PROC_PAR_TC_CPR_FSBITS_LSB_ID];
		compr_FSmax = ((uint16_t) tc.TC_pkt_data[PROC_PAR_TC_CPR_FSMAX_MSB_ID]
				<< 8U)
				| (uint16_t) tc.TC_pkt_data[PROC_PAR_TC_CPR_FSMAX_LSB_ID];
		compr_prescaling =
				(uint16_t) tc.TC_pkt_data[PROC_PAR_TC_CPR_PRESCALING_ID];
		det_final_ima_offset =
				((uint16_t) tc.TC_pkt_data[PROC_PAR_TC_IMG_OFFSET_MSB_ID] << 8U)
						| (uint16_t) tc.TC_pkt_data[PROC_PAR_TC_IMG_OFFSET_LSB_ID];
		raw_lines_number = tc.TC_pkt_data[PROC_PAR_TC_RAW_LINES_ID];

		/*
		 * fill the global structure proc_param_tab
		 */
		switch (DPU) {
		case 0: { /* broadcast */
			asw_set_proc_param_table_values(&proc_param_tab[0], proc_mode,
					Chi2_pix_thrhold, RON, ffstat_box_size,
					ffstat_box_Start_coord, compr_bck_size, compr_FSbits,
					compr_FSmax, compr_prescaling, det_final_ima_offset,
					raw_lines_number);
			asw_set_proc_param_table_values(&proc_param_tab[1], proc_mode,
					Chi2_pix_thrhold, RON, ffstat_box_size,
					ffstat_box_Start_coord, compr_bck_size, compr_FSbits,
					compr_FSmax, compr_prescaling, det_final_ima_offset,
					raw_lines_number);
			break;
		}
		case 1: { /* DPU A */
			asw_set_proc_param_table_values(&proc_param_tab[0], proc_mode,
					Chi2_pix_thrhold, RON, ffstat_box_size,
					ffstat_box_Start_coord, compr_bck_size, compr_FSbits,
					compr_FSmax, compr_prescaling, det_final_ima_offset,
					raw_lines_number);
			break;
		}
		case 2: { /* DPU B */
			asw_set_proc_param_table_values(&proc_param_tab[1], proc_mode,
					Chi2_pix_thrhold, RON, ffstat_box_size,
					ffstat_box_Start_coord, compr_bck_size, compr_FSbits,
					compr_FSmax, compr_prescaling, det_final_ima_offset,
					raw_lines_number);
			break;
		}
		}

		/*
		 * prepare the command packet. If DPU = 0, the same table should be
		 * broadcasted to both DPUs
		 */

		uint8_t DPU_id = 0;

		switch (DPU) {
		case 0:
		case 1: {
			DPU_id = 0; /* DPU A */
			break;
		}
		case 2: {
			DPU_id = 1; /* DPU B */
			break;
		}
			// DEFAULT: at this level, DPU should have been checked...
		}

		/*
		 * prepare the 1553 message data
		 */

		command_id = 0; // not meaningful, EUCL_OPD_TN_7-004 2.1 20/01/16
		msg_data[0] = (command_id << COMMAND_ID_OFFSET)
				| (uint16_t) proc_param_tab[DPU_id].table_ID;
		/* postponed before pushing command in a sendlist */
		pos = 2;
		msg_data[pos] = proc_param_tab[DPU_id].processing_mode;
		pos++;
		for (i = 0; i < 4; i++) {
			msg_data[pos + i] =
					((uint16_t) proc_param_tab[DPU_id].Chi2_pixel_threshold[2
							* i] << 8U)
							| (uint16_t) proc_param_tab[DPU_id].Chi2_pixel_threshold[(2
									* i) + 1];
		}
		pos += i; // pos = 7

		i = 0;
		for (i = 0; i < 4; i++) {
			msg_data[pos + i] = ((uint16_t) proc_param_tab[DPU_id].RON[2 * i]
					<< 8U) | (uint16_t) proc_param_tab[DPU_id].RON[(2 * i) + 1];
		}
		pos += i; // pos=11
		msg_data[pos] = proc_param_tab[DPU_id].final_frame_stat_box_size;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].final_frame_stat_box_xStart;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].final_frame_stat_box_yStart;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].compression_block_size;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].compression_FSbits;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].compression_FSmax;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].compression_preScaling_factor;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].det_chain_final_image_offset;
		pos++;
		msg_data[pos] = proc_param_tab[DPU_id].raw_lines_number;

		length = pos + 1; // length in DW of the parameters component
		for (i = length; i < BLOCK_SIZE_IN_WORDS16; i++) {
			msg_data[i] = 0;
		}

		/*
		 * prepare command word and send it
		 */
		switch (DPU) {
		case 0: {
			/* Broadcast */
			proc_param_tab[0].counter = ++ICU_command_counter;
			proc_param_tab[1].counter = proc_param_tab[0].counter;

			msg_data[1] = proc_param_tab[0].counter;
			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
					RT_BROADCAST_1553, RECEIVE_BIT_1553, PROC_PARAM_TAB_SA,
					length, CMD_MSG_INDEX_DPU_1);
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
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu1_active_rt, msg_data[0],
						proc_param_tab[0].counter, msg_data[0],
						proc_param_tab[1].counter, NULL );

				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu2_active_rt, msg_data[0],
						proc_param_tab[1].counter, msg_data[0],
						proc_param_tab[0].counter, NULL );
			}
			break;
		}
		case 1: {
			/* Only to DPU_1 */
			proc_param_tab[0].counter = ++ICU_command_counter;
			msg_data[1] = proc_param_tab[0].counter;

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt),
					dpu1_active_rt, RECEIVE_BIT_1553, PROC_PARAM_TAB_SA, length,
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
						TM_1_8_BC_1553_RW_ERROR, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			} else {
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu1_active_rt, msg_data[0],
						proc_param_tab[0].counter, 0, 0, NULL );
			}
			break;
		}
		case 2: {
			/* Only to DPU_2 */
			proc_param_tab[1].counter = ++ICU_command_counter;
			msg_data[1] = proc_param_tab[1].counter;

			status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
					(BASE_COMMAND_CF + (uint8_t) dpu2_active_rt),
					dpu2_active_rt, RECEIVE_BIT_1553, PROC_PARAM_TAB_SA, length,
					CMD_MSG_INDEX_DPU_2);
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
				status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						(uint8_t) dpu2_active_rt, msg_data[0],
						proc_param_tab[1].counter, 0, 0, NULL );
			}
			break;
		}
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_3_3_DPU_tx_sysConf(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t error_param_length = 0;
	uint16_t TRP_err_WD = 0, DRB_EDAC_enabling = 0, DBB_EDAC_enabling = 0;
	uint8_t exp_WD_poll_time = 0, dith_WD_poll_time = 0,
			detect_WD_poll_time = 0, bus_WD_poll_time = 0;
	uint16_t DC_DC_thresh;
	uint16_t DCU_thresh;
	uint16_t DBB_thresh;
	uint16_t CPU_thresh;
	uint16_t temp_thresh_cnt = 0;
	uint16_t length = 0, command_id = 0;
	uint16_t msg_data[BLOCK_SIZE_IN_WORDS16];
	uint16_t size_in_bytes = 0;

	/*
	 * Extract parameters from the input array
	 * the first four bytes are the function ID
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != SYS_PARAM_TC_BYTE_NUMBER) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		exp_WD_poll_time = tc.TC_pkt_data[SYS_PARAM_EXP_TERMIN_WD];
		dith_WD_poll_time = tc.TC_pkt_data[SYS_PARAM_DITH_TERMIN_WD];
		detect_WD_poll_time = tc.TC_pkt_data[SYS_PARAM_DET_TRANSM_TERMIN_WD];
		bus_WD_poll_time = tc.TC_pkt_data[SYS_PARAM_1553_WD_ID];
		TRP_err_WD = ((uint16_t) tc.TC_pkt_data[SYS_PARAM_TRP_ERROR_WD_MSB]
				<< 8U) | (uint16_t) tc.TC_pkt_data[SYS_PARAM_TRP_ERROR_WD_LSB];
		DRB_EDAC_enabling =
				((uint16_t) tc.TC_pkt_data[SYS_PARAM_DRB_EDAC_ON_OFF_MSB] << 8U)
						| (uint16_t) tc.TC_pkt_data[SYS_PARAM_DRB_EDAC_ON_OFF_LSB];
		DBB_EDAC_enabling =
				((uint16_t) tc.TC_pkt_data[SYS_PARAM_DBB_EDAC_ON_OFF_MSB] << 8U)
						| (uint16_t) tc.TC_pkt_data[SYS_PARAM_DBB_EDAC_ON_OFF_LSB];
		DC_DC_thresh = ((uint16_t) tc.TC_pkt_data[SYS_PARAM_DC_DC_TEMP_MSB]
				<< 8U) | (uint16_t) tc.TC_pkt_data[SYS_PARAM_DC_DC_TEMP_LSB];
		DCU_thresh = ((uint16_t) tc.TC_pkt_data[SYS_PARAM_DCU_TEMP_MSB] << 8U)
				| (uint16_t) tc.TC_pkt_data[SYS_PARAM_DCU_TEMP_LSB];
		DBB_thresh = ((uint16_t) tc.TC_pkt_data[SYS_PARAM_DBB_TEMP_MSB] << 8U)
				| (uint16_t) tc.TC_pkt_data[SYS_PARAM_DBB_TEMP_LSB];
		CPU_thresh = ((uint16_t) tc.TC_pkt_data[SYS_PARAM_CPU_TEMP_MSB] << 8U)
				| (uint16_t) tc.TC_pkt_data[SYS_PARAM_CPU_TEMP_LSB];
		temp_thresh_cnt = ((uint16_t) tc.TC_pkt_data[SYS_PARAM_TEMP_COUNTER_MSB]
				<< 8U) | (uint16_t) tc.TC_pkt_data[SYS_PARAM_TEMP_COUNTER_LSB];

		/*
		 * fill the global structure.
		 */
		asw_set_sys_config_table_values(&sys_config_tab, exp_WD_poll_time,
				dith_WD_poll_time, detect_WD_poll_time, bus_WD_poll_time,
				TRP_err_WD, DRB_EDAC_enabling, DBB_EDAC_enabling, DC_DC_thresh,
				DCU_thresh, DBB_thresh, CPU_thresh, temp_thresh_cnt);

		/*
		 * prepare the command packet, to be sent to RT 31
		 */

		command_id = 0; // not meaningful, EUCL_OPD_TN_7-004 2.1 20/01/16
		msg_data[0] = (command_id << COMMAND_ID_OFFSET)
				| (uint16_t) sys_config_tab.table_ID;
		msg_data[2] = (int16_t) sys_config_tab.exp_WD_time;
		msg_data[3] = (int16_t) sys_config_tab.dither_WD_time;
		msg_data[4] = (int16_t) sys_config_tab.det_transm_WD_time;
		msg_data[5] = (int16_t) sys_config_tab.bus1553_WD_time;
		msg_data[6] = sys_config_tab.TRP_error_WD_time;
		msg_data[7] = sys_config_tab.DRB_EDAC_ON_OFF;
		msg_data[8] = sys_config_tab.DBB_EDAC_ON_OFF;
		msg_data[9] = sys_config_tab.DC_DC_temp_thresh;
		msg_data[10] = sys_config_tab.DCU_temp_thresh;
		msg_data[11] = sys_config_tab.DBB_temp_thresh;
		msg_data[12] = sys_config_tab.CPU_temp_thresh;
		msg_data[13] = sys_config_tab.temp_thresh_cnt;
		length = 14;

		/*
		 * prepare command word and send it
		 */
		sys_config_tab.counter = ++ICU_command_counter;
		msg_data[1] = sys_config_tab.counter;

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, msg_data,
				(BASE_COMMAND_CF + (uint8_t) dpu1_active_rt), RT_BROADCAST_1553,
				RECEIVE_BIT_1553, SYS_CONFIG_TAB_SA, length,
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
			 * push the command info in the command history array for the completion TM
			 * (Service 1)
			 */
			status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					(uint8_t) dpu1_active_rt, msg_data[0],
					sys_config_tab.counter, msg_data[0], sys_config_tab.counter,
					NULL );

			status = asw_push_cmd_tag(tc.data_field.ack, tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					(uint8_t) dpu2_active_rt, msg_data[0],
					sys_config_tab.counter, msg_data[0], sys_config_tab.counter,
					NULL );
		}
	}
	return (status);
}

/*
 * ********************************************************************************************
 * Auxiliary functions
 * ********************************************************************************************
 */
void asw_set_dither_table_values(dither_config_table_t *dither_table,
		uint8_t exp_number, uint8_t DPMEM, uint16_t dith_time,
		const uint8_t *exp_group_number, const uint8_t *exp_framePerGroup,
		const uint16_t *exp_dl1, const uint16_t *exp_dl2,
		const uint16_t *exp_instr_mode) {
	uint8_t i = 0;

	/*	dither_table->counter = ++ICU_command_counter;  */
	dither_table->number_of_exposures = exp_number;
	dither_table->DPMEM = DPMEM;
	dither_table->dither_max_time = dith_time;
	for (i = 0; i < exp_number; i++) {
		dither_table->exposure_param[i].group_number = exp_group_number[i];
		dither_table->exposure_param[i].framesPerGroup = exp_framePerGroup[i];
		dither_table->exposure_param[i].dropLines1 = exp_dl1[i];
		dither_table->exposure_param[i].dropLines2 = exp_dl2[i];
		dither_table->exposure_param[i].instrument_mode = exp_instr_mode[i];
	}
	for (i = exp_number; i < MAX_EXPOSURES_IN_DITHER; i++) {
		dither_table->exposure_param[i].group_number = 0;
		dither_table->exposure_param[i].framesPerGroup = 0;
		dither_table->exposure_param[i].dropLines1 = 0;
		dither_table->exposure_param[i].dropLines2 = 0;
		dither_table->exposure_param[i].instrument_mode = 0;
	}

	return;
}

void asw_set_proc_param_table_values(proc_param_table_t *proc_par_tab,
		uint16_t proc_mode, const uint8_t *Chi2_pix_thrhold, const uint8_t *RON,
		uint16_t ffstat_box_size, const uint16_t *ffstat_box_Start_coord,
		uint16_t compr_bck_size, uint16_t compr_FSbits, uint16_t compr_FSmax,
		uint16_t compr_prescal, int16_t det_final_ima_offset,
		uint8_t raw_lines_number) {
	uint8_t i = 0;
	proc_par_tab->processing_mode = proc_mode;
	for (i = 0; i < 8; i++) {
		proc_par_tab->Chi2_pixel_threshold[i] = Chi2_pix_thrhold[i];
	}
	for (i = 0; i < 8; i++) {
		proc_par_tab->RON[i] = RON[i];
	}
	proc_par_tab->final_frame_stat_box_size = ffstat_box_size;
	proc_par_tab->final_frame_stat_box_xStart = ffstat_box_Start_coord[0];
	proc_par_tab->final_frame_stat_box_yStart = ffstat_box_Start_coord[1];
	proc_par_tab->compression_block_size = compr_bck_size;
	proc_par_tab->compression_FSbits = compr_FSbits;
	proc_par_tab->compression_FSmax = compr_FSmax;
	proc_par_tab->compression_preScaling_factor = compr_prescal;
	proc_par_tab->det_chain_final_image_offset = det_final_ima_offset;
	proc_par_tab->raw_lines_number = raw_lines_number;
	return;
}

void asw_set_sys_config_table_values(sys_config_table_t *sys_conf_tab,
		uint8_t exp_WD, uint8_t dith_WD, uint8_t det_WD, uint8_t bus_WD,
		uint16_t TRP_err_WD, uint16_t DRB_EDAC_enab, uint16_t DBB_EDAC_enab,
		uint16_t DC_DC_thresh, uint16_t DCU_thresh, uint16_t DBB_thresh,
		uint16_t CPU_thresh, uint16_t temp_thresh_cnt) {

	//	sys_config_tab->counter = ++ICU_command_counter;
	sys_conf_tab->exp_WD_time = (int8_t) exp_WD;
	sys_conf_tab->dither_WD_time = (int8_t) dith_WD;
	sys_conf_tab->det_transm_WD_time = (int8_t) det_WD;
	sys_conf_tab->bus1553_WD_time = (int8_t) bus_WD;
	sys_conf_tab->TRP_error_WD_time = TRP_err_WD;
	sys_conf_tab->DRB_EDAC_ON_OFF = DRB_EDAC_enab;
	sys_conf_tab->DBB_EDAC_ON_OFF = DBB_EDAC_enab;
	sys_conf_tab->DC_DC_temp_thresh = DC_DC_thresh;
	sys_conf_tab->DCU_temp_thresh = DCU_thresh;
	sys_conf_tab->DBB_temp_thresh = DBB_thresh;
	sys_conf_tab->temp_thresh_cnt = temp_thresh_cnt;

	return;
}
