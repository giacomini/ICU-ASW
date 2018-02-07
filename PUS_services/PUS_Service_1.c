/**
 * \file	PUS_Service_1.c
 *
 * \brief	PUS Service 1 management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Sep 11, 2015
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_1_success(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint8_t sub_type) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t pkt_id = 0, pkt_length = 0, TM_pack_seq = 0, grouping_flag = 0;
	uint32_t coarseTime = 0, fineTime = 0;
	uint16_t TM_pack_fineTime = 0, crc = 0;
	bool_t alreadyRead = 0;
	/* set the first two fields of packet header  */
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;
	uint8_t serial_buffer[TM_PKT_MAX_DATA_LENGTH_BYTES];

	/* No check on APID to avoid APID check Error during validation */
	/* APID used for packet creation, not the one received in TC - it could be wrong!! */
	pkt_id = (version_num << TM_VERSION_OFFSET) | (tm_type << TM_TYPE_OFFSET)
			| (data_field_header_flag << TM_DF_HEADER_FLAG_OFFSET)
			| (uint16_t) ICU_ASW_APID;

	memcpy(&(serial_buffer[0]), &pkt_id, sizeof(uint16_t));

	/*
	 * Packet data lenght = # octects in data field packet - 1
	 * data_field_header = 10 octects, fixed
	 * data_source = 6 octects + param_length
	 * packet_error_control = 2 octects, fixed
	 */
	pkt_length = 15;
	memcpy(&serial_buffer[4], &pkt_length, sizeof(uint16_t));

	/*
	 * prepare packet data field
	 */
	serial_buffer[6] = 16; /* pus_version - fixed: 00010000 */
	serial_buffer[7] = 1; /* pus type - fixed: 1 */
	serial_buffer[8] = sub_type;
	serial_buffer[9] = 1; /* destination ID - FIXME */

	/*
	 * prepare data source: TC packet ID + TC packet Sequence Control (total 4
	 * Bytes)
	 */
	memcpy(&serial_buffer[16], &TC_packet_ID, sizeof(uint16_t));
	memcpy(&serial_buffer[18], &TC_seq_control, sizeof(uint16_t));

	/*
	 * serialize packet for checksum
	 * define total size for buffer =
	 *    = packet header length (6 Bytes) + TM_pack->TM_packet_length + 1
	 */
	pkt_length = pkt_length + 1 + 6;

	/*
	 * grouping flag: 11(bin) - stand-alone packet
	 */
	grouping_flag = 3;

	/* Assign TM packet general counter */
	TM_pack_seq = (grouping_flag << TM_GROUPING_FLAG_OFFSET)
			| (TM_ss_counter[0] << TM_SSCOUNT_OFFSET);
	memcpy(&(serial_buffer[2]), &TM_pack_seq, sizeof(uint16_t));

	/* increase the TM packet general counter */
	icu_sem_lock(TM_cnt_sem_Id, 5);
	TM_ss_counter[0] = (TM_ss_counter[0] + 1) % TM_MAX_SEQ_COUNTER;
	icu_sem_unlock(TM_cnt_sem_Id);

	icu_obt_latch_obt();
	icu_obt_read_latched_obt(&coarseTime, &fineTime, &alreadyRead);

	TM_pack_fineTime = (uint16_t) ((fineTime & 0xFFFF0000) >> 16);

	memcpy(&(serial_buffer[10]), &coarseTime, sizeof(uint32_t));
	memcpy(&(serial_buffer[14]), &TM_pack_fineTime, sizeof(uint16_t));

	/* create_checksum adds the last DW */
	crc = asw_create_checksum(serial_buffer, (pkt_length - 2));

	/* Insert message in the queue */
	icu_sem_lock(queue_sem_id, 5);
	rtems_status = rtems_message_queue_send(pus_1_queue_id,
			(void *) serial_buffer, pkt_length);
	queues_with_data = queues_with_data | QUEUE_PUS_1_DATA_IN;
	icu_sem_unlock(queue_sem_id);

	return (status);
}

icu_status_code asw_PUS_1_failure(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint16_t error_code,
		uint8_t param_length, const uint8_t *param_list, uint8_t sub_type) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t pkt_id = 0, pkt_length = 0, TM_pack_seq = 0, grouping_flag = 0;
	uint32_t coarseTime = 0, fineTime = 0;
	uint16_t TM_pack_fineTime = 0, crc = 0;
	bool_t alreadyRead = 0;

	/* set the first two fields of packet header  */
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;
	uint8_t serial_buffer[TM_PKT_MAX_DATA_LENGTH_BYTES];

	/* No check on APID to avoid APID check Error during validation */
	/* APID used for packet creation, not the one received in TC - it could be wrong!! */
	pkt_id = (version_num << TM_VERSION_OFFSET) | (tm_type << TM_TYPE_OFFSET)
			| (data_field_header_flag << TM_DF_HEADER_FLAG_OFFSET)
			| (uint16_t) ICU_ASW_APID;

	memcpy(&(serial_buffer[0]), &pkt_id, sizeof(uint16_t));

	/*
	 * Packet data lenght = # octects in data field packet - 1
	 * data_field_header = 10 octects, fixed
	 * data_source = 6 octects + param_length
	 * packet_error_control = 2 octects, fixed
	 */
	pkt_length = (18 + param_length) - 1;
	memcpy(&serial_buffer[4], &pkt_length, sizeof(uint16_t));

	/*
	 * prepare packet data field
	 */
	serial_buffer[6] = 16; /* pus_version - fixed: 00010000 */
	serial_buffer[7] = 1; /* pus type - fixed: 1 */
	serial_buffer[8] = sub_type;
	serial_buffer[9] = 1; /* destination ID - FIXME */

	/*
	 * prepare data source: TC packet ID + TC packet Sequence Control (total 4
	 * Bytes)
	 */
	memcpy(&serial_buffer[16], &TC_packet_ID, sizeof(uint16_t));
	memcpy(&serial_buffer[18], &TC_seq_control, sizeof(uint16_t));
	memcpy(&serial_buffer[20], &error_code, sizeof(uint16_t));

	if (((param_length != 0) && (param_list != NULL)) == TRUE) {
		memcpy(&serial_buffer[22], param_list, param_length);
	}
	/*
	 * serialize packet for checksum
	 * define total size for buffer =
	 *    = packet header length (6 Bytes) + TM_pack->TM_packet_length + 1
	 */
	pkt_length = pkt_length + 1 + 6;

	/*
	 * grouping flag: 11(bin) - stand-alone packet
	 */
	grouping_flag = 3;

	/* Assign TM packet general counter */
	TM_pack_seq = (grouping_flag << TM_GROUPING_FLAG_OFFSET)
			| (TM_ss_counter[0] << TM_SSCOUNT_OFFSET);
	memcpy(&(serial_buffer[2]), &TM_pack_seq, sizeof(uint16_t));

	/* increase the TM packet general counter */
	icu_sem_lock(TM_cnt_sem_Id, 5);
	TM_ss_counter[0] = (TM_ss_counter[0] + 1) % TM_MAX_SEQ_COUNTER;
	icu_sem_unlock(TM_cnt_sem_Id);

	icu_obt_latch_obt();
	icu_obt_read_latched_obt(&coarseTime, &fineTime, &alreadyRead);

	TM_pack_fineTime = (uint16_t) ((fineTime & 0xFFFF0000) >> 16);

	memcpy(&(serial_buffer[10]), &coarseTime, sizeof(uint32_t));
	memcpy(&(serial_buffer[14]), &TM_pack_fineTime, sizeof(uint16_t));

	/* create_checksum adds the last DW */
	crc = asw_create_checksum(serial_buffer, (pkt_length - 2));

	/* Insert message in the queue */
	icu_sem_lock(queue_sem_id, 5);
	rtems_status = rtems_message_queue_send(pus_1_queue_id,
			(void *) serial_buffer, pkt_length);
	queues_with_data = queues_with_data | QUEUE_PUS_1_DATA_IN;
	icu_sem_unlock(queue_sem_id);

	return (status);
}

icu_status_code asw_PUS_1_1_TM_TC_accept_ok(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t subtype = 1;

	/*
	 * CHECK ON INPUT PARAMETERS: demanded to next function
	 */
	status = asw_PUS_1_success(TC_packet_ID, TC_seq_control, dest_ID, subtype);

	return (status);
}

icu_status_code asw_PUS_1_2_TM_TC_accept_fail(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint16_t error_code,
		uint8_t param_length, const uint8_t *param_list) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t subtype = 2;

	/*
	 * CHECK ON INPUT PARAMETERS: next function
	 */

	status = asw_PUS_1_failure(TC_packet_ID, TC_seq_control, dest_ID,
			error_code, param_length, param_list, subtype);

	return (status);
}

icu_status_code asw_PUS_1_7_TM_TC_exec_ok(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t subtype = 7;

	/*
	 * CHECK ON INPUT PARAMETERS: demanded to next function
	 */

	status = asw_PUS_1_success(TC_packet_ID, TC_seq_control, dest_ID, subtype);

	return (status);
}

icu_status_code asw_PUS_1_8_TM_TC_exec_fail(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint16_t error_code,
		uint8_t param_length, const uint8_t *param_list) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t subtype = 8;

	/*
	 * CHECK ON INPUT PARAMETERS: demanded to next function
	 */

	status = asw_PUS_1_failure(TC_packet_ID, TC_seq_control, dest_ID,
			error_code, param_length, param_list, subtype);

	return (status);
}
