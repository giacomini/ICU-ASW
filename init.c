/**
 * \file	init.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	29-01-2016
 *
 */

#define CONFIGURE_INIT
#define ASM_WPF_AREA asm (".equ pf_area, WPF_BASE_RAM_ADDRESS")
#include "rtems_configuration.h"
#include <rtems/confdefs.h>
#include <rtems/asm.h>

/* Declaration of assembly routines implemented in DeathReport.S */
/**
 * \brief Function used to read all SPARC windows registers.
 * \param[in] start_address First address where registers must be copied
 * \return First writable address
 */
uint32_t asw_create_dr_regs(uint32_t start_address);

uint32_t count_idle = 0;
rtems_interval ticks_begin = 0, ticks_end = 0;
uint8_t n_task = 0;
rtems_tcb* thread_ctrl[12] = { NULL };
uint16_t cpu_load = 0;
uint32_t dr_addr = 0x40600038U;
uint32_t health_addr = 0x40600000U;
bool_t obt_synch_enable = TRUE;

Handler_1553_t handler_1553 = { { 0 }, { 0 }, { 0 }, { 0 }, { 0, 0, 0, 0, 0 },
		0, 0, 0, 0, 0 };

/* uint16_t MDUMP_data_length = 0; */
mem_dump_flag_t mem_dump_active = { 0, 0, 0, 0 };

ICU_settings_t icu_settings = { 0x30U, 0x7FFFU, 0xAU, 0x30U, 0x1FEU, 0xAU,
		0x30U, 0x1FEU, 0xAU, 0x3FFU, 0x3E8, 0x1770U, 0x1770U, 0x19U, 0x1770U,
		0xBB8, 0x7000U, 0x7FFFU, 0x7000U, 0x7FFFU, 0x3CU, 0x1770U, 0x1770U };

rtems_id task_rt_id = 0;
rtems_id task_bc_id = 0;
rtems_id task_tc_id = 0;
rtems_id task_exec_id = 0;
rtems_id task_atr_id = 0;
rtems_id task_tm_id = 0;
rtems_id task_idle_id = 0;
rtems_id task_fdir_id = 0;
rtems_id task_tm_acq_id = 0;
rtems_id taskTC_6_5_Id = 0;
rtems_id task_wp_exec_id = 0;
rtems_id task_monitor_id = 0;

rtems_id fm_sem_id = 0;
rtems_id spi_sem_id = 0;
rtems_id queue_sem_id = 0;
rtems_id mem_reg_sem_Id = 0;
rtems_id monit_db_sem_Id = 0;
rtems_id not_mon_db_sem_Id = 0;
rtems_id TM_cnt_sem_Id = 0;
rtems_id cmd_ver_tab_sem_Id = 0;
rtems_id send_list_sem_id = 0;

rtems_id wheel_timer_id = 0;
rtems_id tc_timer_id = 0;
rtems_id timer_no_wd_id = 0;
rtems_id dpu1_tc_timer_id = 0;
rtems_id dpu2_tc_timer_id = 0;

rtems_id ev_5_4_queue_id = 0;
rtems_id ev_5_3_queue_id = 0;
rtems_id ev_5_2_queue_id = 0;
rtems_id ev_5_1_queue_id = 0;
rtems_id pus_1_queue_id = 0;
rtems_id pus_3_queue_id = 0;
rtems_id hk_4_queue_id = 0;
rtems_id hk_3_queue_id = 0;
rtems_id hk_2_queue_id = 0;
rtems_id hk_1_queue_id = 0;
rtems_id diag_0_queue_id = 0;
rtems_id diag_1_queue_id = 0;
rtems_id mem_dump_queue_id = 0;
uint16_t queues_with_data = 0;

rtems_task Init(rtems_task_argument arg) {
	rtems_status_code status = RTEMS_SUCCESSFUL;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	rtems_name task_rt_name, task_bc_name, task_tc_name, task_atr_name,
			task_tm_name, task_idle_name, task_fdir_name, task_tm_acq_name,
			task_exec_name, taskTC_6_5_name, task_wp_name, task_monitor_name;
	rtems_name fm_sem_name, spi_sem_name, queue_sem_name, memregSemName,
			mon_db_sem_name, not_mon_db_sem_name, TM_cnt_sem_name,
			cmd_ver_tab_sem_name, send_list_sem_name;
	rtems_name ev_5_4_queue_name, ev_5_3_queue_name, ev_5_2_queue_name,
			ev_5_1_queue_name, pus_1_queue_name, hk_4_queue_name,
			hk_3_queue_name, hk_2_queue_name, hk_1_queue_name,
			diag_1_queue_name, diag_0_queue_name, mem_dump_queue_name,
			pus_3_queue_name;
	rtems_name ext_name;
	rtems_name wheel_timer_name, tc_timer_name, timer_no_wd_name,
			dpu1_tc_timer_name, dpu2_tc_timer_name;

	rtems_isr_entry old_handle_1553, old_handle_irq_10, old_handle_edac_09, old_handle_edac_20;
	rtems_interrupt_level level = 0;
	rtems_id ext_id = 0;
	rtems_extensions_table myExt = { asw_myCreate, NULL, NULL, NULL,
			asw_mySwitch, NULL, NULL, NULL };
	uint32_t irq_int_status = 0, ret = 0, rt_sub_tx_init = 0,
			rt_sub_rx_init = 0, rt_mc_init = 0;
	uint16_t i = 0, tasks_init = 0, queues_init = 0, dsw_comp_init = 0;
	uint16_t fwa_gwa_single_error = 0, fwa_gwa_double_error = 0, pic_irq_mask =
			0, sems_init = 0;
	uint8_t fdl_error = 0, timers_init = 0, dsw_error = 0;
	uint8_t errors[5] = { 0 }, eeprom_data[4] = { 0 };
	uint32_t par_id_table[DAS_PARAM_NM] = { NI_SA_TMP_1_CRYO, NI_SA_TMP_2_CRYO,
			NI_SA_TMP_3_CRYO, NI_SA_TMP_4_CRYO, NI_SA_TMP_5_CRYO,
			NI_SA_TMP_6_CRYO, NI_CSS_TMP_1_CRYO, NI_CSS_TMP_2_CRYO,
			NI_SSS_TMP_1_CRYO, NI_SSS_TMP_2_CRYO, NI_FWA_TMP_CRYO,
			NI_GWA_TMP_CRYO, NI_SA_TMP_1_FR, NI_SA_TMP_2_FR, NI_SA_TMP_3_FR,
			NI_SA_TMP_4_FR, NI_SA_TMP_5_FR, NI_SA_TMP_6_FR, NI_CSS_TMP_1_FR,
			NI_CSS_TMP_2_FR, NI_SSS_TMP_1_FR, NI_SSS_TMP_2_FR, NI_FWA_TMP_FR,
			NI_GWA_TMP_FR, FWA_MOT_PHA_I, FWA_MOT_PHB_I, GWA_MOT_PHA_I,
			GWA_MOT_PHB_I, NI_CU_LED_V_1, NI_CU_LED_V_2, NI_CU_LED_V_3,
			NI_CU_LED_V_4, NI_CU_LED_V_5, ICU_5V, ICU_5V_CURRENT, ICU_CDPU_TMP,
			ICU_LVPS_TMP, ASM_TSM1_GAIN, ASM_TSM1_OFFSET, SSS_CHAIN_CRYO_OFFSET,
			SSS_CRYO_CHAIN_GAIN, CSS_CRYO_CHAIN_OFFSET, CSS_CRYO_CHAIN_GAIN,
			CMD_DAS_ALARMS_READ_REQ, CMD_MOT_HS_READ_REQ, OBT_COARSE_HIGH,
			OBT_COARSE_LOW, OBT_FINE_HIGH, OBT_FINE_LOW };
	icu_tm_acq_manager_conf_t config_das_acq;
	uint8_t acq_tab_par_idxs[DAS_PARAM_NM] = { 0 };
	uint16_t par_ID = 0;
	uint16_t idx = 0;
	/* Used to test hs_search and hs_check procedures nr_par_to_be_acq = 8  and par_id_table[DAS_PARAM_NM] = {FWA_MOT_PHA_I, FWA_MOT_PHB_I, GWA_MOT_PHA_I, GWA_MOT_PHB_I}; */
	//uint32_t par_id_table[DAS_PARAM_NM] = {OBT_COARSE_HIGH, OBT_COARSE_LOW, OBT_FINE_HIGH, OBT_FINE_LOW, OBT_COARSE_HIGH, OBT_COARSE_LOW, OBT_FINE_HIGH, OBT_FINE_LOW};

	/*Disable interrupt */
	rtems_interrupt_disable(level);

	/* Disable EDAC and forced isr*/
	*((volatile uint32_t *) 0x80000008U) = 0xC8000000U;

	/*
	 * Initialize RTEMS time - to be used by PUS Service 5
	 */
	rtems_time_of_day TimeSet = { 2017, 2, 21, 15, 8, 30, 1 };
	status = rtems_clock_set(&TimeSet);

	/*Initialize crc table*/
	asw_init_CRC16_table();

	/*
	 * Initialize DAS acquisition settings
	 */
	config_das_acq.settling_time_0 = icu_settings.acq_st0;
	config_das_acq.settling_time_1 = icu_settings.acq_st1;
	config_das_acq.settling_time_2 = icu_settings.acq_st2;
	config_das_acq.settling_time_3 = icu_settings.acq_st3;
	config_das_acq.sync_acq_disable = FALSE;
	config_das_acq.sync_acq_timeout = icu_settings.acq_sync_timeout;
	config_das_acq.cyclic_mode_enable = TRUE;
	config_das_acq.cyclic_period = 500;

	/*Initialize all drivers and configure 1553 RT and BC cores*/
	icu_runtime_init_dsw_startup();

	dsw_error = asw_configure_rt(&rt_sub_rx_init, &rt_sub_tx_init, &rt_mc_init);
	if (dsw_error == 0) {
		dsw_comp_init = 1;
	}

	dsw_error = asw_configure_bc();
	if (dsw_error == 0) {
		dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 1);
	}

	icu_eeprom_init();
	dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 2);

	icu_MDPA_timer_ctrl_init();
	dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 3);

	icu_FPGA_timer_ctrl_init();
	dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 4);

	icu_watchdog_init();
	dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 5);

	icu_intmngr_fpga_init();
	dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 6);

	icu_intmngr_mdpa_init();
	dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 7);

	ret = icu_spi_ctrl_initialize();
	if (ret == 0) {
		dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 8);
	}

	ret = icu_led_drv_initialize((uint32_t)icu_settings.leds_max_duty_cycle, (uint32_t)icu_settings.leds_max_delay);
	if (ret == 0) {
		dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 9);
	}

	ret = icu_heater_initialize();
	if (ret == 0) {
		dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 10);
	}

	ret = icu_tm_acq_mng_initialize(&config_das_acq);
	if (ret == 0) {
		/* Set default parameters to be acquired and start acquisition */
		ret = icu_tm_acq_mng_load_whole_tm_id_table(DAS_PARAM_NM, par_id_table);

		if (ret != 0) {
			errors[0] = 0x4; //load_table_ids funct
			memcpy(&errors[1], &ret, sizeof(uint32_t));
			icu_status = asw_PUS_5_TM_report(EVENT_TM_ACQ_ERROR, 5, errors);
		} else {
			asw_from_das_IDs_to_ASW_Idxs(par_id_table, acq_tab_par_idxs,
					DAS_PARAM_NM);
			dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 11);
		}
	} else {
		errors[0] = 0x2; //init funct
		memcpy(&errors[1], &ret, sizeof(uint32_t));
		icu_status = asw_PUS_5_TM_report(EVENT_TM_ACQ_ERROR, 5, errors);
	}

	ret = icu_fwa_gwa_drv_initialize();
	if (ret == 0) {
		dsw_comp_init = dsw_comp_init | ((uint16_t) 1U << 12);
	}

	/* Set watchdog startup parameters*/
	ret = icu_watchdog_self_test();
	icu_watchdog_set_interval_to_interrupt(icu_settings.wd_t1);
	icu_watchdog_set_interval_from_interrupt_to_reset(icu_settings.wd_t2);
	icu_watchdog_start_count();

	/*OBT loading*/
	ret = icu_obt_self_test();
	if (ret != 0) {
		errors[0] = 1;
		icu_status = asw_PUS_5_TM_report(EVENT_OBT_ERROR, 1, errors);
	}

	ret = icu_obt_write_current_obt(0, 0);
	/*Copy all zeros in the SA 29 Tx*/
	memset(ptr_time_tx, 0, 5 * sizeof(uint16_t));
	if (ret != 0) {
		errors[0] = (uint8_t) 1U << 1U;
		icu_status = asw_PUS_5_TM_report(EVENT_OBT_ERROR, 1, errors);
	}

	/* EEPROM self-test */

	ret = icu_eeprom_self_test(eeprom_data);
	if (ret != 0) {
		errors[0] = 2; //selftest
		memcpy(&errors[1], &eeprom_data[0], 4 * sizeof(uint8_t));
		icu_status = asw_PUS_5_TM_report(EVENT_EEPROM_ERROR, 5, errors);
	}

	/* Initialize reset mask */
	ret = icu_reset_set_mask(
			RESET_WATCHDOG_RST_SOURCE | RESET_1553_RST_SOURCE
					| RESET_CPULOCK_RST_SOURCE);

	/* Copy wheel profiles from EEPROM */
	/* Assign memory area to wheel profiles */
	ASM_WPF_AREA;
	asw_store_wheels_profile_in_RAM(&fwa_gwa_single_error,
			&fwa_gwa_double_error, &fdl_error);
	errors[0] = fdl_error;
	errors[1] = (uint8_t) ((fwa_gwa_single_error & 0xFF00U) >> 8U);
	errors[2] = (uint8_t) (fwa_gwa_single_error & 0xFFU);
	errors[3] = (uint8_t) ((fwa_gwa_double_error & 0xFF00U) >> 8U);
	errors[4] = (uint8_t) (fwa_gwa_double_error & 0xFFU);

	if (((fdl_error >= 3) || (fwa_gwa_double_error > 0)) == TRUE) {
		//No FDL or profile loaded
		icu_status = asw_PUS_5_TM_report(EVENT_WHEEL_PROFILE_NOT_LOADED, 5,
				errors);
	} else {
		if (((fdl_error == 1) || (fwa_gwa_single_error > 0)) == TRUE) {
			icu_status = asw_PUS_5_TM_report(EVENT_NOT_NOMINAL_WPF_LOADED, 5,
					errors);
		}
	}

	/*Initialize telemetry*/
	asw_initialize_DB();

	/* Initialize fdir structure */
	asw_initialize_fdir();

	/* PUS Service 3 reports */
	asw_PUS_3_initialize_HK_reports();
	asw_PUS_3_init_DIAG_reports();

	/* Enable HK 1 packet */
	asw_PUS_3_enable_HK_report_flag(1);

	/*
	 * Initialize all events and enable all except DPU events
	 */
	asw_PUS_5_initialize_EID();
	for (i = 0; i < ICU_ASW_EVENT_NUMBER; i++) {
		asw_PUS_5_enable_event(i);
	}
	asw_PUS_5_disable_dpu_events();

	/*
	 * Create RTEMS extension
	 */
	ext_name = rtems_build_name('E', 'X', 'T', '1');
	status = rtems_extension_create(ext_name, &myExt, &ext_id);

	/*Create semaphores*/
	fm_sem_name = rtems_build_name('F', 'M', '-', '-');
	spi_sem_name = rtems_build_name('S', 'P', 'I', '-');
	queue_sem_name = rtems_build_name('Q', 'U', 'S', 'M');
	memregSemName = rtems_build_name('M', 'E', 'M', 'R');
	not_mon_db_sem_name = rtems_build_name('D', 'B', 'S', '-');
	mon_db_sem_name = rtems_build_name('M', 'D', 'B', 'S');
	TM_cnt_sem_name = rtems_build_name('T', 'M', 'S', 'M');
	cmd_ver_tab_sem_name = rtems_build_name('C','M','D','V');
	send_list_sem_name = rtems_build_name('S','L','S','M');

	status = rtems_semaphore_create(fm_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&fm_sem_id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = 1;
	}
	status = rtems_semaphore_create(spi_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&spi_sem_id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 1);
	}
	status = rtems_semaphore_create(queue_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&queue_sem_id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 2);
	}
	status = rtems_semaphore_create(memregSemName, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&mem_reg_sem_Id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 3);
	}
	status = rtems_semaphore_create(mon_db_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&monit_db_sem_Id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 4);
	}
	status = rtems_semaphore_create(not_mon_db_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&not_mon_db_sem_Id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 5);
	}
	status = rtems_semaphore_create(TM_cnt_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&TM_cnt_sem_Id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 6);
	}

	status = rtems_semaphore_create(cmd_ver_tab_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&cmd_ver_tab_sem_Id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 7);
	}

	status = rtems_semaphore_create(send_list_sem_name, 1,
			(uint32_t) RTEMS_PRIORITY | (uint32_t) RTEMS_BINARY_SEMAPHORE, 20,
			&send_list_sem_id);
	if (status == RTEMS_SUCCESSFUL) {
		sems_init = sems_init | ((uint16_t) 1U << 8);
	}

	/* Create queues */
	ev_5_4_queue_name = rtems_build_name('E', '5', '4', 'Q');
	ev_5_3_queue_name = rtems_build_name('E', '5', '3', 'Q');
	ev_5_2_queue_name = rtems_build_name('E', '5', '2', 'Q');
	ev_5_1_queue_name = rtems_build_name('E', '5', '1', 'Q');
	pus_1_queue_name = rtems_build_name('P', 'U', '1', 'Q');
	pus_3_queue_name = rtems_build_name('P', 'U', '3', 'Q');
	hk_4_queue_name = rtems_build_name('H', 'K', '4', 'Q');
	hk_3_queue_name = rtems_build_name('H', 'K', '3', 'Q');
	hk_2_queue_name = rtems_build_name('H', 'K', '2', 'Q');
	hk_1_queue_name = rtems_build_name('H', 'K', '1', 'Q');
	diag_1_queue_name = rtems_build_name('D', 'G', '1', 'Q');
	diag_0_queue_name = rtems_build_name('D', 'G', '0', 'Q');
	mem_dump_queue_name = rtems_build_name('M', 'D', 'P', 'Q');

	status = rtems_message_queue_create(ev_5_4_queue_name, 5, 64,
			RTEMS_DEFAULT_ATTRIBUTES, &ev_5_4_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = 1;
	}
	status = rtems_message_queue_create(ev_5_3_queue_name, 5, 64,
			RTEMS_DEFAULT_ATTRIBUTES, &ev_5_3_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 1);
	}
	status = rtems_message_queue_create(ev_5_2_queue_name, 5, 64,
			RTEMS_DEFAULT_ATTRIBUTES, &ev_5_2_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 2);
	}
	status = rtems_message_queue_create(ev_5_1_queue_name, 5, 64,
			RTEMS_DEFAULT_ATTRIBUTES, &ev_5_1_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 3);
	}
	status = rtems_message_queue_create(pus_1_queue_name, 5, 64,
			RTEMS_DEFAULT_ATTRIBUTES, &pus_1_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 4);
	}
	status = rtems_message_queue_create(pus_3_queue_name, 2, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &pus_3_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 5);
	}
	status = rtems_message_queue_create(hk_4_queue_name, 5, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &hk_4_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 6);
	}
	status = rtems_message_queue_create(hk_3_queue_name, 5, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &hk_3_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 7);
	}
	status = rtems_message_queue_create(hk_2_queue_name, 5, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &hk_2_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 8);
	}
	status = rtems_message_queue_create(hk_1_queue_name, 5, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &hk_1_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 9);
	}
	status = rtems_message_queue_create(diag_1_queue_name, 2, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &diag_1_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 10);
	}
	status = rtems_message_queue_create(diag_0_queue_name, 2, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &diag_0_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 11);
	}
	status = rtems_message_queue_create(mem_dump_queue_name, 2, 1024,
			RTEMS_DEFAULT_ATTRIBUTES, &mem_dump_queue_id);
	if (status == RTEMS_SUCCESSFUL) {
		queues_init = queues_init | ((uint16_t) 1U << 12);
	}

	/* Create Timers */
	wheel_timer_name = rtems_build_name('W', 'H', 'T', 'R');
	tc_timer_name = rtems_build_name('F', 'C', 'T', 'R');
	timer_no_wd_name = rtems_build_name('N', 'O', 'W', 'D');
	dpu1_tc_timer_name = rtems_build_name('D', 'P', 'U', '1');
	dpu2_tc_timer_name = rtems_build_name('D', 'P', 'U', '2');

	status = rtems_timer_create(wheel_timer_name, &wheel_timer_id);
	if (status == RTEMS_SUCCESSFUL) {
		timers_init = 1;
	}

	status = rtems_timer_create(tc_timer_name, &tc_timer_id);
	if (status == RTEMS_SUCCESSFUL) {
		timers_init = timers_init | (1U << 1);
	}

	status = rtems_timer_create(timer_no_wd_name, &timer_no_wd_id);
	if (status == RTEMS_SUCCESSFUL) {
		timers_init = timers_init | (1U << 2);
	}

	status = rtems_timer_create(dpu1_tc_timer_name, &dpu1_tc_timer_id);
	if (status == RTEMS_SUCCESSFUL) {
		timers_init = timers_init | (1U << 3);
	}

	status = rtems_timer_create(dpu2_tc_timer_name, &dpu2_tc_timer_id);
	if (status == RTEMS_SUCCESSFUL) {
		timers_init = timers_init | (1U << 4);
	}

	/*Initialize TM protocol*/
	asw_telemetry_reset();

	/*Create RTEMS tasks*/
	task_rt_name = rtems_build_name('R', 'T', 'E', 'T');
	task_bc_name = rtems_build_name('B', 'C', 'O', 'T');
	task_tc_name = rtems_build_name('T', 'C', 'D', 'T');
	task_atr_name = rtems_build_name('A', 'T', 'R', 'T');
	task_tm_name = rtems_build_name('T', 'M', 'Y', 'T');
	task_idle_name = rtems_build_name('I', 'D', 'L', 'T');
	task_exec_name = rtems_build_name('T', 'C', 'E', 'T');
	task_fdir_name = rtems_build_name('F', 'D', 'R', 'T');
	task_tm_acq_name = rtems_build_name('T', 'M', 'A', 'T');
	taskTC_6_5_name = rtems_build_name('M', 'D', 'P', 'T');
	task_wp_name = rtems_build_name('W', 'H', 'L', 'T');
	task_monitor_name = rtems_build_name('M', 'N', 'R', 'T');

	status = rtems_task_create(task_rt_name, 15, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_rt_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_rt_id, asw_task_rt, 0);
		if (status == RTEMS_SUCCESSFUL) {
			status = rtems_task_suspend(task_rt_id);
			tasks_init = 1;
		}
	}

	status = rtems_task_create(task_bc_name, 18, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_bc_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_bc_id, asw_task_bc, 0);
		if (status == RTEMS_SUCCESSFUL) {
			tasks_init = tasks_init | ((uint16_t) 1U << 1);
		}
	}

	status = rtems_task_create(task_tc_name, 20, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_tc_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_tc_id, asw_task_tc_manager, 0);
		if (status == RTEMS_SUCCESSFUL) {
			status = rtems_task_suspend(task_tc_id);
			tasks_init = tasks_init | ((uint16_t) 1U << 2);
		}
	}

	status = rtems_task_create(task_atr_name, 19, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_atr_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_atr_id, asw_task_atr, 0);
		if (status == RTEMS_SUCCESSFUL) {
			status = rtems_task_suspend(task_atr_id);
			tasks_init = tasks_init | ((uint16_t) 1U << 3);
		}
	}

	status = rtems_task_create(task_tm_name, 19, 2 * RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_tm_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_tm_id, asw_task_tm, 0);
		if (status == RTEMS_SUCCESSFUL) {
			tasks_init = tasks_init | ((uint16_t) 1U << 4);
		}
	}

	status = rtems_task_create(task_idle_name, 50, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_idle_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_idle_id, asw_task_idle, 0);
		if (status == RTEMS_SUCCESSFUL) {
			tasks_init = tasks_init | ((uint16_t) 1U << 5);
		}
	}

	status = rtems_task_create(task_exec_name, 20, 2 * RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &task_exec_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_exec_id, asw_task_tc_exec, 0);
		if (status == RTEMS_SUCCESSFUL) {
			status = rtems_task_suspend(task_exec_id);
			tasks_init = tasks_init | ((uint16_t) 1U << 6);
		}
	}

	status = rtems_task_create(task_fdir_name, 10, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_NO_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_fdir_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_fdir_id, asw_task_fdir, 0);
		if (status == RTEMS_SUCCESSFUL) {
			status = rtems_task_suspend(task_fdir_id);
			tasks_init = tasks_init | ((uint16_t) 1U << 7);
		}
	}

	status = rtems_task_create(task_tm_acq_name, 20, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_tm_acq_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_tm_acq_id, asw_task_tm_acq,
				(rtems_task_argument) acq_tab_par_idxs);
		if (status == RTEMS_SUCCESSFUL) {
			tasks_init = tasks_init | ((uint16_t) 1U << 8);
		}
	}

	status = rtems_task_create(taskTC_6_5_name, 20, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &taskTC_6_5_Id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(taskTC_6_5_Id, asw_taskTC_6_5, 0);
		if (status == RTEMS_SUCCESSFUL) {
			status = rtems_task_suspend(taskTC_6_5_Id);
			tasks_init = tasks_init | ((uint16_t) 1U << 9);
		}
	}

	status = rtems_task_create(task_wp_name, 18, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_wp_exec_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_wp_exec_id, asw_task_wheel_exec, 0);
		if (status == RTEMS_SUCCESSFUL) {
			status = rtems_task_suspend(task_wp_exec_id);
			tasks_init = tasks_init | ((uint16_t) 1U << 10);
		}
	}

	status = rtems_task_create(task_monitor_name, 16, RTEMS_MINIMUM_STACK_SIZE,
			RTEMS_PREEMPT, RTEMS_DEFAULT_ATTRIBUTES, &task_monitor_id);
	if (status == RTEMS_SUCCESSFUL) {
		status = rtems_task_start(task_monitor_id, asw_task_monitor, 0);
		if (status == RTEMS_SUCCESSFUL) {
			tasks_init = tasks_init | ((uint16_t) 1U << 11);
		}
	}

	/* Create Health report */
	asw_create_health_report(tasks_init, sems_init, queues_init, timers_init,
			dsw_comp_init, rt_sub_tx_init, rt_sub_rx_init, rt_mc_init);

	/* In case of error during components initialization try to reboot */
	if ((timers_init != ASW_TIMER_MASK) || (sems_init != ASW_SEMAPHORE_MASK)
			|| (tasks_init != ASW_TASKS_MASK)
			|| (dsw_comp_init != ASW_DSW_COMPONENTS_MASK)
			|| (queues_init != ASW_QUEUES_MASK)) {

		/* Create death report */
		asw_create_death_report();

		/* To be enabled */
		icu_reset_set_forced_warm_reset_info(ASW_RESET_INIT_FAILURE,
				(uint32_t) dr_addr);
		icu_reset_generate_sw_reset();
	}

	/*Enable interrupt*/
	/* Install ISRs */
	rtems_mask_interrupt(0x01);
	rtems_mask_interrupt(0x07);

	//FPGA IRQ 2
	ret = icu_intmngr_fpga_get_module_mask(FPGA_IRQ2_WD_T1, &irq_int_status);
	ret = icu_intmngr_fpga_set_module_mask(FPGA_IRQ2_WD_T1,
			irq_int_status | FPGA_IRQ2_WD_T1_ENABLE_ALL);
	ret = icu_intmngr_fpga_get_module_mask(FPGA_IRQ2_TM_ACQ_ERR,
			&irq_int_status);
	ret = icu_intmngr_fpga_set_module_mask(FPGA_IRQ2_TM_ACQ_ERR,
			(irq_int_status | FPGA_IRQ2_TM_ACQ_ERR_ENABLE_ALL) & ~FPGA_IRQ2_TM_ACQ_ERR_SYNCACQ_TIMEOUT);
	ret = icu_intmngr_fpga_get_module_mask(FPGA_IRQ2_EEPROM, &irq_int_status);
	ret = icu_intmngr_fpga_set_module_mask(FPGA_IRQ2_EEPROM,
			irq_int_status | FPGA_IRQ2_EEPROM_ENABLE_ALL);

	/* FPGA IRQ 3 */
//	ret = icu_intmngr_fpga_get_module_mask(FPGA_IRQ3_MOTOR_PROFILE_END, &irq_int_status);
//	ret = icu_intmngr_fpga_set_module_mask(FPGA_IRQ3_MOTOR_PROFILE_END,
//			irq_int_status | FPGA_IRQ3_MOTOR_PROFILE_END_ENABLE_ALL);
//	ret = icu_intmngr_fpga_get_module_mask(FPGA_IRQ3_MOTOR_ERR, &irq_int_status);
//	ret = icu_intmngr_fpga_set_module_mask(FPGA_IRQ3_MOTOR_ERR,
//			irq_int_status | FPGA_IRQ3_MOTOR_ERR_ENABLE_ALL);

	pic_irq_mask = icu_intmngr_mdpa_pic_get_irq_mask();
	icu_intmngr_mdpa_pic_set_irq_mask(
			pic_irq_mask | (uint16_t)MDPA_PIC_INT_MILBUS | (uint16_t)MDPA_PIC_INT_PARALLEL_IO3);

	status = rtems_interrupt_catch(asw_handler_mil_1553, LEON_TRAP_TYPE(12),
			&old_handle_1553);
	status = rtems_interrupt_catch(asw_fpga_irq2_handler_irq, LEON_TRAP_TYPE(10),
			&old_handle_irq_10);
	status = rtems_interrupt_catch(asw_handler_edac_double_error,
			SPARC_SYNCHRONOUS_TRAP(0x09), &old_handle_edac_09);
	status = rtems_interrupt_catch(asw_handler_edac_double_error,
			SPARC_SYNCHRONOUS_TRAP(0x20), &old_handle_edac_20);
	icu_intmngr_mdpa_pic_clear_irq_pending(MDPA_PIC_INT_PARALLEL_IO2);
	icu_intmngr_mdpa_pic_clear_irq_pending(MDPA_PIC_INT_PARALLEL_IO3);
	icu_intmngr_mdpa_pic_clear_irq_pending(MDPA_PIC_INT_MILBUS);

	/* Set ASW_version HK parameter */
	par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 18U;
	idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

	icu_sem_lock(not_mon_db_sem_Id, 5);
	db_param[idx].current_value = (uint8_t) ASW_VERSION;
	icu_sem_unlock(not_mon_db_sem_Id);

	/* Set ICU state --> To be enabled for transitions steps */
	icu_state_machine = ICU_STARTUP;
	par_ID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 4U;
	idx = asw_PUS_3_ID_to_not_mon_DB_idx(par_ID);

	icu_sem_lock(not_mon_db_sem_Id, 5);
	db_param[idx].current_value = (uint8_t) icu_state_machine;
	icu_sem_unlock(not_mon_db_sem_Id);

	/* Notify SpaceCraft that initialization is completed and ICU is in STARTUP mode*/
	icu_status = asw_PUS_5_TM_report(EVENT_STARTUP_COMPLETED, 0, NULL);

	/* Set watchdog default parameters*/
	icu_watchdog_stop_count();
	icu_watchdog_set_interval_to_interrupt(WDG_TO_INT_MIN_INTERVAL);
	icu_watchdog_set_interval_from_interrupt_to_reset(WDG_TO_RES_MIN_INTERVAL);
	wd_activation = TRUE;

	rtems_interrupt_enable(level);

	status = rtems_task_delete(RTEMS_SELF);
}

/*
 * Redefinition of the RTEMS standard function used to manage fatal error
 */

boolean app_safe_state_handler(Internal_errors_Source the_source,
		boolean is_internal, uint32_t the_error) {

	/* Create death report */
	asw_create_death_report();

	/* To be enabled */
	icu_reset_set_forced_warm_reset_info(ASW_RESET_RTEMS_FATAL_ERROR,
			(uint32_t) dr_addr);
	icu_reset_generate_sw_reset();
	return FALSE;
}

boolean asw_myCreate(rtems_tcb *current_task, rtems_tcb *new_task) {

	thread_ctrl[n_task] = (rtems_tcb*) new_task;
	n_task++;
	return TRUE;
}

rtems_extension asw_mySwitch(rtems_tcb *current_task, rtems_tcb *heir_task) {

	rtems_status_code status = RTEMS_SUCCESSFUL;

	if (current_task->Object.id == task_idle_id) {
		status = rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &ticks_end);
		count_idle = count_idle + (ticks_end - ticks_begin);
	} else {
		if (heir_task->Object.id == task_idle_id) {
			status = rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT,
					&ticks_begin);

		}
	}

}

void asw_create_health_report(uint16_t tasks_init, uint16_t sems_init,
		uint16_t queues_init, uint8_t timers_init, uint16_t dsw_comp_init,
		uint32_t rt_sub_tx_init, uint32_t rt_sub_rx_init, uint32_t rt_mc_init) {
	uint32_t ret = 0;
	icu_led_drv_status_t led_status;
	bool_t status = TRUE;
	uint32_t n_byte = 0;
	uint16_t count1 = 0, count2 = 0;

	/* Calibration Unit status */
	ret = icu_led_drv_get_status(&led_status);
	memcpy((icu_led_drv_status_t*) (health_addr) + n_byte, &led_status,
			sizeof(icu_led_drv_status_t));
	n_byte = n_byte + sizeof(icu_led_drv_status_t);

	/* WD */
	status = icu_watchdog_is_watchdog_started();
	count1 = WDG_TO_INT_MIN_INTERVAL;
	count2 = WDG_TO_RES_MIN_INTERVAL;

	memcpy((void*) (health_addr) + n_byte, &status, sizeof(bool_t));
	n_byte = n_byte + sizeof(bool_t);
	memcpy((void*) (health_addr) + n_byte, &count1, sizeof(uint16_t));
	n_byte = n_byte + sizeof(uint16_t);
	memcpy((void*) (health_addr) + n_byte, &count2, sizeof(uint16_t));
	n_byte = n_byte + sizeof(uint16_t);

	/* 1553 RT */
	memcpy((void*) (health_addr) + n_byte, &rt_sub_rx_init, sizeof(uint32_t));
	n_byte = n_byte + sizeof(uint32_t);
	memcpy((void*) (health_addr) + n_byte, &rt_sub_tx_init, sizeof(uint32_t));
	n_byte = n_byte + sizeof(uint32_t);
	memcpy((void*) (health_addr) + n_byte, &rt_mc_init, sizeof(uint32_t));
	n_byte = n_byte + sizeof(uint32_t);

	/* Drivers components initialization */
	memcpy((void*) (health_addr) + n_byte, &dsw_comp_init, sizeof(uint16_t));
	n_byte = n_byte + sizeof(uint16_t);

	/* Status of RTEMS objects initialized by ASW */
	memcpy((void*) (health_addr) + n_byte, &tasks_init, sizeof(uint16_t));
	n_byte = n_byte + sizeof(uint16_t);
	memcpy((void*) (health_addr) + n_byte, &queues_init, sizeof(uint16_t));
	n_byte = n_byte + sizeof(uint16_t);
	memcpy((void*) (health_addr) + n_byte, &sems_init, sizeof(uint16_t));
	n_byte = n_byte + sizeof(uint16_t);
	memcpy((void*) (health_addr) + n_byte, &timers_init, sizeof(uint8_t));
	n_byte = n_byte + sizeof(uint8_t);
	/* Clear other bits of adderss */
	memset((void*) (health_addr) + n_byte, 0, 3 * sizeof(uint8_t));
	return;
}

void asw_create_death_report(void) {
	rtems_status_code status = RTEMS_SUCCESSFUL;
	rtems_error_message err_msg;
	int16_t i = 0;
	uint8_t j = 0;
	uint32_t end_address = 0;
	uint32_t ret = 0;
	uint32_t n_byte = 0;
	uint32_t reset_cause = 0;
	uint32_t death_rep_addr = 0;

	/* Copy window registers */
	end_address = asw_create_dr_regs(dr_addr);

	/* Copy reset cause */
	ret = icu_reset_get_reset_source();
	memcpy(((void*) end_address), &ret, sizeof(uint32_t));
	n_byte += sizeof(uint32_t);

	/* Copy warm reset register */
	ret = icu_reset_get_forced_warm_reset_info(&reset_cause, &death_rep_addr);
	memcpy(((void*) end_address) + n_byte, &reset_cause, sizeof(uint32_t));
	n_byte += sizeof(uint32_t);

	/* Copy Thread Control structure of all tasks */
	for (j = 0; j < n_task; j++) {
		memcpy(((void*) end_address) + n_byte, &thread_ctrl[n_task],
				sizeof(rtems_tcb));
		n_byte += sizeof(rtems_tcb);
	}

	/* Copy actual state of ICU-ASW */
	memcpy(((void*) end_address) + n_byte, &icu_state_machine,
			sizeof(state_machine_t));
	n_byte += sizeof(state_machine_t);

	/* copy non-fatal error */
	i = 1;
	while (i < 201) {
		status = rtems_error_get_latest_non_fatal_by_offset(-i, &err_msg);
		if (status == RTEMS_SUCCESSFUL) {
			memcpy(((void*) end_address) + n_byte, &err_msg,
					sizeof(rtems_error_message));
			n_byte += sizeof(rtems_error_message);
			i++;
		} else {
			break;
		}

	}

	/* copy fatal error */
	i = 1;
	while (i < 101) {
		status = rtems_error_get_latest_fatal_by_offset(-i, &err_msg);
		if (status == RTEMS_SUCCESSFUL) {
			memcpy(((void*) end_address) + n_byte, &err_msg,
					sizeof(rtems_error_message));
			n_byte += sizeof(rtems_error_message);
			i++;
		} else {
			break;
		}

	}

	return;
}
