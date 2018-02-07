/**
 * \file	tasks.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	29-01-2016
 *
 */

#include "rtems_configuration.h"

uint8_t tc_data[1016] = { 0 };

rtems_task asw_task_rt(rtems_task_argument arg) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint32_t ret = 0;
	uint8_t sl_index = 0;
	uint8_t errors[3] = { 0, 0, 0 };
	uint8_t cf = 0;

	while (1) {
		cf = (uint8_t) handler_1553.current_comm_frame;

		/* Refresh watchdog */
		icu_watchdog_refresh();

		if (((icu_state_machine > ICU_STARTUP)
				&& (((dpu1_status != CPU_OFF) || (dpu2_status != CPU_OFF))
						== TRUE))== TRUE) {
			sl_index = cf % MAX_NUMBER_OF_SENDLIST;

			/* Transmit sendlist */

			if (icu_mil_std_1553_bc_get_send_list_state(
					sl_index) == SEND_LIST_READY) {
				icu_status = asw_transmit_sendlist(sl_index);

				if (icu_status != ICU_SUCCESSFUL) {
					errors[0] = 1U << 3U;
					errors[1] = 0;
					errors[2] = 0;
					icu_status = asw_PUS_5_TM_report(EVENT_SENDLIST_ERROR, 3,
							errors);
				}
			}

		}

		/*Start TaskManager*/
		switch (handler_1553.current_comm_frame) {
		case 0:
			/* OBT synch */
			if (obt_synch_enable == TRUE) {
				icu_obt_trigger_load_preloaded_obt();
			}

			/* If there is a wheel movement to start */
			if ((wheel_flag & WP_TO_BE_STARTED) > 0) {
				rtems_status = rtems_timer_fire_after(wheel_timer_id, 470,
						&asw_wheels_start_movement, NULL );
			}
			/* Restart WD counter */
			if (wd_activation == TRUE) {
				icu_watchdog_start_count();
				wd_activation = FALSE;
			}

			/* Check if GTAB could be commanded - only when on DPUs run ASW*/
			if (((dpu1_status >= CPU_ASW_INIT) && (dpu1_status <= CPU_MANUAL) && (dpu1_tc_running == 0)) == TRUE) {
				asw_command_GTAB_to_DPUs(1);
			} else {
				dpu1_tc_running = 0;
			}

			if (((dpu2_status >= CPU_ASW_INIT) && (dpu2_status <= CPU_MANUAL) && (dpu2_tc_running == 0)) == TRUE) {
				asw_command_GTAB_to_DPUs(2);
			} else {
				dpu2_tc_running = 0;
			}

			/*
			 * Set ICU CPU usage inside the DB
			 */
			cpu_load = (uint16_t) (100 - (count_idle / 10));
			count_idle = 0;
			break;
		case ATR_1_CF:
			/*
			 * Create a new ATR only if there are messages inside queues or no TM protocol reset operation is scheduled
			 */
			if (((queues_with_data != 0) && (tm_reset_active == FALSE))== TRUE) {
				rtems_status = rtems_task_restart(task_atr_id, 0);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					errors[0] = (((uint16_t) 1U << 3) & 0xFF00U) >> 8U;
					errors[1] = (1U << 3) & 0xFFU;
					icu_status = asw_PUS_5_TM_report(EVENT_TASK_NOT_STARTED, 2,
							errors);
				}
			}
			break;
		case DTD_CF:
			/*Check for new DTD*/
			ret = asw_check_new_DTD(tc_data);
			if (ret == 0) {
				rtems_status = rtems_task_restart(task_tc_id, 0);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					errors[0] = (((uint16_t) 1U << 2) & 0xFF00U) >> 8U;
					errors[1] = (1U << 2) & 0xFFU;
					icu_status = asw_PUS_5_TM_report(EVENT_TASK_NOT_STARTED, 2,
							errors);
				}
			}
			break;
		case ATC_1_CF:
			/* Initialize protocol reset in case of ATC loss*/
			if (handler_1553.ATC.all_bits != handler_1553.ATR.all_bits) {
				asw_telemetry_reset();
			}
			break;
		case ATR_2_CF:
			/*
			 * Create a new ATR only if there are messages inside queues or no TM protocol reset operation is scheduled
			 */
			if (((queues_with_data != 0) && (tm_reset_active == FALSE))== TRUE) {
				rtems_status = rtems_task_restart(task_atr_id, 0);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					errors[0] = (((uint16_t) 1U << 3) & 0xFF00U) >> 8U;
					errors[1] = (1U << 3) & 0xFFU;
					icu_status = asw_PUS_5_TM_report(EVENT_TASK_NOT_STARTED, 2,
							errors);
				}
			}
			break;
		case ATC_2_CF:
			/* Initialize protocol reset in case of ATC loss*/
			if (handler_1553.ATC.all_bits != handler_1553.ATR.all_bits) {
				asw_telemetry_reset();
			}
			break;
		case OBT_CF:
			/*Read OBT message received on SA29R*/
			asw_read_last_OBT_received();
			break;
		case DPU_DUMP_CF:
			/*Restart Task 6,5 for memory dump vs DPUs*/
			if (((mem_dump_active.dpu1_asw == 1)
					|| (mem_dump_active.dpu1_bsw == 1)
					|| (mem_dump_active.dpu2_asw == 1)
					|| (mem_dump_active.dpu2_bsw == 1)) == TRUE) {
				rtems_status = rtems_task_resume(taskTC_6_5_Id);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					errors[0] = (((uint16_t) 1U << 9) & 0xFF00U) >> 8U;
					errors[1] = (1U << 9) & 0xFFU;
					icu_status = asw_PUS_5_TM_report(EVENT_TASK_NOT_STARTED, 2,
							errors);
				}
			}
			break;
		default:
			break;
		}

		/*Read last messages received since last call*/
		ret = asw_check_last_rcv_cmd_word(msg_buffer, number_of_msg);

		/* Restart task BC */
		if (((icu_state_machine > ICU_STARTUP)
				&& (((dpu1_status != CPU_OFF) || (dpu2_status != CPU_OFF))
						== TRUE))== TRUE) {

			/* Prepare next sendlist to be trasmitted */

			if (icu_mil_std_1553_bc_get_send_list_state(
					(cf + 1) % MAX_NUMBER_OF_SENDLIST) == SEND_LIST_FREE) {
				icu_status = asw_prepare_sendlist(
						(cf + 1) % COMMUNICATION_FRAME_NUMBER);

				if (icu_status != ICU_SUCCESSFUL) {
					errors[0] = 1U;
					errors[1] = 0;
					errors[2] = 0;
					icu_status = asw_PUS_5_TM_report(EVENT_SENDLIST_ERROR, 3,
							errors);
				}
			}

			/* Retrieve data from a exchanged sendlist */
			/* In order to avoid values minor than zero to read the previous sendlist trasmitted (current_sl + 10 - 1) operation is used */
			if (icu_mil_std_1553_bc_get_send_list_state(
					(handler_1553.last_sendlist_executed + 9)
							% MAX_NUMBER_OF_SENDLIST) == SEND_LIST_EXCHANGED) {
				icu_status = asw_read_sendlist(
						(handler_1553.last_sendlist_executed + 9)
								% MAX_NUMBER_OF_SENDLIST);
				if (icu_status != ICU_SUCCESSFUL) {
					errors[0] = 1U << 1U;
					errors[1] = 0;
					errors[2] = 0;
					icu_status = asw_PUS_5_TM_report(EVENT_SENDLIST_ERROR, 3,
							errors);
				}
			}

		}

		rtems_status = rtems_task_suspend(RTEMS_SELF );
	}
}

rtems_task asw_task_bc(rtems_task_argument arg) {
	uint8_t errors[3] = { 0, 0, 0 };
	rtems_status_code status = RTEMS_SUCCESSFUL;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint8_t sl_index = 0;
	uint16_t sl_ctr_wrd = 0;
	uint32_t ret = 0;

	/* When starts process the last sendlist read and then enter in the loop */
	handler_1553.last_sendlist_processed = (handler_1553.last_sendlist_read + 9)
			% MAX_NUMBER_OF_SENDLIST;

	/* Process a sendlist already acquired */
	while (1) {

		sl_index = (handler_1553.last_sendlist_processed + 1)
				% MAX_NUMBER_OF_SENDLIST;

		/* Check sendlist index */
		if (sl_index < MAX_NUMBER_OF_SENDLIST) {

			/* Read Sendlist Control Word */
			ret = icu_mil_std_1553_bc_get_sendlist_control_word(
					(uint32_t) sl_index, &sl_ctr_wrd);

			/* Check result and control word */
			if (((ret == 0) && (sl_ctr_wrd == 0)) == TRUE) {
				asw_sendlist_interpreter(sl_index);
			} else {
				/* In case function icu_mil_std_1553_bc_get_sendlist_control_word returns with an error */
				if (ret != 0) {
					errors[0] = 1U << 4U;
					errors[1] = 0;
					errors[2] = 0;
					status = asw_PUS_5_TM_report(EVENT_SENDLIST_ERROR, 3,
							errors);
				} else {
					/* Sendlist control word not zero */
					errors[0] = 1U << 5U;
					memcpy(&errors[1], &sl_ctr_wrd, sizeof(uint16_t));
					status = asw_PUS_5_TM_report(EVENT_SENDLIST_ERROR, 3,
							errors);
				}
			}
		} else {
			/* Sendlist Processing error */
			errors[0] = 1U << 2U;
			errors[1] = 0;
			errors[2] = 0;
			icu_status = asw_PUS_5_TM_report(EVENT_SENDLIST_ERROR, 3, errors);
		}

		/* Try to read next sendlist in any case */
		handler_1553.last_sendlist_processed = sl_index;

		/* In case all sendlists read are processed delay task execution */
		if (handler_1553.last_sendlist_processed
				== handler_1553.last_sendlist_read) {
			status = rtems_task_wake_after(40);
		}
	}
}

rtems_task asw_task_tc_manager(rtems_task_argument arg) {
	uint16_t tc_check = 0;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	icu_status_code status = ICU_SUCCESSFUL;

	telecommand = asw_create_tc(tc_data);

	/*Check if the received TC is correct*/
	tc_check = asw_validate_command(telecommand, tc_data, &ptrTC);

	if (tc_check == CMD_OK) {
		/*Send acceptance telemetry*/
		if ((telecommand.data_field.ack & PUS_1_1_GEN_FLAG) > 0) {
			status = asw_PUS_1_1_TM_TC_accept_ok(telecommand.pkt_id.all_bits,
					telecommand.pkt_seq.all_bits,
					telecommand.data_field.sourceId);
		}
		/*Restart task to execute TC*/
		status = rtems_task_restart(task_exec_id, 0);

	} else {
		status = asw_PUS_1_2_TM_TC_accept_fail(telecommand.pkt_id.all_bits,
				telecommand.pkt_seq.all_bits, telecommand.data_field.sourceId,
				tc_check, 0, NULL );
	}

	/*suspend current task*/
	rtems_status = rtems_task_suspend(task_tc_id);
}

rtems_task asw_task_tc_exec(rtems_task_argument arg) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	icu_status_code status = ICU_SUCCESSFUL;
	telecommand_t tc = telecommand;

	/* Start tc timer */
	rtems_status = rtems_timer_fire_after(tc_timer_id, ptrTC.timeOut * 90,
			asw_tc_timeout_isr, (void *) &tc);

	/* Execute function related to TC received */
	if (ptrTC.ptrf != NULL ) {
		status = ptrTC.ptrf(tc);
	}

	/*Check if the function returns with an error*/
	if (status != ICU_SUCCESSFUL) {
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId, (uint16_t) status,
				0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	}

	/* Stop tc timer */
	rtems_status = rtems_timer_cancel(tc_timer_id);

	rtems_status = rtems_task_suspend(RTEMS_SELF );
}

rtems_task asw_task_atr(rtems_task_argument arg) {

	TransferDescriptorDataWord_t atr = { 0 };
	uint8_t atr_counter = 0, sa = 0;
	rtems_status_code status = RTEMS_SUCCESSFUL;
	size_t n_byte = 0, block_size = 0;
	uint32_t ret = 0, number_msgs = 0;
	uint8_t ptr_data[MIL_1553_MAX_TM_PKT_LENGTH] = { 0 };
	uint32_t max_ev_pkt = 4;

	/* If counters of ATR and ATC are different no new telemetry will be created */
	if (handler_1553.ATR.blockCount != handler_1553.ATC.blockCount) {
		status = rtems_task_suspend(RTEMS_SELF );
	}

	/* Suspend if there is no TM  */
	if (queues_with_data == 0) {
		status = rtems_task_suspend(RTEMS_SELF );
	}

	/*Check TM queues in order of priority
	 * Event and PUS Service 1 queues are checked only for CF 8
	 * these queues transmit data on SA 12Tx
	 */

	if ((queues_with_data >= QUEUE_PUS_1_DATA_IN)
			&& (handler_1553.current_comm_frame == 8)) {
		/* In this case trasmission subaddress is 12*/
		sa = SA_EV_AND_PUS_1;
		/*Clear sub address to be written*/
		ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

		/* check events and PUS Service 1 queues in the following order
		 * event 5,4 queue
		 * event 5,3 queue
		 * event 5,2 queue
		 * event 5,1 queue
		 * PUS service 1 queue
		 * Max 4 event packet ina TM Block in order of severity
		 */

		/* 5,4 event */
		if ((queues_with_data & QUEUE_5_4_DATA_IN) != 0) {
			status = rtems_message_queue_get_number_pending(ev_5_4_queue_id,
					&number_msgs);
			while ((max_ev_pkt > 0) && (number_msgs > 0) > 0) {
				status = rtems_message_queue_receive(ev_5_4_queue_id, ptr_data,
						&block_size, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);
				/* Copy  TM packet */
				memcpy((void*) (ptr_evt_pus_1) + n_byte, ptr_data, block_size);

				n_byte += block_size;

				number_msgs--;
				max_ev_pkt--;
			}

			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_5_4_DATA_IN;
			}
		}

		/* Event 5,3 */
		if (((queues_with_data & QUEUE_5_3_DATA_IN) != 0)
				&& (max_ev_pkt > 0) > 0) {
			status = rtems_message_queue_get_number_pending(ev_5_3_queue_id,
					&number_msgs);
			while ((max_ev_pkt > 0) && (number_msgs > 0) > 0) {
				status = rtems_message_queue_receive(ev_5_3_queue_id, ptr_data,
						&block_size, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);
				/* Copy  TM packet */
				memcpy((void*) (ptr_evt_pus_1) + n_byte, ptr_data, block_size);

				n_byte += block_size;

				number_msgs--;
				max_ev_pkt--;
			}

			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_5_3_DATA_IN;
			}
		}

		/* Event 5,2 */
		if (((queues_with_data & QUEUE_5_2_DATA_IN) != 0)
				&& (max_ev_pkt > 0) > 0) {
			status = rtems_message_queue_get_number_pending(ev_5_2_queue_id,
					&number_msgs);
			while ((max_ev_pkt > 0) && (number_msgs > 0) > 0) {
				status = rtems_message_queue_receive(ev_5_2_queue_id, ptr_data,
						&block_size, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);
				/* Copy  TM packet */
				memcpy((void*) (ptr_evt_pus_1) + n_byte, ptr_data, block_size);

				n_byte += block_size;

				number_msgs--;
				max_ev_pkt--;
			}

			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_5_2_DATA_IN;
			}
		}

		/* Event 5,1 */
		if (((queues_with_data & QUEUE_5_1_DATA_IN) != 0)
				&& (max_ev_pkt > 0) > 0) {
			status = rtems_message_queue_get_number_pending(ev_5_1_queue_id,
					&number_msgs);
			while ((max_ev_pkt > 0) && (number_msgs > 0) > 0) {
				status = rtems_message_queue_receive(ev_5_1_queue_id, ptr_data,
						&block_size, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);
				/* Copy  TM packet */
				memcpy((void*) (ptr_evt_pus_1) + n_byte, ptr_data, block_size);
				n_byte += block_size;

				number_msgs--;
				max_ev_pkt--;
			}

			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_5_1_DATA_IN;
			}
		}

		/* check if there are TM data in PUS Service 1 queue */
		if ((queues_with_data & QUEUE_PUS_1_DATA_IN) != 0) {

			status = rtems_message_queue_get_number_pending(pus_1_queue_id,
					&number_msgs);

			/* Acquire all possible PUS Service 1 packet
			 * block_size <= 996 is used to avoid to have an overflow of data to be written in the SubAddress
			 */

			while (((number_msgs > 0) && (n_byte <= 992)) > 0) {
				status = rtems_message_queue_receive(pus_1_queue_id, ptr_data,
						&block_size, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

				/* Copy  TM packet */
				memcpy((void*) (ptr_evt_pus_1) + n_byte, ptr_data, block_size);
				n_byte += block_size;

				number_msgs--;
			}

			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_PUS_1_DATA_IN;
			}

		}

	} else {

		/* check all other queues in the following order
		 * Memory dump queue
		 * PUS Service 3 queue
		 * Diagnostic 1 queue
		 * Diagnostic 0 queue
		 * HK 4 queue
		 * HK 3 queue
		 * HK 2 queue
		 * HK 1 queue
		 */

		if ((queues_with_data & QUEUE_M_DUMP_DATA_IN) != 0) {
			status = rtems_message_queue_receive(mem_dump_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_DUMP;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_dump != NULL ) {
				memcpy(ptr_dump, ptr_data, n_byte);
			}

			status = rtems_message_queue_get_number_pending(mem_dump_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_M_DUMP_DATA_IN;
			}
		}

		else if ((queues_with_data & QUEUE_PUS_3_DATA_IN) != 0) {
			status = rtems_message_queue_receive(pus_3_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_TM_PUS_3;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_pus_3 != NULL ) {
				memcpy(ptr_pus_3, ptr_data, n_byte);
			}

			status = rtems_message_queue_get_number_pending(pus_3_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_PUS_3_DATA_IN;
			}
		}

		else if ((queues_with_data & QUEUE_DIAG_0_DATA_IN) != 0) {
			status = rtems_message_queue_receive(diag_0_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_DIAG_0;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_diag_0 != NULL ) {
				memcpy(ptr_diag_0, ptr_data, n_byte);
			}

			status = rtems_message_queue_get_number_pending(diag_0_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_DIAG_0_DATA_IN;
			}
		}

		else if ((queues_with_data & QUEUE_DIAG_1_DATA_IN) != 0) {
			status = rtems_message_queue_receive(diag_1_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_DIAG_1;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_diag_1 != NULL ) {
				memcpy(ptr_diag_1, ptr_data, n_byte);
			}

			status = rtems_message_queue_get_number_pending(diag_1_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_DIAG_1_DATA_IN;
			}
		}

		else if ((queues_with_data & QUEUE_HK_4_DATA_IN) != 0) {
			status = rtems_message_queue_receive(hk_4_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_HK_4;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_hk_4 != NULL ) {
				memcpy(ptr_hk_4, ptr_data, n_byte);
			}

			status = rtems_message_queue_get_number_pending(hk_4_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_HK_4_DATA_IN;
			}
		}

		else if ((queues_with_data & QUEUE_HK_3_DATA_IN) != 0) {
			status = rtems_message_queue_receive(hk_3_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_HK_3;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_hk_3 != NULL ) {
				memcpy(ptr_hk_3, ptr_data, n_byte);
			}

			status = rtems_message_queue_get_number_pending(hk_3_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_HK_3_DATA_IN;
			}
		}

		else if ((queues_with_data & QUEUE_HK_2_DATA_IN) != 0) {
			status = rtems_message_queue_receive(hk_2_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_HK_2;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_hk_2 != NULL ) {
				memcpy(ptr_hk_2, ptr_data, n_byte);
			}
			status = rtems_message_queue_get_number_pending(hk_2_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_HK_2_DATA_IN;
			}
		}

		else {

			status = rtems_message_queue_receive(hk_1_queue_id, ptr_data,
					&n_byte, RTEMS_NO_WAIT, RTEMS_NO_TIMEOUT);

			sa = SA_HK_1;

			/*Clear sub address to be written*/
			ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(sa, 1);

			if (ptr_hk_1 != NULL ) {
				memcpy(ptr_hk_1, ptr_data, n_byte);
			}

			status = rtems_message_queue_get_number_pending(hk_1_queue_id,
					&number_msgs);
			if (number_msgs == 0) {
				queues_with_data = queues_with_data & ~QUEUE_HK_1_DATA_IN;
			}

		}

	}

	if (n_byte != 0) {
		/* create new ATR */
		atr_counter = asw_create_ATR_counter();

		/* Set transfer descriptor message */
		atr = asw_set_transfer_descriptor(n_byte, 0, 0, 1, sa, atr_counter);

		/* Update atr counter inside ASW 1553 Handler */
		handler_1553.ATR.all_bits = atr.all_bits;

		/* Copy new ATR message into related subaddress */
		memcpy(ptr_atr, &atr, sizeof(uint32_t));
	}

	status = rtems_task_suspend(RTEMS_SELF );
}

rtems_task asw_task_tm(rtems_task_argument arg) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	icu_status_code status = ICU_SUCCESSFUL;
	uint32_t rep_mask = 0;
	uint16_t i = 0, j = 0;
	uint8_t pos_found = 0, k = 0;

	while (1) {
		i = 0;
		j = 0;
		k = 0;
		pos_found = 0;
		/*ICU_ASW_APID = minimun APID        APID_NUMBER = currently maximum APID +
		 * 1  --> see tm.h */
		/*
		 * CHECK ON INPUT PARAMETERS:
		 * 		APID shall be allowed
		 * 		SID_number shall be < TM_MAX_REPORT_NUMBER
		 * 		SID_number shall be existing
		 * 		SID_number shall be enabled
		 */
		for (j = 0; j < (APID_NUMBER - ICU_ASW_APID); j++) {
			/*
			 * HK telemetry
			 */
			rep_mask = TM_report_flag[j];

			for (i = 0; i < TM_MAX_REPORT_NUMBER; i++) {

				if ((rep_mask % 2) == 1) {

					SID[j][i].generation_cnt--;
					if (SID[j][i].generation_cnt == 0) {
						status = asw_PUS_3_25_TM_HK_report(j + ICU_ASW_APID,
								i + 1);
						SID[j][i].generation_cnt =
								SID[j][i].collection_interval;
					}
				}

				rep_mask >>= 1;
			}

			/*
			 * Diagnostic telemetry
			 */
			rep_mask = TM_DIAG_report_flag[j];
			for (i = 0; i < TM_DIAG_MAX_REPORT_NUMBER; i++) {

				if ((rep_mask % 2) == 1) {

					diag_SID[j][i].generation_cnt--;
					if (diag_SID[j][i].generation_cnt == 0) {

						/* Select sampling space */
						while ((k < TM_DIAG_MAX_SIMULT_REPORT_NUMB)
								&& (pos_found == 0)) {
							if ((((uint16_t) diag_sampling_space[k][0] << 8U)
									| (uint16_t) diag_sampling_space[k][1])
									== diag_SID[j][i].SID_number) {
								pos_found = k;
							}

							k++;
						}
						/*
						 * check if the data collection is finished
						 */
						if ((diag_sampling_space[pos_found][2] & 1U) == 0) {

							status = asw_PUS_3_26_TM_DIAG_report(
									j + ICU_ASW_APID,
									diag_SID[j][i].SID_number);
							diag_SID[j][i].generation_cnt =
									diag_SID[j][i].collection_interval;
							/*
							 * state that data has been used; start again the collection
							 */
							diag_sampling_space[pos_found][2] =
									diag_sampling_space[pos_found][2]
											| (1U << 1);

						} else {
							/*
							 * since it should be time to generate the packet, but the collection has not
							 * ended, force the generation counter to +1, so next cycle the task check again
							 * if the collection is finished
							 */
							diag_SID[j][i].generation_cnt++;
						}

					}

				}

				rep_mask >>= 1;
			}

		}

		rtems_status = rtems_task_wake_after(1000);
	}
}

rtems_task asw_task_idle(rtems_task_argument arg) {
	uint32_t *start_address = (uint32_t *) 0x40000000;
	uint32_t value = 0;

	while (1) {
		if (start_address == (uint32_t *) 0x40700000) {
			start_address = (uint32_t *) 0x40000000;
		}
		value = *start_address;

		start_address = (uint32_t *) (start_address + 1);
	}
}

rtems_task asw_task_fdir(rtems_task_argument arg) {
	icu_status_code icu_status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t evt_id = 0;
	uint32_t size_in_byte = 0;
	uint32_t param = 0;
	uint8_t errors[8] = { 0 };
	uint8_t i = 0;
	uint8_t num_note = 0;
	uint16_t fdir_on_off = 0;
	uint16_t fdir_mode_par_ID = 0;
	uint16_t idx = 0;
	uint32_t err_funct = 0;

	/* Set HK parameter FDIR Mode */
	fdir_mode_par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 20U;
	idx = asw_PUS_3_ID_to_not_mon_DB_idx(fdir_mode_par_ID);

	while (1) {

		/* Generate PUS event packet */
		rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_0,
				&evt_id);
		rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_1,
				&size_in_byte);

		/* RTEMS_NOTEPAD to be read */
		if ((size_in_byte % 4) == 0) {
			num_note = (uint8_t) (size_in_byte / 4);
		} else {
			num_note = (uint8_t) ((size_in_byte / 4) + 1);
		}

		/* error function error */
		if (size_in_byte != 0) {
			rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_2,
					&err_funct);
			errors[0] = (uint8_t) err_funct;
		}

		/* Copy error params in a container */
		for (i = 0; i < num_note; i++) {
			rtems_status = rtems_task_get_note(RTEMS_SELF, RTEMS_NOTEPAD_3 + i,
					&param);
			memcpy(&errors[(i * 4) + 1], &param, sizeof(uint32_t));
		}

		/* Size +1 --> function that generate the error */
		icu_status = asw_PUS_5_TM_report((uint16_t) evt_id,
				(uint8_t) (size_in_byte + 1), errors);

		/* Check if FDIR is activated and if there is an FDIR procedure to be executed */
		icu_sem_lock(not_mon_db_sem_Id, 5);
		fdir_on_off = db_param[idx].current_value;
		icu_sem_unlock(not_mon_db_sem_Id);

		if (fdir_on_off == 1) {
			if (fdir_collection[evt_id].en_dis == TRUE) {
				fdir_collection[evt_id].ptrf();
			}
		}
		rtems_status = rtems_task_suspend(RTEMS_SELF );
	}
}

rtems_task asw_task_tm_acq(rtems_task_argument arg) {
	uint32_t ret = 0;
	icu_tm_acq_manager_status_t acq_status;
	rtems_status_code status = RTEMS_SUCCESSFUL;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint16_t values[DAS_PARAM_NM] = { 0 };
	uint8_t cycle_nr = 0;
	uint16_t task_par_id = 0;
	uint16_t idx = 0, idx_long = 0;
	uint8_t i = 0;
	uint8_t errors[5] = { 0 };

	/* Get DB index of first task status parameter */
	task_par_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 5U;
	idx = asw_PUS_3_ID_to_not_mon_DB_idx(task_par_id);

	ret = icu_tm_acq_mng_start_tm_acq();
	if (ret != 0) {
		errors[0] = 0x8; //start_tm_acq funct
		memcpy(&errors[1], &ret, sizeof(uint32_t));
		icu_status = asw_PUS_5_TM_report(EVENT_TM_ACQ_ERROR, 5, errors);
		status = rtems_task_suspend(RTEMS_SELF );
	}

	/*
	 * Task will acquire data
	 * to change TM driver configuration must stop drivers execution and reconfigure it
	 */

	while (1) {
		ret = icu_tm_acq_mng_get_status(&acq_status);

		if (acq_status.tm_buffer_data_available == 1) {
			ret = icu_tm_acq_mng_get_tms(DAS_PARAM_NM, values);
			asw_copy_data_in_das_db((uint8_t *) arg, values, DAS_PARAM_NM,
					cycle_nr);
			cycle_nr = (cycle_nr + 1) % 3;
		}

		/* Copy task status on DB */
		for (i = 0; i < (CONFIGURE_MAXIMUM_TASKS - 1); i++) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			idx_long = db_param[idx + (uint16_t) i].current_value;
			db_long_curr_value[idx_long][0] = 0;
			db_long_curr_value[idx_long][1] =
					(uint32_t) (thread_ctrl[i]->current_state);
			icu_sem_unlock(not_mon_db_sem_Id);
		}

		status = rtems_task_wake_after(500);
	}
}

rtems_task asw_task_monitor(rtems_task_argument arg) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t ret = 0;
	uint32_t edac_status = 0;
	uint32_t edac_fail_addr = 0;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint8_t last_dpu1_stat = CPU_OFF, last_dpu2_stat = CPU_OFF;

	while (1) {
		/* Check EDAC single error */
		ret = icu_edac_mng_scrub_single_fail(&edac_status, &edac_fail_addr);
		if ((ret != 0) && (edac_status != 0)) {
			icu_status = asw_PUS_5_TM_report(EVENT_EDAC_ERROR, 0, NULL );
		}

		/* Check CU status and voltage if is on */
		//asw_monitor_leds();
		/* DPU errors*/
		//asw_monitor_dpu_errors();
		/* CMD_DAS_ALARMS_READ_REQ ( 60 is ok) */
		//asw_monitor_das_alarms();
		/*ICU_CDPU_TMP*/
		//asw_monitor_cdpu_tmp();
		/*ICU_LVPS_TMP */
		//asw_monitor_lvps_tmp();
		/* FWA_MOT_PHA_I, FWA_MOT_PHB_I, GWA_MOT_PHA_I, GWA_MOT_PHB_I if is on */
		//asw_monitor_wheels();
		/* Check interrupt not used */
		//asw_monitor_masked_irq();
		/* Change profile in case of DPU BSW<->ASW transitions */
		if (((dpu1_status != last_dpu1_stat) || (dpu2_status != last_dpu2_stat))
				== TRUE) {
			/* DPU 1 */
			if (((last_dpu1_stat == CPU_UNDEFINED)
					|| (last_dpu1_stat == CPU_OFF)
					|| (last_dpu1_stat == CPU_BOOT)) == TRUE) {
				asw_prepare_bus_profile();
			} else {
				/* In case last value read is an ASW status */
				if (((dpu1_status >= CPU_ASW_INIT)
						&& (dpu1_status <= CPU_MANUAL)) == TRUE) {
					/* Nothing to do */
				} else {
					asw_prepare_bus_profile();
				}
			}
			/* Change DPU_1 last value*/
			last_dpu1_stat = dpu1_status;

			/* DPU 2 */
			if (((last_dpu2_stat == CPU_UNDEFINED)
					|| (last_dpu2_stat == CPU_OFF)
					|| (last_dpu2_stat == CPU_BOOT)) == TRUE) {
				asw_prepare_bus_profile();
			} else {
				/* In case last value read is an ASW status */
				if (((dpu2_status >= CPU_ASW_INIT)
						&& (dpu2_status <= CPU_MANUAL)) == TRUE) {
					/* Nothing to do */
				} else {
					asw_prepare_bus_profile();
				}
			}
			/* Change DPU_2 last value*/
			last_dpu2_stat = dpu2_status;

			/* Switch active profile */
			/* Select structure msg_counter to be updated */
			if (bus_profile_msg_counter == bus_profile_msg_counter_0) {
				bus_profile_msg_counter = bus_profile_msg_counter_1;
				init_bus_profile_msg_counter = init_bus_profile_msg_counter_1;
			} else {
				bus_profile_msg_counter = bus_profile_msg_counter_0;
				init_bus_profile_msg_counter = init_bus_profile_msg_counter_0;
			}

			/* Check if HK7 (DPU-BSW) must be enable/disable */
			if (((last_dpu2_stat == CPU_BOOT) || (last_dpu1_stat == CPU_BOOT))
					== TRUE) {
				/* Enabled HK7 */
				asw_PUS_3_enable_HK_report_flag(7);
			} else {
				/* Disable HK7 */
				asw_PUS_3_disable_HK_rep_flag(7);
			}

		}
		rtems_status = rtems_task_wake_after(500);

	}
}
