/**
 * \file	PUS_Service_8_1.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	16-05-2017
 *
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_8_1_2_6_set_planning_ID(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint32_t plan_id = 0;
	uint16_t size_in_bytes = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != 8) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Set new planning ID */
		memcpy(&plan_id, &tc.TC_pkt_data[4], sizeof(uint32_t));

		icu_sem_lock(not_mon_db_sem_Id, 5);
		db_long_curr_value[DB_PLANNING_ID_IDX][0] = 0;
		db_long_curr_value[DB_PLANNING_ID_IDX][1] = plan_id;
		icu_sem_unlock(not_mon_db_sem_Id);
		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_2_3_setDPUsAddress(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t dpu_1 = 0, dpu_2 = 0, errors = 0;
	uint16_t size_in_bytes = 0;
	uint16_t dpu1_rt_id = 0, dpu2_rt_id;
	uint16_t dpu_status_id = 0, dpu_mode = 0;
	uint16_t dpu1_rt_index = 0, dpu2_rt_index = 0;
	uint16_t dpu_stat_index = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/* Check parameters */
	if (size_in_bytes != 6) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		dpu_1 = tc.TC_pkt_data[4];
		dpu_2 = tc.TC_pkt_data[5];
		if (dpu_1 > 2) {
			errors = 1;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (dpu_2 > 2) {
			errors = 2;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * Set DPU 1 and DPU 2 RT Addresses
			 * Zero means disabled (default)
			 */
			if (dpu_1 > 0) {
				if (dpu_1 == 1) {
					dpu1_active_rt = 1;
				} else {
					dpu1_active_rt = 2;
				}
				dpu1_status = CPU_UNDEFINED;
			} else {
				dpu1_active_rt = 0;
				dpu1_status = CPU_OFF;
			}

			if (dpu_2 > 0) {
				if (dpu_2 == 1) {
					dpu2_active_rt = 3;
				} else {
					dpu2_active_rt = 4;
				}
				dpu2_status = CPU_UNDEFINED;
			} else {
				dpu2_active_rt = 0;
				dpu2_status = CPU_OFF;
			}

			if (errors != 0) {
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			} else {
				/*
				 * update DB values
				 */
				dpu1_rt_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
						| 2U;
				dpu1_rt_index = asw_PUS_3_ID_to_not_mon_DB_idx(dpu1_rt_id);

				dpu2_rt_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
						| 3U;
				dpu2_rt_index = asw_PUS_3_ID_to_not_mon_DB_idx(dpu2_rt_id);

				dpu_status_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
						| 17U;
				dpu_stat_index = asw_PUS_3_ID_to_not_mon_DB_idx(dpu_status_id);

				if (((dpu1_rt_index < DB_NOT_MONITORED_PARAM_NUMB)&& (dpu2_rt_index < DB_NOT_MONITORED_PARAM_NUMB) && (dpu_stat_index < DB_NOT_MONITORED_PARAM_NUMB))== TRUE) {
					icu_sem_lock(not_mon_db_sem_Id, 5);
					db_param[dpu1_rt_index].current_value = dpu1_active_rt;
					db_param[dpu2_rt_index].current_value = dpu2_active_rt;
					/* compose DPU MODE parameter */
					dpu_mode = (uint16_t) (dpu1_status << 8) | (uint16_t) (dpu2_status);
					db_param[dpu_stat_index].current_value = dpu_mode;
					icu_sem_unlock(not_mon_db_sem_Id);

					if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
						status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
					}
				}
			}
		}

	}

	return (status);
}

icu_status_code asw_PUS_8_1_2_8_set_thresholds(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t i = 0;
	uint16_t idx = 0;
	uint16_t size_in_bytes = 0, size_expected = 0;
	uint16_t th_shold_min = 0, th_shold_max = 0, par_id = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	size_expected = (tc.TC_pkt_data[4] * 3 * 2) + 1;

	/* Check parameters */
	if (size_in_bytes != size_expected) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		for (i = 0; i < tc.TC_pkt_data[5]; i++) {
			memcpy(&par_id, &tc.TC_pkt_data[(6 * i) + 5], sizeof(uint16_t));
			memcpy(&th_shold_min, &tc.TC_pkt_data[(6 * i) + 7],
					sizeof(uint16_t));
			memcpy(&th_shold_max, &tc.TC_pkt_data[(6 * i) + 9],
					sizeof(uint16_t));

			idx = asw_PUS_3_ID_to_mon_DB_idx(par_id);

			if (idx < DB_MONITORED_PARAM_NUMB) {
				icu_sem_lock(monit_db_sem_Id, 5);
				monitored_db_param[idx].lower_bound = th_shold_min;
				monitored_db_param[idx].upper_bound = th_shold_max;
				icu_sem_unlock(monit_db_sem_Id);

				if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
					status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId);
				}
			}

		}
	}

	return (status);
}
