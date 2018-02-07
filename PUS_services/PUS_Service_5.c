/*
 * PUS_Service_5.c
 *
 *  Created on: Aug 6, 2015
 *      Author: D. Bonino
 */

#include "PUS_Service_5.h"
#include "../rtems_configuration.h"
#include "tm.h"

icu_status_code asw_PUS_5_TM_report(uint16_t event_idx,
		uint8_t parameter_length, const uint8_t *parameter_list) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t TM_pack_id = 0, TM_pack_seq = 0, TM_pack_length = 0;
	uint16_t grouping_flag = 3; /* stand-alone packet */
	uint16_t APID = ICU_ASW_APID;
	uint16_t crc = 0, i = 0;
	uint16_t length = 0;
	uint8_t serial_buffer[MIL_1553_MAX_TM_PKT_LENGTH];
	uint8_t row = 0, shift = 0, evt_transmit = 0;
	uint32_t mask = 1;
	rtems_clock_time_value current_time;
	uint32_t sec_sum = 0, microsec_sum = 0;
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;
	uint32_t coarseTime = 0, fineTime = 0;
	uint16_t TM_pack_fineTime = 0;
	bool_t alreadyRead = 0;

	/* first check if event is enabled */
	shift = (uint8_t) (event_idx % 32);
	row = (uint8_t) (event_idx >> 5); // id / 32
	mask = 1U << shift;

	if ((mask & event_flag[row]) == 0) {
		/*
		 * FIXME
		 */
	} else {
		rtems_status = rtems_clock_get(RTEMS_CLOCK_GET_TIME_VALUE,
				&current_time);
		/*
		 * Event enabled.
		 * Check if it is an event for a operation event (priority = 1) or for an error condition.
		 */
		if (EID[event_idx].event_priority == 1) {
			/*
			 * This event report a normal operational condition. No need for check for repetition
			 * or for error data.
			 */
			evt_transmit = 1;
		} else {
			EID[event_idx].repetition_cnt++;

			/*
			 * The event is transmitted if:
			 * - time of last event = 0 --> it is the first event from boot! Sent.
			 * - time from last event sent is >= transm_delta_t
			 * - time from last event sent < transm_delta_t, but this event has been
			 * repetead more than repetition_maximum times
			 */
			if ((EID[event_idx].last_transmission_seconds == 0)
					&& (EID[event_idx].last_transmission_microseconds == 0)) {
				evt_transmit = 1;
			} else {
				/*
				 * evaluate if it is time to generate an event TM packet, retrieving the current time
				 * and comparing it with (last_trasmission_time + Delta_time)
				 */
				sec_sum = EID[event_idx].last_transmission_seconds
						+ EID[event_idx].transm_delta_seconds;
				microsec_sum = EID[event_idx].last_transmission_microseconds
						+ EID[event_idx].transm_delta_microseconds;

				sec_sum += microsec_sum / TOD_MICROSECONDS_PER_SECOND;
				microsec_sum = microsec_sum % TOD_MICROSECONDS_PER_SECOND;

				if (current_time.seconds >= sec_sum) {
					/*	&& (current_time.microseconds >= microsec_sum)) { */
					evt_transmit = 1;
				} else {
					if (EID[event_idx].repetition_cnt
							>= EID[event_idx].repetition_maximum) {
						/*
						 * generate an event with different ID? FIXME
						 */
						evt_transmit = 1;
					}
				}
			}
		}

		if (evt_transmit == 1) {
			TM_pack_id = (version_num << TM_VERSION_OFFSET) | (tm_type
					<< TM_TYPE_OFFSET) | (data_field_header_flag
					<< TM_DF_HEADER_FLAG_OFFSET) | APID;

			/*
			 * Packet data lenght = # octects in data field packet - 1
			 * data_field_header = 10 octects, fixed
			 * data_source = event ID (2 Bytes) + param_length + eventually occurence counter (2 Bytes)
			 * packet_error_control = 2 octects, fixed
			 */
			TM_pack_length = (14 + parameter_length) - 1;
			if (EID[event_idx].event_priority > 1) {
				/* event for an error condition - add counter */
				TM_pack_length += 2;
			}
			/*
			 * packet primary header - still missing packet sequence control
			 */
			memcpy(&(serial_buffer[0]), &TM_pack_id, sizeof(uint16_t));
			memcpy(&(serial_buffer[4]), &TM_pack_length, sizeof(uint16_t));
			/*
			 * packet data header - still missing OBT
			 */
			serial_buffer[6] = 16; /* pus_version - fixed: 00010000 */
			serial_buffer[7] = 5; /* pus type - fixed: 3 */
			serial_buffer[8] = EID[event_idx].event_priority; /* pus subtype */
			serial_buffer[9] = 1; /* destination ID - FIXME */

			/*
			 * packet data source
			 */
			memcpy(&(serial_buffer[16]), &(EID[event_idx].event_ID), sizeof(uint16_t));
			for (i = 0; i < parameter_length; i++) {
				serial_buffer[18 + i] = parameter_list[i];
			}

			if (((EID[event_idx].event_priority > 1) && ((20 + parameter_length) < MIL_1553_MAX_TM_PKT_LENGTH)) == TRUE) {
				memcpy(&(serial_buffer[18 + parameter_length]), &(EID[event_idx].repetition_cnt), sizeof(uint16_t));
			}

			/*
			 * add missing information in headers
			 */
			/* Assign TM packet general counter */
			TM_pack_seq = (grouping_flag << TM_GROUPING_FLAG_OFFSET)
					| (TM_ss_counter[APID - ICU_ASW_APID] << TM_SSCOUNT_OFFSET);

			memcpy(&(serial_buffer[2]), &TM_pack_seq, sizeof(uint16_t));

			/* increase the TM packet general counter */
			icu_sem_lock(TM_cnt_sem_Id, 5);
			TM_ss_counter[APID - ICU_ASW_APID] =
					(TM_ss_counter[APID - ICU_ASW_APID] + 1) % TM_MAX_SEQ_COUNTER;
			icu_sem_unlock(TM_cnt_sem_Id);

			/* add OBT */
			icu_obt_latch_obt();
			icu_obt_read_latched_obt(&coarseTime, &fineTime, &alreadyRead);

			TM_pack_fineTime = (uint16_t) ((fineTime & 0xFFFF0000) >> 16);

			memcpy(&(serial_buffer[10]), &coarseTime, sizeof(uint32_t));
			memcpy(&(serial_buffer[14]), &TM_pack_fineTime, sizeof(uint16_t));

			/* asw_create_checksum function adds the crc */
			length = TM_pack_length + 1 + 6;
			crc = asw_create_checksum(serial_buffer, length - 2);

			/*Write event inside queues*/
			switch (EID[event_idx].event_priority) {
			case 1:
				icu_sem_lock(queue_sem_id, 5);
				rtems_status = rtems_message_queue_send(ev_5_1_queue_id,
						(void *) serial_buffer, length);
				queues_with_data |= QUEUE_5_1_DATA_IN;
				icu_sem_unlock(queue_sem_id);
				break;
			case 2:
				icu_sem_lock(queue_sem_id, 5);
				rtems_status = rtems_message_queue_send(ev_5_2_queue_id,
						(void *) serial_buffer, length);
				queues_with_data |= QUEUE_5_2_DATA_IN;
				icu_sem_unlock(queue_sem_id);
				break;
			case 3:
				icu_sem_lock(queue_sem_id, 5);
				rtems_status = rtems_message_queue_send(ev_5_3_queue_id,
						(void *) serial_buffer, length);
				queues_with_data |= QUEUE_5_3_DATA_IN;
				icu_sem_unlock(queue_sem_id);
				break;
			case 4:
				icu_sem_lock(queue_sem_id, 5);
				rtems_status = rtems_message_queue_send(ev_5_4_queue_id,
						(void *) serial_buffer, length);
				queues_with_data |= QUEUE_5_4_DATA_IN;
				icu_sem_unlock(queue_sem_id);
				break;
			default:
				break;
			}

			EID[event_idx].repetition_cnt = 0;
			EID[event_idx].last_transmission_seconds = current_time.seconds;
			EID[event_idx].last_transmission_microseconds
					= current_time.microseconds;
		} /* end --- if (evt_transmit == 1) */
	} /* end --- else of (event is enabled) */

	return (status);
}

icu_status_code asw_PUS_5_5_enable_event_gen(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t NEID = 0;
	uint16_t event_ID[MAX_EVENT_NUMBER], ev_idx = 0, tmp_idx = 0;
	uint8_t row = 0, shift = 0;
	uint32_t mask = 0;
	uint8_t event_check = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t size_in_bytes = 0, k = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	/*
	 * no need for range control due to parameter size (should be < 256) !!
	 */
	NEID = tc.TC_pkt_data[0];
	/*
	 * data_source_length is even, while parameter size is odd. Add 1
	 */
	if (size_in_bytes != (uint16_t) (2 + (NEID * 2))) {
		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {

		if (event_ID != NULL) {
			memcpy(event_ID, &(tc.TC_pkt_data[1]), NEID * sizeof(uint16_t));
			for (k = NEID; k < MAX_EVENT_NUMBER; k++) {
				event_ID[k] = 0;
			}
		}
		/*
		 * check each event
		 */
		k = 0;
		while ((k < (uint16_t) NEID) && (event_check == 0)) {
			if ((event_ID[k] < NI_ICU_EVENT_ID_MIN) || (event_ID[k]
					> NI_ICU_EVENT_ID_MAX)) {
				event_check = (uint8_t) k;
			}
			k++;
		}

		if (event_check != 0) {
			error_param_length = 1;
			error_param[0] = 2 + event_check;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			for (k = 0; k < (uint16_t) NEID; k++) {
				/*
				 * find the index of event_ID[k] - they are ordered
				 */
				ev_idx = event_ID[k] - 256;

				/* first check if event is yet enabled */
				tmp_idx = ev_idx;
				row = (uint8_t) (tmp_idx >> 5); // id / 32
				shift = (uint8_t) (ev_idx % 32);

				mask = 1U << shift;

				if ((mask & event_flag[row]) == 0) {
					/* event not enabled - enable it */
					event_flag[row] = event_flag[row] | (mask);
				}
			}

			/*
			 * everything was ok! Generate a TM(1,7)
			 */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}
		} /* end else (event_id in correct range) */

	} /* end else (tc data size not correct */

	return (status);
}

icu_status_code asw_PUS_5_6_disable_event_gen(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t NEID = 0;
	uint16_t event_ID[MAX_EVENT_NUMBER], ev_idx = 0, tmp_idx = 0;
	uint8_t row = 0, shift = 0;
	uint32_t mask = 0;
	uint8_t event_check = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint16_t size_in_bytes = 0, k = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	/*
	 * no need for range control due to parameter size (should be < 256) !!
	 */
	NEID = tc.TC_pkt_data[0];
	/*
	 * data_source_length is even, while parameter size is odd. Add 1
	 */
	if (size_in_bytes != (uint16_t) (2 + (NEID * 2))) {

		error_param_length = 0;
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		if (event_ID != NULL) {
			memcpy(event_ID, &(tc.TC_pkt_data[1]), NEID * sizeof(uint16_t));

			for (k = NEID; k < MAX_EVENT_NUMBER; k++) {
				event_ID[k] = 0;
			}
		}
		/*
		 * check each event
		 */
		k = 0;
		while ((k < (uint16_t) NEID) && (event_check == 0)) {
			if ((event_ID[k] < NI_ICU_EVENT_ID_MIN) || (event_ID[k]
					> NI_ICU_EVENT_ID_MAX)) {
				event_check = (uint8_t) k;
			}
			k++;
		}

		if (event_check != 0) {
			error_param_length = 1;
			error_param[0] = 2 + event_check;
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/*
			 * for each event ID, disable it
			 */

			for (k = 0; k < (uint16_t) NEID; k++) {
				/*
				 * find the index of event_ID[k] - they are ordered
				 */
				ev_idx = event_ID[k] - 256;

				tmp_idx = ev_idx;
				row = (uint8_t) (tmp_idx >> 5U); // id / 32
				shift = (uint8_t) (ev_idx % 32);
				mask = 1U << shift;

				if ((mask & event_flag[row]) != 0) {
					/* event not disabled - disable it */
					event_flag[row] &= ~(mask);
				}

			}
			/*
			 * everything was ok! Generate a TM(1,7)
			 */
			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId);
			}

		} /* end else (event_id in correct range) */
	} /* end else (tc data size not correct */

	return (status);
}
