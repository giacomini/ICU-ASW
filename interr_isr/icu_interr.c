/**
 * \file	icu_interr.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	15-03-2017
 *
 */

#include "../../rtems_configuration.h"

rtems_task asw_handler_mil_1553(rtems_vector_number vector) {

	rtems_status_code rtems_status = ICU_SUCCESSFUL;
	rtems_interrupt_level level = 0;
	uint32_t rt_err_reg_value = 0;
	uint32_t rt_nom_reg_value = 0;
	uint32_t rt_rst_reg_value = 0;
	uint32_t ret = 0;
	uint16_t sa = 0, dwc = 0;

	/*Disable interrupt */
	rtems_interrupt_disable(level);

	/* Checks errors on 1553 RT */
	rt_err_reg_value = icu_mil_std_1553_rt_drv_get_error_irq_reg_value();

	/* Read nominal register */
	rt_nom_reg_value = icu_mil_std_1553_rt_drv_get_nominal_irq_reg_value();

	/* Synch bit = 2 TrOk = 1...check = 0x3 */
	if ((0x3U & rt_nom_reg_value) == 3) {

		/* RT interrupt */
		number_of_msg = 30;

		ret = icu_mil_std_1553_rt_drv_get_last_msgs(&msg_index, msg_buffer,
				&number_of_msg);

		/* Check synch...read just index 0 because is the last synch message
		 * received that causes interrupt handler */
		if (number_of_msg > 0) {
			sa = msg_buffer[number_of_msg - 1].cmd_word.subaddress;
			dwc = msg_buffer[number_of_msg - 1].cmd_word.data_word_count;

			if (((sa == 31)	&& (dwc	== 1)) == TRUE) {
				handler_1553.current_comm_frame = 0;
			} else {
				if (((sa == 31)	&& (dwc == 17)) == TRUE) {
					ret = icu_mil_std_1553_rt_drv_get_mode_code_data_word(17,
							&handler_1553.current_comm_frame);
				}
			}

			rtems_status = rtems_task_resume(task_rt_id);
			if (rtems_status != RTEMS_SUCCESSFUL) {
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_0, EVENT_TASK_NOT_STARTED);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_1, 2);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_2, 1); //isr
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_3, ((uint32_t) 1U << 16));
				rtems_status = rtems_task_resume(task_fdir_id);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					rtems_error_report(NON_FATAL_ERROR, rtems_status);
				}
			}
		}
	} else if (rt_err_reg_value != 0) {
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
				EVENT_RT_1553_ERROR);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1, 4);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2, 1); //isr
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3, rt_err_reg_value); //isr
		rtems_status = rtems_task_resume(task_fdir_id);
		if (rtems_status != RTEMS_SUCCESSFUL) {
			rtems_error_report(NON_FATAL_ERROR, rtems_status);
		}
	} else {
		/* Check and clean reset interrupt on RT */
		rt_rst_reg_value = icu_mil_std_1553_rt_drv_get_reset_irq_reg_value();
	}

	/*Enable interrupt*/
	icu_intmngr_mdpa_pic_clear_irq_pending(MDPA_PIC_INT_MILBUS);
	rtems_interrupt_enable(level);
}

rtems_task asw_handler_edac_double_error(rtems_vector_number vector) {
	rtems_interrupt_level level = 0;

	/*Disable interrupt */
	rtems_interrupt_disable(level);

	/* Set reset cause */
	if (vector == 0x09) {
		icu_reset_set_forced_warm_reset_info(ASW_RESET_RAM_EDAC_DOUBLE_ERR, dr_addr);
	} else {
		icu_reset_set_forced_warm_reset_info(ASW_RESET_MDPA_EDAC_DOUBLE_ERR, dr_addr);
	}
	/* Create death report */
	asw_create_death_report();

	/* Generate SW reset */
	icu_reset_generate_sw_reset();

	/*Enable interrupt*/
	rtems_interrupt_enable(level);

}

rtems_task asw_fpga_irq2_handler_irq(rtems_vector_number vector) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t status = 0, ret = 0;
	rtems_interrupt_level level = 0;

	/* Module to be compared  for IRQ 2 and submodule
	 * FPGA_IRQ2_TM_ACQ_ERR_STATUS_BIT
	 * FPGA_IRQ2_EEPROM_STATUS_BIT
	 * FPGA_IRQ2_WD_T1_STATUS_BIT
	 */

	/*Disable interrupt */
	rtems_interrupt_disable(level);

	ret = icu_intmngr_fpga_get_status(FPGA_IRQ2, &status);

	/* TM acquisition Error*/
	if (((uint32_t)status & (uint32_t) FPGA_IRQ2_TM_ACQ_ERR_STATUS_BIT)!= 0){
		ret = icu_intmngr_fpga_get_module_pending(FPGA_IRQ2_TM_ACQ_ERR,
				&status);
		if (status != 0) {
			ret = icu_intmngr_fpga_clear_module_pending(FPGA_IRQ2_TM_ACQ_ERR,
					status);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
					EVENT_TM_ACQ_ERROR);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1,
					4);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2,
					1); //1 tm_acq isr function
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3,
					status);
			rtems_status = rtems_task_resume(task_fdir_id);
			if (rtems_status != RTEMS_SUCCESSFUL) {
				rtems_error_report(NON_FATAL_ERROR, rtems_status);
			}
		}
	}
	/* EEPROM */
	else if (((uint32_t)status & (uint32_t) FPGA_IRQ2_EEPROM_STATUS_BIT)!= 0){
		ret = icu_intmngr_fpga_get_module_pending(FPGA_IRQ2_EEPROM, &status);
		if (status != 0) {
			ret = icu_intmngr_fpga_clear_module_pending(FPGA_IRQ2_EEPROM,
					status);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
					EVENT_EEPROM_ERROR);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1,
					4);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2,
					1); //isr
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3,
					status);
			rtems_status = rtems_task_resume(task_fdir_id);
			if (rtems_status != RTEMS_SUCCESSFUL) {
				rtems_error_report(NON_FATAL_ERROR, rtems_status);
			}
		}
	}
	/* Watchdog */
	else {
		if (((uint32_t)status & (uint32_t) FPGA_IRQ2_WD_T1_STATUS_BIT) != 0){
			icu_watchdog_stop_count();
			ret = icu_intmngr_fpga_get_module_pending(FPGA_IRQ2_WD_T1, &status);
			if (status != 0) {
				ret = icu_intmngr_fpga_clear_module_pending(FPGA_IRQ2_WD_T1,
						status);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_0, EVENT_WATCHDOG_T1_EXPIRATION);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_1, 4);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_2, 1); //isr
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_3, status);
				rtems_status = rtems_task_resume(task_fdir_id);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					rtems_error_report(NON_FATAL_ERROR, rtems_status);
				}

				/* Start self timer */
#if 0
				rtems_status = rtems_timer_fire_after(timer_no_wd_id, 500,
						asw_no_wd_isr, NULL);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					rtems_error_report(NON_FATAL_ERROR, rtems_status);
				}
#endif
			}
		}
	}

	icu_intmngr_mdpa_pic_clear_irq_pending(MDPA_PIC_INT_PARALLEL_IO3);

	/*Enable interrupt*/
	rtems_interrupt_enable(level);

}

rtems_timer_service_routine asw_wheels_start_movement(rtems_id id, void *argument) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	rtems_interrupt_level level = 0;

	/*Disable interrupt */
	rtems_interrupt_disable(level);

	rtems_status = rtems_task_restart(task_wp_exec_id, 0);
	if (rtems_status != RTEMS_SUCCESSFUL) {
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
				EVENT_TASK_NOT_STARTED);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1, 2);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2, 1); //isr
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3,
				(((uint32_t) 1U << 10U) << 16U));
		rtems_status = rtems_task_resume(task_fdir_id);
		if (rtems_status != RTEMS_SUCCESSFUL) {
			rtems_error_report(NON_FATAL_ERROR, rtems_status);
		}
	}

	/* Disable bit WP_TO_BE_STARTED */
	wheel_flag = wheel_flag & ~WP_TO_BE_STARTED;

	/*Enable interrupt*/
	rtems_interrupt_enable(level);
}

rtems_timer_service_routine asw_tc_timeout_isr(rtems_id id, void *argument) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	rtems_interrupt_level level = 0;
	telecommand_t *tc = (telecommand_t *) argument;

	/*Disable interrupt */
	rtems_interrupt_disable(level);

	status = asw_PUS_1_8_TM_TC_exec_fail(tc->pkt_id.all_bits,
			tc->pkt_seq.all_bits, tc->data_field.sourceId, TM_1_8_EXEC_ERROR, 0,
			NULL );
	rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
			EVENT_CMD_NOT_EXECUTED);
	rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1, 0);
	rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2, 1); //isr
	rtems_status = rtems_task_resume(task_fdir_id);
	if (rtems_status != RTEMS_SUCCESSFUL) {
		rtems_error_report(NON_FATAL_ERROR, rtems_status);
	}

	rtems_status = rtems_task_suspend(task_exec_id);

	/*Enable interrupt*/
	rtems_interrupt_enable(level);

}

rtems_timer_service_routine asw_no_wd_isr(rtems_id id, void *argument) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t ret = 0;
	uint8_t tc_data[1016];
	rtems_interrupt_level level = 0;

	/*Disable interrupt */
	rtems_interrupt_disable(level);

	/*
	 * Set ICU CPU usage inside the DB
	 * Using (10 * 2) instead of 10 cause cycle is 2 Hz (not 1 Hz)
	 */
	cpu_load = (uint16_t) (100 - (count_idle / (10 * 2)));
	count_idle = 0;

	/*Read last messages received since last call, in this case in particular check if there is a new ATC*/
	ret = icu_mil_std_1553_rt_drv_get_last_msgs(&msg_index, msg_buffer,
			&number_of_msg);
	ret = asw_check_last_rcv_cmd_word(msg_buffer, number_of_msg);

	/* Check if there is a new TC */
	ret = asw_check_new_DTD(tc_data);
	if (ret == 0) {
		rtems_status = rtems_task_restart(task_tc_id, 0);
		if (rtems_status != RTEMS_SUCCESSFUL) {
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
					EVENT_TASK_NOT_STARTED);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1,
					2);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2,
					1); //isr
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3,
					(((uint32_t) 1U << 2U) << 16U));
			rtems_status = rtems_task_resume(task_fdir_id);
			if (rtems_status != RTEMS_SUCCESSFUL) {
				rtems_error_report(NON_FATAL_ERROR, rtems_status);
			}
		}
	}

	/* try to generate a new TM block */
	if ((queues_with_data != 0) && (tm_reset_active == FALSE)) {
		rtems_status = rtems_task_restart(task_atr_id, 0);
		if (rtems_status != RTEMS_SUCCESSFUL) {
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
					EVENT_TASK_NOT_STARTED);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1,
					2);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2,
					1); //isr
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3,
					(((uint16_t) 1U << 3U) << 16U));
			rtems_status = rtems_task_resume(task_fdir_id);
			if (rtems_status != RTEMS_SUCCESSFUL) {
				rtems_error_report(NON_FATAL_ERROR, rtems_status);
			}
		}
	}

	/* Reset timer */
	rtems_status = rtems_timer_reset(timer_no_wd_id);

	/*Enable interrupt*/
	rtems_interrupt_enable(level);
}

void asw_monitor_masked_irq(void) {
	uint32_t bc_err_reg_value = 0;
	uint32_t bc_nom_reg_value = 0;
	uint32_t bc_res_reg_value = 0;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;

	/* Check MILBUS 1553 BC interrupt */
	bc_err_reg_value = icu_mil_std_1553_bc_drv_get_error_irq_reg_value();
	if (bc_err_reg_value != 0) {
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
				EVENT_BC_1553_ERROR);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1, 0);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2, 1); //isr
		rtems_status = rtems_task_resume(task_fdir_id);
		if (rtems_status != RTEMS_SUCCESSFUL) {
			rtems_error_report(NON_FATAL_ERROR, rtems_status);
		}
	}

	/* in case of reset or nominal only clean registers */
	bc_nom_reg_value = icu_mil_std_1553_rt_drv_get_nominal_irq_reg_value();
	bc_res_reg_value = icu_mil_std_1553_rt_drv_get_reset_irq_reg_value();

	return;
}
