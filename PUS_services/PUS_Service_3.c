/**
 * \file	PUS_Service_3.c
 *
 * \brief	PUS Service 3
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Apr 30, 2015
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_3_1_set_new_HK_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t i = 0, index = 0, tmp = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t monitor_mask = 1U << PARAM_MONIT_FLAG_OFFSET;
	uint16_t SID_num = 0, collection_int = 0, n_par = 0;
	uint16_t size_in_bytes = 0;
	uint16_t par[TM_REPORT_MAX_PARAM_NUMBER];
	uint16_t data_size = 0;
	uint32_t mask = 1;

	/*
	 * TODO check status - should be...
	 */

	/*
	 * extract parameters from input array
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	memcpy(&SID_num, &tc.TC_pkt_data[0], sizeof(uint16_t));
	memcpy(&collection_int, &tc.TC_pkt_data[2], sizeof(uint16_t));
	memcpy(&n_par, &tc.TC_pkt_data[4], sizeof(uint16_t));

	/*
	 * CHECK ON INPUT PARAMETERS:
	 *                 SID_number shall be <= TM_MAX_REPORT_NUMBER
	 *                 size_in_bytes should be 4 (from TC) + 6 + n_par * 2
	 */
	if (TM_report_number[tc.pkt_id.processId - ICU_ASW_APID]
			== TM_MAX_REPORT_NUMBER) {
		/*
		 * the maximum number of HK report yet reached. Refuse the new one
		 */
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_PUS3_TOO_MANY_HK_REP, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (n_par > TM_REPORT_MAX_PARAM_NUMBER) {
		error_param_length = 1;
		error_param[0] = 3; // npar is the 3-rd parameter of TC
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (size_in_bytes != (uint16_t) (6 + (n_par * 2))) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (SID_num > TM_MAX_REPORT_NUMBER) {
		/* TBC if needed */
		error_param_length = 1;
		error_param[0] = 1; // SID is the 1-st parameter of TC
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * check if it yet existing. If yes, return with an error
		 */
		mask = 1U << (SID_num - 1);
		if ((mask & TM_report_exists_flag[tc.pkt_id.processId - ICU_ASW_APID])
				!= 0) {
			error_param_length = 1;
			error_param[0] = 1; // SID is the 1-st parameter of TC
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			if (par != NULL ) {
				memcpy(par, &(tc.TC_pkt_data[6]), n_par * sizeof(uint16_t));
			}
			// !!!!!!!!! TBC : 0 should not be a valid parameter index !!!!!!!!!!!
			for (i = n_par; i < TM_REPORT_MAX_PARAM_NUMBER; i++) {
				par[i] = 0;
			}
			SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].SID_number =
					SID_num;
			SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].param_number =
					n_par;
			SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].collection_interval =
					collection_int;
			SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].generation_cnt =
					collection_int;

			/*
			 * prepare the data descriptor matrix
			 */
			for (i = 0; i < n_par; i++) {
				/*
				 * check if the parameters is monitored
				 */
				tmp = (par[i] & monitor_mask) >> PARAM_MONIT_FLAG_OFFSET;
				if (tmp == PARAM_MONITORED) {
					index = asw_PUS_3_ID_to_mon_DB_idx(par[i]);
					if (index < DB_MONITORED_PARAM_NUMB) {
						icu_sem_lock(monit_db_sem_Id, 5);
						data_size += monitored_db_param[index].size;
						icu_sem_unlock(monit_db_sem_Id);
					}
				} else {
					index = asw_PUS_3_ID_to_not_mon_DB_idx(par[i]);
					if (index < DB_MONITORED_PARAM_NUMB) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						data_size += db_param[index].size;
						icu_sem_unlock(not_mon_db_sem_Id);
					}
				}
				SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].SID_data[i].DB_index =
						index;
				SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].SID_data[i].DB_table =
						tmp;
			}

			if (data_size > TM_3_MAX_DATA_LENGTH_BYTE) {
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_PUS3_REPORT_SIZE_ERROR, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			} else {
				/*
				 * set to disabled by default and declare it is existing
				 */
				if ((((SID_num - 1) >= 0)
						&& ((SID_num - 1) < (8 * sizeof(uint32_t)))) == TRUE) {
					TM_report_flag[tc.pkt_id.processId - ICU_ASW_APID] &=
							~((uint32_t) 1U << (SID_num - 1));
					TM_report_exists_flag[tc.pkt_id.processId - ICU_ASW_APID] =
							TM_report_exists_flag[tc.pkt_id.processId
									- ICU_ASW_APID]
									| ((uint32_t) 1U << (SID_num - 1));
				}
				/*
				 * increment the counter of reports
				 */
				TM_report_number[tc.pkt_id.processId - ICU_ASW_APID]++;

				/*
				 * everything was ok! Generate a TM(1,7)
				 */
				if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
					status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId);
				}
			}

		} /* end else (SID yet existing) */
	} /* end else on parameters checks */

	return (status);
}

icu_status_code asw_PUS_3_3_clear_HK_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t sid_id = 0;
	uint8_t NSID = 0, exist_error = 0, i = 0, j = 0;
	uint16_t SID_num[TM_MAX_REPORT_NUMBER];

	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t size_in_bytes = 0;

	/*
	 * TODO check status - should be...
	 */

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	/*
	 * extract parameters from input array
	 */
	NSID = tc.TC_pkt_data[0];

	if (NSID > TM_MAX_REPORT_NUMBER) {
		error_param_length = 1;
		error_param[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (size_in_bytes != (uint16_t) (1 + (NSID * 2))) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		if (SID_num != NULL ) {
			memcpy(SID_num, &(tc.TC_pkt_data[1]), NSID * sizeof(uint16_t));
		}
		for (i = NSID; i < TM_MAX_REPORT_NUMBER; i++) {
			SID_num[i] = 0;
		}

		/*
		 *         SID_number shall be < TM_MAX_REPORT_NUMBER
		 */
		i = 0;
		while ((i < NSID) && (exist_error == 0)) {
			/* needed for the control of existence/enabled flags */
			if (((SID_num[i] > TM_MAX_REPORT_NUMBER) || (SID_num[i] == 0)) == TRUE) {
				error_param_length = 1;
				error_param[0] = 2 + i;
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				exist_error = 1;
			}
			i++;
		}

		if (exist_error == 0) {

			for (j = 0; j < NSID; j++) {
				sid_id = SID_num[j] - 1;
				if (((sid_id >= 0) && (sid_id < (8 * sizeof(uint32_t))))
						== TRUE) {
					TM_report_flag[tc.pkt_id.processId - ICU_ASW_APID] &=
							~((uint32_t) 1U << sid_id);
					TM_report_exists_flag[tc.pkt_id.processId - ICU_ASW_APID] &=
							~((uint32_t) 1U << sid_id);
				}
				/*
				 * set the collection interval and the number of parameters to zero
				 */
				SID[tc.pkt_id.processId - ICU_ASW_APID][sid_id].SID_number = 0;
				SID[tc.pkt_id.processId - ICU_ASW_APID][sid_id].param_number =
						0;
				SID[tc.pkt_id.processId - ICU_ASW_APID][sid_id].collection_interval =
						0;
				SID[tc.pkt_id.processId - ICU_ASW_APID][sid_id].generation_cnt =
						0;

				TM_report_number[tc.pkt_id.processId - ICU_ASW_APID]--;
			}

			/*
			 * everything was ok! Generate a TM(1,7)
			 */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}

	}/* end else on parameters checks */

	return (status);
}

icu_status_code asw_PUS_3_5_enable_HK_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t sid_id = 0;
	uint8_t NSID = 0, exist_error = 0, i = 0;
	uint32_t mask = 0;
	uint16_t SID_number[TM_MAX_REPORT_NUMBER], size_in_bytes = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;

	/*
	 * TODO check status - should be...
	 */

	/*
	 * extract parameters from input array
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	NSID = tc.TC_pkt_data[0];

	if (NSID > TM_MAX_REPORT_NUMBER) {
		error_param_length = 1;
		error_param[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
	} else if (size_in_bytes != (uint16_t) (1 + (NSID * 2))) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract last parameter
		 */
		if (SID_number != NULL ) {
			memcpy(SID_number, &(tc.TC_pkt_data[1]), NSID * sizeof(uint16_t));
		}
		for (i = NSID; i < TM_MAX_REPORT_NUMBER; i++) {
			SID_number[i] = 0;
		}

		/*
		 * CHECK ON INPUT PARAMETERS:
		 *                 SID_number shall be < TM_MAX_REPORT_NUMBER
		 */
		i = 0;
		while ((i < NSID) && (exist_error == 0)) {
			if (((SID_number[i] > TM_MAX_REPORT_NUMBER) || (SID_number[i] == 0)) == TRUE) {
				error_param_length = 1;
				error_param[0] = 2 + i;
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				exist_error = 1;
			} else {
				/*
				 * enable/disable flag: TM_report_flag=(SID_n, ..., SID_1)
				 * check if SID is existing
				 */
				sid_id = SID_number[i] - 1;
				if (((sid_id >= 0) && (sid_id < (8 * sizeof(uint32_t))))
						== TRUE) {
					mask = (uint32_t) 1U << sid_id;
				}
				if ((mask
						& TM_report_exists_flag[tc.pkt_id.processId
								- ICU_ASW_APID]) == 0) {
					error_param_length = 1;
					error_param[0] = 2 + i;
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					exist_error = 1;
				}
			}
			i++;
		}

		if (exist_error == 0) {
			for (i = 0; i < NSID; i++) {
				if (((sid_id >= 0) && (sid_id < (8 * sizeof(uint32_t))))
						== TRUE) {
					TM_report_flag[tc.pkt_id.processId - (uint16_t) ICU_ASW_APID] =
							TM_report_flag[tc.pkt_id.processId
									- (uint16_t) ICU_ASW_APID]
									| ((uint32_t) 1U << sid_id);
					SID[tc.pkt_id.processId - (uint16_t) ICU_ASW_APID][sid_id].generation_cnt =
							SID[tc.pkt_id.processId - (uint16_t) ICU_ASW_APID][sid_id].collection_interval;
				}
			}
			/* everything was ok! Generate a TM(1,7) */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	} /* end else on NSID and size checks */

	return (status);
}

icu_status_code asw_PUS_3_6_disable_HK_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t sid_id = 0;
	uint8_t NSID = 0, exist_error = 0, i = 0;
	uint32_t mask = 0;
	uint16_t SID_number[TM_MAX_REPORT_NUMBER];

	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t size_in_bytes = 0;

	/*
	 * TODO check status - should be...
	 */

	/*
	 * extract parameters from input array
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	NSID = tc.TC_pkt_data[0];

	if (NSID > TM_MAX_REPORT_NUMBER) {
		error_param_length = 1;
		error_param[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (size_in_bytes != (uint16_t) (1 + (NSID * 2))) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * extract last parameter
		 */
		if (SID_number != NULL ) {
			memcpy(SID_number, &(tc.TC_pkt_data[1]), NSID * sizeof(uint16_t));
		}
		for (i = NSID; i < TM_MAX_REPORT_NUMBER; i++) {
			SID_number[i] = 0;
		}

		/*
		 * CHECK ON INPUT PARAMETERS:
		 *                 SID_number shall be < TM_MAX_REPORT_NUMBER
		 */
		i = 0;
		while ((i < NSID) && (exist_error == 0)) {
			if (((SID_number[i] > TM_MAX_REPORT_NUMBER) || (SID_number[i] == 0)) == TRUE) {
				error_param_length = 1;
				error_param[0] = 2 + i;
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				exist_error = 1;
			} else {
				sid_id = SID_number[i] - 1;
				/*
				 * enable/disable flag: TM_report_flag=(SID_n, ..., SID_1)
				 * first check if it is existing
				 */
				if (((sid_id >= 0) && (sid_id < (8 * sizeof(uint32_t))))
						== TRUE) {
					mask = (uint32_t) 1U << sid_id;
				}
				if ((mask
						& TM_report_exists_flag[tc.pkt_id.processId
								- ICU_ASW_APID]) == 0) {
					error_param_length = 1;
					error_param[0] = 2 + i;
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				}
				exist_error = 1;
			}
			i++;
		}

		if (exist_error == 0) {
			for (i = 0; i < NSID; i++) {
				sid_id = SID_number[i] - 1;
				if (((sid_id >= 0) && (sid_id < (8 * sizeof(uint32_t))))
						== TRUE) {
					mask = (uint32_t) 1U << sid_id;
				}
				TM_report_flag[tc.pkt_id.processId - ICU_ASW_APID] &= ~(mask);
			}

			/*
			 * everything was ok! Generate a TM(1,7)
			 */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	} /* end else on check on parameter size and NSID */

	return (status);
}

/*
 * generate TM(3,10) packet
 */
icu_status_code asw_PUS_3_9_get_HK_report_def(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t SID_number = 0;
	uint32_t mask = 0;

	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t size_in_bytes = 0;

	/*
	 * TODO check status - should be...
	 */

	/*
	 * extract parameters from input array
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	memcpy(&SID_number, &tc.TC_pkt_data[0], sizeof(uint16_t));

	if (size_in_bytes != 2) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (((SID_number > TM_MAX_REPORT_NUMBER) || (SID_number == 0)) == TRUE) {
		error_param_length = 1;
		error_param[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		if ((((SID_number - 1) >= 0)
				&& ((SID_number - 1) < (8 * sizeof(uint32_t)))) == TRUE) {
			mask = (uint32_t) 1U << (SID_number - 1);
		}
		/*
		 * check if report is existing, otherwise halt
		 */
		if ((mask & TM_report_exists_flag[tc.pkt_id.processId - ICU_ASW_APID])
				== 0) {
			error_param_length = 1;
			error_param[0] = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			status = asw_PUS_3_10_TM_HK_report_def(tc.pkt_id.processId,
					SID_number);
			if (status != ICU_SUCCESSFUL) {
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_PUS3_TM_PKT_NOT_CREATED, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				/*                add an event   */
			} else {
				if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
					status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId);
				}
			}
		}
	}

	return (status);
}

icu_status_code asw_PUS_3_10_TM_HK_report_def(uint16_t APID,
		uint16_t SID_number) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	TM_packet_t TM_packet;
	uint16_t param_number = 0, sid_id = 0, data_length = 0, length = 0, crc = 0;
	uint16_t i = 0, table_id = 0, table_index = 0;
	uint8_t sub_type = 10;
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;
	uint8_t serial_buffer[MIL_1553_MAX_TM_PKT_LENGTH];

	/* set the first field of packet header  */
	TM_packet.TM_packet_ID = (uint16_t)((uint16_t)(version_num << TM_VERSION_OFFSET)
			| (uint16_t)(tm_type << TM_TYPE_OFFSET)
			| (uint16_t)(data_field_header_flag << TM_DF_HEADER_FLAG_OFFSET)
			| APID);

	sid_id = SID_number - 1;

	if (sid_id < TM_MAX_REPORT_NUMBER) {
		param_number = SID[APID - ICU_ASW_APID][sid_id].param_number;
	}

	/*
	 * Packet data lenght = # octects in data field packet - 1
	 * data_field_header = 10 octects, fixed
	 * data_source = SID + Collect Interval + NPAR + NPAR * par_id = (NPAR + 3) *
	 * 2 Bytes
	 * packet_error_control = 2 octects, fixed
	 */
	TM_packet.TM_packet_length = (12 + ((param_number + 3) * 2)) - 1;

	/*
	 * prepare packet data field
	 */
	asw_PUS_TM_set_data_header(&TM_packet, 3, sub_type);

	/*
	 * prepare data source: SID (2 Bytes) + collection Interval (2 Bytes) +
	 *                         + NPAR (2 Bytes) + parameters ID list (2 Bytes * NPAR)
	 */
	length = TM_packet.TM_packet_length + 1 + 6;
	data_length = param_number + 3;
	TM_packet.TM_data_source[0] = SID_number;

	if (sid_id < TM_MAX_REPORT_NUMBER) {
		TM_packet.TM_data_source[1] =
				SID[APID - ICU_ASW_APID][sid_id].collection_interval;
		TM_packet.TM_data_source[2] = param_number;

		for (i = 0; i < (uint16_t) (data_length - 3); i++) {
			table_id = SID[APID - ICU_ASW_APID][sid_id].SID_data[i].DB_table;
			if (table_id == PARAM_MONITORED) {
				table_index =
						SID[APID - ICU_ASW_APID][sid_id].SID_data[i].DB_index;

				if (table_index < DB_MONITORED_PARAM_NUMB) {
					icu_sem_lock(monit_db_sem_Id, 5);
					TM_packet.TM_data_source[i + 3] =
							monitored_db_param[table_index].parID;
					icu_sem_unlock(monit_db_sem_Id);
				}

			} else if (table_id == PARAM_NOT_MONITORED) {
				table_index =
						SID[APID - ICU_ASW_APID][sid_id].SID_data[i].DB_index;
				if (table_index < DB_MONITORED_PARAM_NUMB) {
					icu_sem_lock(not_mon_db_sem_Id, 5);
					TM_packet.TM_data_source[i + 3] =
							db_param[table_index].parID;
					icu_sem_unlock(not_mon_db_sem_Id);
				}
			} else {
				/*
				 * FIXME
				 */
			}
		}
	}

	for (i = data_length; i < TM_PKT_MAX_DATA_LENGTH_DW; i++) {
		TM_packet.TM_data_source[i] = 0;
	}

	/*
	 * spare to fill all packet to an integer number of DW
	 * with 16-bit parameters, it is not necessary
	 */
	TM_packet.spare = NULL;

	/*
	 * serialize packet for checksum
	 * define total size for buffer =
	 *    = packet header length (6 Bytes) + TM_pack->TM_packet_length + 1
	 */
	/* at this level, still missing the packet_error_control dataword */
	asw_serialize_TM(&TM_packet, serial_buffer, data_length);

	crc = asw_create_checksum(serial_buffer, length - 2);
	TM_packet.packet_error_control = crc;

	/*Write message inside queues*/
	icu_sem_lock(queue_sem_id, 5);
	rtems_status = rtems_message_queue_send(pus_3_queue_id,
			(void *) serial_buffer, length);
	queues_with_data |= QUEUE_PUS_3_DATA_IN;
	icu_sem_unlock(queue_sem_id);

	if (rtems_status != RTEMS_SUCCESSFUL) {
		status = ICU_RTEMS_QUEUE_ERROR;
	}

	return (status);
}

icu_status_code asw_PUS_3_25_TM_HK_report(uint16_t APID, uint16_t SID_number) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t TM_pack_id = 0, TM_pack_seq = 0, TM_pack_length = 0;
	uint16_t grouping_flag = 0;
	uint16_t param_number = 0, length = 0, crc = 0, i = 0, par_idx = 0;
	uint16_t long_idx = 0, tmp = 0;
	uint16_t collection_int;
	uint16_t table_id = 0, table_index = 0;
	uint8_t serial_buffer[MIL_1553_MAX_TM_PKT_LENGTH];
	uint32_t coarseTime = 0, fineTime = 0;
	uint16_t TM_pack_fineTime = 0;
	bool_t alreadyRead = 0;

	/* set the first field of packet header */
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;

	/*
	 * CHECK ON INPUT PARAMETERS done by calling functions
	 *                 SID_number shall be < TM_MAX_REPORT_NUMBER
	 */

	TM_pack_id = (version_num << (uint16_t) TM_VERSION_OFFSET)
			| (tm_type << (uint16_t) TM_TYPE_OFFSET)
			| (data_field_header_flag << (uint16_t) TM_DF_HEADER_FLAG_OFFSET) | APID;

	/*
	 * retrieve the number of parameters in the SID report
	 */
	if ((SID_number - 1) < TM_MAX_REPORT_NUMBER) {
		param_number = SID[APID - ICU_ASW_APID][SID_number - 1].param_number;
	}

	memcpy(&(serial_buffer[0]), &TM_pack_id, sizeof(uint16_t));
	serial_buffer[6] = 16; /* pus_version - fixed: 00010000 */
	serial_buffer[7] = 3; /* pus type - fixed: 3 */
	serial_buffer[8] = 25; /* pus subtype - fixed: 25 */
	serial_buffer[9] = 1; /* destination ID - FIXME */

	memcpy(&(serial_buffer[16]), &SID_number, sizeof(uint16_t));

	i = 18; /* starting point for parameters values */
	for (par_idx = 0; par_idx < param_number; par_idx++) {
		table_id =
				SID[APID - ICU_ASW_APID][SID_number - 1].SID_data[par_idx].DB_table;
		if (table_id == PARAM_MONITORED) {
			table_index =
					SID[APID - ICU_ASW_APID][SID_number - 1].SID_data[par_idx].DB_index;

			if (table_index < DB_MONITORED_PARAM_NUMB) {
				switch (monitored_db_param[table_index].size) {
				case (UINT16_TYPE):
					if (i < (uint16_t) (MIL_1553_MAX_TM_PKT_LENGTH - 2)) {
						icu_sem_lock(monit_db_sem_Id, 5);
						memcpy(&(serial_buffer[i]),
								&(monitored_db_param[table_index].current_value),
								sizeof(uint16_t));
						icu_sem_unlock(monit_db_sem_Id);
						i += 2;
					}
					break;
				case (UINT32_TYPE):
					icu_sem_lock(monit_db_sem_Id, 5);
					long_idx = monitored_db_param[table_index].current_value;
					icu_sem_unlock(monit_db_sem_Id);

					if (((i < (uint16_t) (MIL_1553_MAX_TM_PKT_LENGTH - 4))
							&& (long_idx < DB_LONG_PARAM_NUMBER)) == TRUE) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						memcpy(&(serial_buffer[i]),
								&(db_long_curr_value[long_idx][1]),
								sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
						i += 4;
					}
					break;
				default:
					break;
				}
			}
		} else { /*  table_id == PARAM_NOT_MONITORED */
			table_index =
					SID[APID - ICU_ASW_APID][SID_number - 1].SID_data[par_idx].DB_index;
			if (table_index < DB_NOT_MONITORED_PARAM_NUMB) {
				switch (db_param[table_index].size) {
				case (UINT8_TYPE):
					if (i < MIL_1553_MAX_TM_PKT_LENGTH) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						serial_buffer[i] =
								(uint8_t) db_param[table_index].current_value;
						icu_sem_unlock(not_mon_db_sem_Id);
						i++;
					}
					break;
				case (UINT16_TYPE):
					if (i < (uint16_t) (MIL_1553_MAX_TM_PKT_LENGTH - 2)) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						memcpy(&(serial_buffer[i]),
								&(db_param[table_index].current_value),
								sizeof(uint16_t));
						icu_sem_unlock(not_mon_db_sem_Id);
						i += 2;
					}
					break;
				case (UINT32_TYPE):
					long_idx = db_param[table_index].current_value;
					if (((i < (uint16_t) (MIL_1553_MAX_TM_PKT_LENGTH - 4))
							&& (long_idx < DB_LONG_PARAM_NUMBER)) == TRUE) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						memcpy(&(serial_buffer[i]),
								&(db_long_curr_value[long_idx][1]),
								sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
						i += 4;
					}
					break;
				case (UINT48_TYPE):
					long_idx = db_param[table_index].current_value;
					if (((i < (uint16_t) (MIL_1553_MAX_TM_PKT_LENGTH - 6))
							&& (long_idx < DB_LONG_PARAM_NUMBER)) == TRUE) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						tmp = (uint16_t) (db_long_curr_value[long_idx][0])
								& 0xFFFFU;
						memcpy(&(serial_buffer[i]), &tmp, sizeof(uint16_t));
						memcpy(&(serial_buffer[i + 2]),
								&(db_long_curr_value[long_idx][1]),
								sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
						i += 6;
					}
					break;
				case (UINT64_TYPE):
					long_idx = db_param[table_index].current_value;
					if (((i < (uint16_t) (MIL_1553_MAX_TM_PKT_LENGTH - 8))
							&& (long_idx < DB_LONG_PARAM_NUMBER)) == TRUE) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						memcpy(&(serial_buffer[i]),
								&(db_long_curr_value[long_idx][0]),
								sizeof(uint32_t));
						memcpy(&(serial_buffer[i + 4]),
								&(db_long_curr_value[long_idx][1]),
								sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
						i += 8;
					}
					break;
				default:
					break;
				}
			}
		}
	}

	/*
	 * define total size for buffer = i + CRC (2 Bytes)
	 */
	length = i + 2; // in Byte

	/*
	 * Before evaluating crc, fill with all packet parts missing
	 */
	/*
	 * Packet data lenght = # octects in data field packet - 1
	 * primary_header = 6 Bytes
	 * data_field_header = 10 octects, fixed - time = 48 bit
	 * data_source = (#parameters + 1)*2 octects
	 * packet_error_control = 2 octects, fixed
	 */
	TM_pack_length = length - 6 - 1;
	memcpy(&(serial_buffer[4]), &TM_pack_length, sizeof(uint16_t));

	/*
	 * grouping flag: 11(bin) - stand-alone packet
	 */
	grouping_flag = 3;

	/* Assign TM packet general counter */
	TM_pack_seq = (grouping_flag << TM_GROUPING_FLAG_OFFSET)
			| (TM_ss_counter[APID - ICU_ASW_APID] << TM_SSCOUNT_OFFSET);
	memcpy(&(serial_buffer[2]), &TM_pack_seq, sizeof(uint16_t));

	/* increase the TM packet general counter */
	icu_sem_lock(TM_cnt_sem_Id, 5);
	TM_ss_counter[APID - ICU_ASW_APID] =
			(TM_ss_counter[APID - ICU_ASW_APID] + 1) % TM_MAX_SEQ_COUNTER;
	icu_sem_unlock(TM_cnt_sem_Id);

	icu_obt_latch_obt();
	icu_obt_read_latched_obt(&coarseTime, &fineTime, &alreadyRead);

	TM_pack_fineTime = (uint16_t) ((fineTime & 0xFFFF0000) >> 16);

	memcpy(&(serial_buffer[10]), &coarseTime, sizeof(uint32_t));
	memcpy(&(serial_buffer[14]), &TM_pack_fineTime, sizeof(uint16_t));

	crc = asw_create_checksum(serial_buffer, length - 2);

	/* Create 1553 message and push it inside the queue based on SID */
	switch (SID_number) {
	case 1:
		icu_sem_lock(queue_sem_id, 5);
		rtems_status = rtems_message_queue_send(hk_1_queue_id,
				(void *) serial_buffer, length);
		queues_with_data |= QUEUE_HK_1_DATA_IN;
		icu_sem_unlock(queue_sem_id);
		break;
	case 2:
		icu_sem_lock(queue_sem_id, 5);
		rtems_status = rtems_message_queue_send(hk_2_queue_id,
				(void *) serial_buffer, length);
		queues_with_data |= QUEUE_HK_2_DATA_IN;
		icu_sem_unlock(queue_sem_id);
		break;
	case 3:
		icu_sem_lock(queue_sem_id, 5);
		rtems_status = rtems_message_queue_send(hk_3_queue_id,
				(void *) serial_buffer, length);
		queues_with_data |= QUEUE_HK_3_DATA_IN;
		icu_sem_unlock(queue_sem_id);
		break;
	case 4:
		icu_sem_lock(queue_sem_id, 5);
		rtems_status = rtems_message_queue_send(hk_4_queue_id,
				(void *) serial_buffer, length);
		queues_with_data |= QUEUE_HK_4_DATA_IN;
		icu_sem_unlock(queue_sem_id);
		break;
	default:
		/*
		 * push user-created HK packets into a queue based on collection interval
		 */
		icu_sem_lock(queue_sem_id, 5);
		collection_int =
				SID[APID - ICU_ASW_APID][SID_number - 1].collection_interval;
		if (collection_int <= TM_QUEUE_1_MAX_COLLECTION_INT) {
			/*
			 * push message in queue 1
			 */
			rtems_status = rtems_message_queue_send(hk_1_queue_id,
					(void *) serial_buffer, length);
			queues_with_data |= QUEUE_HK_1_DATA_IN;

		} else if (((collection_int > TM_QUEUE_1_MAX_COLLECTION_INT)
				&& (collection_int <= TM_QUEUE_2_MAX_COLLECTION_INT)) == TRUE) {
			/*
			 * push message in queue 2
			 */
			rtems_status = rtems_message_queue_send(hk_2_queue_id,
					(void *) serial_buffer, length);
			queues_with_data |= QUEUE_HK_2_DATA_IN;

		} else {
			/*
			 * push message in queue 3
			 */
			rtems_status = rtems_message_queue_send(hk_3_queue_id,
					(void *) serial_buffer, length);
			queues_with_data |= QUEUE_HK_3_DATA_IN;
		}
		icu_sem_unlock(queue_sem_id);
		break;
	}

	if (rtems_status != RTEMS_SUCCESSFUL) {
		status = ICU_RTEMS_QUEUE_ERROR;
	}

	return (status);
}

icu_status_code asw_PUS_3_130_set_HK_frequency(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t i = 0;
	uint8_t nsid = 0, exist_error = 0;
	uint32_t mask = 1;
	uint16_t sid[TM_MAX_REPORT_NUMBER], frequencies[TM_MAX_REPORT_NUMBER];

	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t size_in_bytes = 0;

	/*
	 * TODO check status - should be...
	 */

	/*
	 * extract parameters from input array
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	nsid = tc.TC_pkt_data[0];

	if (nsid > TM_MAX_REPORT_NUMBER) {
		error_param_length = 1;
		error_param[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (size_in_bytes != (uint16_t) (1 + (nsid * 4))) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		for (i = 0; i < nsid; i++) {
			sid[i] = (uint16_t) ((uint16_t) tc.TC_pkt_data[1 + (4 * i)] << 8U)
					| (uint16_t) tc.TC_pkt_data[2 + (4 * i)];
			frequencies[i] = (uint16_t) ((uint16_t) tc.TC_pkt_data[3 + (4 * i)]
					<< 8) | (uint16_t) tc.TC_pkt_data[4 + (4 * i)];
		}

		i = 0;
		while ((i < nsid) && (exist_error == 0)) {
			// check if it greater than the maximum allowed - useful for using flags
			if (sid[i] > TM_MAX_REPORT_NUMBER) {
				error_param_length = 1;
				error_param[0] = (uint8_t) (2 + (2 * i)); // param (2 + 2 * i + 1) is the collection interval
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				exist_error = 1;
			} else {
				// check if it is existing...
				if ((((sid[i] - 1) >= 0)
						&& ((sid[i] - 1) < (8 * sizeof(uint32_t)))) == TRUE) {
					mask = (uint32_t) 1U << (sid[i] - 1);
				}
				if ((mask
						& TM_report_exists_flag[tc.pkt_id.processId
								- ICU_ASW_APID]) == 0) {
					error_param_length = 1;
					error_param[0] = (uint8_t) (2 + (2 * i));
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					exist_error = 1;
				}
			}
			i++;
		}

		if (exist_error == 0) {
			for (i = 0; i < nsid; i++) {
				SID[tc.pkt_id.processId - ICU_ASW_APID][sid[i] - 1].collection_interval =
						frequencies[i];
				SID[tc.pkt_id.processId - ICU_ASW_APID][sid[i] - 1].generation_cnt =
						frequencies[i];
			}
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	} /* end else on check of size and nsid */

	return (status);
}

icu_status_code asw_PUS_3_135_get_enabled_HK(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param_length = 0;
	uint16_t size_in_bytes = 0;

	/*
	 * TODO check status - should be...
	 */

	/*
	 * extract parameters from input array
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != 0) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		status = asw_PUS_3_136_TM_HK_active_rep(tc.pkt_id.processId);

		if (status != ICU_SUCCESSFUL) {
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_PUS3_TM_PKT_NOT_CREATED, 0, NULL );
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	}

	return (status);
}

icu_status_code asw_PUS_3_136_TM_HK_active_rep(uint16_t APID) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	TM_packet_t TM_packet;
	uint16_t NSID = 0, crc, length, data_length, i = 0;
	uint8_t sub_type = 136, idx = 0;
	uint32_t temp_flag;
	uint8_t serial_buffer[MIL_1553_MAX_TM_PKT_LENGTH];

	/* set the first field of packet header - common to all TM packets */
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;

	TM_packet.TM_packet_ID = (version_num << (uint16_t) TM_VERSION_OFFSET)
			| (tm_type << (uint16_t) TM_TYPE_OFFSET)
			| (data_field_header_flag << (uint16_t) TM_DF_HEADER_FLAG_OFFSET) | APID;

	/*
	 * the enabled reports have bit 1 in TM_report_flag=(SID_n, ...., SID_1)
	 */
	NSID = asw_bitCount(TM_report_flag[APID - ICU_ASW_APID]);
	/*
	 * Packet data lenght = # octects in data field packet - 1
	 * data_field_header = 10 octects, fixed
	 * data_source = NSID (2Bytes) + (Collect Interval + SID) * NSID = (2*NSID +
	 * 1) * 2 Bytes
	 * packet_error_control = 2 octects, fixed
	 */
	TM_packet.TM_packet_length = (12 + (((2 * NSID) + 1) * 2)) - 1;

	/*
	 * prepare packet data field
	 */
	asw_PUS_TM_set_data_header(&TM_packet, 3, sub_type);

	/*
	 * define total size for buffer =
	 *    = packet header length (6 Bytes) + TM_pack->TM_packet_length + 1
	 *
	 * prepare data source: NSID (2 Bytes) + [SID + collection interval] * NSID
	 *                                     (2 Bytes * (2 * NSID))
	 */

	length = TM_packet.TM_packet_length + 1 + 6;
	data_length = (2 * NSID) + 1;
	temp_flag = TM_report_flag[APID - ICU_ASW_APID];
	TM_packet.TM_data_source[0] = NSID;

	/*
	 * enable/disable flag: TM_report_flag=(bit_0, ..., bit_n-1)
	 * SID_1 = bit_n-1
	 * ...
	 * SID_n = bit_0
	 */
	for (i = 1; i <= TM_MAX_REPORT_NUMBER; i++) {
		if ((temp_flag & 1U) == 1) {
			idx++;
			// SID_number = i
			TM_packet.TM_data_source[(2 * idx) - 1] = i;
			TM_packet.TM_data_source[2 * idx] =
					SID[APID - ICU_ASW_APID][i - 1].collection_interval;
		}
		temp_flag = temp_flag >> 1;
	}

	for (i = data_length; i < TM_PKT_MAX_DATA_LENGTH_DW; i++) {
		TM_packet.TM_data_source[i] = 0;
	}

	/* spare to fill all packet to an integer number of DW */
	/*
	 * with 16-bit parameters, it is not necessary
	 */
	TM_packet.spare = NULL;

	/*
	 * serialize packet for checksum
	 */
	/* at this level, still missing the packet_error_control dataword */
	asw_serialize_TM(&TM_packet, serial_buffer, data_length);

	crc = asw_create_checksum(serial_buffer, length - 2);
	TM_packet.packet_error_control = crc;

	/*Write event inside queues*/
	icu_sem_lock(queue_sem_id, 5);
	rtems_status = rtems_message_queue_send(pus_3_queue_id,
			(void *) serial_buffer, length);
	queues_with_data |= QUEUE_PUS_3_DATA_IN;
	icu_sem_unlock(queue_sem_id);

	if (rtems_status != RTEMS_SUCCESSFUL) {
		status = ICU_RTEMS_QUEUE_ERROR;
	}

	return (status);
}

icu_status_code asw_PUS_3_140_get_HK_onDemand(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint32_t mask = 1;
	uint8_t NSID = 0, i = 0;
	uint16_t APID[TM_MAX_ON_DEMAND_REPORT_NUMBER],
			sid[TM_MAX_ON_DEMAND_REPORT_NUMBER];

	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t size_in_bytes = 0;
	uint8_t error_counter = 0;

	/*
	 * TODO check status - should be...
	 */

	/*
	 * extract parameters from input array
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	NSID = tc.TC_pkt_data[0];

	if (NSID > TM_MAX_REPORT_NUMBER) {
		error_param_length = 1;
		error_param[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (size_in_bytes != (uint16_t) (1 + (NSID * 4))) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		for (i = 0; i < NSID; i++) {
			APID[i] = (uint16_t) ((uint16_t) tc.TC_pkt_data[1 + (4 * i)] << 8U)
					| (uint16_t) tc.TC_pkt_data[2 + (4 * i)];
			sid[i] = (uint16_t) ((uint16_t) tc.TC_pkt_data[3 + (4 * i)] << 8U)
					| (uint16_t) tc.TC_pkt_data[4 + (4 * i)];
		}

		/*
		 * CHECK ON INPUT PARAMETERS:
		 *                 APID shall be allowed
		 *                 SID_number shall be < TM_MAX_REPORT_NUMBER
		 *                 SID_number shall be existing
		 *                 SID_number shall be enabled
		 */

		while (((i < NSID) && (error_counter == 0)) == TRUE) {
			if (((APID[i] < ICU_ASW_APID) || (APID[i] >= APID_NUMBER)) == TRUE) {
				error_param_length = 1;
				error_param[0] = (uint8_t) (2 + (2 * i));
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				error_counter++;
			} else if (sid[i] > TM_MAX_REPORT_NUMBER) {
				error_param_length = 1;
				error_param[0] = (uint8_t) (2 + (2 * i) + 1);
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				error_counter++;
			} else {

				if ((((sid[i] - 1) >= 0)
						&& ((sid[i] - 1) < (8 * sizeof(uint32_t)))) == TRUE) {
					mask = (uint32_t) 1U << (sid[i] - 1);
				}
				if ((mask & TM_report_exists_flag[APID[i] - ICU_ASW_APID])
						== 0) {
					error_param_length = 1;
					error_param[0] = (uint8_t) (2 + (2 * i) + 1);
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					error_counter++;
				} else {
					if ((mask & TM_report_flag[APID[i] - ICU_ASW_APID]) == 0) {
						error_param_length = 1;
						error_param[0] = (uint8_t) (2 + (2 * i) + 1);
						status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId,
								TM_1_8_TC_PARAM_OUT_OF_BOUNDS,
								error_param_length, error_param);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						error_counter++;
					}
				}
			}

			i++;
		}

		for (i = 0; i < NSID; i++) {
			// generate one occurrence for each required report
			status = asw_PUS_3_25_TM_HK_report(APID[i], sid[i]);
			// !!!!!!!!!!!!!!!!!!!!!!!!!!TBC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			// halt if one report generation has failed
			if (status != ICU_SUCCESSFUL) {
				error_param_length = 0;
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_PUS3_TM_PKT_NOT_CREATED, error_param_length, NULL );
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				error_counter++;
			}
		}

		/*
		 * everything was ok! Generate a TM(1,7)
		 */
		if ((((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) && (error_counter == 0)) == TRUE) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	} /* end else on check of size and NSID */

	return (status);
}

/*
 * ancillary functions
 */
int16_t asw_absval(int16_t i) {
	return ((i < 0) ? -i : i);
}
