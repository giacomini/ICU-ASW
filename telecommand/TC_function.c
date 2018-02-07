/**
 * \file	TC_function.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	May 05, 2015
 *
 */
#include "../rtems_configuration.h"

/**
 * 	\brief	 crc table is stored in this variable
 */
static uint16_t CRC16Table[256];

mask_TC_t ptrTC = { 0 };
telecommand_t telecommand = { .pkt_id = { 0 }, .pkt_seq = { 0 }, 0,
		.data_field = { 0 }, { 0 }, 0 };

/*Tc command table*/
mask_TC_t tc_table[NUMBER_OF_TC] = {
		{ 3, 1, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_1_set_new_HK_report },
		{ 3, 2, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_2_set_new_DIAG_report },
		{ 3, 3, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_3_clear_HK_report },
		{ 3, 4, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_4_clear_DIAG_report },
		{ 3, 5, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_5_enable_HK_report },
		{ 3, 6, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_6_disable_HK_report },
		{ 3, 7, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_7_enable_DIAG_report },
		{ 3, 8, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_8_disable_DIAG_report },
		{ 3, 9, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_9_get_HK_report_def },
		{ 3, 11, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_11_get_DIAG_rep_def },
		{ 3, 130, 0,(uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_130_set_HK_frequency },
		{ 3, 131, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_131_set_DIAG_freq },
		{ 3, 135, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_135_get_enabled_HK },
		{ 3, 137, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_137_get_enabled_DIAG },
		{ 3, 140, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_140_get_HK_onDemand },
		{ 3, 141, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_3_141_get_DIAG_onDemand },
		{ 5, 5, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_5_5_enable_event_gen },
		{ 5, 6, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_5_6_disable_event_gen },
		{ 6, 2, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_6_2_load_memory },
		{ 6, 5, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_6_5_dump_memory },
		{ 6, 9, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_6_9_check_memory },
		{ 9, 130, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_9_130_dis_time_synch },
		{ 9, 131, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_9_131_en_time_synch },
		{ 17, 1, 0, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_17_1_connection_test } };

/*PUS TC 8,1 table*/
mask_TC_t tc_8_1[NUMBER_OF_PUS_8_1_TCS] = {
		{ 8, 1, 0x10001U, (uint8_t) ICU_STANDBY, 10, asw_PUS_8_1_1_1_to_startup },
		{ 8, 1, 0x10002U,	(uint8_t) ICU_STARTUP | (uint8_t) ICU_IDLE | (uint8_t) ICU_SAFE, 10, asw_PUS_8_1_1_2_to_standby },
		{ 8, 1, 0x10003U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_1_3_to_idle },
		{ 8, 1, 0x10004U, (uint8_t) ICU_IDLE, 10, asw_PUS_8_1_1_4_to_observation },
		{ 8, 1, 0x10005U, (uint8_t) ICU_IDLE, 10, asw_PUS_8_1_1_5_to_manual },
		{ 8, 1, 0x10007U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL | (uint8_t) ICU_SAFE, 40, asw_PUS_8_1_1_7_to_boot },
		{ 8, 1, 0x20002U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL | (uint8_t) ICU_SAFE, 10, asw_PUS_8_1_2_2_set_fdir_mode },
		{ 8, 1, 0x20003U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_2_3_setDPUsAddress },
		{ 8, 1, 0x20004U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL | (uint8_t) ICU_SAFE, 10, asw_PUS_8_1_2_4_enable_fdir },
		{ 8, 1, 0x20005U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL | (uint8_t) ICU_SAFE, 10, asw_PUS_8_1_2_5_disable_fdir },
		{ 8, 1, 0x20006U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL | (uint8_t) ICU_SAFE, 10, asw_PUS_8_1_2_6_set_planning_ID },
		{ 8, 1, 0x20007U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_2_7_watchdog_en_dis },
		{ 8, 1, 0x20008U, (uint8_t) ICU_STARTUP | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_2_8_set_thresholds },
		{ 8, 1, 0x3000EU, (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_3_14_DPU_tx_dither },
		{ 8, 1, 0x3000FU, (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_3_15_DPU_tx_procPar },
		{ 8, 1, 0x30003U, (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_3_3_DPU_tx_sysConf },
		{ 8, 1, 0x40001U, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_4_1_SCE_IPCEXP },
		{ 8, 1, 0x40002U, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_4_2_SCE_KTCEXP },
		{ 8, 1, 0x40003U, (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_4_3_SCE_EXP },
		{ 8, 1, 0x40004U, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_4_4_SCE_FMODE },
		{ 8, 1, 0x40007U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_4_7_SCE_BOOT },
		{ 8, 1, 0x40008U, (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_4_8_SCE_ALIVE },
		{ 8, 1, 0x40009U, (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_4_9_SCE_SHSK },
		{ 8, 1, 0x50001U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_1_CPU_RBT },
		{ 8, 1, 0x50002U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_2_CPU_SST },
		{ 8, 1, 0x50003U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_3_CPU_STM },
		{ 8, 1, 0x50004U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_4_CPU_STDM },
		{ 8, 1, 0x50005U, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_5_DRB_SHSK },
		{ 8, 1, 0x5000BU, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_11_CPU_NOP },
		{ 8, 1, 0x5000CU, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_12_CPU_RSTC },
		{ 8, 1, 0x5000EU, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_14_CPU_WDG },
		{ 8, 1, 0x5000FU, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_15_CPU_MTST },
		{ 8, 1, 0x50010U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_16_CPU_MBE },
		{ 8, 1, 0x50012U, (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_18_CPU_DABT },
		{ 8, 1, 0x50014U, (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_5_20_DRB_CSPW },
		{ 8, 1, 0x60001U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_6_1_DCU_PWONOFF },
		{ 8, 1, 0x60007U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_6_7_DCU_SYNC },
		{ 8, 1, 0x60008U, (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_6_8_DCU_RST },
		{ 8, 1, 0x6000BU, (uint8_t) ICU_IDLE | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_6_11_DCU_SHSK }, /* check if it will be used in standby */
		{ 8, 1, 0x80001U, (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_8_x_move_wheels }, /*Execute profile */
		{ 8, 1, 0x80006U, (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_8_x_move_wheels }, /*Find Home position */
		{ 8, 1, 0x80008U, (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_8_x_move_wheels }, /*Check home position */
		{ 8, 1, 0x80009U, (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_8_x_move_wheels }, /*Connection check */
		{ 8, 1, 0x90001U, (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_9_1_leds_on_off },
		{ 8, 1, 0x90002U, (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_9_2_leds_reset },
		{ 8, 1, 0xA0001U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_10_1_heater_on_off }, /* FIXME if heaters control is possible in observation - check with IOCD!! */
		{ 8, 1, 0xA0002U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_IDLE | (uint8_t) ICU_OBSERVATION | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_10_2_heaters_reset },
		{ 8, 1, 0xB0001U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 60, asw_PUS_8_1_11_1_load_ASW_img },
		{ 8, 1, 0xB0003U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_11_3_jump_to_ASW },
		{ 8, 1, 0xB0007U, (uint8_t) ICU_STANDBY | (uint8_t) ICU_MANUAL, 10, asw_PUS_8_1_11_7_BSW_FDIR } };

void asw_tc_set_pkt_id(const uint8_t *tc_data) {
	uint16_t tmp = 0;

	/* set packet_id_section */
	tmp = *(tc_data);
	tmp = tmp << 8;
	tmp = tmp | (uint16_t) *(tc_data + 1);

	telecommand.pkt_id.version = (uint8_t) ((tmp & TC_VERSION_MASK) >> 13);
	telecommand.pkt_id.type = (uint8_t) ((tmp & TC_TYPE_MASK) >> 12);
	telecommand.pkt_id.data_field =
			(uint8_t) ((tmp & TC_DATA_FIELD_MASK) >> 11);
	telecommand.pkt_id.processId = tmp & TC_PROCESSID_MASK;
	return;
}

void asw_tc_set_pkt_seq(const uint8_t *tc_data) {
	uint16_t tmp = 0;

	/* set sequence number */
	tmp = *(tc_data + 2);
	tmp = tmp << 8;
	tmp = tmp | (uint16_t) *(tc_data + 3);
	telecommand.pkt_seq.seq_number = (uint8_t) ((tmp & TC_SEQUENCE_NUMBER_MASK)
			>> 14);
	telecommand.pkt_seq.seq_count = tmp & TC_SEQUENCE_COUNTER_MASK;
	return;
}

void asw_tc_set_pkt_length(const uint8_t *tc_data) {
	uint16_t tmp = 0;

	/* set data length */
	tmp = *(tc_data + 4);
	tmp = tmp << 8;
	tmp = tmp | (uint16_t) *(tc_data + 5);
	telecommand.TC_pkt_length = tmp;
	return;
}

void asw_tc_set_pkt_data_field(const uint8_t *tc_data) {

	telecommand.data_field.flag = (*(tc_data + 6) & TC_DATA_FIELD_FLAG) >> 7;
	telecommand.data_field.pus_version = (*(tc_data + 6) & TC_DATA_FIELD_PUS)
			>> 4;
	telecommand.data_field.ack = *(tc_data + 6) & TC_DATA_FIELD_ACK;
	telecommand.data_field.service_type = *(tc_data + 7);
	telecommand.data_field.service_subtype = *(tc_data + 8);
	telecommand.data_field.sourceId = *(tc_data + 9);

	return;
}

void asw_tc_set_pkt_data(const uint8_t *tc_data) {

	int16_t num_data = 0;
	uint16_t a = 0;

	num_data = (telecommand.TC_pkt_length - 6) + 1;

	if (((num_data > 0) && (num_data < TC_MAX_DATA_SIZE)) == TRUE) {
		for (a = 0; a < num_data; a++) {
			telecommand.TC_pkt_data[a] = *(tc_data + 10 + a);
		}
	}

	return;
}

void asw_tc_set_pkt_checksum(const uint8_t *tc_data) {

	uint16_t tmp = 0;

	tmp = *(tc_data + telecommand.TC_pkt_length + 5); /* 5 = telecommand.TC_pkt_length + 6 + 1 - 2 */
	tmp = tmp << 8;
	tmp = tmp | (uint16_t) *(tc_data + telecommand.TC_pkt_length + 6); /* 6 = telecommand.TC_pkt_length + 6 + 1 - 1 */

	telecommand.TC_err_control = tmp;

	return;
}

telecommand_t asw_create_tc(uint8_t *tc_data) {

	asw_tc_set_pkt_id(tc_data);
	asw_tc_set_pkt_seq(tc_data);
	asw_tc_set_pkt_length(tc_data);
	asw_tc_set_pkt_data_field(tc_data);
	asw_tc_set_pkt_data(tc_data);
	asw_tc_set_pkt_checksum(tc_data);

	return (telecommand);
}

uint16_t asw_validate_command(telecommand_t tc, uint8_t tc_data[],
		mask_TC_t *ptr_mask_tc) {

	uint16_t crc16 = 0, i = 0;
	uint16_t ret = CMD_OK;
	uint32_t tc_id = 0;
	uint8_t tc_found = 0;
	uint8_t err = 0;

	/* Check APID */
	if (tc.pkt_id.processId != ICU_ASW_APID) {
		ret = ILLEGAL_APID_ERROR;
	}
	/*Check TC data size limits*/
	else if ((((tc.TC_pkt_length + 7) > MIL_1553_MAX_TC_PKT_LENGTH)
			|| ((tc.TC_pkt_length + 7) < MIL_1553_MIN_TC_PKT_LENGTH)
			|| ((tc.TC_pkt_length + 7) != (uint16_t) handler_1553.DTD.size))
			== TRUE) {
		ret = ERROR_TC_LENGTH;
	} else {
		/* Evaluate CRC */
		/*5 = (num_of_header_bytes + 1) - num_of_checksum_bytes  ==> (6 + 1) - 2*/
		crc16 = asw_create_checksum(tc_data, tc.TC_pkt_length + 5);
		if (crc16 != tc.TC_err_control) {
			ret = INCORRECT_CHECKSUM;
		}
		/* Check PUS type */
		else if (((tc.data_field.service_type == 3)
				|| (tc.data_field.service_type == 5)
				|| (tc.data_field.service_type == 6)
				|| (tc.data_field.service_type == 8)
				|| (tc.data_field.service_type == 9)
				|| (tc.data_field.service_type == 17)) == FALSE) {
			ret = ILLEGAL_TYPE;
		} else {
			/* Check subtype */
			err = asw_check_subtype(tc.data_field.service_type,
					tc.data_field.service_subtype);
			if (err != 0) {
				ret = ILLEGAL_SUBTYPE;
			}
			/* Check fixed fields of PUS packet */
			else if (((tc.pkt_id.version != 0) || (tc.pkt_id.type != 1)
					|| (tc.pkt_id.data_field != 1)) == TRUE) {
				ret = ILLEGAL_PACKET_HEADER;
			} else {
				/* Search and retrieve TC info from TC tables */
				/*If service is 8,1 a different tc table will be used*/
				if ((tc.data_field.service_type == 8)
						&& (tc.data_field.service_subtype == 1)) {

					/* Read Function ID */
					memcpy(&tc_id, &tc.TC_pkt_data[0], sizeof(uint32_t));

					for (i = 0; i < NUMBER_OF_PUS_8_1_TCS; i++) {

						if (tc_id == tc_8_1[i].cmd_id) {
							memcpy(ptr_mask_tc, &tc_8_1[i], sizeof(tc_8_1[i]));
							tc_found = 1;
							break;
						}
					}
				} else {

					i = 0;
					for (i = 0; i < NUMBER_OF_TC; i++) {
						if ((tc.data_field.service_type == tc_table[i].type)
								&& (tc.data_field.service_subtype
										== tc_table[i].subtype)) {
							memcpy(ptr_mask_tc, &tc_table[i],
									sizeof(tc_table[i]));
							tc_found = 1;
							break;
						}
					}
				}
				/* FunctionID not found */
				if (tc_found == 0) {
					ret = CMD_ID_NOT_FOUND;
				}
				/* Check if TC could be executed in current state */
				else if ((ptr_mask_tc->modes & icu_state_machine) == 0) {
					ret = CMD_NOT_EXECUTABLE;
				} else {
					/* Command OK */
					ret = CMD_OK;
				}

			}
		}
	}

	return (ret);
}

void asw_init_CRC16_table(void) {

	uint16_t tmp = 0;
	uint16_t i = 0;

	/*Polynomial used to calculate and check the checksum is equal to x^16 + x^12 + x^5 + 1 */

	for (i = 0; i < 256; ++i) {
		tmp = 0;

		if ((i & 1U) != 0) {
			tmp = tmp ^ 0x1021U;
		}

		if ((i & 2U) != 0) {
			tmp = tmp ^ 0x2042U;
		}

		if ((i & 4U) != 0) {
			tmp = tmp ^ 0x4084U;
		}

		if ((i & 8U) != 0) {
			tmp = tmp ^ 0x8108U;
		}

		if ((i & 16U) != 0) {
			tmp = tmp ^ 0x1231U;
		}

		if ((i & 32U) != 0) {
			tmp = tmp ^ 0x2462U;
		}

		if ((i & 64U) != 0) {
			tmp = tmp ^ 0x48C4U;
		}

		if ((i & 128U) != 0) {
			tmp = tmp ^ 0x9188U;
		}

		CRC16Table[i] = tmp;
	}

	return;
}

uint16_t asw_create_checksum(uint8_t *pData, uint16_t numBytes) {

	uint32_t i = 0;
	uint16_t crc = 0;

	/*Init with 0xFFFF*/
	crc = 0xFFFF;

	/*Calculate CRC*/
	for (i = 0; i < (uint32_t) numBytes; i++) {
		crc = (((crc << 8U) & 0xFF00U)
				^ CRC16Table[(((crc >> 8U) ^ (uint16_t) pData[i]) & 0x00FFU)]);
	}

	*(pData + numBytes) = (uint8_t) ((crc & 0xFF00U) >> 8U);
	*(pData + numBytes + 1) = (uint8_t) (crc & 0x00FFU);

	return (crc);
}

uint16_t asw_create_ISO_checksum(uint8_t values[], uint16_t n) {
	uint16_t check = 0;
	int16_t i = 0;
	uint8_t C1 = 0;
	uint8_t C0 = 0;

	for (i = 0; i < n; i++) {
		C0 = (C0 + values[i]) % 255;
		C1 = (C1 + C0) % 255;
	}

	C0 = -((C0 + C1) % 254);

	if (C0 == 0) {
		values[n] = 255;
	} else {
		values[n] = C0;
	}

	if (C1 == 0) {
		values[n + 1] = 255;
	} else {
		values[n + 1] = C1;
	}

	check = ((uint16_t) values[n] << 8U) | (uint16_t) values[n + 1];

	return (check);
}

uint8_t asw_decript_ISO_checksum(uint8_t values[], uint16_t n) {
	uint8_t result = 0;
	uint8_t C0 = 0;
	uint8_t C1 = 0;
	uint16_t i = 0;

	for (i = 0; i < n; i++) {
		C0 = (C0 + values[i]) % 255;
		C1 = (C1 + C0) % 255;
	}

	if ((C0 == 0) && (C1 == 0)) {
		result = 0;
	} else {
		result = 1;
	}

	return (result);
}

uint8_t asw_check_subtype(uint8_t pus_type, uint8_t pus_subtype) {
	uint8_t ret = 1;

	switch (pus_type) {
	case 3:
		if ((((pus_subtype >= 1) && (pus_subtype < 10)) || (pus_subtype == 11)
				|| (pus_subtype == 130) || (pus_subtype == 131)
				|| (pus_subtype == 135) || (pus_subtype == 137)
				|| (pus_subtype == 140) || (pus_subtype == 141)) == TRUE) {
			ret = 0;
		}
		break;
	case 5:
		if (((pus_subtype == 5) || (pus_subtype == 6)) == TRUE) {
			ret = 0;
		}
		break;
	case 6:
		if (((pus_subtype == 2) || (pus_subtype == 5) || (pus_subtype == 9))
				== TRUE) {
			ret = 0;
		}
		break;
	case 8:
		if (pus_subtype == 1) {
			ret = 0;
		}
		break;
	case 9:
		if (((pus_subtype == 130) || (pus_subtype == 131)) == TRUE) {
			ret = 0;
		}
		break;
	case 17:
		if (pus_subtype == 1) {
			ret = 0;
		}
		break;
	default:
		break;
	}

	return (ret);
}
