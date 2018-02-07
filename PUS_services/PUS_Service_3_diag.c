/**
 * \file	PUS_Service_3_diag.c
 *
 * \brief	PUS Service 3: diagnostic services
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Jun 9, 2017
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_3_2_set_new_DIAG_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t i = 0, idx = 0, tmp = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t monitor_mask = 1U << PARAM_MONIT_FLAG_OFFSET;
	uint16_t SID_num = 0, collection_int = 0, n_par = 0, index_pos = 6,
			nfa_rep = 0;
	uint16_t nfa = 0, nrep[TM_DIAG_MAX_FIXED_ARR_NUMBER] = {0},
			n_par2[TM_DIAG_MAX_FIXED_ARR_NUMBER] = {0};
	uint8_t err_check = 0;
	uint16_t size_in_bytes = 0;
	uint16_t total_param_size = 0;
	uint16_t oversampled_par_size = 0;
	uint16_t par[TM_DIAG_MAX_SINGLE_PARAM_NUMBER];
	uint16_t par2[TM_DIAG_MAX_FIXED_ARR_NUMBER][TM_DIAG_MAX_OVERSAMP_PAR_NUMB];
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

	if (((SID_num > 0) && (SID_num <= 32)) == TRUE) {
		mask = (uint32_t) 1U << (SID_num - 1);
	}
	/*
	 * CHECK ON INPUT PARAMETERS:
	 *                 SID_number shall be <= TM_MAX_REPORT_NUMBER
	 *                 size_in_bytes should be 4 (from TC) + 6 + n_par * 2
	 */
	if (TM_DIAG_report_number[tc.pkt_id.processId - ICU_ASW_APID]
			== TM_DIAG_MAX_REPORT_NUMBER) {
		/*
		 * the maximum number of DIAG report yet reached. Refuse the new one
		 */
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_PUS3_TOO_MANY_DIAG_REP, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if ((mask
			& TM_DIAG_report_exists_flag[tc.pkt_id.processId - ICU_ASW_APID])
			!= 0) {
		/*
		 * check if it yet existing. If yes, return with an error
		 */
		error_param_length = 1;
		error_param[0] = 1; // SID is the 1-st parameter of TC
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (n_par > TM_DIAG_MAX_SINGLE_PARAM_NUMBER) {
		error_param_length = 1;
		error_param[0] = 3; // npar is the 3-rd parameter of TC
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	}
	/* Check if there are minimum n_par parameters plus 2 bytes for NFA parameter */
	else if ((index_pos + (uint16_t) ((n_par * 2) + 2)) >= size_in_bytes) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * retrieve parameters sampled once
		 */
		if (par != NULL ) {
			memcpy(par, &(tc.TC_pkt_data[6]), n_par * sizeof(uint16_t));
		}
		for (i = n_par; i < TM_DIAG_MAX_SINGLE_PARAM_NUMBER; i++) {
			par[i] = 0;
		}

		index_pos = index_pos + (uint16_t) (n_par * 2); /* in Bytes */
		memcpy(&nfa, &tc.TC_pkt_data[index_pos], sizeof(uint16_t));
		index_pos += 2;

		if (nfa > TM_DIAG_MAX_FIXED_ARR_NUMBER) {
			error_param_length = 1;
			error_param[0] = (uint8_t) ((index_pos + 1) / 2); /* position in the TC data source */
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			err_check = 1;
		} else {
			nfa_rep = 0;
			while ((nfa_rep < nfa) && (err_check == 0)) {
				/* Check if TC data  are compatible with nfa declaration*/
				if ((uint16_t) (index_pos + 4) >= size_in_bytes) {
					error_param_length = 0;
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length,
							NULL );
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					err_check = 1;
				} else {

					if (((nfa_rep >= 0)
							&& (nfa_rep < TM_DIAG_MAX_FIXED_ARR_NUMBER))
							== TRUE) {
						if (((nrep != NULL )&& (n_par2 != NULL))== TRUE) {
							memcpy(&nrep[nfa_rep], &tc.TC_pkt_data[index_pos], sizeof(uint16_t));
							memcpy(&n_par2[nfa_rep], &tc.TC_pkt_data[index_pos + 2], sizeof(uint16_t));
						}
					}

					if (((nrep[nfa_rep] > TM_DIAG_MAX_REPETITION_NUMBER) || (nrep != NULL)) == TRUE) {
						error_param_length = 1;
						error_param[0] = (uint8_t) ((index_pos + 3) / 2); /* position in the TC data source */
						status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId,
								TM_1_8_TC_PARAM_OUT_OF_BOUNDS,
								error_param_length, error_param);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						err_check = 1;
					} else if (((n_par2[nfa_rep] > TM_DIAG_MAX_OVERSAMP_PAR_NUMB) || (n_par2 != NULL)) == TRUE) {
						error_param_length = 1;
						error_param[0] = (uint8_t) ((index_pos + 5) / 2); /* position in the TC data source */
						status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId,
								TM_1_8_TC_PARAM_OUT_OF_BOUNDS,
								error_param_length, error_param);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						err_check = 1;
					} else if ((uint16_t) (index_pos
									+ (uint16_t) (n_par2[nfa_rep] * 2))
							>= size_in_bytes) {
						error_param_length = 0;
						status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId,
								TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length,
								NULL );
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						err_check = 1;
					} else {
						index_pos += 4;

						/* retrieve oversampled parameters IDs */
						for (i = 0; i < n_par2[nfa_rep]; i++) {
							memcpy(&par2[nfa_rep][i], &tc.TC_pkt_data[index_pos], sizeof(uint16_t));
							index_pos += 2;
						}
						nfa_rep++;
					}
				}
				/* end while */
			}
		}

		if (err_check == 0) {
			/*
			 * prepare the data descriptor matrix for single sampled parameters and compute parameter size
			 */
			for (i = 0; i < n_par; i++) {
				/*
				 * check if the parameters is monitored
				 */
				tmp = (par[i] & monitor_mask) >> PARAM_MONIT_FLAG_OFFSET;
				if (tmp == PARAM_MONITORED) {
					idx = asw_PUS_3_ID_to_mon_DB_idx(par[i]);
					if (idx < DB_MONITORED_PARAM_NUMB) {
						icu_sem_lock(monit_db_sem_Id, 5);
						total_param_size += monitored_db_param[idx].size;
						icu_sem_unlock(monit_db_sem_Id);
					}
				} else {
					idx = asw_PUS_3_ID_to_not_mon_DB_idx(par[i]);
					if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						total_param_size += db_param[idx].size;
						icu_sem_unlock(not_mon_db_sem_Id);
					}
				}
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].SID_data[i].DB_index =
						idx;
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].SID_data[i].DB_table =
						tmp;
			}
			/*
			 * prepare the data descriptor matrix for oversampled parameters and compute parameters size
			 */
			diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].fixed_array_number =
					nfa;
			for (nfa_rep = 0; nfa_rep < nfa; nfa_rep++) {
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].os_repetition_number[nfa_rep] =
						nrep[nfa_rep];
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].os_param_number[nfa_rep] =
						n_par2[nfa_rep];
				oversampled_par_size = 0;

				for (i = 0; i < n_par2[nfa_rep]; i++) {
					/*
					 * check if the parameter is monitored
					 */
					tmp = (par2[nfa_rep][i] & monitor_mask)
							>> PARAM_MONIT_FLAG_OFFSET;
					if (tmp == PARAM_MONITORED) {
						idx = asw_PUS_3_ID_to_mon_DB_idx(par2[nfa_rep][i]);
						if (idx < DB_MONITORED_PARAM_NUMB) {
							icu_sem_lock(monit_db_sem_Id, 5);
							oversampled_par_size +=
									monitored_db_param[idx].size;
							icu_sem_unlock(monit_db_sem_Id);
						}
						if (i == 0) {
							/* system is ICU - no other check on other parameters FIXME */
							diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num
									- 1].fa_param_system = 0;
						}
					} else {
						idx = asw_PUS_3_ID_to_not_mon_DB_idx(par2[nfa_rep][i]);
						if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
							icu_sem_lock(not_mon_db_sem_Id, 5);
							oversampled_par_size += db_param[idx].size;
							icu_sem_unlock(not_mon_db_sem_Id);
						}
						if (i == 0) {
							/* check if system is ICU or DPU */
							if ((par2[nfa_rep][i] >> PARAM_SYSTEM_OFFSET)
									== SYS_ICU) {
								diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num
										- 1].fa_param_system = 0;
							} else {
								diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num
										- 1].fa_param_system = 1;
							}
						}

					}

					diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].DIAG_data[nfa_rep][i].DB_index =
							idx;
					diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].DIAG_data[nfa_rep][i].DB_table =
							tmp;
				}
				oversampled_par_size *= nrep[nfa_rep];
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].fa_length[nfa_rep] =
						oversampled_par_size;
				total_param_size += oversampled_par_size;
			}

			if (total_param_size > (TM_3_MAX_DATA_LENGTH_BYTE)) {
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_PUS3_REPORT_SIZE_ERROR, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
				err_check = 1;
			}

			if (err_check == 0) {
				/*
				 * finally validate the Diagnostic SID
				 */
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].SID_number =
						SID_num;
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].param_number =
						n_par;
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].collection_interval =
						collection_int;
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_num - 1].generation_cnt =
						collection_int;

				/*
				 * set to disabled by default and declare it is existing
				 */
				if ((((SID_num - 1) >= 0)
						&& ((SID_num - 1) < (8 * sizeof(uint32_t)))) == TRUE) {
					TM_DIAG_report_flag[tc.pkt_id.processId - ICU_ASW_APID] &=
							~((uint32_t) 1U << (SID_num - 1));
					TM_DIAG_report_exists_flag[tc.pkt_id.processId
							- ICU_ASW_APID] =
							TM_report_exists_flag[tc.pkt_id.processId
									- ICU_ASW_APID]
									| ((uint32_t) 1U << (SID_num - 1));
				}
				/*
				 * increment the counter of reports
				 */
				TM_DIAG_report_number[tc.pkt_id.processId - ICU_ASW_APID]++;

				/*
				 * everything was ok! Generate a TM(1,7)
				 */
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		} /* end if (err_check == 0) */
	} /* end else if(n_par > TM_DIAG_MAX_SINGLE_PARAM_NUMBER) */

	return (status);
}

icu_status_code asw_PUS_3_4_clear_DIAG_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t sid_id = 0;
	uint8_t NSID = 0, exist_error = 0, i = 0;
	uint8_t sid_found = 0, stored_idx = 0;
	uint32_t mask = 0;
	uint16_t SID_number[TM_DIAG_MAX_REPORT_NUMBER];

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

	if (NSID > TM_DIAG_MAX_REPORT_NUMBER) {
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
		if (SID_number != NULL ) {
			memcpy(SID_number, &(tc.TC_pkt_data[1]), NSID * sizeof(uint16_t));
		}
		for (i = NSID; i < TM_DIAG_MAX_REPORT_NUMBER; i++) {
			SID_number[i] = 0;
		}

		/*
		 * CHECK ON INPUT PARAMETERS:
		 *                 SID_number shall be < TM_MAX_REPORT_NUMBER
		 */
		i = 0;
		while ((i < NSID) && (exist_error == 0)) {
			/* FIXME SID can be non consecutive */
			if (((SID_number[i] > TM_DIAG_MAX_REPORT_NUMBER) || (SID_number[i] == 0)) == TRUE) {
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
				if (((sid_id >= 0) && (sid_id < (8 * sizeof(uint32_t))))
						== TRUE) {
					mask = (uint32_t) 1U << sid_id;
				}
				/*
				 * state it is not existing, disable it
				 */
				TM_DIAG_report_exists_flag[tc.pkt_id.processId - ICU_ASW_APID] &=
						~mask;
				TM_DIAG_report_flag[tc.pkt_id.processId - ICU_ASW_APID] &=
						~mask;
				/*
				 * free the SID_data tag
				 */
				diag_SID[tc.pkt_id.processId - ICU_ASW_APID][sid_id].SID_number =
						0;
				/*
				 * free storage area - if it is assigned to this report
				 */

				while ((stored_idx < TM_DIAG_MAX_SIMULT_REPORT_NUMB)
						&& (sid_found == 0)) {
					if (((uint16_t) ((uint16_t) diag_sampling_space[stored_idx][0]
							<< 8U)
							| (uint16_t) diag_sampling_space[stored_idx][1])
							== SID_number[i]) {
						sid_found = 1;
						/* free storage area and set flag to zero */
						diag_sampling_space[stored_idx][0] = 0;
						diag_sampling_space[stored_idx][1] = 0;
						diag_sampling_space[stored_idx][2] = 0;
					} else {
						stored_idx++;
					}
				}
			}
			i++;
		}

		if (exist_error == 0) {
			/*
			 * everything was ok! Generate a TM(1,7)
			 */
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	} /* end else on check on parameter size and NSID */

	return (status);
}

icu_status_code asw_PUS_3_7_enable_DIAG_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t sid_id = 0;
	uint8_t NSID = 0, i = 0;
	uint8_t exist_error = 0;
	uint8_t storage_error = 0;
	uint8_t sid_found = 0, stored_idx = 0, nfa = 0;
	uint32_t mask = 0;
	uint16_t SID_number[TM_DIAG_MAX_REPORT_NUMBER];
	uint16_t total_fa_length = 0;
	uint16_t fa_pos = 0;
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

	if (NSID > TM_DIAG_MAX_SIMULT_REPORT_NUMB) {
		error_param_length = 1;
		error_param[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (size_in_bytes != (uint16_t) (1 + (NSID * 2))) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (((asw_bitCount(TM_DIAG_report_flag[0]) + (uint16_t) NSID))
			> TM_DIAG_MAX_SIMULT_REPORT_NUMB) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_PUS3_TOO_MANY_DIAG_ON, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		if (SID_number != NULL ) {
			memcpy(SID_number, &(tc.TC_pkt_data[1]), NSID * sizeof(uint16_t));
		}
		for (i = NSID; i < TM_DIAG_MAX_REPORT_NUMBER; i++) {
			SID_number[i] = 0;
		}

		i = 0;
		while ((i < NSID) && (exist_error == 0)) {
			if (((SID_number[i] > TM_DIAG_MAX_REPORT_NUMBER) || (SID_number == 0)) == TRUE) {
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
						& TM_DIAG_report_exists_flag[tc.pkt_id.processId
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

		i = 0;

		if (exist_error == 0) {
			while ((i < NSID) && (storage_error == 0)) {
				sid_found = 0;
				stored_idx = 0;
				while ((stored_idx < TM_DIAG_MAX_SIMULT_REPORT_NUMB)
						&& (sid_found == 0)) {
					if ((((uint16_t) ((uint16_t) diag_sampling_space[stored_idx][0]
							<< 8U)
							| (uint16_t) diag_sampling_space[stored_idx][1]))
							== 0) {
						sid_found = 1;
						memcpy(&(diag_sampling_space[stored_idx][0]),
								&(SID_number[i]), sizeof(uint16_t));
						diag_sampling_space[stored_idx][2] =
								diag_sampling_space[stored_idx][2] | 1U;
						diag_sampling_space[stored_idx][2] =
								diag_sampling_space[stored_idx][2]
										& ~((uint8_t) 1U
												<< TM_DIAG_FLAG_PKT_WRITTEN_OFFSET);
						/*
						 * 0 = ICU parameters
						 * 1 = DPU parameters
						 */
						if (diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_number[i]
								- 1].fa_param_system == 0) {
							diag_sampling_space[stored_idx][2] =
									diag_sampling_space[stored_idx][2]
											& ~((uint8_t) 1U
													<< TM_DIAG_FLAG_ICU_PAR_OFFSET);
						} else {
							diag_sampling_space[stored_idx][2] =
									diag_sampling_space[stored_idx][2]
											| ((uint8_t) 1U
													<< TM_DIAG_FLAG_ICU_PAR_OFFSET);
						}
						/*
						 * define spaces for each FA
						 */
						total_fa_length = 0;
						fa_pos = TM_DIAG_STORED_DATA_HEAD_OFFSET;
						for (nfa = 0;
								nfa
										< (uint8_t) diag_SID[tc.pkt_id.processId
												- ICU_ASW_APID][SID_number[i]
												- 1].fixed_array_number;
								nfa++) {
							total_fa_length +=
									diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_number[i]
											- 1].fa_length[nfa];
							memcpy(
									&(diag_sampling_space[stored_idx][5
											+ (nfa * 4)]), &fa_pos,
									sizeof(uint16_t));
							memcpy(
									&(diag_sampling_space[stored_idx][7
											+ (nfa * 4)]), &fa_pos,
									sizeof(uint16_t));
							fa_pos +=
									diag_SID[tc.pkt_id.processId - ICU_ASW_APID][SID_number[i]
											- 1].fa_length[nfa];
						}
						memcpy(&(diag_sampling_space[stored_idx][3]),
								&total_fa_length, sizeof(uint16_t));
					} else {
						stored_idx++;
					}
				}

				asw_PUS_3_enable_DIAG_rep_flag(SID_number[i]);
				i++;
			}
			if (storage_error == 0) {
				/*
				 * everything was ok! Generate a TM(1,7)
				 */
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	} /* end else on check on parameter size and NSID */

	return (status);
}

icu_status_code asw_PUS_3_8_disable_DIAG_report(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t sid_id = 0;
	uint8_t i = 0, NSID = 0, exist_error = 0;
	uint8_t storage_error = 0;
	uint8_t sid_found = 0, stored_idx = 0;
	uint32_t mask = 0;
	uint16_t SID_number[TM_DIAG_MAX_REPORT_NUMBER];

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

	if (NSID > TM_DIAG_MAX_REPORT_NUMBER) {
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
		if (SID_number != NULL ) {
			memcpy(SID_number, &(tc.TC_pkt_data[1]), NSID * sizeof(uint16_t));
		}
		for (i = NSID; i < TM_DIAG_MAX_REPORT_NUMBER; i++) {
			SID_number[i] = 0;
		}

		/*
		 * CHECK ON INPUT PARAMETERS:
		 *                 SID_number shall be < TM_MAX_REPORT_NUMBER
		 */
		i = 0;
		while ((i < NSID) && (exist_error == 0)) {
			/* FIXME SID number can be non consecutive */
			if (((SID_number[i] > TM_DIAG_MAX_REPORT_NUMBER) || (SID_number[i] == 0)) == TRUE) {
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
						& TM_DIAG_report_exists_flag[tc.pkt_id.processId
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

		i = 0;
		if (exist_error == 0) {
			while ((i < NSID) && (storage_error == 0)) {
				sid_id = SID_number[i] - 1;
				if (((sid_id >= 0) && (sid_id < (8 * sizeof(uint32_t))))
						== TRUE) {
					mask = (uint32_t) 1U << sid_id;
				}
				TM_DIAG_report_flag[tc.pkt_id.processId - ICU_ASW_APID] &=
						~(mask);

				/*
				 * set flags for data storage:
				 * - stop data collection
				 * - free storage area
				 */

				while ((stored_idx < TM_DIAG_MAX_SIMULT_REPORT_NUMB)
						&& (sid_found == 0)) {
					if (((uint16_t) ((uint16_t) diag_sampling_space[stored_idx][0]
							<< 8U)
							| (uint16_t) diag_sampling_space[stored_idx][1])
							== SID_number[i]) {
						sid_found = 1;
					} else {
						stored_idx++;
					}
				}

				if (sid_found == 0) {
					storage_error = 1;
					error_param_length = 1;
					error_param[0] = 2 + i;
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_PUS3_TOO_MANY_DIAG_ON, 0, NULL );
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				} else {
					/* free storage area */
					diag_sampling_space[stored_idx][0] = 0;
					diag_sampling_space[stored_idx][1] = 0;
					/* data collection is not on going */
					diag_sampling_space[stored_idx][2] =
							diag_sampling_space[stored_idx][2] & ~1U;
					/* TM packet has not been prepared */
					diag_sampling_space[stored_idx][2] =
							diag_sampling_space[stored_idx][2]
									& ~(1U << TM_DIAG_FLAG_PKT_WRITTEN_OFFSET);
				}
				i++;
			}

			/*
			 * everything was ok! Generate a TM(1,7)
			 */
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	} /* end else on check on parameter size and NSID */

	return (status);
}

icu_status_code asw_PUS_3_11_get_DIAG_rep_def(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_3_12_TM_DIAG_rep_def(uint16_t APID, uint16_t SID_number) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_3_26_TM_DIAG_report(uint16_t APID, uint16_t SID_number) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t TM_pack_id = 0, TM_pack_seq = 0, TM_pack_length = 0;
	uint16_t stored_idx = 0;
	uint16_t NPAR = 0;
	uint16_t par_idx = 0;
	uint16_t length = 0, crc = 0, i = 0;
	uint16_t tm_array_idx = 0;
	uint16_t long_idx = 0, tmp = 0;
	uint16_t total_fa_length = 0;
	uint16_t table_id = 0, table_index = 0;
	uint8_t serial_buffer[MIL_1553_MAX_TM_PKT_LENGTH];
	uint8_t sid_found = 0;
	uint8_t tmp_mask = 0;
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
			| (data_field_header_flag << (uint16_t) TM_DF_HEADER_FLAG_OFFSET)
			| APID;

	/*
	 * retrieve the number of single value parameters in the SID report
	 */
	NPAR = diag_SID[APID - ICU_ASW_APID][SID_number - 1].param_number;

	memcpy(&(serial_buffer[0]), &TM_pack_id, sizeof(uint16_t));
	serial_buffer[6] = 16; /* pus_version - fixed: 00010000 */
	serial_buffer[7] = 3; /* pus type - fixed: 3 */
	serial_buffer[8] = 26; /* pus subtype - fixed: 26 */
	serial_buffer[9] = 1; /* destination ID - FIXME */

	memcpy(&(serial_buffer[16]), &SID_number, sizeof(uint16_t));

	tm_array_idx = 18; /* starting point for parameters values */
	for (par_idx = 0; par_idx < NPAR; par_idx++) {
		table_id =
				diag_SID[APID - ICU_ASW_APID][SID_number - 1].SID_data[par_idx].DB_table;
		if (table_id == PARAM_MONITORED) {
			table_index =
					diag_SID[APID - ICU_ASW_APID][SID_number - 1].SID_data[par_idx].DB_index;
			if (((table_index < DB_MONITORED_PARAM_NUMB)&& (tm_array_idx < MIL_1553_MAX_TM_PKT_LENGTH))== TRUE) {
				switch (monitored_db_param[table_index].size) {
					case (UINT16_TYPE):
					icu_sem_lock(monit_db_sem_Id, 5);
					memcpy(&(serial_buffer[tm_array_idx]), &(monitored_db_param[table_index].current_value), sizeof(uint16_t));
					icu_sem_unlock(monit_db_sem_Id);
					tm_array_idx += 2;
					break;
					case (UINT32_TYPE):
					icu_sem_lock(monit_db_sem_Id, 5);
					long_idx = monitored_db_param[table_index].current_value;
					icu_sem_unlock(monit_db_sem_Id);
					if (long_idx < DB_LONG_PARAM_NUMBER) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						memcpy(&(serial_buffer[tm_array_idx]), &(db_long_curr_value[long_idx][1]), sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
					}
					tm_array_idx += 4;
					break;
					default:
					break;
				}
			}
		} else { /* table_id == PARAM_NOT_MONITORED */
			table_index =
			diag_SID[APID - ICU_ASW_APID][SID_number - 1].SID_data[par_idx].DB_index;
			if (((table_index < DB_NOT_MONITORED_PARAM_NUMB) && (tm_array_idx < MIL_1553_MAX_TM_PKT_LENGTH)) == TRUE) {
				switch (db_param[table_index].size) {
					case (UINT8_TYPE):
					icu_sem_lock(not_mon_db_sem_Id, 5);
					serial_buffer[tm_array_idx] =
					(uint8_t) db_param[table_index].current_value;
					icu_sem_unlock(not_mon_db_sem_Id);
					tm_array_idx++;
					break;
					case (UINT16_TYPE):
					icu_sem_lock(not_mon_db_sem_Id, 5);
					memcpy(&(serial_buffer[tm_array_idx]),
							&(db_param[table_index].current_value),
							sizeof(uint16_t));
					icu_sem_unlock(not_mon_db_sem_Id);
					tm_array_idx += 2;
					break;
					case (UINT32_TYPE):
					long_idx = db_param[table_index].current_value;
					if (long_idx < DB_LONG_PARAM_NUMBER) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						memcpy(&(serial_buffer[tm_array_idx]),
								&(db_long_curr_value[long_idx][1]), sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
					}
					tm_array_idx += 4;
					break;
					case (UINT48_TYPE):
					long_idx = db_param[table_index].current_value;
					if (long_idx < DB_LONG_PARAM_NUMBER) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						tmp = (uint16_t) (db_long_curr_value[long_idx][0]) & 0xFFFFU;
						memcpy(&(serial_buffer[tm_array_idx]), &tmp, sizeof(uint16_t));
						memcpy(&(serial_buffer[tm_array_idx + 2]),
								&(db_long_curr_value[long_idx][1]), sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
					}
					tm_array_idx += 6;
					break;
					case (UINT64_TYPE):
					long_idx = db_param[table_index].current_value;
					if (long_idx < DB_LONG_PARAM_NUMBER) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						memcpy(&(serial_buffer[tm_array_idx]),
								&(db_long_curr_value[long_idx][0]), sizeof(uint32_t));
						memcpy(&(serial_buffer[tm_array_idx + 4]),
								&(db_long_curr_value[long_idx][1]), sizeof(uint32_t));
						icu_sem_unlock(not_mon_db_sem_Id);
					}
					tm_array_idx += 8;
					break;
					default:
					break;
				}
			}
		}
	}

			/*
			 * insert Fixed Array values - starting point is  - tm_array_idx -
			 */
	while ((i < TM_DIAG_MAX_SIMULT_REPORT_NUMB) && (sid_found == 0)) {
		if (((uint16_t) ((uint16_t) diag_sampling_space[i][0] << 8U)
				| (uint16_t) diag_sampling_space[i][1]) == SID_number) {
			sid_found = 1;
			stored_idx = i;
		}
		i++;
	}
	/* check flag */
	//	diag_sampling_space[stored_idx][2]
	if (stored_idx < TM_DIAG_MAX_SIMULT_REPORT_NUMB) {
		total_fa_length =
				((uint16_t) ((uint16_t) diag_sampling_space[stored_idx][3] << 8U)
						| (uint16_t) diag_sampling_space[stored_idx][4]);
		for (i = 0; i < total_fa_length; i++) {
			serial_buffer[tm_array_idx + i] =
					diag_sampling_space[stored_idx][TM_DIAG_STORED_DATA_HEAD_OFFSET
							+ i];
		}
		tm_array_idx += total_fa_length;
	}
	/*
	 * set flags for data storing handling
	 * 1) TM packet has been prepared
	 * 2) if a one-shot request, disable the generation of the packet and free the storage area
	 */
	diag_sampling_space[stored_idx][2] = diag_sampling_space[stored_idx][2]
			| (1U << 1);

	tmp_mask = diag_sampling_space[stored_idx][2] >> 2U;
	if (tmp_mask == 1) {
		if ((((SID_number - 1) >= 0) && ((SID_number - 1) < (8 * sizeof(uint32_t)))) == TRUE) {
			/* TM(3,26) generated after a TC(3,141) command: one shot DIAG packet */
			TM_DIAG_report_flag[APID - ICU_ASW_APID] = TM_DIAG_report_flag[APID
					- ICU_ASW_APID] & ~((uint32_t)1U << (SID_number - 1));
			diag_sampling_space[stored_idx][0] = 0;
			diag_sampling_space[stored_idx][1] = 0;
		}
	}
	/*
	 * spare to fill all packet to an integer number of DW
	 */
	if ((tm_array_idx % 2) != 0) {
		serial_buffer[tm_array_idx] = 0;
		tm_array_idx++; /* now tm_array_idx is the length! */
	}

	/*
	 * define total size for buffer = i + CRC (2 Bytes)
	 */
	length = tm_array_idx + 2; // in Byte

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
	 * Assign TM packet general counter
	 * grouping flag: 11(bin) - stand-alone packet
	 */
	TM_pack_seq = (3U << TM_GROUPING_FLAG_OFFSET)
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

	/* Create 1553 message and push it inside the appropriate queue */
	if (tmp_mask == 0) {
		icu_sem_lock(queue_sem_id, 5);
		rtems_status = rtems_message_queue_send(diag_1_queue_id,
				(void *) serial_buffer, length);
		queues_with_data |= QUEUE_DIAG_1_DATA_IN;
		icu_sem_unlock(queue_sem_id);
	} else {
		/* one shot packet */
		icu_sem_lock(queue_sem_id, 5);
		rtems_status = rtems_message_queue_send(diag_0_queue_id,
				(void *) serial_buffer, length);
		queues_with_data |= QUEUE_DIAG_0_DATA_IN;
		icu_sem_unlock(queue_sem_id);

	}

	if (rtems_status != RTEMS_SUCCESSFUL) {
		status = ICU_RTEMS_QUEUE_ERROR;
	}

	return (status);
}

icu_status_code asw_PUS_3_131_set_DIAG_freq(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_3_137_get_enabled_DIAG(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_3_138_TM_DIAG_en_rep(uint16_t APID) {
	icu_status_code status = ICU_SUCCESSFUL;
	return (status);
}

icu_status_code asw_PUS_3_141_get_DIAG_onDemand(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	/*
	 * enable the generation of the packet
	 * prepare storage area
	 */
	return (status);
}
