/**
 * \file	PUS_5_DB_def.c
 *
 * \brief	PUS Service 5 auxiliary variables and functions
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Sep 3, 2015
 */

#include "PUS_5_DB_def.h"
#include <public/basic_types.h>

void asw_PUS_5_initialize_EID(void) {
	uint16_t id = 0;

	/*
	 * Initialize the events structures
	 */

	/*
	 * Task not started event
	 * It has 1 parameter (2 bytes) to report task that did not start:
	 * bit[0..11] asw_task_rt, asw_task_bc, asw_task_tc, asw_task_atr, asw_task_tm, asw_task_idle, asw_task_exec, asw_task_fdir, asw_task_tm_acq, asw_taskTC_6_5, asw_task_wp_exec, task_monitor.
	 * bit[12..15] Not used
	 */
	EID[EVENT_TASK_NOT_STARTED].event_ID = 324;
	EID[EVENT_TASK_NOT_STARTED].event_priority = 4;
	EID[EVENT_TASK_NOT_STARTED].transm_delta_seconds = 1;
	EID[EVENT_TASK_NOT_STARTED].transm_delta_microseconds = 1;
	EID[EVENT_TASK_NOT_STARTED].last_transmission_seconds = 0;
	EID[EVENT_TASK_NOT_STARTED].last_transmission_microseconds = 0;
	EID[EVENT_TASK_NOT_STARTED].repetition_cnt = 0;
	EID[EVENT_TASK_NOT_STARTED].repetition_maximum = 1;

	/*
	 * ICU EEPROM error
	 * It is composed by 2 parameters:
	 * 1)Function generating error (1 byte):
	 * 0x1: isr;
	 * 0x2: selftest;
	 * 2) Code (4 bytes) returned by function selftest or FPGA_IRQ2_EEPROM register
	 */
	EID[EVENT_EEPROM_ERROR].event_ID = 325;
	EID[EVENT_EEPROM_ERROR].event_priority = 4;
	EID[EVENT_EEPROM_ERROR].transm_delta_seconds = 1;
	EID[EVENT_EEPROM_ERROR].transm_delta_microseconds = 1;
	EID[EVENT_EEPROM_ERROR].last_transmission_seconds = 0;
	EID[EVENT_EEPROM_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_EEPROM_ERROR].repetition_cnt = 0;
	EID[EVENT_EEPROM_ERROR].repetition_maximum = 1;

	/* EDAC error - no parameters */
	EID[EVENT_EDAC_ERROR].event_ID = 272;
	EID[EVENT_EDAC_ERROR].event_priority = 2;
	EID[EVENT_EDAC_ERROR].transm_delta_seconds = 1;
	EID[EVENT_EDAC_ERROR].transm_delta_microseconds = 1;
	EID[EVENT_EDAC_ERROR].last_transmission_seconds = 0;
	EID[EVENT_EDAC_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_EDAC_ERROR].repetition_cnt = 0;
	EID[EVENT_EDAC_ERROR].repetition_maximum = 1;

	/*
	 * ICU TM acquisition manager error
	 * It is composed by 2 parameters:
	 * 1)Function generating error (1 byte):
	 * 0x1: isr;
	 * 0x2: init;
	 * 0x4: load_table_ids;
	 * 0x8: start_tm_acq
	 * 2) Code returned by function (4 bytes) or FPGA_IRQ2_TM_ACQ_ERR register in case of isr
	 */
	EID[EVENT_TM_ACQ_ERROR].event_ID = 326;
	EID[EVENT_TM_ACQ_ERROR].event_priority = 4;
	EID[EVENT_TM_ACQ_ERROR].transm_delta_seconds = 1;
	EID[EVENT_TM_ACQ_ERROR].transm_delta_microseconds = 1;
	EID[EVENT_TM_ACQ_ERROR].last_transmission_seconds = 0;
	EID[EVENT_TM_ACQ_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_TM_ACQ_ERROR].repetition_cnt = 0;
	EID[EVENT_TM_ACQ_ERROR].repetition_maximum = 1;

	/* 1553 BC event - no parameters */
	EID[EVENT_BC_1553_ERROR].event_ID = 320;
	EID[EVENT_BC_1553_ERROR].event_priority = 3;
	EID[EVENT_BC_1553_ERROR].transm_delta_seconds = 0;
	EID[EVENT_BC_1553_ERROR].transm_delta_microseconds = 0;
	EID[EVENT_BC_1553_ERROR].last_transmission_seconds = 0;
	EID[EVENT_BC_1553_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_BC_1553_ERROR].repetition_cnt = 0;
	EID[EVENT_BC_1553_ERROR].repetition_maximum = 1;

	/*
	 * 1553 RT event
	 * parameters: the error code from DSW (4 Bytes)
	 */
	EID[EVENT_RT_1553_ERROR].event_ID = 321;
	EID[EVENT_RT_1553_ERROR].event_priority = 3;
	EID[EVENT_RT_1553_ERROR].transm_delta_seconds = 0;
	EID[EVENT_RT_1553_ERROR].transm_delta_microseconds = 0;
	EID[EVENT_RT_1553_ERROR].last_transmission_seconds = 0;
	EID[EVENT_RT_1553_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_RT_1553_ERROR].repetition_cnt = 0;
	EID[EVENT_RT_1553_ERROR].repetition_maximum = 1;

	/*
	 * Watchdog T1 Expiration
	 * It has only one parameter that is FPGA_IRQ2_WD_T1 register (4 bytes)
	 */
	EID[EVENT_WATCHDOG_T1_EXPIRATION].event_ID = 323;
	EID[EVENT_WATCHDOG_T1_EXPIRATION].event_priority = 3;
	EID[EVENT_WATCHDOG_T1_EXPIRATION].transm_delta_seconds = 0;
	EID[EVENT_WATCHDOG_T1_EXPIRATION].transm_delta_microseconds = 0;
	EID[EVENT_WATCHDOG_T1_EXPIRATION].last_transmission_seconds = 0;
	EID[EVENT_WATCHDOG_T1_EXPIRATION].last_transmission_microseconds = 0;
	EID[EVENT_WATCHDOG_T1_EXPIRATION].repetition_cnt = 0;
	EID[EVENT_WATCHDOG_T1_EXPIRATION].repetition_maximum = 1;

	/*
	 * Notify that NOT valid FDL or wheel profile was loaded by ASW
	 * It is composed by 3 parameters:
	 * 1) fdl_errors (1 byte): 0 means OK, 1 nominal FDL not loaded, 3 No FDl loaded
	 * 2) fwa_gwa_single_errors (2 bytes): bits 1 means that nominal profile are not loaded
	 * 3) fwa_gwa_double_errors (2 bytes): bits 1 means that redundant profile are not loaded
 	 */
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].event_ID = 289;
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].event_priority = 3;
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].transm_delta_seconds = 0;
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].transm_delta_microseconds = 0;
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].last_transmission_seconds = 0;
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].last_transmission_microseconds = 0;
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].repetition_cnt = 0;
	EID[EVENT_WHEEL_PROFILE_NOT_LOADED].repetition_maximum = 1;

	/*
	 * Notify a wheels operations error
	 * It is composed by 3 parameters:
	 * motor_id (1 Byte)
	 * on going operation (1 Byte)
	 * error code from DSW (4 Byte)
	 * operation phase (1 byte)
	 */
	EID[EVENT_WHEELS_MOVEMENT_ERROR].event_ID = 291;
	EID[EVENT_WHEELS_MOVEMENT_ERROR].event_priority = 3;
	EID[EVENT_WHEELS_MOVEMENT_ERROR].transm_delta_seconds = 0;
	EID[EVENT_WHEELS_MOVEMENT_ERROR].transm_delta_microseconds = 0;
	EID[EVENT_WHEELS_MOVEMENT_ERROR].last_transmission_seconds = 0;
	EID[EVENT_WHEELS_MOVEMENT_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_WHEELS_MOVEMENT_ERROR].repetition_cnt = 0;
	EID[EVENT_WHEELS_MOVEMENT_ERROR].repetition_maximum = 1;

	/*
	 * Notify that CU parameter is out of bound
	 * It is composed by 3 parameters:
	 * parID (2 Bytes)
	 * par_Value (2 Bytes)
	 * threshold (2 Bytes)
	 */
	EID[EVENT_CU_WARNING].event_ID = 304;
	EID[EVENT_CU_WARNING].event_priority = 3;
	EID[EVENT_CU_WARNING].transm_delta_seconds = 0;
	EID[EVENT_CU_WARNING].transm_delta_microseconds = 0;
	EID[EVENT_CU_WARNING].last_transmission_seconds = 0;
	EID[EVENT_CU_WARNING].last_transmission_microseconds = 0;
	EID[EVENT_CU_WARNING].repetition_cnt = 0;
	EID[EVENT_CU_WARNING].repetition_maximum = 1;

	/*
	 * Notify that a wheel phase is out of bound
	 * It is composed by 3 parameters:
	 * parID (2 Bytes)
	 * par_Value (2 Bytes)
	 * threshold (2 Bytes)
	 */
	EID[EVENT_WHEELS_WARNING].event_ID = 305;
	EID[EVENT_WHEELS_WARNING].event_priority = 3;
	EID[EVENT_WHEELS_WARNING].transm_delta_seconds = 0;
	EID[EVENT_WHEELS_WARNING].transm_delta_microseconds = 0;
	EID[EVENT_WHEELS_WARNING].last_transmission_seconds = 0;
	EID[EVENT_WHEELS_WARNING].last_transmission_microseconds = 0;
	EID[EVENT_WHEELS_WARNING].repetition_cnt = 0;
	EID[EVENT_WHEELS_WARNING].repetition_maximum = 1;

	/*
	 * Notify that a wheel phase is out of bound
	 * It is composed by 3 parameters:
	 * parID (2 Bytes)
	 * par_Value (2 Bytes)
	 * threshold (2 Bytes)
	 */
	EID[EVENT_TEMPERATURE_WARNING].event_ID = 306;
	EID[EVENT_TEMPERATURE_WARNING].event_priority = 3;
	EID[EVENT_TEMPERATURE_WARNING].transm_delta_seconds = 0;
	EID[EVENT_TEMPERATURE_WARNING].transm_delta_microseconds = 0;
	EID[EVENT_TEMPERATURE_WARNING].last_transmission_seconds = 0;
	EID[EVENT_TEMPERATURE_WARNING].last_transmission_microseconds = 0;
	EID[EVENT_TEMPERATURE_WARNING].repetition_cnt = 0;
	EID[EVENT_TEMPERATURE_WARNING].repetition_maximum = 1;

	/*
	 * Notify an error reading DAS_ALARMS register
	 * It is composed by 3 parameters:
	 * parID (2 Bytes)
	 * par_Value (2 Bytes)
	 * threshold (2 Bytes)
	 */
	EID[EVENT_DAS_WARNING].event_ID = 307;
	EID[EVENT_DAS_WARNING].event_priority = 3;
	EID[EVENT_DAS_WARNING].transm_delta_seconds = 0;
	EID[EVENT_DAS_WARNING].transm_delta_microseconds = 0;
	EID[EVENT_DAS_WARNING].last_transmission_seconds = 0;
	EID[EVENT_DAS_WARNING].last_transmission_microseconds = 0;
	EID[EVENT_DAS_WARNING].repetition_cnt = 0;
	EID[EVENT_DAS_WARNING].repetition_maximum = 1;

	/*
	 * Notify that a wheel phase is out of bound
	 * It is composed by 3 parameters:
	 * parID (2 Bytes)
	 * par_Value (2 Bytes)
	 * threshold (2 Bytes)
	 */
	EID[EVENT_TEMPERATURE_ALARM].event_ID = 308;
	EID[EVENT_TEMPERATURE_ALARM].event_priority = 4;
	EID[EVENT_TEMPERATURE_ALARM].transm_delta_seconds = 0;
	EID[EVENT_TEMPERATURE_ALARM].transm_delta_microseconds = 0;
	EID[EVENT_TEMPERATURE_ALARM].last_transmission_seconds = 0;
	EID[EVENT_TEMPERATURE_ALARM].last_transmission_microseconds = 0;
	EID[EVENT_TEMPERATURE_ALARM].repetition_cnt = 0;
	EID[EVENT_TEMPERATURE_ALARM].repetition_maximum = 1;

	/*
	 * Notify wheels operations
	 * It is composed by 3 parameters:
	 * motor_id: FWA or GWA (1 byte)
	 * type of notification (1 byte): parameter composed by the current operation and a notification [end of movement, start of movement or home sensor offset]
	 * offset (2 byte): offset in case of home check procedure, it is fixed to zero for all other notifications
	 * hs_read (4 bytes): Values read for the Home sensor status
	 */
	EID[EVENT_WHEEL_NOTIFICATION].event_ID = 288;
	EID[EVENT_WHEEL_NOTIFICATION].event_priority = 1;
	EID[EVENT_WHEEL_NOTIFICATION].last_transmission_seconds = 0;
	EID[EVENT_WHEEL_NOTIFICATION].last_transmission_microseconds = 0;
	EID[EVENT_WHEEL_NOTIFICATION].transm_delta_seconds = 1;
	EID[EVENT_WHEEL_NOTIFICATION].transm_delta_microseconds = 0;
	EID[EVENT_WHEEL_NOTIFICATION].repetition_cnt = 0;
	EID[EVENT_WHEEL_NOTIFICATION].repetition_maximum = 0;

	/* ICU in SAFE state - no parameters*/
	EID[EVENT_ICU_SAFE_STATE].event_ID = 328;
	EID[EVENT_ICU_SAFE_STATE].event_priority = 1;
	EID[EVENT_ICU_SAFE_STATE].last_transmission_seconds = 0;
	EID[EVENT_ICU_SAFE_STATE].last_transmission_microseconds = 0;
	EID[EVENT_ICU_SAFE_STATE].transm_delta_seconds = 1;
	EID[EVENT_ICU_SAFE_STATE].transm_delta_microseconds = 0;
	EID[EVENT_ICU_SAFE_STATE].repetition_cnt = 0;
	EID[EVENT_ICU_SAFE_STATE].repetition_maximum = 0;

	/*
	 * Incorrect tab ID
	 * It is composed by 2 arguments:
	 * 1) Subaddress (1 byte)
	 * 2) TableID read (2 Bytes)
	 */

	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].event_ID = 331;
	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].event_priority = 2;
	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].last_transmission_seconds = 0;
	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].last_transmission_microseconds = 0;
	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].transm_delta_seconds = 1;
	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].transm_delta_microseconds = 0;
	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].repetition_cnt = 0;
	EID[EVENT_DPU_TAB_ID_NOT_CORRECT].repetition_maximum = 1;

	/*
	 * Notify that NOT nominal FDL or wheel profile was loaded by ASW
	 * It is composed by 3 parameters:
	 * 1) fdl_errors (1 byte): 0 means OK, 1 nominal FDL not loaded, 3 No FDl loaded
	 * 2) fwa_gwa_single_errors (2 bytes): bits 1 means that nominal profile are not loaded
	 * 3) fwa_gwa_double_errors (2 bytes): bits 1 means that redundant profile are not loaded
	 */
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].event_ID = 290;
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].event_priority = 2;
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].transm_delta_seconds = 0;
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].transm_delta_microseconds = 0;
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].last_transmission_seconds = 0;
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].last_transmission_microseconds = 0;
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].repetition_cnt = 0;
	EID[EVENT_NOT_NOMINAL_WPF_LOADED].repetition_maximum = 1;

	/* Time event
	 * parameter: error generating function (1 Byte):
	 *   1 - self test
	 *   2 - write OBT
	 *   4 - preload OBT
	 */
	EID[EVENT_OBT_ERROR].event_ID = 327;
	EID[EVENT_OBT_ERROR].event_priority = 2;
	EID[EVENT_OBT_ERROR].transm_delta_seconds = 2;
	EID[EVENT_OBT_ERROR].transm_delta_microseconds = 0;
	EID[EVENT_OBT_ERROR].last_transmission_seconds = 0;
	EID[EVENT_OBT_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_OBT_ERROR].repetition_cnt = 0;
	EID[EVENT_OBT_ERROR].repetition_maximum = 10;

	/*
	 * The OBT read on SA 29 is not updated
	 * parameter: the received OBT (10 Bytes)
	 */
	EID[EVENT_OBT_NOT_UPDATED].event_ID = 322;
	EID[EVENT_OBT_NOT_UPDATED].event_priority = 3;
	EID[EVENT_OBT_NOT_UPDATED].transm_delta_seconds = 2;
	EID[EVENT_OBT_NOT_UPDATED].transm_delta_microseconds = 0;
	EID[EVENT_OBT_NOT_UPDATED].last_transmission_seconds = 0;
	EID[EVENT_OBT_NOT_UPDATED].last_transmission_microseconds = 0;
	EID[EVENT_OBT_NOT_UPDATED].repetition_cnt = 0;
	EID[EVENT_OBT_NOT_UPDATED].repetition_maximum = 2;

	/*
	 * Sendlist error event
	 * It is composed by 2 parameters:
	 * 1) The first parameter is used to identify the error (1 byte). Values could be:
	 * 		1 Error during creation of a sendlist
	 * 		2 Error when ASW try to read a sendlist
	 * 		4 Error during processing of a sendlist
	 * 		8 Error during transmission of a sendlist
	 * 		16 icu_mil_std_1553_bc_get_sendlist_control_word function returns with an error
	 * 		32 Sendlist Control register returned by icu_mil_std_1553_bc_get_sendlist_control_word function is no-zero value.
	 * 2) Sendlist Control register value (2 bytes). It has non-zero value only in case of Sendlist Control register error.
	 */
	EID[EVENT_SENDLIST_ERROR].event_ID = 330;
	EID[EVENT_SENDLIST_ERROR].event_priority = 2;
	EID[EVENT_SENDLIST_ERROR].transm_delta_seconds = 1;
	EID[EVENT_SENDLIST_ERROR].transm_delta_microseconds = 1;
	EID[EVENT_SENDLIST_ERROR].last_transmission_seconds = 0;
	EID[EVENT_SENDLIST_ERROR].last_transmission_microseconds = 0;
	EID[EVENT_SENDLIST_ERROR].repetition_cnt = 0;
	EID[EVENT_SENDLIST_ERROR].repetition_maximum = 1;

	/* Command not executed */
	EID[EVENT_CMD_NOT_EXECUTED].event_ID = 332;
	EID[EVENT_CMD_NOT_EXECUTED].event_priority = 2;
	EID[EVENT_CMD_NOT_EXECUTED].transm_delta_seconds = 1;
	EID[EVENT_CMD_NOT_EXECUTED].transm_delta_microseconds = 1;
	EID[EVENT_CMD_NOT_EXECUTED].last_transmission_seconds = 0;
	EID[EVENT_CMD_NOT_EXECUTED].last_transmission_microseconds = 0;
	EID[EVENT_CMD_NOT_EXECUTED].repetition_cnt = 0;
	EID[EVENT_CMD_NOT_EXECUTED].repetition_maximum = 1;

	/* Exposure terminated - 1 param: rt_address of DPU finishing exposure (2 Bytes) */
	EID[EVENT_END_OF_EXPOSURE].event_ID = 384;
	EID[EVENT_END_OF_EXPOSURE].event_priority = 1;
	EID[EVENT_END_OF_EXPOSURE].transm_delta_seconds = 0;
	EID[EVENT_END_OF_EXPOSURE].transm_delta_microseconds = 0;
	EID[EVENT_END_OF_EXPOSURE].last_transmission_seconds = 0;
	EID[EVENT_END_OF_EXPOSURE].last_transmission_microseconds = 0;
	EID[EVENT_END_OF_EXPOSURE].repetition_cnt = 0;
	EID[EVENT_END_OF_EXPOSURE].repetition_maximum = 0;

	/* Data transmission to MMU terminated - 1 param: rt_address of DPU finishing exposure (2 Bytes) */
	EID[EVENT_END_OF_MMU_TRANSMISSION].event_ID = 385;
	EID[EVENT_END_OF_MMU_TRANSMISSION].event_priority = 1;
	EID[EVENT_END_OF_MMU_TRANSMISSION].transm_delta_seconds = 0;
	EID[EVENT_END_OF_MMU_TRANSMISSION].transm_delta_microseconds = 0;
	EID[EVENT_END_OF_MMU_TRANSMISSION].last_transmission_seconds = 0;
	EID[EVENT_END_OF_MMU_TRANSMISSION].last_transmission_microseconds = 0;
	EID[EVENT_END_OF_MMU_TRANSMISSION].repetition_cnt = 0;
	EID[EVENT_END_OF_MMU_TRANSMISSION].repetition_maximum = 0;

	/* ICU has finished startup operation correctly - no parameters */
	EID[EVENT_STARTUP_COMPLETED].event_ID = 329;
	EID[EVENT_STARTUP_COMPLETED].event_priority = 1;
	EID[EVENT_STARTUP_COMPLETED].transm_delta_seconds = 0;
	EID[EVENT_STARTUP_COMPLETED].transm_delta_microseconds = 0;
	EID[EVENT_STARTUP_COMPLETED].last_transmission_seconds = 0;
	EID[EVENT_STARTUP_COMPLETED].last_transmission_microseconds = 0;
	EID[EVENT_STARTUP_COMPLETED].repetition_cnt = 0;
	EID[EVENT_STARTUP_COMPLETED].repetition_maximum = 0;

	/* Disable all events */
	for (id = 0; id < 7; id++) {
		event_flag[id] = 0;
	}

	return;
}

void asw_PUS_5_enable_event(uint16_t id) {
	uint16_t row = 0, shift = 0, tmp = 0;

	/* Enable event */
	tmp = id;
	shift = tmp % 32;
	row = tmp >> 5U; // id / 32
	if (((row < MAX_EVENT_FLAG_SIZE) && (shift >= 0)
			&& (shift < (uint16_t) (8U * sizeof(event_flag[0])))) == TRUE) {
		event_flag[row] = event_flag[row] | ((uint32_t) 1U << shift);
	}
	return;
}

void asw_PUS_5_disable_event(uint16_t id) {
	uint16_t row = 0, shift = 0, tmp = 0;

	/* Enable event */
	tmp = id;
	shift = tmp % 32;
	row = tmp >> 5U; // id / 32
	if (((row < MAX_EVENT_FLAG_SIZE) && (shift >= 0)
			&& (shift < (uint16_t) (8U * sizeof(event_flag[0])))) == TRUE) {
		event_flag[row] = event_flag[row] & ~((uint32_t) 1U << shift);
	}
	return;
}

void asw_PUS_5_enable_dpu_events(void) {
	asw_PUS_5_enable_event(EVENT_DPU_TAB_ID_NOT_CORRECT);
	asw_PUS_5_enable_event(EVENT_SENDLIST_ERROR);
	asw_PUS_5_enable_event(EVENT_BC_1553_ERROR);
	return;
}

void asw_PUS_5_disable_dpu_events(void) {
	asw_PUS_5_disable_event(EVENT_DPU_TAB_ID_NOT_CORRECT);
	asw_PUS_5_disable_event(EVENT_SENDLIST_ERROR);
	asw_PUS_5_disable_event(EVENT_BC_1553_ERROR);
	return;
}
