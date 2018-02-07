/**
 * \file	PUS_Service_8_1_leds.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	27-04-2017
 *
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_8_1_9_1_leds_on_off(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t duty_cycle = 0;
	uint8_t errors[4] = { 0 };
	uint32_t ret = 0;
	uint16_t size_in_bytes = 0;
	uint8_t error_param_length = 0;
	uint16_t parID = 0;
	uint16_t par_index = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	memcpy(&duty_cycle, &tc.TC_pkt_data[6], sizeof(uint16_t));

	/* Check parameters */
	if (size_in_bytes != 8) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (tc.TC_pkt_data[4] > LED_5) {
		/* led_id parameter out of bounds */
		errors[0] = 1;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if ((duty_cycle > 1023) || (duty_cycle < 1)) {
		/* Power_level out of bounds */
		errors[0] = 3;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		parID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_CU << PARAM_SUB_SYSTEM_OFFSET) | 1U;
		par_index = asw_PUS_3_ID_to_not_mon_DB_idx(parID);

		/* if power_level is equal to zero turn off the led */
		if (tc.TC_pkt_data[5] == 0) {
			ret = icu_led_drv_stop_excitation();
			if (ret == 0) {
				if (par_index < DB_NOT_MONITORED_PARAM_NUMB) {
					icu_sem_lock(not_mon_db_sem_Id, 5);
					db_param[par_index].current_value = 0;
					icu_sem_unlock(not_mon_db_sem_Id);
				}

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
		} else {
			if (wheel_flag == 0) {
				ret = icu_led_drv_start_excitation((uint32_t) tc.TC_pkt_data[4],
						tc.TC_pkt_data[5], (uint32_t) duty_cycle);

				if (ret == 0) {
					if (par_index < DB_NOT_MONITORED_PARAM_NUMB) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						db_param[par_index].current_value = 1U
								<< tc.TC_pkt_data[4];
						icu_sem_unlock(not_mon_db_sem_Id);
					}

					if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
						status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId);
					}
				} else {
					memcpy(&errors[0], &ret, sizeof(uint32_t));
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_EXEC_ERROR, 4, errors);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				}
			} else {
				/* Motors enabled */
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_ICU_STATUS_ERROR, 0, NULL );
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			}
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_9_2_leds_reset(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t errors[4];
	uint32_t ret = 0;
	uint16_t size_in_bytes = 0;
	uint8_t error_param_length = 0;
	uint16_t parID = 0;
	uint16_t par_index = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	if (size_in_bytes != 4) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Reset leds */
		ret = icu_led_drv_reset();

		if (ret == 0) {
			parID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_CU << PARAM_SUB_SYSTEM_OFFSET)
							| 1U;
			par_index = asw_PUS_3_ID_to_not_mon_DB_idx(parID);
			if (par_index < DB_NOT_MONITORED_PARAM_NUMB) {
				icu_sem_lock(not_mon_db_sem_Id, 5);
				db_param[par_index].current_value = 0;
				icu_sem_unlock(not_mon_db_sem_Id);
			}

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

void asw_monitor_leds(void) {
//	uint16_t led_on_over_vol_h = icu_settings.leds_on + icu_settings.leds_offset;
//	uint16_t led_on_over_vol_l = icu_settings.leds_on - icu_settings.leds_offset;
//	uint16_t leds_off_over_vol_h = icu_settings.leds_off - icu_settings.leds_offset;
//	uint16_t leds_off_over_vol_l = icu_settings.leds_off - icu_settings.leds_offset;
	uint16_t parID = 0;
	uint16_t par_index = 0;
	uint16_t cu_status = 0;
	icu_led_drv_status_t led_status;
	uint32_t ret = 0;
	//uint16_t led_1 = 0, led_2 = 0, led_3 = 0, led_4 = 0, led_5 = 0;

	ret = icu_led_drv_get_status(&led_status);

	parID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_CU << PARAM_SUB_SYSTEM_OFFSET) | 1U;
	par_index = asw_PUS_3_ID_to_not_mon_DB_idx(parID);

	if (par_index < DB_NOT_MONITORED_PARAM_NUMB) {
		icu_sem_lock(not_mon_db_sem_Id, 5);
		cu_status = db_param[par_index].current_value;
		icu_sem_unlock(not_mon_db_sem_Id);
	}

	if (cu_status != 0) {
		switch (cu_status % 0x1F) {
		case 0x1U:
			//Led_1 <= MAX and other -0.10 < led_x < 0.10
			break;
		case 0x2U:
			//Led_2 <= MAX and other -0.10 < led_x < 0.10
			break;
		case 0x4U:
			//Led_3 <= MAX and other -0.10 < led_x < 0.10
			break;
		case 0x8U:
			//Led_4 <= MAX and other -0.10 < led_x < 0.10
			break;
		case 0x10U:
			//Led_5 < MAX and other -0.10 < led_x < 0.10
			break;
		default:
			break;
		}
	}

	return;
}
