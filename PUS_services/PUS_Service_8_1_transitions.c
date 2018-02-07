/**
 * \file	PUS_Service_8_1_transitions.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	10-05-2017
 *
 */

#include "../rtems_configuration.h"

state_machine_t icu_state_machine = ICU_BOOT;

icu_status_code asw_PUS_8_1_1_1_to_startup(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint16_t par_ID = 0;
	uint16_t idx = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 4) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Disable DPU HK packets */
		asw_PUS_3_disable_HK_rep_flag(2);
		asw_PUS_3_disable_HK_rep_flag(3);
		asw_PUS_3_disable_HK_rep_flag(4);
		asw_PUS_3_disable_HK_rep_flag(5);
		asw_PUS_3_disable_HK_rep_flag(6);

		/* Disable DPU events and fdir */
		asw_PUS_5_disable_dpu_events();

		icu_state_machine = ICU_STARTUP;
		par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
		idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

		if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_param[idx].current_value = (uint8_t) icu_state_machine;
			icu_sem_unlock(not_mon_db_sem_Id);
		}

		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_1_2_to_standby(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint16_t par_ID = 0;
	uint16_t idx = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 4) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Disable DPU HK packets */
		asw_PUS_3_disable_HK_rep_flag(2);
		asw_PUS_3_disable_HK_rep_flag(3);
		asw_PUS_3_disable_HK_rep_flag(4);
		asw_PUS_3_disable_HK_rep_flag(5);
		asw_PUS_3_disable_HK_rep_flag(6);

		/* Enable DPU events and fdir */
		asw_PUS_5_enable_dpu_events();

		icu_state_machine = ICU_STANDBY;
		par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
		idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

		if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_param[idx].current_value = (uint8_t) icu_state_machine;
			icu_sem_unlock(not_mon_db_sem_Id);
		}

		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	}
	return (status);
}

icu_status_code asw_PUS_8_1_1_3_to_idle(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint16_t par_ID = 0;
	uint16_t idx = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 4) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Enable DPU HK packets */
		asw_PUS_3_enable_HK_report_flag(2);
		asw_PUS_3_enable_HK_report_flag(3);
		asw_PUS_3_enable_HK_report_flag(4);
		asw_PUS_3_enable_HK_report_flag(5);
		asw_PUS_3_enable_HK_report_flag(6);

		icu_state_machine = ICU_IDLE;

		par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
		idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

		if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_param[idx].current_value = (uint8_t) icu_state_machine;
			icu_sem_unlock(not_mon_db_sem_Id);
		}

		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}

	}
	return (status);
}

icu_status_code asw_PUS_8_1_1_4_to_observation(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint16_t par_ID = 0;
	uint16_t idx = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 4) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		icu_state_machine = ICU_OBSERVATION;

		par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
		idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

		if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_param[idx].current_value = (uint8_t) icu_state_machine;
			icu_sem_unlock(not_mon_db_sem_Id);
		}

		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}

	}
	return (status);
}

icu_status_code asw_PUS_8_1_1_5_to_manual(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint16_t par_ID = 0;
	uint16_t idx = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 4) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		icu_state_machine = ICU_MANUAL;

		par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
		idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

		if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_param[idx].current_value = (uint8_t) icu_state_machine;
			icu_sem_unlock(not_mon_db_sem_Id);
		}

		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}

	}

	return (status);
}

icu_status_code asw_PUS_8_1_1_7_to_boot(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t size_in_bytes = 0;
	uint16_t par_ID = 0;
	uint16_t idx = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	/*Check parameters*/
	if (size_in_bytes != 4) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		icu_state_machine = ICU_BOOT;

		par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
		idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

		if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_param[idx].current_value = (uint8_t) icu_state_machine;
			icu_sem_unlock(not_mon_db_sem_Id);
		}

		rtems_status = rtems_task_wake_after(2500);
		icu_reset_set_forced_warm_reset_info(ASW_RESET_BY_TC,
				(uint32_t) dr_addr);
		icu_reset_generate_sw_reset();
	}
	return (status);
}

void asw_icu_to_safe(void) {
	uint32_t ret = 0;
	uint16_t par_ID = 0;
	uint16_t idx = 0;

	icu_status_code status = ICU_SUCCESSFUL;

	if (icu_state_machine != ICU_STARTUP) {
		/* Command DPUs in SAFE */

		/* Disable wheels and CU */
		icu_fwa_gwa_drv_stop();
		ret = icu_led_drv_stop_excitation();

		/* Create death report */
		asw_create_death_report();

		/* Generate packet 5,1 only if ICU is not in SAFE_STATE */
		if (icu_state_machine != ICU_SAFE) {
			status = asw_PUS_5_TM_report(EVENT_ICU_SAFE_STATE, 0, NULL );
		}

		icu_state_machine = ICU_SAFE;

		par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
				| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
		idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

		if (idx < DB_NOT_MONITORED_PARAM_NUMB) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			db_param[idx].current_value = (uint8_t) icu_state_machine;
			icu_sem_unlock(not_mon_db_sem_Id);
		}
	}

	return;
}
