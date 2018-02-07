/**
 * \file	PUS_Service_8_1_FDIR.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	27-06-2017
 *
 */

#include "../rtems_configuration.h"

void asw_initialize_fdir(void) {
	int8_t i = 0;
	uint16_t fdir_mode_par_ID = 0;
	uint16_t idx = 0, idx_long = 0;
	uint32_t value = 0;

	/* Get DB index of single_fdir_enabled parameter */
	fdir_mode_par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 21U;
	idx = asw_PUS_3_ID_to_not_mon_DB_idx(fdir_mode_par_ID);

	fdir_collection[EVENT_TASK_NOT_STARTED].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_TASK_NOT_STARTED].en_dis = TRUE;

	fdir_collection[EVENT_EEPROM_ERROR].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_EEPROM_ERROR].en_dis = TRUE;

	fdir_collection[EVENT_EDAC_ERROR].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_EDAC_ERROR].en_dis = TRUE;

	fdir_collection[EVENT_TM_ACQ_ERROR].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_TM_ACQ_ERROR].en_dis = TRUE;

	fdir_collection[EVENT_BC_1553_ERROR].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_BC_1553_ERROR].en_dis = TRUE;

	fdir_collection[EVENT_RT_1553_ERROR].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_RT_1553_ERROR].en_dis = TRUE;

	fdir_collection[EVENT_WATCHDOG_T1_EXPIRATION].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_WATCHDOG_T1_EXPIRATION].en_dis = TRUE;

	fdir_collection[EVENT_WHEEL_PROFILE_NOT_LOADED].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_WHEEL_PROFILE_NOT_LOADED].en_dis = TRUE;

	fdir_collection[EVENT_WHEELS_MOVEMENT_ERROR].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_WHEELS_MOVEMENT_ERROR].en_dis = TRUE;

	fdir_collection[EVENT_CU_WARNING].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_CU_WARNING].en_dis = TRUE;

	fdir_collection[EVENT_WHEELS_WARNING].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_WHEELS_WARNING].en_dis = TRUE;

	fdir_collection[EVENT_TEMPERATURE_WARNING].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_TEMPERATURE_WARNING].en_dis = TRUE;

	fdir_collection[EVENT_DAS_WARNING].ptrf = &asw_icu_to_safe;
	fdir_collection[EVENT_DAS_WARNING].en_dis = TRUE;

	for (i = 0; i < MAX_NUMBER_OF_FDIR; i++) {
		if (((i >= 0) && ((uint32_t) i < (8U * sizeof(value)))) == TRUE) {
			value = value | ((uint32_t) 1U << i);
		}
	}

	if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
		icu_sem_lock(not_mon_db_sem_Id, 5);
		idx_long = db_param[idx].current_value;
		if (idx_long < DB_LONG_PARAM_NUMBER) {
			db_long_curr_value[idx_long][0] = 0;
			db_long_curr_value[idx_long][1] = value;
		}
		icu_sem_unlock(not_mon_db_sem_Id);
	}
	/* Set general fdir mode HK parameter to enabled */
	fdir_mode_par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 20U;
	idx = asw_PUS_3_ID_to_not_mon_DB_idx(fdir_mode_par_ID);

	if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
		icu_sem_lock(not_mon_db_sem_Id, 5);
		db_param[idx].current_value = 1;
		icu_sem_unlock(not_mon_db_sem_Id);
	}
}

icu_status_code asw_PUS_8_1_2_2_set_fdir_mode(telecommand_t tc) {
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint8_t on_off_bit = 0;
	uint8_t errors = 1;
	uint16_t fdir_mode_par_ID = 0;
	uint16_t idx = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 5) {
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		on_off_bit = tc.TC_pkt_data[4];

		if (on_off_bit > 1) {
			icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
			icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {

			/* Set HK parameter FDIR Mode */
			fdir_mode_par_ID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
							| 20U;
			idx = asw_PUS_3_ID_to_not_mon_DB_idx(fdir_mode_par_ID);

			if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
				icu_sem_lock(not_mon_db_sem_Id, 5);
				db_param[idx].current_value = on_off_bit;
				icu_sem_unlock(not_mon_db_sem_Id);
			}

			/* Generate TM 1,7 */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				icu_status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	}
	return (icu_status);
}

icu_status_code asw_PUS_8_1_2_4_enable_fdir(telecommand_t tc) {
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint8_t errors = 1;
	uint16_t idx = 0, idx_long = 0;
	uint16_t fdir_mode_par_ID = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 5) {
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {

		if (tc.TC_pkt_data[4] >= MAX_NUMBER_OF_FDIR) {
			icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
			icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {

			/* Get DB index of single_fdir_enabled parameter */
			fdir_mode_par_ID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
							| 21U;
			idx = asw_PUS_3_ID_to_not_mon_DB_idx(fdir_mode_par_ID);

			fdir_collection[tc.TC_pkt_data[4]].en_dis = TRUE;

			if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
				icu_sem_lock(not_mon_db_sem_Id, 5);
				idx_long = db_param[idx].current_value;
				if (((idx_long < DB_LONG_PARAM_NUMBER)
						&& ((uint32_t) tc.TC_pkt_data[4]
								< (8U * sizeof(db_long_curr_value[idx_long][1]))))
						== TRUE) {
					if (((tc.TC_pkt_data[4] >= 0)
							&& (tc.TC_pkt_data[4] < (8 * sizeof(uint32_t))))
							== TRUE) {
						db_long_curr_value[idx_long][1] =
								db_long_curr_value[idx_long][1]
										| ((uint32_t) 1U << tc.TC_pkt_data[4]);
						db_long_curr_value[idx_long][0] = 0;

					}
				}
				icu_sem_unlock(not_mon_db_sem_Id);
			}

			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				icu_status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	}
	return (icu_status);
}

icu_status_code asw_PUS_8_1_2_5_disable_fdir(telecommand_t tc) {
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint8_t errors = 1;
	uint16_t idx = 0, idx_long = 0;
	uint16_t fdir_mode_par_ID = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 5) {
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {

		if (tc.TC_pkt_data[4] >= MAX_NUMBER_OF_FDIR) {
			icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
			icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {

			/* Get DB index of single_fdir_enabled parameter */
			fdir_mode_par_ID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET)
							| 21U;
			idx = asw_PUS_3_ID_to_not_mon_DB_idx(fdir_mode_par_ID);

			fdir_collection[tc.TC_pkt_data[4]].en_dis = FALSE;

			if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
				icu_sem_lock(not_mon_db_sem_Id, 5);
				idx_long = db_param[idx].current_value;
				if (((idx_long < DB_LONG_PARAM_NUMBER)
						&& ((uint32_t) tc.TC_pkt_data[4]
								< (8U * sizeof(db_long_curr_value[idx_long][1])))
						&& (tc.TC_pkt_data[4] >= 0)) == TRUE) {
					db_long_curr_value[idx_long][1] =
							db_long_curr_value[idx_long][1]
									& ~((uint32_t) 1U << tc.TC_pkt_data[4]);
					db_long_curr_value[idx_long][0] = 0;
				}
				icu_sem_unlock(not_mon_db_sem_Id);
			}

			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				icu_status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		}
	}

	return (icu_status);
}
