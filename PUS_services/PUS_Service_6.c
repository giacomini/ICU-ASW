/**
 * \file	PUS_Service_6.c
 *
 * \brief	PUS Service 6 management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Mar 3, 2016
 */

#include "../rtems_configuration.h"
#include <public/mem_utils.h>
#include "interface/PUS_Service_6.h"

/***********************************************
 * private functions and variables
 ***********************************************/
uint8_t asw_pus6_mem_compare_crc(uint8_t *array, uint16_t size, uint16_t chksum);
void asw_pus6_mem_read(const uint8_t *address, uint32_t num_of_bytes,
		uint8_t *data);
/*
 * Calculate ISO checksum of a memory area of arbitrary length
 */
uint32_t asw_pus6_mem_calculate_ISO_checksum(uint8_t mem_id, uint32_t address,
		uint32_t size, uint16_t *checksum);

/**********************************************
 * public functions
 **********************************************/
icu_status_code asw_PUS_6_2_load_memory(telecommand_t tc) {
	// EUCL-TAST-ICD-1-003 V.6: memory_ID == 2 B  &  no more checksum

	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t load_res = 1, cmd_idx = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint8_t memory_data[TC_MAX_DATA_SIZE + 2]; // last two Bytes for CRC16
	uint8_t tab_idx = 0;
	uint8_t mem_ID_sys = 0, mem_ID_subsys = 0;
	uint8_t block_run_err = 0;
	uint16_t loop_index = 0, table_number = 0, buff_index = 0;
	uint16_t block_num = 0, crc = 0, table_length = 0, rt_addr = 0,
			size_in_bytes = 0, i = 0;
	uint16_t j = 0;
	uint16_t mem_load_tab[DPU_LOAD_MAX_DW];
	uint16_t load_cmd_counter = 0;
	uint32_t coarseTime = 0, fineTime = 0;
	bool_t alreadyRead;
	uint16_t msg_index_in_sl = 0;
	uint32_t start_address = 0;
	uint32_t size = 0; /* in SAU */
	uint16_t data_length = 0; /* in Bytes */

	//	uint32_t eeprom_error = 0;
	//	uint32_t failing_addr; // for eeprom_write_bytes()
	// CommandWord_t rx_cmd_word;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	mem_ID_sys = tc.TC_pkt_data[0];
	mem_ID_subsys = tc.TC_pkt_data[1];
	block_num = ((uint16_t) tc.TC_pkt_data[2] << 8U)
			| (uint16_t) tc.TC_pkt_data[3]; // number of non consecutive blocks

	/*
	 * check on parameters
	 * -- memory_ID
	 */
	if ((mem_ID_sys > MEM_SYS_DPU2_IDX)
			|| ((mem_ID_sys == MEM_SYS_ICU_IDX)
					&& ((mem_ID_subsys != MEM_SUBSYS_ICU_RAM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_ICU_REGISTER_IDX)))
			|| (((mem_ID_sys == MEM_SYS_DPU1_BSW_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_BSW_IDX))
					&& ((mem_ID_subsys != MEM_SUBSYS_DPU_BSW_RAM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_BSW_RAM_IDX)))
			|| (((mem_ID_sys == MEM_SYS_DPU1_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_IDX))
					&& ((mem_ID_subsys != MEM_SUBSYS_DPU_CPU_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_DBB_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_DRB_IDX)
							&& (!((mem_ID_subsys >= MEM_SUBSYS_DPU_DCU1_IDX)
									&& (mem_ID_subsys <= MEM_SUBSYS_DPU_DCU8_IDX)))))) {
		error_param_length = 1;
		error_param[0] = 1; // this means: parameters number 1 is wrong
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if ((block_num == 0)
			|| (block_num > MAX_NON_CONSEC_BLOCKS_TO_LOAD)) {
		/*
		 * block_num should be in [1,10] - EUCL-TAST-ICD-1-003 V.5
		 */
		error_param_length = 1;
		error_param[0] = 2; // this means: parameters number 2 is wrong
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * start the loop on block_num for loading memory
		 * switch on memory index - DPU ASW or BSW or ICU
		 */
		loop_index = 4; // starting index in the input parameter array

		if ((mem_ID_sys == MEM_SYS_DPU1_IDX)
				|| (mem_ID_sys == MEM_SYS_DPU2_IDX)) {
			// TODO: check the status of the DPU, should be MANUAL, INIT, SAFE
			// otherwise... ICU_DPU_ILLEGAL_STATE
			// return PUS(1,8) con errore TM_1_8_DPU_STATUS_ERROR

			/*
			 * for each non-consecutive address, prepare the MEMREG_LOAD_TAB# and load
			 * them
			 * !!!!!!!!!!!!!!!!!!!!!   TBC   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			 * for DPU, the ICU ASW will not accept non consecutive block
			 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			 */

			if (block_num != 1) {
				error_param_length = 1;
				error_param[0] = 2; // this means: parameters number 2 is wrong
				status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
						error_param);
				status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			} else {

				memcpy(&start_address, &(tc.TC_pkt_data[loop_index]),
						sizeof(uint32_t));
				memcpy(&size, &(tc.TC_pkt_data[loop_index + 4]),
						sizeof(uint32_t)); // number of SAUs

				data_length = (uint16_t) (size * PUS6_DPU_DCU_SAU); // in Byte
				loop_index = (uint16_t) (loop_index + 8 + data_length);

				/*
				 * checks:
				 * - data length: limited to size of MEMREG_TAB
				 * - correctness of the size of the TC parameters list
				 */
				if ((data_length == 0)
						|| (data_length > DPU_MEMREG_TAB_MAX_LENGTH_BYTE)) {
					error_param_length = 1;
					error_param[0] = 4; // this means: parameters number 4 is wrong
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				} else if (size_in_bytes != loop_index) {
					error_param_length = 0;
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length,
							NULL );
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				} else {
					/* check of memory limits is done by DPU */
					for (i = 0; i < data_length; i++) {
						memory_data[i] = tc.TC_pkt_data[loop_index + 8 + i];
					}
					/*
					 * compute the CRC16 to add to DPU tables
					 * createChecksum adds the last DW with the computed CRC
					 */
					crc = asw_create_checksum(memory_data, data_length);

					data_length += 2; // now length of mem_data is 2 bytes more for CRC!!

					if (mem_ID_sys == MEM_SYS_DPU1_IDX) {
						rt_addr = dpu1_active_rt;
						msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
						tab_idx = 0;
					} else {
						rt_addr = dpu2_active_rt;
						msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
						tab_idx = 0;
					}

					/*
					 * fill the MEMREG_LOAD_TAB1-16 on SA 1-16, 32 DW maximum each. Last DW is the CRC16
					 * since data_length <= 1004, table_number can not be > 16
					 * -1 useful if data_length % 64 = 0, to avoid to add one more table - data_length, here, can not be zero
					 */
					if (data_length <= (DPU_MEMREG_TAB_1_MAX_DW * 2)) {
						table_number = 1;
					} else {
						table_number = (uint16_t) (((data_length - 1
								- (DPU_MEMREG_TAB_1_MAX_DW * 2))
								/ (DPU_MEMREG_TAB_2_16_MAX_DW * 2)) + 2);
					}

					i = 0;
					load_res = 1;
					while ((i < table_number) && (load_res == 1)) {
						if (i == 0) {
							/* fill table 1 - header different from other tables */
							mem_load_tab[0] =
									(uint16_t) (((uint16_t) MEMREG_LOAD_TAB1
											<< 8U) | 0U);
							icu_obt_latch_obt();
							icu_obt_read_latched_obt(&coarseTime, &fineTime,
									&alreadyRead);
							memcpy(&(mem_load_tab[2]), &coarseTime,
									sizeof(uint32_t));
							mem_load_tab[4] = (uint16_t) ((fineTime
									& 0xFFFF0000U) >> 16U);
							/* the following values are only for SA 1 */
							mem_load_tab[5] = mem_ID_subsys;
							/*FIXME: Check if data length must consider crc length (2 bytes) or not */
							mem_load_tab[6] = (data_length - 2) / 2;
							mem_load_tab[7] = (uint16_t) ((start_address
									& 0xFFFF0000U) >> 16U);
							mem_load_tab[8] = (uint16_t) (start_address
									& 0x0000FFFFU);
							load_cmd_counter = ++ICU_command_counter;
							mem_load_tab[1] = load_cmd_counter;

							/*
							 * fill with data
							 */
							if (data_length <= (DPU_MEMREG_TAB_1_MAX_DW * 2)) {
								memcpy(&(mem_load_tab[9]),
										&memory_data[buff_index], data_length);
							} else {
								memcpy(&(mem_load_tab[9]),
										&memory_data[buff_index],
										(DPU_MEMREG_TAB_1_MAX_DW * 2));
							}
							buff_index = DPU_MEMREG_TAB_1_MAX_DW * 2;
						} else if ((i != 0) && (i == (table_number - 1))) {
							/* fill last table - size can be different from other tables */

							table_length = (uint16_t) ((data_length
									- (DPU_MEMREG_TAB_1_MAX_DW * 2))
									% (DPU_MEMREG_TAB_2_16_MAX_DW * 2)); // can be zero, if data_length = 64
							if (table_length == 0) {
								table_length = DPU_MEMREG_TAB_2_16_MAX_DW * 2;
							}

							memcpy(&(mem_load_tab[5]), &memory_data[buff_index],
									table_length);
						} else {
							table_length = DPU_MEMREG_TAB_2_16_MAX_DW * 2;
							memcpy(&(mem_load_tab[5]), &memory_data[buff_index],
									table_length);
							buff_index = i * (DPU_MEMREG_TAB_2_16_MAX_DW * 2);
						}

						/*
						 * prepare command word and send it
						 */
						status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY,
								mem_load_tab,
								(uint8_t) (BASE_MEM_LOAD_CF + rt_addr), rt_addr,
								RECEIVE_BIT_1553, (i + 1), CMD_WORD_32_DW,
								(MEM_LOAD_FIRST_MSG_INDEX + i));

						if (status != ICU_SUCCESSFUL) {
							error_param_length = sizeof(icu_status_code);
							/*
							 * status is now containing the error code from
							 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
							 */
							memcpy(error_param, &status,
									sizeof(icu_status_code));
							status = asw_PUS_1_8_TM_TC_exec_fail(
									tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
									tc.data_field.sourceId,
									TM_1_8_BC_1553_RW_ERROR, error_param_length,
									error_param);
							status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED,
									0, NULL );
							status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0,
									NULL );
							load_res = 0;
						}
						i++;
					}

					if (load_res == 1) {
						/*
						 * send CPU_MLOAD command
						 */
						mem_load_tab[0] =
								((uint16_t) CPU_MLOAD_COMMAND_ID << 8U) | 1U;
						mem_load_tab[1] = load_cmd_counter;
						mem_load_tab[2] = mem_ID_subsys;
						/*FIXME: Check if data length must consider crc length (2 bytes) or not */
						mem_load_tab[3] = (uint16_t) ((data_length - 2) / 2);
						mem_load_tab[4] = (uint16_t) (start_address
								& 0x0000FFFFU);
						mem_load_tab[5] = (uint16_t) ((start_address
								& 0xFFFF0000U) >> 16U);

						status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY,
								mem_load_tab,
								(uint8_t) (BASE_COMMAND_CF + rt_addr), rt_addr,
								RECEIVE_BIT_1553, COMMAND_DPU_SA, 6,
								msg_index_in_sl);
						if (status != ICU_SUCCESSFUL) {
							/*
							 * problem loading SA tables. Prepare TM(1,8)
							 * status is now containing the error code from
							 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
							 */
							error_param_length = sizeof(icu_status_code);
							memcpy(error_param, &status,
									sizeof(icu_status_code));
							status = asw_PUS_1_8_TM_TC_exec_fail(
									tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
									tc.data_field.sourceId,
									TM_1_8_BC_1553_RW_ERROR, error_param_length,
									NULL );
							status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0,
									NULL );
							status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED,
									0, NULL );
						} else {
							/*
							 * save info in the DPU command history - it will be responsability of the CMD_VER_TAB
							 * the production of TM(1,7/8)
							 */
							status = asw_push_cmd_tag(tc.data_field.ack,
									tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
									tc.data_field.sourceId, tab_idx,
									mem_load_tab[0], load_cmd_counter, 0,
									0, &cmd_idx);
						}
					}
				}
			}
		} /* end if (mem_ID_sys == MEM_SYS_DPU1/DPU2_IDX) */

		if (mem_ID_sys == MEM_SYS_ICU_IDX) {
			/*
			 * we are writing in the ICU RAM
			 * in this case we can accept non consecutive blocks
			 */
			i = 0;
			while ((i < block_num) && (block_run_err == 0)) {
				memcpy(&start_address, &(tc.TC_pkt_data[loop_index]),
						sizeof(uint32_t));
				memcpy(&size, &(tc.TC_pkt_data[loop_index + 4]),
						sizeof(uint32_t)); // number of SAUs
				if (mem_ID_subsys == MEM_SUBSYS_ICU_REGISTER_IDX) {
					data_length = (uint16_t) size * PUS6_ICU_REGISTER_SAU; // in Byte
				} else {
					data_length =
							(uint16_t) size * PUS6_ICU_RAM_EEPROM_PROM_SAU; // in Byte
				}

				if (!(icu_mem_utils_ram_check_limits(start_address, data_length))) { // TRUE=1/FALSE=0
					error_param_length = 1;
					error_param[0] = 7 + (3 * (uint8_t) block_num); // error on parameter
					/*
					 * The error could also be due to a problem on previous parameters, e.g.
					 * if previous length
					 * was uncorrect, I do not read an address... who knows?
					 * prepare TM(1,8) - TC failure
					 */
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
					block_run_err = 1;
				} else {
					for (j = 0; j < data_length; j++) {
						memory_data[j] = tc.TC_pkt_data[(uint16_t) (loop_index
								+ 8 + j)];
					}

					icu_mem_utils_mem_copy((uint8_t *) start_address,
							memory_data, data_length);

					loop_index = (uint16_t) (loop_index + 8 + data_length);
					i++;
				}
			}

			if (block_run_err == 0) {
				/*
				 * now check the size of the parameters
				 */
				if (size_in_bytes != loop_index) {
					error_param_length = 0;
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length,
							NULL );
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				} else {
					/*
					 * everything was ok! Generate a TM(1,7)
					 */
					if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
						status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId);
					}
				}
			}
		} /* end  if(memory_ID == ICU_RAM_INDEX) */

		if ((mem_ID_sys == MEM_SYS_DPU1_BSW_IDX)
				|| (mem_ID_sys == MEM_SYS_DPU1_BSW_IDX)) {
			/* BSW */
		}
	}/* end else (check on block number and memory_IDs) */
	return (status);
}

icu_status_code asw_PUS_6_5_dump_memory(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t ret = 0;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0,
			i = 0, error_status = 0;
	uint8_t mem_ID_sys = 0;
	uint8_t mem_ID_subsys = 0;
	uint16_t size_in_bytes = 0;
	uint32_t start_address = 0, SAU_length = 0, length = 0; // SAU_length in SAU, length in Bytes
	uint32_t eeprom_error_code = 0, p_failing_addr = 0;
	uint8_t dump_data[MAX_MEM_RW_DATA_LENGTH];
	uint32_t packet_length = 0, j = 0;
	uint8_t packet_data[MAX_TM_6_6_DATA_LENGTH_BYTE], TM_packet_number = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	/*
	 * extract parameters from the input array
	 */
	mem_ID_sys = tc.TC_pkt_data[0];
	mem_ID_subsys = tc.TC_pkt_data[1];
	memcpy(&start_address, &(tc.TC_pkt_data[2]), sizeof(uint32_t));
	memcpy(&SAU_length, &(tc.TC_pkt_data[6]), sizeof(uint32_t)); // number of SAUs

	/*
	 * check on parameters
	 * -- size of received parameters
	 * -- memory_ID
	 */
	if (size_in_bytes != 10) {
		error_param_length = 0;
		/* prepare TM(1,8) - TC failure */
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if ((mem_ID_sys > MEM_SYS_DPU2_IDX)
			|| ((mem_ID_sys == MEM_SYS_ICU_IDX)
					&& ((mem_ID_subsys != MEM_SUBSYS_ICU_RAM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_ICU_PROM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_ICU_EEPROM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_ICU_REGISTER_IDX)))
			|| (((mem_ID_sys == MEM_SYS_DPU1_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_IDX))
					&& ((mem_ID_subsys != MEM_SUBSYS_DPU_CPU_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_DBB_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_DRB_IDX)
							&& (!((mem_ID_subsys >= MEM_SUBSYS_DPU_DCU1_IDX)
									&& (mem_ID_subsys <= MEM_SUBSYS_DPU_DCU8_IDX)))
							&& (!((mem_ID_subsys >= MEM_SUBSYS_DPU_SCE1_IDX)
									&& (mem_ID_subsys <= MEM_SUBSYS_DPU_SCE8_IDX)))))
			|| (((mem_ID_sys == MEM_SYS_DPU1_BSW_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_BSW_IDX))
					&& ((mem_ID_subsys != MEM_SUBSYS_DPU_BSW_EEPROM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_BSW_RAM_IDX)))) {
		error_param_length = 1;
		error_param[0] = 1; // this means: parameters number 1 is wrong
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * compute data length
		 */
		switch (mem_ID_sys) {
		case MEM_SYS_ICU_IDX:
			if (mem_ID_subsys == MEM_SUBSYS_ICU_REGISTER_IDX) {
				length = SAU_length * PUS6_ICU_REGISTER_SAU;
			} else {
				length = SAU_length * PUS6_ICU_RAM_EEPROM_PROM_SAU;
			}
			break;
		case MEM_SYS_DPU1_BSW_IDX:
		case MEM_SYS_DPU2_BSW_IDX:
			length = SAU_length * PUS6_DPU_BSW_SAU;
			break;
		case MEM_SYS_DPU1_IDX:
		case MEM_SYS_DPU2_IDX:
			if ((mem_ID_subsys >= MEM_SUBSYS_DPU_SCE1_IDX)
					&& (mem_ID_subsys <= MEM_SUBSYS_DPU_SCE8_IDX)) {
				length = SAU_length * PUS6_SCE_SAU; // in Byte
			} else {
				length = SAU_length * PUS6_DPU_DCU_SAU; // in Byte
			}
			break;
		}

		/*
		 * check the length against maximum dump length
		 */
		if ((length == 0) || (length > MAX_MEM_RW_DATA_LENGTH)) {
			error_param_length = 1;
			error_param[0] = 3; // this means: parameters number 4 is wrong
			status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
					error_param);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			/* from here the function exits */
		} else {

			if ((mem_ID_sys == MEM_SYS_DPU1_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU1_IDX)) {

				/*
				 * managed by a task, also generation of TM(1,7/8) at the end of process
				 */
				/*
				 * first notepad: tc_pkt_id (2 Bytes)
				 * second notepad: tc_pkt_src (2 B)
				 * third notepad: tc_data_field header - for source and acknowledge (4 B)
				 * fourth notepad: memory_ID (2 Bytes)
				 * fifth notepad: start_address ( 4 B)
				 * sixth notepad: length in Bytes (4 B)
				 */
				rtems_status = rtems_task_set_note(taskTC_6_5_Id,
						RTEMS_NOTEPAD_0, tc.pkt_id.all_bits);
				rtems_status = rtems_task_set_note(taskTC_6_5_Id,
						RTEMS_NOTEPAD_1, tc.pkt_seq.all_bits);
				rtems_status = rtems_task_set_note(taskTC_6_5_Id,
						RTEMS_NOTEPAD_2, tc.data_field.all_bits);
				rtems_status = rtems_task_set_note(taskTC_6_5_Id,
						RTEMS_NOTEPAD_3,
						((uint16_t) mem_ID_sys << 8U)
								| (uint16_t) mem_ID_subsys);
				rtems_status = rtems_task_set_note(taskTC_6_5_Id,
						RTEMS_NOTEPAD_4, start_address);
				rtems_status = rtems_task_set_note(taskTC_6_5_Id,
						RTEMS_NOTEPAD_5, length);

				rtems_status = rtems_task_restart(taskTC_6_5_Id, 0);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					status = ICU_UNSATISFIED;
				}
				/*
				 * now function exits and task can start
				 */
			} else if ((mem_ID_sys == MEM_SYS_DPU1_BSW_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_BSW_IDX)) {
				/*
				 * TODO BSW
				 */
			} else if (mem_ID_sys == MEM_SYS_ICU_IDX) {
				/* system = ICU */

				if (mem_ID_subsys == MEM_SUBSYS_ICU_RAM_IDX) {

					if (!(icu_mem_utils_ram_check_limits(start_address, length))) { // TRUE=1/FALSE=0
						error_param_length = 1;
						error_param[0] = 2; // error on parameter 2: start address - or length
						/* prepare TM(1,8) - TC failure */
						status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId,
								TM_1_8_TC_PARAM_OUT_OF_BOUNDS,
								error_param_length, error_param);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						error_status = 1;
					} else {
						asw_pus6_mem_read((uint8_t *) start_address, length,
								dump_data);
					}
				} // end  if(memory_ID == ICU_RAM_INDEX)

				if (mem_ID_subsys == MEM_SUBSYS_ICU_EEPROM_IDX) {
					if (!(icu_mem_utils_eeprom_check_limits(start_address,
							length))) { // TRUE=1/FALSE=0
						error_param_length = 1;
						error_param[0] = 2; // error on parameter 2: start address - or length
						/* prepare TM(1,8) - TC failure */
						status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId,
								TM_1_8_TC_PARAM_OUT_OF_BOUNDS,
								error_param_length, error_param);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
						error_status = 1;
					} else {
						eeprom_error_code = icu_eeprom_read_bytes(start_address,
								length, dump_data, &p_failing_addr);
						/* SUCCESS == 0 */
						if (eeprom_error_code != 0) {
							error_param_length = 4;
							memcpy(&error_param[0], &eeprom_error_code,
									sizeof(uint32_t));
							/* prepare TM(1,8) - TC failure */
							status = asw_PUS_1_8_TM_TC_exec_fail(
									tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
									tc.data_field.sourceId,
									TM_1_8_PUS6_EEPROM_READ_ERROR,
									error_param_length, error_param);
							status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED,
									0, NULL );
						}
						error_status = 1;
					}
				} /* end if (memory_ID == ICU_EEPROM_INDEX) */

				if (error_status == 0) {
					/*
					 * generation of PUS(6,6) TM packets
					 * compute the maximum size of each TM packet
					 */
					TM_packet_number = (uint8_t) (((length - 1)
							/ MAX_TM_6_6_DATA_LENGTH_BYTE) + 1); // in Bytes

					i = 0;
					while ((i < TM_packet_number) && (ret == 0)) {
						if (i == TM_packet_number - 1) {
							packet_length =
									length % MAX_TM_6_6_DATA_LENGTH_BYTE;
							if (packet_length == 0) {
								packet_length = MAX_TM_6_6_DATA_LENGTH_BYTE;
							}

						} else {
							packet_length = MAX_TM_6_6_DATA_LENGTH_BYTE;
						}

						for (j = 0; j < packet_length; j++) {
							packet_data[j] =
									dump_data[(MAX_TM_6_6_DATA_LENGTH_BYTE * i)
											+ j];
						}
						/* useful? */
						for (j = packet_length; j < MAX_TM_6_6_DATA_LENGTH_BYTE;
								j++) {
							packet_data[j] = 0;
						}
						/*
						 * if ret is different from 0, the while exits
						 */
						ret = asw_PUS_6_6_TM_mem_dump_report(
								tc.pkt_id.processId,
								((uint16_t) mem_ID_sys << 8U)
										| (uint16_t) mem_ID_subsys,
								start_address, packet_length, packet_data);
						start_address += MAX_TM_6_6_DATA_LENGTH_BYTE;
						i++;
					}

					if (ret == 0) {
						/*
						 * everything was ok! Generate a TM(1,7)
						 */
						if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
							status = asw_PUS_1_7_TM_TC_exec_ok(
									tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
									tc.data_field.sourceId);
						}
					} else {
						error_param_length = 4;
						memcpy(&error_param[0], &ret, sizeof(uint32_t));
						/* prepare TM(1,8) - TC failure */
						status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId,
								TM_1_8_PUS6_TM_PKT_NOT_CREATED,
								error_param_length, error_param);
						status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
								NULL );
					}
				}
			} /* end else sys = ICU */
		} /* end else check data_length */
	} /* end else check input parameters */

	return (status);
}

rtems_task asw_taskTC_6_5(rtems_task_argument argument) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	icu_status_code status = ICU_SUCCESSFUL;
	uint32_t ret = 0;
	uint8_t DPU_idx = 0;
	uint8_t history_cmd_idx = 0; //, wait_cnt = 0;
	uint8_t error_param_length = 0, error_list[PUS_1_MAX_ERROR_PARAM_LENGTH];
	uint8_t memreg_data[DPU_MEMREG_TAB_MAX_LENGTH_BYTE];
	uint8_t tm_6_6_data[MAX_TM_6_6_DPU_DATA_LENGTH_BYTE]; // 906 Bytes
	uint8_t mem_ID_sys = 0;
	uint8_t mem_ID_subsys = 0;
	TC_pkt_header_id TC_packet_ID;
	TC_pkt_header_seq TC_seq_control;
	TC_pkt_data_field TC_data_field;
	uint16_t dump_cycles = 0, comm_frame = 0, rt_addr = 0, dump_idx = 0;
	uint16_t memreg_tab[6];
	uint32_t start_address = 0;
	uint32_t i = 0, j = 0;
	uint32_t param = 0;
	uint32_t length = 0, dump_length = 0; // length in Bytes
	uint16_t msg_index_in_sl = 0;

	/*
	 * extract parameters from the task notepad
	 * first notepad: tc_pkt_id (2 Bytes)
	 * second notepad: tc_pkt_src (2 B)
	 * third notepad: tc_data_field header - for source and acknowledge (4 B)
	 * fourth notepad: memory_ID (2 Bytes)
	 * fifth notepad: start_address ( 4 B)
	 * sixth notepad: length in Bytes (4 B)
	 */
	rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_0, &param);
	TC_packet_ID.all_bits = (uint16_t) (param & 0x0000FFFFU);
	rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_1, &param);
	TC_seq_control.all_bits = (uint16_t) (param & 0x0000FFFFU);
	rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_2, &param);
	TC_data_field.all_bits = (uint16_t) (param & 0x0000FFFFU);
	rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_3, &param);
	mem_ID_sys = (uint8_t) ((param & 0x0000FF00U) >> 8U);
	mem_ID_subsys = (uint8_t) (param & 0x000000FFU);
	rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_4,
			&start_address);
	rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_5, &length);

	/*
	 * checks on DPU status are done by DPU
	 */

	if (mem_ID_sys == MEM_SYS_DPU1_IDX) {
		rt_addr = dpu1_active_rt;
		DPU_idx = 0;
		msg_index_in_sl = CMD_MSG_INDEX_DPU_1;
	} else {
		rt_addr = dpu2_active_rt;
		DPU_idx = 1;
		msg_index_in_sl = CMD_MSG_INDEX_DPU_2;
	}
	comm_frame = BASE_MEM_DUMP_CF + rt_addr;

	/*
	 * maximum retrievable size with one MDUMP command:
	 * DPU_MEMREG_TAB_MAX_LENGTH_BYTE = (22 + (15 * 26)) * 2 Byte = 824 Bytes
	 * Each TM packet (6.6) is limited to this size  FIXME
	 * MAX_TM_6_6_DPU_DATA_LENGTH_BYTE = DPU_MEMREG_TAB_MAX_LENGTH_BYTE
	 */
	dump_cycles = (uint16_t) (length / MAX_TM_6_6_DPU_DATA_LENGTH_BYTE);
	if ((length % MAX_TM_6_6_DPU_DATA_LENGTH_BYTE)!= 0){
		dump_cycles += 1;
	};

	dump_idx = 0;

	while ((dump_idx < dump_cycles) && (ret == 0)) {
		if (dump_idx == (dump_cycles - 1)) {
			if ((length % MAX_TM_6_6_DPU_DATA_LENGTH_BYTE)!= 0){
				dump_length = length % MAX_TM_6_6_DPU_DATA_LENGTH_BYTE;
			} else {
				dump_length = MAX_TM_6_6_DPU_DATA_LENGTH_BYTE;
			}
		} else {
			dump_length = MAX_TM_6_6_DPU_DATA_LENGTH_BYTE;
		}
		memreg_tab[0] = ((uint16_t) CPU_MDUMP_COMMAND_ID << 8U) | 1U;
		memreg_tab[1] = ++ICU_command_counter;
		memreg_tab[2] = mem_ID_subsys;
		memreg_tab[3] = (uint16_t) ((dump_length + 1) / 2); // in DW - length can be odd
		memreg_tab[4] = (uint16_t) (start_address & 0x0000FFFFU);
		memreg_tab[5] = (uint16_t) ((start_address & 0xFFFF0000U) >> 16U);

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, memreg_tab,
				(uint8_t) comm_frame, rt_addr, RECEIVE_BIT_1553, COMMAND_DPU_SA,
				6, msg_index_in_sl);

		if (status != ICU_SUCCESSFUL) {
			error_param_length = sizeof(icu_status_code);
			/*
			 * status is now containing the error code from
			 * DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
			 */
			memcpy(error_list, &status, sizeof(icu_status_code));
			status = asw_PUS_1_8_TM_TC_exec_fail(TC_packet_ID.all_bits,
					TC_seq_control.all_bits, TC_data_field.sourceId,
					TM_1_8_BC_1553_RW_ERROR, error_param_length, error_list);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
			rtems_status = rtems_task_suspend(RTEMS_SELF );
		}

		/*
		 * save info in the DPU command history and update the number of SA for MDUMP to be read - to be used by CMD_VER_TAB
		 */
		status = asw_push_cmd_tag(TC_data_field.ack, TC_packet_ID.all_bits,
				TC_seq_control.all_bits, TC_data_field.sourceId, DPU_idx,
				memreg_tab[0], memreg_tab[1], 0, 0, &history_cmd_idx);
		/*
		 MDUMP_data_length = memreg_tab[3] + 1;
		 */

		/*
		 * prepare DPU_MEMREG tab in memory
		 */
		DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_cmd_id = (uint8_t) memreg_tab[0];
		DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_ICU_counter = memreg_tab[1];
		DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_mem_ID = mem_ID_subsys;
		DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_requested_size = memreg_tab[3];
		DPU_MEMREG_DUMP_tab[DPU_idx].MDUMP_start_address = start_address;

		/*
		 * wait until the MDUMP command was successfully executed
		 */

		rtems_status = rtems_task_suspend(RTEMS_SELF );

		/*
		 * End of Memory dump process in any case
		 */
		if (DPU_idx == 0) {
			mem_dump_active.dpu1_asw = 0;
			mem_dump_active.dpu1_bsw = 0;
		} else {
			mem_dump_active.dpu2_asw = 0;
			mem_dump_active.dpu2_bsw = 0;
		}

		/*
		 * if the task has been resumed, it means that the command has been
		 * verified and it has been successfully executed
		 */

		/* mark this command_tag as reusable
		 * all other fields are reset by asw_push_cmd_tag function */
		icu_sem_lock(cmd_ver_tab_sem_Id, 5);
		DPU_cmd_history[DPU_idx][history_cmd_idx].useFlag = 0;
		icu_sem_unlock(cmd_ver_tab_sem_Id);
		/*
		 * set the ICU_counter and set length to zero in the MEMREG_TAB
		 */
		icu_sem_lock(mem_reg_sem_Id, 5);
		DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_ICU_counter =
				ICU_command_counter; // not clear if useful fixme
		/*
		 * retrieve the content of MEMREG_DUMP_TAB1-16 on SA 1-16, 32 DW maximum
		 * each. No CRC16 foreseen for MDUMP
		 */
		if (DPU_MEMREG_DUMP_tab[DPU_idx].effective_mem_length
				!= (uint16_t) dump_length) { /* (dump_length + 2) */
			error_param_length = 0;
			/* prepare TM(1,8) - TC failure */
			status = asw_PUS_1_8_TM_TC_exec_fail(TC_packet_ID.all_bits,
					TC_seq_control.all_bits, TC_data_field.sourceId,
					TM_1_8_PUS6_DPU_DUMP_LGHT_ERROR, error_param_length, NULL );
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
			rtems_status = rtems_task_suspend(RTEMS_SELF );
		}

		for (j = 0;
				j < (uint32_t) DPU_MEMREG_DUMP_tab[DPU_idx].effective_mem_length;
				j++) {
			memreg_data[j] = DPU_MEMREG_DUMP_tab[DPU_idx].memory_data[j];
		}
		DPU_MEMREG_DUMP_tab[DPU_idx].effective_mem_length = 0;

		icu_sem_unlock(mem_reg_sem_Id);

		/* check CRC */
		/*
		 crc = ((uint16_t) memreg_data[dump_length] << 8U)
		 | (uint16_t) memreg_data[dump_length + 1];
		 check_res = asw_pus6_mem_compare_crc(memreg_data,
		 (uint16_t) (dump_length + 2), crc);
		 if (check_res != 1) {
		 error_param_length = 0;
		 status = asw_PUS_1_8_TM_TC_exec_fail(TC_packet_ID.all_bits,
		 TC_seq_control.all_bits, TC_data_field.sourceId,
		 TM_1_8_PUS6_DPU_CRC_ERROR, error_param_length, NULL );
		 status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		 rtems_status = rtems_task_suspend(RTEMS_SELF);
		 }
		 */

		/*
		 * write PUS(6,6)
		 */
		for (i = 0; i < dump_length; i++) {
			tm_6_6_data[i] = memreg_data[i];
		}
		ret = asw_PUS_6_6_TM_mem_dump_report(TC_packet_ID.processId,
				((uint16_t) mem_ID_sys << 8U) | (uint16_t) mem_ID_subsys,
				start_address, dump_length, tm_6_6_data);

		if (ret != 0) {
			error_param_length = 4;
			memcpy(&error_list[0], &ret, sizeof(uint32_t));
			/* prepare TM(1,8) - TC failure */
			status = asw_PUS_1_8_TM_TC_exec_fail(TC_packet_ID.all_bits,
					TC_seq_control.all_bits, TC_data_field.sourceId,
					TM_1_8_PUS6_TM_PKT_NOT_CREATED, error_param_length,
					error_list);
			status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {

			/*
			 * for next cycle
			 */
			start_address += dump_length;
			dump_idx++;
		}
	} // end on while (dump_cycle)

	if (status == ICU_SUCCESSFUL) {
		/*
		 * everything was ok! Generate a TM(1,7)
		 */
		if (((TC_data_field.ack) & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(TC_packet_ID.all_bits,
					TC_seq_control.all_bits, TC_data_field.sourceId);
		}
	} else {
		// boh? generate an event? FIXME
	}

	rtems_status = rtems_task_suspend(RTEMS_SELF );

}

uint32_t asw_PUS_6_6_TM_mem_dump_report(uint16_t APID, uint16_t memory_ID,
		uint32_t start_address, uint32_t length, const uint8_t *mem_data) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t ret = 0;
	TM_packet_t TM_packet;
	uint8_t packet_buffer[MIL_1553_MAX_TM_PKT_LENGTH];
	uint16_t packet_length = 0;
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;
	uint16_t data_length = 0, crc = 0, i = 0;

	/* set the first field of packet header  */
	TM_packet.TM_packet_ID = (version_num << TM_VERSION_OFFSET)
			| (tm_type << TM_TYPE_OFFSET)
			| (data_field_header_flag << TM_DF_HEADER_FLAG_OFFSET) | APID;

	/*
	 * Packet data lenght = # octects in data field packet - 1
	 * data_field_header = 10 octects, fixed
	 * data_source = memory_ID (2B) + start_address (4B) + lenght (4B) + lenght
	 * Bytes = 10+lenght Bytes
	 * 	 * if SAU == 1, length could be odd
	 * packet_error_control = 2 octects, fixed
	 */
	TM_packet.TM_packet_length = (12 + 10 + (uint16_t) length) - 1;

	/*
	 * prepare packet data field
	 */
	asw_PUS_TM_set_data_header(&TM_packet, 6, 6);

	/*
	 * prepare data source: memory_ID (2 Bytes) + start_address (4 Bytes) +
	 * 			+ length (4 Bytes) + dump data
	 */

	data_length = (uint16_t) ((length + 1 + 10) / 2); // in DW - (+ 1 has no effect if length
	// is even, but adds a DW if length is
	// odd)
	TM_packet.TM_data_source[0] = memory_ID;
	TM_packet.TM_data_source[1] = (uint16_t) ((start_address & 0xFFFF0000U)
			>> 16U);
	TM_packet.TM_data_source[2] = (uint16_t) (start_address & 0x0000FFFFU);
	TM_packet.TM_data_source[3] = (uint16_t) ((length & 0xFFFF0000U) >> 16U);
	TM_packet.TM_data_source[4] = (uint16_t) (length & 0x0000FFFFU);

	for (i = 5; i < data_length; i++) {
		if (i == (uint16_t) (length - 1)) {
			if ((i % 2) == 0) {
				TM_packet.TM_data_source[i] = ((uint16_t) mem_data[2 * (i - 5)]
						<< 8U) | (uint16_t) mem_data[(2 * (i - 5)) + 1];
			} else {
				TM_packet.TM_data_source[i] = (uint16_t) (((uint16_t) mem_data[2
						* (i - 5)] << 8U) & 0xFF00U);
			}
		} else {
			TM_packet.TM_data_source[i] = ((uint16_t) mem_data[2 * (i - 5)]
					<< 8U) | (uint16_t) mem_data[(2 * (i - 5)) + 1];
		}
	}

	for (i = data_length + 5; i < TM_PKT_MAX_DATA_LENGTH_DW; i++) {
		TM_packet.TM_data_source[i] = 0;
	}

	/*
	 * spare to fill all packet to an integer number of DW
	 */
	TM_packet.spare = NULL;

	/*
	 * serialize packet for checksum
	 * define total size for buffer =
	 *    = packet header length (6 Bytes) + TM_pack->TM_packet_length + 1
	 */
	packet_length = (TM_packet.TM_packet_length) + 1 + 6; // in Byte
	/* at this level, still missing the packet_error_control dataword */
	asw_serialize_TM(&TM_packet, packet_buffer, data_length);
	crc = asw_create_checksum(packet_buffer, packet_length - 2);
	TM_packet.packet_error_control = crc;

	/* Insert message in the queue */
	icu_sem_lock(queue_sem_id, 5);
	rtems_status = rtems_message_queue_send(mem_dump_queue_id,
			(void *) packet_buffer, packet_length);
	queues_with_data |= QUEUE_M_DUMP_DATA_IN;
	icu_sem_unlock(queue_sem_id);

	ret = (uint32_t) rtems_status;
	return (ret);
}

icu_status_code asw_PUS_6_9_check_memory(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t error_param[PUS_1_MAX_ERROR_PARAM_LENGTH], error_param_length = 0;
	uint8_t mem_ID_sys = 0;
	uint8_t mem_ID_subsys = 0;
	uint16_t size_in_bytes = 0;
	uint32_t start_address = 0, SAU_length = 0, length = 0; // SAU_length in SAU, length in Bytes
	uint32_t error_code = SUCCESS, ret = SUCCESS;
	uint16_t checksum = 0;

	/*
	 * extract parameters from the input array
	 */
	mem_ID_sys = tc.TC_pkt_data[0];
	mem_ID_subsys = tc.TC_pkt_data[1];
	memcpy(&start_address, &(tc.TC_pkt_data[2]), sizeof(uint32_t));
	memcpy(&SAU_length, &(tc.TC_pkt_data[6]), sizeof(uint32_t)); // number of SAUs

	/*
	 * check on size of the received parameters
	 */
	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	if (size_in_bytes != 10) {
		error_param_length = 0;
		/* prepare TM(1,8) - TC failure */
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, error_param_length, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if ((mem_ID_sys > MEM_SYS_DPU2_IDX)
			|| ((mem_ID_sys == MEM_SYS_ICU_IDX)
					&& ((mem_ID_subsys != MEM_SUBSYS_ICU_RAM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_ICU_PROM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_ICU_EEPROM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_ICU_REGISTER_IDX)))
			|| (((mem_ID_sys == MEM_SYS_DPU1_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_IDX))
					&& ((mem_ID_subsys != MEM_SUBSYS_DPU_CPU_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_DBB_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_DRB_IDX)
							&& (!((mem_ID_subsys >= MEM_SUBSYS_DPU_DCU1_IDX)
									&& (mem_ID_subsys <= MEM_SUBSYS_DPU_DCU8_IDX)))
							&& (!((mem_ID_subsys >= MEM_SUBSYS_DPU_SCE1_IDX)
									&& (mem_ID_subsys <= MEM_SUBSYS_DPU_SCE8_IDX)))))
			|| (((mem_ID_sys == MEM_SYS_DPU1_BSW_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_BSW_IDX))
					&& ((mem_ID_subsys != MEM_SUBSYS_DPU_BSW_EEPROM_IDX)
							&& (mem_ID_subsys != MEM_SUBSYS_DPU_BSW_RAM_IDX)))) {
		error_param_length = 1;
		error_param[0] = 1; // this means: parameters number 1 is wrong
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length, error_param);
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		if (mem_ID_sys == MEM_SYS_ICU_IDX) {

			/*
			 * for ICU RAM and EEPROM, length can be greater than the maximum dump size!
			 */
			if (mem_ID_subsys == MEM_SUBSYS_ICU_RAM_IDX) {
				length = (uint16_t) SAU_length * PUS6_ICU_RAM_EEPROM_PROM_SAU; // in Byte
				error_code = asw_pus6_mem_calculate_ISO_checksum(mem_ID_subsys,
						start_address, length, &checksum);
				if (error_code != 0) {
					error_param_length = 1;
					error_param[0] = 2; // error on parameter 2: start address - or length
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				}
			} // end  if(memory_ID == ICU_RAM_INDEX)

			if (mem_ID_subsys == MEM_SUBSYS_ICU_EEPROM_IDX) {
				length = (uint16_t) SAU_length * PUS6_ICU_RAM_EEPROM_PROM_SAU; // in Byte
				error_code = asw_pus6_mem_calculate_ISO_checksum(mem_ID_subsys,
						start_address, length, &checksum);

				if (error_code == ICU_EEPROM_MNG_INVALID_INPUT_PARAM) {
					error_param_length = 1;
					error_param[0] = 3; // error on parameter 3: length
					/* prepare TM(1,8) - TC failure */
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_TC_PARAM_OUT_OF_BOUNDS, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				}
			} // if (memory_ID == ICU_EEPROM_INDEX)

			if (error_code == 0) {
				ret = asw_PUS_6_10_TM_mem_check_rep(tc.pkt_id.processId,
						((uint16_t) mem_ID_sys << 8U)
								| (uint16_t) mem_ID_subsys, start_address,
						length, checksum);
				/* if everything was ok, generate a TM(1,7)  */
				if (ret == SUCCESS) {
					if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
						status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
								tc.pkt_seq.all_bits, tc.data_field.sourceId);
					}
				} else {
					memcpy(&error_param[0], &ret, sizeof(uint32_t));
					error_param_length = 4;
					status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
							tc.pkt_seq.all_bits, tc.data_field.sourceId,
							TM_1_8_PUS6_TM_PKT_NOT_CREATED, error_param_length,
							error_param);
					status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
							NULL );
				}
			}
		} /* end if (mem_ID_sys == MEM_SYS_ICU_IDX) */
		else if ((mem_ID_sys == MEM_SYS_DPU1_IDX)
				|| (mem_ID_sys == MEM_SYS_DPU2_IDX)) {
			// TODO suspended
		} else {
			if ((mem_ID_sys == MEM_SYS_DPU1_BSW_IDX)
					|| (mem_ID_sys == MEM_SYS_DPU2_BSW_IDX)) {
				// TODO
			}
		}
	}
	return (status);
}

uint32_t asw_PUS_6_10_TM_mem_check_rep(uint16_t APID, uint16_t memory_ID,
		uint32_t start_address, uint32_t length, uint16_t checksum) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t ret = 0;
	TM_packet_t TM_packet;
	uint8_t packet_buffer[MIL_1553_MAX_TM_PKT_LENGTH];
	uint16_t packet_length = 0;
	uint16_t version_num = 0, tm_type = 0, data_field_header_flag = 1;
	uint16_t data_length = 0, crc = 0, i = 0;

	/*
	 * Check on input parameters: should have been done by calling function!
	 * TBConfirmed...
	 */

	/* set the first field of packet header  */
	TM_packet.TM_packet_ID = (version_num << TM_VERSION_OFFSET)
			| (tm_type << TM_TYPE_OFFSET)
			| (data_field_header_flag << TM_DF_HEADER_FLAG_OFFSET) | APID;

	/*
	 * Packet data lenght = # octects in data field packet - 1
	 * data_field_header = 10 octects, fixed
	 * data_source = memory_ID (2B) + start_address (4B) + lenght (4B) + checksum
	 * (2B) = 12 B
	 * packet_error_control = 2 octects, fixed
	 */
	TM_packet.TM_packet_length = 24 - 1;

	/*
	 * prepare packet data field
	 */
	asw_PUS_TM_set_data_header(&TM_packet, 6, 10);

	/*
	 * prepare data source: memory_ID (2 Bytes) + start_address (4 Bytes) +
	 * 			+ length (4 Bytes) + dump data
	 */

	data_length = 6; // in DW - (+ 1 has no effect if length is even, but adds a
	// DW if length is odd)
	TM_packet.TM_data_source[0] = memory_ID;
	TM_packet.TM_data_source[1] = (uint16_t) ((start_address & 0xFFFF0000U)
			>> 16);
	TM_packet.TM_data_source[2] = (uint16_t) (start_address & 0x0000FFFFU);
	TM_packet.TM_data_source[3] = (uint16_t) ((length & 0xFFFF0000U) >> 16U);
	TM_packet.TM_data_source[4] = (uint16_t) (length & 0x0000FFFFU);
	TM_packet.TM_data_source[5] = checksum;

	for (i = 6; i < TM_PKT_MAX_DATA_LENGTH_DW; i++) {
		TM_packet.TM_data_source[i] = 0;
	}

	/*
	 * spare to fill all packet to an integer number of DW
	 */
	TM_packet.spare = NULL;

	/*
	 * serialize packet for checksum
	 * define total size for buffer =
	 *    = packet header length (6 Bytes) + TM_pack->TM_packet_length + 1
	 */
	packet_length = (TM_packet.TM_packet_length) + 1 + 6; // in Byte
	/* at this level, still missing the packet_error_control dataword */
	asw_serialize_TM(&TM_packet, packet_buffer, data_length);
	crc = asw_create_checksum(packet_buffer, packet_length - 2);
	TM_packet.packet_error_control = crc;

	/* Insert message in the queue */
	icu_sem_lock(queue_sem_id, 5);
	rtems_status = rtems_message_queue_send(mem_dump_queue_id,
			(void *) packet_buffer, packet_length);
	queues_with_data |= QUEUE_M_DUMP_DATA_IN;
	icu_sem_unlock(queue_sem_id);

	ret = (uint32_t) rtems_status;
	return (ret);
}

/****************************************************************************
 * private functions
 ****************************************************************************/
uint8_t asw_pus6_mem_compare_crc(uint8_t *array, uint16_t size, uint16_t chksum) {
	uint8_t res = 0;
	uint16_t data_checksum = 0;

	data_checksum = asw_create_checksum(array, size - 2);
	if (data_checksum == chksum) {
		res = 1;
	}

	return (res);
}

void asw_pus6_mem_read(const uint8_t *address, uint32_t num_of_bytes,
		uint8_t *data) {
	uint32_t i = 0;
	/*	Read the sub-block starting from address and store it into the user
	 * buffer */
	for (i = 0; i < num_of_bytes; i++) {
		data[i] = address[i];
	}
	return;
}

uint32_t asw_pus6_mem_calculate_ISO_checksum(uint8_t mem_id, uint32_t address,
		uint32_t size, uint16_t *checksum) {
	uint32_t error_code = SUCCESS;
	uint32_t num_bytes = 0, i = 0;
	uint8_t C0 = 0, C1 = 0;
	uint32_t offset = 0;
	uint32_t fail_addr = 0;
	uint8_t data_buffer[MAX_MEM_RW_DATA_LENGTH];

	/* check memory ID */
	if (mem_id == MEM_SUBSYS_ICU_EEPROM_IDX) {
		while (size > 0) {
			if (size < MAX_MEM_RW_DATA_LENGTH) {
				num_bytes = size;
			} else {
				num_bytes = MAX_MEM_RW_DATA_LENGTH;
			}

			/* read block data from EEPROM - this function checks size and memory limits! */
			error_code = icu_eeprom_read_bytes((address + offset), num_bytes,
					data_buffer, &fail_addr);
			if (error_code != SUCCESS) {
				return (error_code);
			}

			/* calculate ISO checksum */
			for (i = 0; i < num_bytes; i++) {
				C0 = (C0 + data_buffer[i]) % 255;
				C1 = (C1 + C0) % 255;
			}

			/* update size to be read */
			if (size < MAX_MEM_RW_DATA_LENGTH) {
				size = 0;
			} else {
				size -= MAX_MEM_RW_DATA_LENGTH;
				/* update memory offset */
				offset += num_bytes;
			}
		}
	} else {
		while (size > 0) {
			if (size < MAX_MEM_RW_DATA_LENGTH) {
				num_bytes = size;
			} else {
				num_bytes = MAX_MEM_RW_DATA_LENGTH;
			}

			if (!(icu_mem_utils_ram_check_limits(address + offset, num_bytes))) {
				error_code = 1;
			}
			/* read block data from RAM */
			asw_pus6_mem_read((uint8_t *) (address + offset), num_bytes,
					data_buffer);

			/* calculate ISO checksum */
			for (i = 0; i < num_bytes; i++) {
				C0 = (C0 + data_buffer[i]) % 255;
				C1 = (C1 + C0) % 255;
			}

			/* update size to be read */
			if (size < MAX_MEM_RW_DATA_LENGTH) {
				size = 0;
			} else {
				size -= MAX_MEM_RW_DATA_LENGTH;
				/* update memory offset */
				offset += num_bytes;
			}
		}
	}
	/*
	 * now calculate ISO checksum
	 * Should be the same as % 254 using unsigned char variable
	 * C0 = ((-(C0 + C1) % 255) + 255) % 255;
	 */
	C0 = -((C0 + C1) % 254);

	if (C0 == 0) {
		C0 = 255;
	}
	if (C1 == 0) {
		C1 = 255;
	}

	*checksum = ((uint16_t) C0 << 8U) | (uint16_t) C1;

	return (error_code);
}
