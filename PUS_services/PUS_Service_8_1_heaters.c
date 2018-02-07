/**
 * \file	PUS_Service_8_1_heaters.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	27-04-2017
 *
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_8_1_10_1_heater_on_off(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t power_level = 0;
	uint8_t errors[4];
	uint32_t ret = 0;
	uint16_t size_in_bytes = 0;
	uint8_t error_param_length = 0;
	uint16_t db_par_ID = 0;
	uint16_t idx = 0;
	icu_heater_id_t heater_id = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	memcpy(&power_level, &tc.TC_pkt_data[5], sizeof(uint16_t));

	/* Check parameters */
	if (size_in_bytes != 7) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (tc.TC_pkt_data[4] > HEATER_SA) {
		/* heater_id parameter out of bounds */
		errors[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (power_level > 1023) {
		/* Power_level out of bounds */
		errors[0] = 2;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Get heater ID */
		if ((uint32_t) tc.TC_pkt_data[4] == 0) {
			heater_id = HEATER_FPA;
		} else {
			heater_id = HEATER_SA;
		}

		/* if power_level is equal to zero turn off the heater */
		if (power_level == 0) {
			ret = icu_heater_switch_off(heater_id);
		} else {
			ret = icu_heater_switch_on(heater_id, power_level);
		}

		if (ret == 0) {

			/* Set HK parameter to power_level value */
			if (heater_id == HEATER_FPA) {
				db_par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_TC << PARAM_SUB_SYSTEM_OFFSET)
						| 2U;
				idx = asw_PUS_3_ID_to_not_mon_DB_idx(db_par_ID);
			} else {
				db_par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_TC << PARAM_SUB_SYSTEM_OFFSET)
						| 1U;
				idx = asw_PUS_3_ID_to_not_mon_DB_idx(db_par_ID);
			}

			if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
				icu_sem_lock(not_mon_db_sem_Id, 5);
				db_param[idx].current_value = power_level;
				icu_sem_unlock(not_mon_db_sem_Id);
			}

			/* Generate TM 1,7 */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		} else {
			memcpy(&errors[0], &ret, sizeof(uint32_t));
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_EXEC_ERROR, 4, errors);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		}

	}
	return (status);
}

icu_status_code asw_PUS_8_1_10_2_heaters_reset(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t errors[4];
	uint32_t ret = 0;
	uint16_t size_in_bytes = 0;
	uint8_t error_param_length = 0;
	uint16_t db_par_ID = 0;
	uint16_t idx_fpa = 0, idx_sa = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 4) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Reset heaters */
		ret = icu_heater_reset();

		if (ret == 0) {

			/* Get DB HK parameters indexes and set to 0*/
			db_par_ID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_TC << PARAM_SUB_SYSTEM_OFFSET)
							| 2U;
			idx_fpa = asw_PUS_3_ID_to_not_mon_DB_idx(db_par_ID);

			db_par_ID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_TC << PARAM_SUB_SYSTEM_OFFSET)
							| 1U;
			idx_sa = asw_PUS_3_ID_to_not_mon_DB_idx(db_par_ID);

			if (((idx_fpa < DB_NOT_MONITORED_PARAM_NUMB)&& (idx_sa < DB_NOT_MONITORED_PARAM_NUMB)) == TRUE) {
				icu_sem_lock(not_mon_db_sem_Id, 5);
				db_param[idx_fpa].current_value = 0;
				db_param[idx_sa].current_value = 0;
				icu_sem_unlock(not_mon_db_sem_Id);
			}

			/* Generate TM 1,7 */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}

		} else {
			memcpy(&errors[0], &ret, sizeof(uint32_t));
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_EXEC_ERROR, 4, errors);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		}
	}
	return (status);
}
