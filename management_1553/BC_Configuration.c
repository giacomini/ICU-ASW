/**
 * \file	BC_Configuration.c
 *
 * \brief	This file manages the configuration of the BC manager
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: 01/feb/2016
 */

#include "../rtems_configuration.h"
#include <public/hw_map.h>

ExchangeMsg_t dpu_1553_msg_chA[MAX_BC_MESSAGES] = { { { 0 } } };
ExchangeMsg_t dpu_1553_msg_chB[MAX_BC_MESSAGES] = { { { 0 } } };
ExchangeMsg_t *dpu_1553_msg = dpu_1553_msg_chA;

uint8_t bus_profile_msg_counter_0[COMMUNICATION_FRAME_NUMBER] = { 0 };
uint8_t bus_profile_msg_counter_1[COMMUNICATION_FRAME_NUMBER] = { 0 };
uint8_t *bus_profile_msg_counter = bus_profile_msg_counter_0;
uint8_t init_bus_profile_msg_counter_0[COMMUNICATION_FRAME_NUMBER] = { 0 };
uint8_t init_bus_profile_msg_counter_1[COMMUNICATION_FRAME_NUMBER] = { 0 };
uint8_t *init_bus_profile_msg_counter = init_bus_profile_msg_counter_0;

uint8_t list_handler[MAX_NUMBER_OF_SENDLIST] = { 0 };
SendListData ptr_sendlist[MAX_NUMBER_OF_SENDLIST] = { { { }, 0 } };
BC_Profile_1553 bus_profile = { { 0 } };

uint8_t dpu1_next_gtab = 0;
uint8_t dpu2_next_gtab = 0;
uint8_t dpu1_tc_running = 0;
uint8_t dpu2_tc_running = 0;
uint16_t dpu1_active_rt = 0;
uint16_t dpu2_active_rt = 0;
uint8_t flag_bc_msgs = 0;

uint8_t dpu1_status = CPU_OFF;
uint8_t dpu2_status = CPU_OFF;
uint8_t dpu1_sce_status = SCE_IDLE;
uint8_t dpu2_sce_status = SCE_IDLE;
uint8_t dpu1_mmu_transm_status = 0;
uint8_t dpu2_mmu_transm_status = 0;

uint8_t asw_configure_bc(void) {
	uint32_t ret = 0;
	uint8_t ret_msg = 0, errors = 0, i = 0, j = 0;

#if 0
	volatile uint32_t *p_pending_mask = (uint32_t *) 0x80000100;
	volatile uint32_t *p_irq_mask = (uint32_t *) 0x80000090;

	(*p_pending_mask) = 0xFC0;
	(*p_irq_mask) |= 0x1000;
#endif
	/*Initialize the Driver*/
	ret = icu_mil_std_1553_bc_drv_init(RAM_BASE_ADDRESS + 0x00400000);
	if (ret != 0) {
		errors = 1;
	}
	/*Configure the Core as Bus Controller*/
	ret = icu_mil_std_1553_bc_drv_configure(TRUE, BC_TIME_OUT_14_MICROSECONDS);
	if (ret != 0) {
		errors = errors | 2U;
	}

	/* Clear bus profile matrix */
	for (i = 0; i < COMMUNICATION_FRAME_NUMBER; i++) {
		for (j = 0; j < MAX_NUMBER_OF_SENDLIST_MSGS; j++) {
			bus_profile[i][j] = CLEAR_BUS_PROFILE;
		}
	}

	/* Create all messages for channel A */
	ret_msg = asw_create_fixed_dpu_msgs();
	if (ret_msg != 0) {
		errors = errors | 4U;
	}

	/* Create all messages for channel B */
	/* Change global variables */
	dpu_1553_msg = dpu_1553_msg_chB;
	flag_bc_msgs = 0 | BC_FLAG_BUS_ID_B;

	/* Clear message counters structure to not repeat Synch messages */
	for (i = 0; i < COMMUNICATION_FRAME_NUMBER; i++) {
		bus_profile_msg_counter_0[i] = 0;
		bus_profile_msg_counter_1[i] = 0;
		init_bus_profile_msg_counter_0[i] = 0;
		init_bus_profile_msg_counter_1[i] = 0;
	}

	ret_msg = asw_create_fixed_dpu_msgs();
	if (ret_msg != 0) {
		errors = errors | 8U;
	}

	/* Clear message counters structure -- only Synch messages are enabled */
	for (i = 0; i < COMMUNICATION_FRAME_NUMBER; i++) {
		bus_profile_msg_counter_0[i] = 1;
		bus_profile_msg_counter_1[i] = 1;
		init_bus_profile_msg_counter_0[i] = 1;
		init_bus_profile_msg_counter_1[i] = 1;
	}

	/* Restore global variable nominal values */
	flag_bc_msgs = 0;
	dpu_1553_msg = dpu_1553_msg_chA;

	/*Initialize DPUs Nominal RT address*/
	dpu1_active_rt = 0;
	dpu2_active_rt = 0;

	return (errors);
}

icu_status_code asw_read_sendlist(uint8_t sl_index) {
	icu_status_code status = 0;
	uint32_t sl_status = 0;
	uint16_t ret = 0;

	ret = icu_mil_std_1553_bc_get_send_list_state((uint32_t) sl_index);

	/*Check if parameter sl_index is correct*/
	if (sl_index >= MAX_NUMBER_OF_SENDLIST) {
		status = ICU_WRONG_PARAMETER;
	} else if (ret != (uint16_t) SEND_LIST_EXCHANGED) {
		status = ICU_BC_1553_ERROR;
	} else {
		sl_status = icu_mil_std_1553_bc_drv_get_send_list(sl_index,
				&ptr_sendlist[sl_index].sendlist,
				&ptr_sendlist[sl_index].n_msgs);
		if (sl_status != 0) {
			status = ICU_BC_1553_ERROR;
		} else {
			/* Update index of the last read sendlist inside 1553 handler structure */
			handler_1553.last_sendlist_read = sl_index;

			/* Send sendlist in a free state */
			if (sl_status != 0) {
				status = ICU_BC_1553_ERROR;
			}
		}
	}

	/* Free sendlist */
	sl_status = icu_mil_std_1553_bc_drv_free_send_list(sl_index);

	return (status);
}

icu_status_code asw_create_msg_to_DPU(uint8_t delay, uint16_t msg_data[],
		const uint8_t cf, uint16_t rt_addr, const uint16_t tr_bit,
		const uint16_t sub_addr, uint16_t word_count,
		const uint16_t bc_msg_index) {

	icu_status_code status = ICU_SUCCESSFUL;
	CommandWord_t cmd_word = { 0 };
	uint32_t ret = 0;

	/* Create command word */
	cmd_word = asw_set_cmd_word(rt_addr, tr_bit, sub_addr, word_count);

	/*Create message*/
	ret = icu_mil_std_1553_bc_drv_create_msg(flag_bc_msgs, delay, cmd_word,
			msg_data, &dpu_1553_msg[bc_msg_index]);

	if (ret == 0) {
		/*Write index in communication frame matrix and increment related counter*/
		bus_profile[cf][bus_profile_msg_counter[cf]] = (uint8_t) bc_msg_index;
		bus_profile_msg_counter[cf]++;
	} else {
		/*status = ICU_BC_1553_ERROR; */
		status = ret;
	}

	return (status);
}

icu_status_code asw_prepare_sendlist(uint8_t cf) {
	uint8_t msg_counter = 0;
	uint8_t sl_index = 0;
	uint8_t i = 0;
	uint32_t ret = 0;
	icu_status_code status = ICU_SUCCESSFUL;

	/*Check if parameter cf is correct*/
	if (cf >= COMMUNICATION_FRAME_NUMBER) {
		status = ICU_WRONG_PARAMETER;
	} else {
		/* Calculate next sendlist index */
		sl_index = cf % MAX_NUMBER_OF_SENDLIST;

		/* Get the value of the message counter */
		msg_counter = bus_profile_msg_counter[cf];

		/* Create sendlist */
		while (i < msg_counter) {
			ret = icu_mil_std_1553_bc_drv_add_msg_to_send_list(sl_index, i,
					&dpu_1553_msg[bus_profile[cf][i]]);
			if (ret != 0) {
				status = ICU_BC_1553_ERROR;
				break;
			}

			i++;
		}

		if (status == ICU_SUCCESSFUL) {

			/* reset message counter of the bus profile to the standard profile */
			bus_profile_msg_counter[cf] = init_bus_profile_msg_counter[cf];

			/* Set sendlist in ready state */
			ret = icu_mil_std_1553_bc_drv_set_ready_send_list(sl_index, FALSE,
					-1);
			if (ret != 0) {
				status = ICU_BC_1553_ERROR;
			}
		}
	}

	return (status);
}

icu_status_code asw_transmit_sendlist(uint8_t sl_index) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint32_t ret = 0;

	/* Check of the sendlist state is already done */
	ret = icu_mil_std_1553_bc_drv_execute_send_list(sl_index);
	if (ret != 0) {
		status = ICU_BC_1553_ERROR;
	}

	/* Update index of the last executed sendlist inside 1553 handler structure */
	handler_1553.last_sendlist_executed = sl_index;

	return (status);
}

icu_status_code asw_clear_bus_profile(uint8_t cf) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t cf_index = 0;
	uint8_t *cf_counters = bus_profile_msg_counter_0;
	uint8_t *init_cf_counters = init_bus_profile_msg_counter_0;

	/* Select structure msg_counter to be updated */
	if (bus_profile_msg_counter == bus_profile_msg_counter_0) {
		cf_counters = bus_profile_msg_counter_1;
		init_cf_counters = init_bus_profile_msg_counter_1;
	} else {
		cf_counters = bus_profile_msg_counter_0;
		init_cf_counters = init_bus_profile_msg_counter_0;
	}

	/*Check if parameter cf is correct*/
	if ((cf < COMMUNICATION_FRAME_NUMBER) || (cf == CLEAR_BUS_PROFILE)) {

		/*Check if parameter cf is correct*/
		if (cf == CLEAR_BUS_PROFILE) {

			/* Enable only Synch messages for all CFs*/
			for (cf_index = 0; cf_index < COMMUNICATION_FRAME_NUMBER;
					cf_index++) {
				cf_counters[cf_index] = 1;
				init_cf_counters[cf_index] = 1;
			}

		} else {
			/* Enable only Synch messages for selected CFs*/
			cf_counters[cf] = 1;
			init_cf_counters[cf] = 1;
		}
	} else {
		status = ICU_WRONG_PARAMETER;
	}

	return (status);
}

void asw_prepare_bus_profile(void) {
	icu_status_code status = ICU_SUCCESSFUL;

	/* Clear the 1553 BC Bus profile for DPU1 Nominal and Redundant*/
	status = asw_clear_bus_profile(CLEAR_BUS_PROFILE);

	//Check which is the software running on DPU...Boot or ASW or undefined
	if (dpu1_status != CPU_OFF) {
		if (dpu1_status == CPU_UNDEFINED) {
			asw_dpu_undef_bc_profile(dpu1_active_rt);
		} else if (dpu1_status == CPU_BOOT) {
			asw_dpu_bsw_bc_profile(dpu1_active_rt);
		} else {
			asw_dpu_asw_bc_profile(dpu1_active_rt);
		}
	}
	/* DPU 2 unit */
	if (dpu2_status != CPU_OFF) {
		if (dpu2_status == CPU_UNDEFINED) {
			asw_dpu_undef_bc_profile(dpu2_active_rt);
		} else if (dpu2_status == CPU_BOOT) {
			asw_dpu_bsw_bc_profile(dpu2_active_rt);
		} else {
			asw_dpu_asw_bc_profile(dpu2_active_rt);
		}
	}

	return;
}

void asw_sendlist_interpreter(uint8_t sl_index) {
	uint8_t i = 0, errors = 0;
	icu_status_code status = ICU_SUCCESSFUL;
	uint8_t length = 0;
	uint16_t tab_id = 0;
	uint8_t ev_errors[3] = { 0, 0, 0 };
	uint32_t n_msg = 0;
	ExchangeMsg_t msg = { { 0 } };
	uint8_t mem_dump1 = 0, mem_dump2 = 0;

	/* Copy Sendlist data in a local structure */
	icu_sem_lock(send_list_sem_id, 5);
	n_msg = ptr_sendlist[sl_index].n_msgs;
	icu_sem_unlock(send_list_sem_id);

	/* Check if there is a mem dump on DPUs */
	if (mem_dump_active.dpu1_bsw == 1) {
		mem_dump1 = 1;
	} else {
		if (mem_dump_active.dpu1_asw == 1) {
			mem_dump1 = 2;
		}
	}

	if (mem_dump_active.dpu2_bsw == 1) {
		mem_dump2 = 1;
	} else {
		if (mem_dump_active.dpu2_asw == 1) {
			mem_dump2 = 2;
		}
	}

	/* Process messages */
	for (i = 0; i < (uint8_t) n_msg; i++) {
		/* Copy message */
		icu_sem_lock(send_list_sem_id, 5);
		msg = ptr_sendlist[sl_index].sendlist.message[i];
		icu_sem_unlock(send_list_sem_id);

		/* Check communication error */
		if (((msg.status_word.message_error != 0) || (msg.status_word.busy != 0))
				== TRUE) {
			errors++;
		} else if (((msg.command_word.rt_address != 31)
				&& (msg.status_word.all_bits == 0)) == TRUE) {
			errors++;
		} else {
			/* If is a Tx message takes data and calls related decoding function */
			if (msg.command_word.tx_rx == 1) {
				/* Check number of DW in case of zero set length to 32 */
				if (msg.command_word.data_word_count == 0) {
					length = 32;
				} else {
					length = msg.command_word.data_word_count;
				}

				/* Read table id */
				tab_id = msg.data[0];

				/* Here there are all interpreters */
				switch (msg.command_word.subaddress) {
				case 1:
				case 2:
				case 3:
				case 4:
				case 5:
				case 6:
				case 7:
				case 8:
					/* DCU HKs or Mem Dump */
					if (((tab_id >= DCU1_HSK_TAB_ID)
							&& (tab_id <= DCU8_HSK_TAB_ID)) == TRUE) {
						asw_DCU_HSK_table_interpreter(msg.data,
								msg.command_word.rt_address);
					}
					/* DPU 1 mem dump */
					else if (((msg.command_word.rt_address < RT_DPU_2_NOM)
							&& (mem_dump1 > 0)) == TRUE) {
						/* DPU ASW mem dump */
						if (mem_dump1 == 2) {
							if (((tab_id >= MEMREG_DUMP_TAB1_ID)
									&& (tab_id <= MEMREG_DUMP_TAB8_ID)) == TRUE) {
								asw_DPU_MEMREG_tab_interpreter(
										msg.command_word.rt_address,
										msg.command_word.subaddress, length,
										msg.data);
							} else {
								ev_errors[0] = msg.command_word.subaddress;
								memcpy(&ev_errors[1], &tab_id,
										sizeof(uint16_t));
								status = asw_PUS_5_TM_report(
										EVENT_DPU_TAB_ID_NOT_CORRECT, 3,
										ev_errors);
							}
						}
						/* MEM DUMP DPU BSW */
						else {
							/* Interpreter */
						}
					}
					/* DPU 2 mem dump */
					else if (((msg.command_word.rt_address > RT_DPU_1_RED)
							&& (mem_dump2 > 0)) == TRUE) {
						/* DPU ASW mem dump */
						if (mem_dump2 == 2) {
							if (((tab_id >= MEMREG_DUMP_TAB1_ID)
									&& (tab_id <= MEMREG_DUMP_TAB8_ID)) == TRUE) {
								asw_DPU_MEMREG_tab_interpreter(
										msg.command_word.rt_address,
										msg.command_word.subaddress, length,
										msg.data);
							} else {
								ev_errors[0] = msg.command_word.subaddress;
								memcpy(&ev_errors[1], &tab_id,
										sizeof(uint16_t));
								status = asw_PUS_5_TM_report(
										EVENT_DPU_TAB_ID_NOT_CORRECT, 3,
										ev_errors);
							}
						}
						/* MEM DUMP DPU BSW */
						else {
							/* Interpreter */
						}
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 9:
				case 10:
				case 11:
				case 12:
				case 13:
				case 14:
				case 15:
				case 16:
					/* SCE tabs */
					if (((tab_id >= SCE1_HSK_TAB1_ID)
							&& (tab_id <= SCE8_HSK_TAB3_ID)) == TRUE) {
						if ((tab_id % 3) == 2) {
							asw_DPU_SCE_HSK_t1_interpreter(msg.data,
									msg.command_word.rt_address);
						} else if ((tab_id % 3) == 0) {
							asw_DPU_SCE_HSK_t2_interpreter(msg.data,
									msg.command_word.rt_address);
						} else {
							asw_DPU_SCE_HSK_t3_interpreter(msg.data,
									msg.command_word.rt_address);
						}
					}
					/* DPU 1 mem dump */
					else if (((msg.command_word.rt_address < RT_DPU_2_NOM)
							&& (mem_dump1 > 0)) == TRUE) {
						/* DPU ASW mem dump */
						if (mem_dump1 == 2) {
							if (((tab_id >= MEMREG_DUMP_TAB9_ID)
									&& (tab_id <= MEMREG_DUMP_TAB16_ID))
									== TRUE) {
								asw_DPU_MEMREG_tab_interpreter(
										msg.command_word.rt_address,
										msg.command_word.subaddress, length,
										msg.data);
							} else {
								ev_errors[0] = msg.command_word.subaddress;
								memcpy(&ev_errors[1], &tab_id,
										sizeof(uint16_t));
								status = asw_PUS_5_TM_report(
										EVENT_DPU_TAB_ID_NOT_CORRECT, 3,
										ev_errors);
							}
						}
						/* MEM DUMP DPU BSW */
						else {
							/* Interpreter */
						}
					}
					/* DPU 2 mem dump */
					else if (((msg.command_word.rt_address > RT_DPU_1_RED)
							&& (mem_dump2 > 0)) == TRUE) {
						/* DPU ASW mem dump */
						if (mem_dump2 == 2) {
							if (((tab_id >= MEMREG_DUMP_TAB9_ID)
									&& (tab_id <= MEMREG_DUMP_TAB16_ID))
									== TRUE) {
								asw_DPU_MEMREG_tab_interpreter(
										msg.command_word.rt_address,
										msg.command_word.subaddress, length,
										msg.data);
							} else {
								ev_errors[0] = msg.command_word.subaddress;
								memcpy(&ev_errors[1], &tab_id,
										sizeof(uint16_t));
								status = asw_PUS_5_TM_report(
										EVENT_DPU_TAB_ID_NOT_CORRECT, 3,
										ev_errors);
							}
						}
						/* MEM DUMP DPU BSW */
						else {
							/* Interpreter */
						}
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 17:
					if (tab_id == CMD_VER_TAB_ID) {
						status = asw_CMD_ver_table_interpreter(
								msg.command_word.rt_address, msg.data);
					} else if (tab_id == BSW_CMD_VER_TAB) {
						/*Call BSW Command Verification Interpreter */
						status = asw_BSW_CMD_ver_tab_interpreter(
								msg.command_word.rt_address, msg.data);
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 18:
					if (tab_id == DPU_STATUS_TAB_ID) {
						asw_DPU_status_tab_interpreter(msg.data,
								msg.command_word.rt_address);
					} else if (tab_id == BSW_DPU_HK_TAB) {
						/* BSW_HK_interpreter */
						asw_DPU_BSW_CPU_DRB_interpreter(msg.data,
								msg.command_word.rt_address);
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 19:
					if (tab_id == BSW_CMD_VER_TAB) {
						/*Call BSW Command Verification Interpreter */
						status = asw_BSW_CMD_ver_tab_interpreter(
								msg.command_word.rt_address, msg.data);
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 20:
					if (tab_id == ASW_ERROR_TAB_ID) {
						/*Call ASW_ERROR_TAB Interpreter -- BSW_EVENT only MDUMP*/
						asw_DPU_error_tab_interpreter(msg.data,
								msg.command_word.rt_address);
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 21:
					if (tab_id == DPU_MONITOR_TAB_ID) {
						asw_DPU_monit_tab_interpreter(msg.data,
								msg.command_word.rt_address);
					} else if (tab_id == BSW_DPU_HK_TAB) {
						/* BSW_HK_interpreter */
						asw_DPU_BSW_DCU_interpreter(msg.data,
								msg.command_word.rt_address);
						if (msg.command_word.rt_address < RT_DPU_2_NOM) {
							dpu1_status = CPU_BOOT;
						} else {
							dpu2_status = CPU_BOOT;
						}
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 22:
					/* Interpreter */
					if (tab_id == DBB_DRB_STATUS_TAB_ID) {
						asw_DBB_DRB_tab_interpreter(msg.data,
								msg.command_word.rt_address);
					} else if (tab_id == ASW_BOOT_REP_TAB_ID) {
						/* Interpreter */
					} else if (tab_id == ASW_DEATH_REP_TAB_ID) {
						/* Interpreter */
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 23:
					if (tab_id == DPU_HSK_TAB_ID) {
						asw_DPU_HSK_table_interpreter(msg.data,
								msg.command_word.rt_address);
					} else {
						ev_errors[0] = msg.command_word.subaddress;
						memcpy(&ev_errors[1], &tab_id, sizeof(uint16_t));
						status = asw_PUS_5_TM_report(
								EVENT_DPU_TAB_ID_NOT_CORRECT, 3, ev_errors);
					}
					break;
				case 29:
					/* Read OBT packet returned by DPUs */
					break;
				case 30:
					/* Wrap Around */
					break;
				default:
					break;
				}

			}

		}

	}

	return;
}

void asw_dpu_undef_bc_profile(uint16_t rt_address) {
	uint8_t *cf_counters = bus_profile_msg_counter_0;
	uint8_t *init_cf_counters = init_bus_profile_msg_counter_0;

	/* Select structure msg_counter to be updated */
	if (bus_profile_msg_counter == bus_profile_msg_counter_0) {
		cf_counters = bus_profile_msg_counter_1;
		init_cf_counters = init_bus_profile_msg_counter_1;
	} else {
		cf_counters = bus_profile_msg_counter_0;
		init_cf_counters = init_bus_profile_msg_counter_0;
	}

	/* Add MONITOR_TAB for ASW / BOOT SW HK 2 (SA 21) to profile */
	cf_counters[BASE_HK_CF + rt_address]++;
	init_cf_counters[BASE_HK_CF + rt_address]++;

	/* Add OBT_RCV (must be 1 only a broadcast) into profile */
	cf_counters[TIME_DISTRIBUTION_CF] = 2;
	init_cf_counters[TIME_DISTRIBUTION_CF] = 2;

	return;
}

void asw_dpu_bsw_bc_profile(uint16_t rt_address) {
	uint8_t *cf_counters = bus_profile_msg_counter_0;
	uint8_t *init_cf_counters = init_bus_profile_msg_counter_0;

	/* Select structure msg_counter to be updated */
	if (bus_profile_msg_counter == bus_profile_msg_counter_0) {
		cf_counters = bus_profile_msg_counter_1;
		init_cf_counters = init_bus_profile_msg_counter_1;
	} else {
		cf_counters = bus_profile_msg_counter_0;
		init_cf_counters = init_bus_profile_msg_counter_0;
	}

	/* Add CMD_VER_1 into profile */
	cf_counters[BASE_CMD_VER_1_CF + rt_address]++;
	init_cf_counters[BASE_CMD_VER_1_CF + rt_address]++;

	/* Add OBT_RCV (Must be 2) and OBT_BACK into profile */
	cf_counters[TIME_DISTRIBUTION_CF] = 2;
	cf_counters[TIME_DISTRIBUTION_CF + rt_address]++;
	init_cf_counters[TIME_DISTRIBUTION_CF] = 2;
	init_cf_counters[TIME_DISTRIBUTION_CF + rt_address]++;

	/* Add HK1 and HK2 into profile */
	cf_counters[BASE_HK_CF + rt_address] += 2;
	init_cf_counters[BASE_HK_CF + rt_address] += 2;

	/* Add CMD_VER_2 into profile */
	cf_counters[CMD_VER_2_CF + rt_address]++;
	init_cf_counters[CMD_VER_2_CF + rt_address]++;

	return;
}

void asw_dpu_asw_bc_profile(uint16_t rt_address) {
	uint8_t *cf_counters = bus_profile_msg_counter_0;
	uint8_t *init_cf_counters = init_bus_profile_msg_counter_0;

	/* Select structure msg_counter to be updated */
	if (bus_profile_msg_counter == bus_profile_msg_counter_0) {
		cf_counters = bus_profile_msg_counter_1;
		init_cf_counters = init_bus_profile_msg_counter_1;
	} else {
		cf_counters = bus_profile_msg_counter_0;
		init_cf_counters = init_bus_profile_msg_counter_0;
	}

	/* Add CMD_VER_1 into profile */
	cf_counters[BASE_CMD_VER_1_CF + rt_address]++;
	init_cf_counters[BASE_CMD_VER_1_CF + rt_address]++;

	/* Add OBT_RCV and OBT_BACK into profile */
	cf_counters[TIME_DISTRIBUTION_CF] = 2;
	cf_counters[TIME_DISTRIBUTION_CF + rt_address]++;
	init_cf_counters[TIME_DISTRIBUTION_CF] = 2;
	init_cf_counters[TIME_DISTRIBUTION_CF + rt_address]++;

	/* Add MONITOR, STATUS, ERROR and DPU_HSK table into profile */
	cf_counters[BASE_HK_CF + rt_address] += 4;
	init_cf_counters[BASE_HK_CF + rt_address] += 4;

	/* Add DCUs HK tables into profile */
	cf_counters[ASW_DCU_SCE_HSK_TAB_CF + rt_address] += DCU_SCE_NUMBER;
	init_cf_counters[ASW_DCU_SCE_HSK_TAB_CF + rt_address] += DCU_SCE_NUMBER;

	return;
}

void asw_SCE_and_DBB_DRB_msgs_in_sl(const uint32_t gtab_hk_table,
		uint16_t rt_addr) {

	/* Update 1553 BC matrix and message_counter variable in case of DBB_DRB table*/
	if (gtab_hk_table == DBB_DRB_TAB) {
		/* Increment number of messages in the related CF*/
		bus_profile_msg_counter[DPU_DBB_DRB_STATUS_TAB_CF + rt_addr]++;
	} else {
		/* SCE HK tables */
		/* Add 8 messages in the related CF */
		bus_profile_msg_counter[ASW_DCU_SCE_HSK_TAB_CF + rt_addr] =
				bus_profile_msg_counter[ASW_DCU_SCE_HSK_TAB_CF + rt_addr]
						+ DCU_SCE_NUMBER;
	}
	return;
}

void asw_command_GTAB_to_DPUs(uint8_t dpu_unit) {
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint16_t rt_addr = 0;
	uint16_t tab = 0;

	if (dpu_unit == 1) {
		rt_addr = dpu1_active_rt;
		tab = dpu1_next_gtab;
		dpu1_next_gtab = (dpu1_next_gtab + 1) % 4;
	} else {
		rt_addr = dpu2_active_rt;
		tab = dpu2_next_gtab;
		dpu2_next_gtab = (dpu2_next_gtab + 1) % 4;
	}

	/*
	 * Commands a GTAB
	 * Check tables to be acquired. If are SCE tables sets the 2nd parameter, the 3rd if is a DBB_DRB table
	 */
	if (tab == DBB_DRB_TAB) {
		/* Acquire DBB */
		icu_status = asw_CPU_GTAB(rt_addr, 1, 1);
	} else {
		/* Acquire SCE */
		icu_status = asw_CPU_GTAB(rt_addr, (tab + 1), 1);
	}

	return;
}

uint8_t asw_create_fixed_dpu_msgs() {
	uint8_t ret = 0, i = 0;
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t dw = 1;

	/* Create synch without data word message
	 * Index of the created message is put at the first position of the related
	 * entry point of the bus profile table
	 */
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			SYNCH_WITHOUT_DW_CF, RT_BROADCAST_1553, TRANSMIT_BIT_1553,
			MODE_CODES_SA, MC_SYNCHRONIZE_WITHOUT_DATA,
			SYNCH_WITHOUT_DW_MSG_INDEX);

	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	bus_profile[SYNCH_WITHOUT_DW_CF][0] = SYNCH_WITHOUT_DW_MSG_INDEX;

	/* Create synch with data word messages
	 * Indexes of the created messages are put at the first position of the
	 * related entry point of the bus profile table
	 */
	for (i = SYNCH_WITH_DW_START_INDEX; i <= SYNCH_WITH_DW_LAST_INDEX; i++) {
		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, &dw, (uint8_t) dw,
				RT_BROADCAST_1553, RECEIVE_BIT_1553, MODE_CODES_SA,
				MC_SYNCHRONIZE_WITH_DATA, (uint16_t) i);
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}
		bus_profile[dw][0] = i;
		dw++;
	}

	/* Assign indexes for memory load */
	for (i = 0; i < MAX_MEM_DUMP_LOAD_MSGS; i++) {
		bus_profile[BASE_MEM_LOAD_CF + RT_DPU_1_NOM][1 + i] =
				MEM_LOAD_FIRST_MSG_INDEX + i;
		bus_profile[BASE_MEM_LOAD_CF + RT_DPU_1_RED][1 + i] =
				MEM_LOAD_FIRST_MSG_INDEX + i;
		bus_profile[BASE_MEM_LOAD_CF + RT_DPU_2_NOM][1 + i] =
				MEM_LOAD_FIRST_MSG_INDEX + i;
		bus_profile[BASE_MEM_LOAD_CF + RT_DPU_2_RED][1 + i] =
				MEM_LOAD_FIRST_MSG_INDEX + i;
	}

	/* Assign indexes for CMDs */
	bus_profile[BASE_COMMAND_CF + RT_DPU_1_NOM][1] = CMD_MSG_INDEX_DPU_1;
	bus_profile[BASE_COMMAND_CF + RT_DPU_1_RED][1] = CMD_MSG_INDEX_DPU_1;
	bus_profile[BASE_COMMAND_CF + RT_DPU_2_NOM][1] = CMD_MSG_INDEX_DPU_2;
	bus_profile[BASE_COMMAND_CF + RT_DPU_2_RED][1] = CMD_MSG_INDEX_DPU_2;

	/*ASW_CMD_VER_TAB or BSW Command Verification#1 Tab SA17Tx*/
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_CMD_VER_1_CF + RT_DPU_1_NOM), RT_DPU_1_NOM, TRANSMIT_BIT_1553,
			BSW_CMDVER1_AND_ASW_CMDVER_SA, CMD_WORD_32_DW,
			(CMD_VER_MSG_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_CMD_VER_1_CF + RT_DPU_1_RED), RT_DPU_1_RED, TRANSMIT_BIT_1553,
			BSW_CMDVER1_AND_ASW_CMDVER_SA, CMD_WORD_32_DW,
			(CMD_VER_MSG_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_CMD_VER_1_CF + RT_DPU_2_NOM), RT_DPU_2_NOM, TRANSMIT_BIT_1553,
			BSW_CMDVER1_AND_ASW_CMDVER_SA, CMD_WORD_32_DW,
			(CMD_VER_MSG_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_CMD_VER_1_CF + RT_DPU_2_RED), RT_DPU_2_RED, TRANSMIT_BIT_1553,
			BSW_CMDVER1_AND_ASW_CMDVER_SA, CMD_WORD_32_DW,
			(CMD_VER_MSG_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	/* OBT messages */
	/* OBT rcv */
	bus_profile[TIME_DISTRIBUTION_CF][1] = TIME_RCV_MSG_INDEX;

	/* OBT BACK */
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(TIME_DISTRIBUTION_CF + RT_DPU_1_NOM), RT_DPU_1_NOM,
			TRANSMIT_BIT_1553, TIME_MSG_SA, DPU_OBT_TM_PKT_DW_SIZE,
			(TIME_RETRIEVE_MSG_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(TIME_DISTRIBUTION_CF + RT_DPU_1_RED), RT_DPU_1_RED,
			TRANSMIT_BIT_1553, TIME_MSG_SA, DPU_OBT_TM_PKT_DW_SIZE,
			(TIME_RETRIEVE_MSG_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(TIME_DISTRIBUTION_CF + RT_DPU_2_NOM), RT_DPU_2_NOM,
			TRANSMIT_BIT_1553, TIME_MSG_SA, DPU_OBT_TM_PKT_DW_SIZE,
			(TIME_RETRIEVE_MSG_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(TIME_DISTRIBUTION_CF + RT_DPU_2_RED), RT_DPU_2_RED,
			TRANSMIT_BIT_1553, TIME_MSG_SA, DPU_OBT_TM_PKT_DW_SIZE,
			(TIME_RETRIEVE_MSG_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	/* ASW_MONITOR_TAB or BSW_HK2 */
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_NOM), RT_DPU_1_NOM, TRANSMIT_BIT_1553,
			BSW_HSK2_AND_ASW_MON_TAB_SA, CMD_WORD_32_DW,
			(MON_TAB_MSG_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_RED), RT_DPU_1_RED, TRANSMIT_BIT_1553,
			BSW_HSK2_AND_ASW_MON_TAB_SA, CMD_WORD_32_DW,
			(MON_TAB_MSG_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_NOM), RT_DPU_2_NOM, TRANSMIT_BIT_1553,
			BSW_HSK2_AND_ASW_MON_TAB_SA, CMD_WORD_32_DW,
			(MON_TAB_MSG_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_RED), RT_DPU_2_RED, TRANSMIT_BIT_1553,
			BSW_HSK2_AND_ASW_MON_TAB_SA, CMD_WORD_32_DW,
			(MON_TAB_MSG_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	/* ASW_STATUS_TAB or BSW_HK1*/
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_NOM), RT_DPU_1_NOM, TRANSMIT_BIT_1553,
			BSW_HSK1_AND_ASW_STATUS_TAB_SA, CMD_WORD_32_DW,
			(STATUS_TAB_MSG_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_RED), RT_DPU_1_RED, TRANSMIT_BIT_1553,
			BSW_HSK1_AND_ASW_STATUS_TAB_SA, CMD_WORD_32_DW,
			(STATUS_TAB_MSG_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_NOM), RT_DPU_2_NOM, TRANSMIT_BIT_1553,
			BSW_HSK1_AND_ASW_STATUS_TAB_SA, CMD_WORD_32_DW,
			(STATUS_TAB_MSG_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_RED), RT_DPU_2_RED, TRANSMIT_BIT_1553,
			BSW_HSK1_AND_ASW_STATUS_TAB_SA, CMD_WORD_32_DW,
			(STATUS_TAB_MSG_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	/* ASW_ERROR_TAB or BSW_ERROR */
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_NOM), RT_DPU_1_NOM, TRANSMIT_BIT_1553,
			BSW_EVENT_REP_AND_ASW_ERROR_SA, CMD_WORD_32_DW,
			(ERROR_MSG_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_RED), RT_DPU_1_RED, TRANSMIT_BIT_1553,
			BSW_EVENT_REP_AND_ASW_ERROR_SA, CMD_WORD_32_DW,
			(ERROR_MSG_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_NOM), RT_DPU_2_NOM, TRANSMIT_BIT_1553,
			BSW_EVENT_REP_AND_ASW_ERROR_SA, CMD_WORD_32_DW,
			(ERROR_MSG_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_RED), RT_DPU_2_RED, TRANSMIT_BIT_1553,
			BSW_EVENT_REP_AND_ASW_ERROR_SA, CMD_WORD_32_DW,
			(ERROR_MSG_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	/* ASW_DPU_HK */
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_NOM), RT_DPU_1_NOM, TRANSMIT_BIT_1553,
			ASW_DPU_HSK_TAB_SA, CMD_WORD_32_DW, (HSK_MSG_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_1_RED), RT_DPU_1_RED, TRANSMIT_BIT_1553,
			ASW_DPU_HSK_TAB_SA, CMD_WORD_32_DW, (HSK_MSG_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_NOM), RT_DPU_2_NOM, TRANSMIT_BIT_1553,
			ASW_DPU_HSK_TAB_SA, CMD_WORD_32_DW, (HSK_MSG_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(BASE_HK_CF + RT_DPU_2_RED), RT_DPU_2_RED, TRANSMIT_BIT_1553,
			ASW_DPU_HSK_TAB_SA, CMD_WORD_32_DW, (HSK_MSG_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	/* DCUs HK messages */
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_1_NOM), RT_DPU_1_NOM,
				TRANSMIT_BIT_1553, (DCU_HSK_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_1_DCU_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_1_RED), RT_DPU_1_RED,
				TRANSMIT_BIT_1553, (DCU_HSK_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_2_DCU_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_2_NOM), RT_DPU_2_NOM,
				TRANSMIT_BIT_1553, (DCU_HSK_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_3_DCU_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_2_RED), RT_DPU_2_RED,
				TRANSMIT_BIT_1553, (DCU_HSK_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_4_DCU_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}
	}

	/* SCEs messages */
	for (i = 0; i < DCU_SCE_NUMBER; i++) {
		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_1_NOM), RT_DPU_1_NOM,
				TRANSMIT_BIT_1553, (SCE_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_1_SCE_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_1_RED), RT_DPU_1_RED,
				TRANSMIT_BIT_1553, (SCE_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_2_SCE_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_2_NOM), RT_DPU_2_NOM,
				TRANSMIT_BIT_1553, (SCE_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_3_SCE_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}

		status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
				(ASW_DCU_SCE_HSK_TAB_CF + RT_DPU_2_RED), RT_DPU_2_RED,
				TRANSMIT_BIT_1553, (SCE_TAB_START_SA + i), CMD_WORD_32_DW,
				(CPU_4_SCE_1_HSK_TAB_INDEX + i));
		if (status != ICU_SUCCESSFUL) {
			ret++;
		}

	}

	/* Memory Dump - copy index for DCU and SCE acquisition*/
	for (i = 0; i < MAX_MEM_DUMP_LOAD_MSGS; i++) {
		bus_profile[BASE_MEM_DUMP_CF + RT_DPU_1_NOM][1 + i] =
				MEM_DUMP_MSG_INDEX_CPU1 + i;
		bus_profile[BASE_MEM_DUMP_CF + RT_DPU_1_RED][1 + i] =
				MEM_DUMP_MSG_INDEX_CPU2 + i;
		bus_profile[BASE_MEM_DUMP_CF + RT_DPU_2_NOM][1 + i] =
				MEM_DUMP_MSG_INDEX_CPU3 + i;
		bus_profile[BASE_MEM_DUMP_CF + RT_DPU_2_RED][1 + i] =
				MEM_DUMP_MSG_INDEX_CPU4 + i;
	}

	/*BSW Command Verification 2 table SA19Tx*/
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(CMD_VER_2_CF + RT_DPU_1_NOM), RT_DPU_1_NOM, TRANSMIT_BIT_1553,
			BSW_CMD_VERIFICATION_2_SA, CMD_WORD_32_DW,
			(CMD_VER_2_MSG_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(CMD_VER_2_CF + RT_DPU_1_RED), RT_DPU_1_RED, TRANSMIT_BIT_1553,
			BSW_CMD_VERIFICATION_2_SA, CMD_WORD_32_DW,
			(CMD_VER_2_MSG_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(CMD_VER_2_CF + RT_DPU_2_NOM), RT_DPU_2_NOM, TRANSMIT_BIT_1553,
			BSW_CMD_VERIFICATION_2_SA, CMD_WORD_32_DW,
			(CMD_VER_2_MSG_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(CMD_VER_2_CF + RT_DPU_2_RED), RT_DPU_2_RED, TRANSMIT_BIT_1553,
			BSW_CMD_VERIFICATION_2_SA, CMD_WORD_32_DW,
			(CMD_VER_2_MSG_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	/* DBB_DRB_STATUS message SA22Tx*/
	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(DPU_DBB_DRB_STATUS_TAB_CF + RT_DPU_1_NOM), RT_DPU_1_NOM,
			TRANSMIT_BIT_1553, ASW_DPU_DBB_DRB_STATUS_TAB_SA, CMD_WORD_32_DW,
			(DBB_DRB_HSK_TAB_INDEX + RT_DPU_1_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(DPU_DBB_DRB_STATUS_TAB_CF + RT_DPU_1_RED), RT_DPU_1_RED,
			TRANSMIT_BIT_1553, ASW_DPU_DBB_DRB_STATUS_TAB_SA, CMD_WORD_32_DW,
			(DBB_DRB_HSK_TAB_INDEX + RT_DPU_1_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(DPU_DBB_DRB_STATUS_TAB_CF + RT_DPU_2_NOM), RT_DPU_2_NOM,
			TRANSMIT_BIT_1553, ASW_DPU_DBB_DRB_STATUS_TAB_SA, CMD_WORD_32_DW,
			(DBB_DRB_HSK_TAB_INDEX + RT_DPU_2_NOM));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	status = asw_create_msg_to_DPU(MSG_INTERGAP_DELAY, NULL,
			(DPU_DBB_DRB_STATUS_TAB_CF + RT_DPU_2_RED), RT_DPU_2_RED,
			TRANSMIT_BIT_1553, ASW_DPU_DBB_DRB_STATUS_TAB_SA, CMD_WORD_32_DW,
			(DBB_DRB_HSK_TAB_INDEX + RT_DPU_2_RED));
	if (status != ICU_SUCCESSFUL) {
		ret++;
	}

	return (ret);
}
