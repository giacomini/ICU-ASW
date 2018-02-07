/**
 * \file	PUS_Service_8_1_fwa_gwa.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	20-04-2017
 *
 */

#include "../rtems_configuration.h"

/* Private variables */
telecommand_t wheel_tc;

/* Global variable initialization */
uint16_t hs_center_search = 0;
uint8_t wheel_flag = 0;
wheel_profile_manager_t wheel_profiles = { { 0, 0, 0 } };

void asw_store_wheels_profile_in_RAM(uint16_t *fwa_gwa_single_error,
		uint16_t *fwa_gwa_double_error, uint8_t *fdl_error) {

	uint32_t ret = 0;
	uint32_t offset = 0;
	uint8_t error_fdl = 0;
	uint32_t fdl_deploy_address[MAX_PROFILES_NUMBER] = { 0 };

	/* Deploy FDL */
	asw_deploy_FDL(fdl_deploy_address, &error_fdl);

	/* In case of no loadable FDL exit without download profiles */
	if (error_fdl > 3) {
		//Set profile error masks...No one loaded
		*fwa_gwa_double_error = 0xFFFF;
		*fwa_gwa_single_error = 0xFFFF;
	} else {

		/* If no error are detected in almost one FDL try to Deploy profiles */
		/* FWA_CW_72 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), FWA_CW_72,
				fwa_gwa_single_error, fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.fwa_cw_72.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_cw_72.degree = 72;
			wheel_profiles.fwa_cw_72.execution_time = 20;
		}

		/* FWA_C_CW_72 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), FWA_C_CW_72,
				fwa_gwa_single_error, fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.fwa_c_cw_72.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_c_cw_72.degree = -72;
			wheel_profiles.fwa_c_cw_72.execution_time = 20;
		}

		/* FWA_CW_144 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), FWA_CW_144,
				fwa_gwa_single_error, fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.fwa_cw_144.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_cw_144.degree = 144;
			wheel_profiles.fwa_cw_144.execution_time = 20;
		}

		/* FWA_C_CW_144 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), FWA_C_CW_144,
				fwa_gwa_single_error, fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.fwa_c_cw_144.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_c_cw_144.degree = -144;
			wheel_profiles.fwa_c_cw_144.execution_time = 20;
		}

		/*
		 * FWA_ALL_ONE_DEGREE
		 * In deploy function used index of profile 1degree step 1
		 * Starting from this point index of eeprom profile indexes and ram_profiles are misplaced
		 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), FWA_CW_1_1_2,
				fwa_gwa_single_error, fwa_gwa_double_error);
		if (ret == 0) {
			/* FWA_CW_1_1_2 */
			wheel_profiles.fwa_cw_1_1_2.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_cw_1_1_2.degree = 1;
			wheel_profiles.fwa_cw_1_1_2.execution_time = 7;

			/* FWA_CW_1_2_3 */
			wheel_profiles.fwa_cw_1_2_3.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_cw_1_2_3.degree = 1;
			wheel_profiles.fwa_cw_1_2_3.execution_time = 7;

			/* FWA_CW_1_3_4 */
			wheel_profiles.fwa_cw_1_3_4.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_cw_1_3_4.degree = 1;
			wheel_profiles.fwa_cw_1_3_4.execution_time = 7;

			/* FWA_CW_1_4_1 */
			wheel_profiles.fwa_cw_1_4_1.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_cw_1_4_1.degree = 1;
			wheel_profiles.fwa_cw_1_4_1.execution_time = 7;

			/* FWA_C_CW_1_1_4 */
			wheel_profiles.fwa_c_cw_1_1_4.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_c_cw_1_1_4.degree = -1;
			wheel_profiles.fwa_c_cw_1_1_4.execution_time = 7;

			/* FWA_C_CW_1_4_3 */
			wheel_profiles.fwa_c_cw_1_4_3.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_c_cw_1_4_3.degree = -1;
			wheel_profiles.fwa_c_cw_1_4_3.execution_time = 7;

			/* FWA_C_CW_1_3_2 */
			wheel_profiles.fwa_c_cw_1_3_2.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_c_cw_1_3_2.degree = -1;
			wheel_profiles.fwa_c_cw_1_3_2.execution_time = 7;

			/* FWA_C_CW_1_2_1 */
			wheel_profiles.fwa_c_cw_1_2_1.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_c_cw_1_2_1.degree = -1;
			wheel_profiles.fwa_c_cw_1_2_1.execution_time = 7;
		}

		/* FWA_FOUR_DEGREE */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 5, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			/* FWA_CW_4 */
			wheel_profiles.fwa_cw_4.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_cw_4.degree = 4;
			wheel_profiles.fwa_cw_4.execution_time = 10;

			/* FWA_C_CW_4 */
			wheel_profiles.fwa_c_cw_4.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_c_cw_4.degree = -4;
			wheel_profiles.fwa_c_cw_4.execution_time = 10;
		}

		/* FWA_TO_ZERO */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 6, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.fwa_to_zero.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_to_zero.degree = -56;
			wheel_profiles.fwa_to_zero.execution_time = 15;
		}

		/* FWA_TO_START */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 7, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.fwa_to_start.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.fwa_to_start.degree = 64;
			wheel_profiles.fwa_to_start.execution_time = 20;
		}

		/* GWA profiles */
		/* GWA_CW_72 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 8, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.gwa_cw_72.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_cw_72.degree = 72;
			wheel_profiles.gwa_cw_72.execution_time = 20;
		}

		/* GWA_C_CW_72 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 9, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.gwa_c_cw_72.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_c_cw_72.degree = -72;
			wheel_profiles.gwa_c_cw_72.execution_time = 20;
		}

		/* GWA_C_144 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 10, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.gwa_cw_144.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_cw_144.degree = 144;
			wheel_profiles.gwa_cw_144.execution_time = 20;
		}

		/* GWA_C_CW_144 */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 11, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.gwa_c_cw_144.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_c_cw_144.degree = -144;
			wheel_profiles.gwa_c_cw_144.execution_time = 20;
		}

		/* GWA_ALL_ONE_DEGREE */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 12, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			/* GWA_CW_1_1_2 */
			wheel_profiles.gwa_cw_1_1_2.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_cw_1_1_2.degree = 1;
			wheel_profiles.gwa_cw_1_1_2.execution_time = 7;

			/* GWA_CW_1_2_3 */
			wheel_profiles.gwa_cw_1_2_3.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_cw_1_2_3.degree = 1;
			wheel_profiles.gwa_cw_1_2_3.execution_time = 7;

			/* GWA_CW_1_3_4 */
			wheel_profiles.gwa_cw_1_3_4.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_cw_1_3_4.degree = 1;
			wheel_profiles.gwa_cw_1_3_4.execution_time = 7;

			/* GWA_CW_1_4_1 */
			wheel_profiles.gwa_cw_1_4_1.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_cw_1_4_1.degree = 1;
			wheel_profiles.gwa_cw_1_4_1.execution_time = 7;

			/* GWA_C_CW_1_1_4 */
			wheel_profiles.gwa_c_cw_1_1_4.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_c_cw_1_1_4.degree = -1;
			wheel_profiles.gwa_c_cw_1_1_4.execution_time = 7;

			/* GWA_C_CW_1_4_3 */
			wheel_profiles.gwa_c_cw_1_4_3.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_c_cw_1_4_3.degree = -1;
			wheel_profiles.gwa_c_cw_1_4_3.execution_time = 7;

			/* GWA_C_CW_1_3_2 */
			wheel_profiles.gwa_c_cw_1_3_2.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_c_cw_1_3_2.degree = -1;
			wheel_profiles.gwa_c_cw_1_3_2.execution_time = 7;

			/* GWA_C_CW_1_2_1 */
			wheel_profiles.gwa_c_cw_1_2_1.address = WPF_BASE_RAM_ADDRESS
					+ offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_c_cw_1_2_1.degree = -1;
			wheel_profiles.gwa_c_cw_1_2_1.execution_time = 7;
		}

		/* GWA_FOUR_DEGREE */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 13, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			/* GWA_CW_4 */
			wheel_profiles.gwa_cw_4.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_cw_4.degree = 4;
			wheel_profiles.gwa_cw_4.execution_time = 10;

			/* GWA_C_CW_4 */
			wheel_profiles.gwa_c_cw_4.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_c_cw_4.degree = -4;
			wheel_profiles.gwa_c_cw_4.execution_time = 10;
		}

		/* GWA_TO_ZERO */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 14, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.gwa_to_zero.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_to_zero.degree = -56;
			wheel_profiles.gwa_to_zero.execution_time = 15;
		}

		/* GWA_TO_START */
		ret = asw_dump_EEPROM_file(fdl_deploy_address,
				(WPF_BASE_RAM_ADDRESS + offset), 15, fwa_gwa_single_error,
				fwa_gwa_double_error);
		if (ret == 0) {
			wheel_profiles.gwa_to_start.address = WPF_BASE_RAM_ADDRESS + offset;
			offset = offset
					+ ((((*(uint32_t*) (WPF_BASE_RAM_ADDRESS + offset))
							& 0xFFFF0000U) >> 16) * 4) + 4;
			wheel_profiles.gwa_to_start.degree = 64;
			wheel_profiles.gwa_to_start.execution_time = 20;
		}
	}
	return;
}

uint32_t asw_dump_EEPROM_file(uint32_t eeprom_address[], uint32_t ram_address,
		uint8_t bit_shift, uint16_t *single_error, uint16_t *double_error) {
	uint32_t ret = 0;

	ret = icu_fm_deploy_OBF_in_SRAM(eeprom_address[bit_shift], ram_address);
	if (ret != 0) {
		if (bit_shift < GWA_CW_72) {
			*single_error = *single_error
					| ((uint16_t) 1U << (uint16_t) bit_shift);
			ret = icu_fm_deploy_OBF_in_SRAM(eeprom_address[bit_shift + 16],
					ram_address);
			if (ret != 0) {
				*double_error = *double_error
						| ((uint16_t) 1U << (uint16_t) bit_shift);
			}
		}
	}

	return (ret);
}

void asw_deploy_FDL(uint32_t profile_addresses[], uint8_t *errors) {
	uint32_t ret = 0;
	uint16_t crc = 0;
	uint16_t crc_calculate = 0;
	uint8_t fdl_header[4] = { 0 };
	uint32_t fail_address = 0;
	uint8_t eeprom_address[MAX_PROFILES_NUMBER * 4] = { 0 };
	uint8_t i = 0, num_profile = 0;
	/*
	 * Read FDL_1 first 4 bytes
	 * Calculate number of profiles and crc
	 * Then read the number of bytes required (4 * number_of_profiles), calculate crc and evaluate it
	 */

	ret = icu_eeprom_read_bytes(FDL_1_ADDRESS, 4, fdl_header, &fail_address);
	num_profile = ((uint16_t) fdl_header[0] << 8U) | (uint16_t) fdl_header[1];
	crc = ((uint16_t) fdl_header[2] << 8U) | (uint16_t) fdl_header[3];
	ret = icu_eeprom_read_bytes(FDL_1_ADDRESS + 4, (num_profile * 4),
			eeprom_address, &fail_address);
	crc_calculate = asw_create_checksum(eeprom_address, num_profile * 4);

	/* In case of error Deploy FDL 2 */

	if (crc != crc_calculate) {
		*errors |= 1U;
		ret = icu_eeprom_read_bytes(FDL_2_ADDRESS, 4, fdl_header,
				&fail_address);
		num_profile = ((uint16_t) fdl_header[0] << 8U)
				| (uint16_t) fdl_header[1];
		crc = ((uint16_t) fdl_header[2] << 8U) | (uint16_t) fdl_header[3];
		ret = icu_eeprom_read_bytes(FDL_2_ADDRESS + 4, (num_profile * 4),
				eeprom_address, &fail_address);
		crc_calculate = asw_create_checksum(eeprom_address, num_profile * 4);
		if (crc != crc_calculate) {
			*errors |= 2U;
		}

	}

	if (*errors < 3) {
		for (i = 0; i < num_profile; i++) {
			profile_addresses[i] = (uint32_t) ((uint32_t) eeprom_address[i * 4]
					<< 24U) | ((uint32_t) eeprom_address[(i * 4) + 1] << 16U)
					| ((uint32_t) eeprom_address[(i * 4) + 2] << 8U)
					| (uint32_t) eeprom_address[(i * 4) + 3];
		}
	}

	return;
}

rtems_task asw_task_wheel_exec(rtems_task_argument argument) {
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint8_t motor_id = 0;
	uint32_t ret = 0;
	uint8_t parameters[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t profile_idx = 0;
	uint8_t errors = 0;

	motor_id = wheel_tc.TC_pkt_data[4];

	/*Execution of a profile */
	if ((wheel_flag & WP_IN_EXECUTION) > 0) {
		/*
		 * In case of a profile execution there is a variable timeout, it is enabled in asw_PUS_8_1_8_1_execute_wp function only in this case
		 */
		profile_idx = wheel_tc.TC_pkt_data[5];

		ret = asw_PUS_8_1_8_1_execute_wp(motor_id, profile_idx, &errors);
		if (ret == 0) {
			parameters[0] = motor_id;
			parameters[1] = START_MOVEMENT;

			icu_status = asw_PUS_5_TM_report(EVENT_WHEEL_NOTIFICATION, 8,
					parameters);
		}

		parameters[1] = WP_IN_EXECUTION;
	}
	/* Home sensor search */
	else if ((wheel_flag & HS_SEARCH) > 0) {
		/* Set Timeout to be delated atr the end of the asw_task_wheel_exec */
		rtems_status = rtems_timer_fire_after(wheel_timer_id, 1800000,
				asw_wheels_start_movement, (void *) &wheel_tc);

		ret = asw_PUS_8_1_8_6_hs_search(motor_id, &errors);
		parameters[1] = HS_SEARCH;
	}
	/* Home sensor check */
	else if ((wheel_flag & HS_CHECK) > 0) {
		/* Set Timeout to be delated atr the end of the asw_task_wheel_exec */
		rtems_status = rtems_timer_fire_after(wheel_timer_id, 120000,
				asw_wheels_start_movement, (void *) &wheel_tc);

		ret = asw_PUS_8_1_8_8_hs_check(motor_id, &errors);
		parameters[1] = HS_CHECK;
	}
	/* Check connection */
	else {
		/* Set Timeout to be delated atr the end of the asw_task_wheel_exec */
		rtems_status = rtems_timer_fire_after(wheel_timer_id, 3000,
				asw_wheels_start_movement, (void *) &wheel_tc);

		ret = asw_PUS_8_1_8_9_conn_check(motor_id);
		parameters[1] = CHECK_CONNECTION;
		parameters[6] = ENERGIZE_PH;
	}

	/* In case of profile execution must wait the end of the movement to deenergize */
	if (((wheel_flag & WP_IN_EXECUTION) > 0) && (ret == 0)) {

		ret = asw_check_wheels_end_movement(motor_id);

		if (ret == 0) {
			/* Assign no zero value to ret to signal an error during execution*/
			parameters[0] = motor_id;
			parameters[1] = END_MOVEMENT;
			icu_status = asw_PUS_5_TM_report(EVENT_WHEEL_NOTIFICATION, 8,
					parameters);
		}

	}

	if (ret == 0) {
		if ((wheel_tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			icu_status = asw_PUS_1_7_TM_TC_exec_ok(wheel_tc.pkt_id.all_bits,
					wheel_tc.pkt_seq.all_bits, wheel_tc.data_field.sourceId);
		}
	} else {
		/* Param cmd_phase already assigned in switch structure*/
		parameters[0] = motor_id;
		parameters[1] = wheel_flag;
		memcpy(&parameters[2], &ret, sizeof(uint32_t));
		parameters[6] = errors;
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(wheel_tc.pkt_id.all_bits,
				wheel_tc.pkt_seq.all_bits, wheel_tc.data_field.sourceId,
				TM_1_8_EXEC_ERROR, 7, parameters);
		icu_status = asw_PUS_5_TM_report(EVENT_WHEELS_MOVEMENT_ERROR, 7,
				parameters);
	}

	/* Deenergize motor */
	ret = icu_fwa_gwa_drv_deenergize();

	/* Force stop driver */
	icu_fwa_gwa_drv_stop();

	if (ret != 0) {
		parameters[0] = motor_id;
		parameters[1] = wheel_flag;
		memcpy(&parameters[2], &ret, sizeof(uint32_t));
		parameters[6] = DEENERGIZE_PH;
		icu_status = asw_PUS_5_TM_report(EVENT_WHEELS_MOVEMENT_ERROR, 7,
				parameters);
	}

	/* Reset wheel movement flag */
	wheel_flag = 0;

	/* Stop profile timer */
	rtems_status = rtems_timer_cancel(wheel_timer_id);

	rtems_status = rtems_task_suspend(task_wp_exec_id);
}

uint32_t asw_PUS_8_1_8_1_execute_wp(uint8_t motor_id, uint8_t profile_idx,
		uint8_t *errors) {
	uint32_t ret = 0;
	int16_t tmp = 0;
	wheel_profile_t wheel_pf = { 0, 0, 0 };
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t parID_pos = 0;
	uint16_t par_index_pos = 0;
	uint16_t parID_act = 0;
	uint16_t par_index_act = 0;

	wheel_pf = asw_select_profile(profile_idx);

	/* Set Timeout to be delated atr the end of the asw_task_wheel_exec */
	if ((wheel_flag & WP_IN_EXECUTION) > 0) {
		rtems_status = rtems_timer_fire_after(wheel_timer_id,
				wheel_pf.execution_time * 2000, asw_wheels_start_movement,
				(void *) &wheel_tc);
	}

	ret = icu_fwa_gwa_drv_energize((uint32_t) motor_id,
			(int8_t) (((*(uint32_t *) wheel_pf.address) & 0xFF00U) >> 8U),
			(int8_t) ((*(uint32_t *) wheel_pf.address) & 0xFFU));

	if (ret != 0) {
		*errors = ENERGIZE_PH;
	} else {
		rtems_status = rtems_task_wake_after(15);
		ret = icu_fwa_gwa_drv_execute_profile(
				(uint16_t) (((*(uint32_t *) wheel_pf.address) & 0xFFFF0000U)
						>> 16U),
				(icu_fwa_gwa_microstep_t*) (wheel_pf.address + 4));
		if (ret != 0) {
			*errors = PROFILE_EXEC_PH;
		} else {
			if (profile_idx < GWA_CW_72) {
				parID_pos = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET)
						| 1U;
				par_index_pos = asw_PUS_3_ID_to_not_mon_DB_idx(parID_pos);
				parID_act = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET)
						| 2U;
				par_index_act = asw_PUS_3_ID_to_not_mon_DB_idx(parID_act);
			} else {
				parID_pos = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET)
						| 1U;
				par_index_pos = asw_PUS_3_ID_to_not_mon_DB_idx(parID_pos);
				parID_act = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
						| ((uint16_t) PARAM_NOT_MONITORED
								<< PARAM_MONIT_FLAG_OFFSET)
						| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
						| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET)
						| 2U;
				par_index_act = asw_PUS_3_ID_to_not_mon_DB_idx(parID_act);
			}

			/* Update DB values */
			icu_sem_lock(not_mon_db_sem_Id, 5);
			tmp = (int16_t) db_param[par_index_pos].current_value
					+ wheel_pf.degree;
			db_param[par_index_pos].current_value = (360 + tmp) % 360;
			db_param[par_index_act].current_value = 1;
			icu_sem_unlock(not_mon_db_sem_Id);
		}

	}

	return (ret);
}

uint32_t asw_PUS_8_1_8_6_hs_search(uint8_t motor_id, uint8_t *errors) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t ret = 0;
	uint16_t i = 0, j = 0;
	bool_t hs_A = FALSE, hs_B = FALSE, new_hs_A = FALSE, new_hs_B = FALSE,
			hs_right_found = FALSE, hs_left_found = FALSE;
	uint8_t threshold = 0;
	uint8_t hs_errors = 0;
	uint16_t start_offset = 0;
	uint16_t hs_right = 0;
	uint16_t hs_left = 0;
	uint8_t pf_idx_offset = 0;
	uint8_t parameters[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint16_t hs_zero_pos = 0;
	int16_t dist = 0;
	uint16_t parID = 0;
	uint16_t par_index = 0;

	/* Get threshold value */
	threshold = wheel_tc.TC_pkt_data[5];

	/* 0 FWA, 1 GWA */
	if (motor_id == 0) {
		pf_idx_offset = 0;
	} else {
		pf_idx_offset = GWA_CW_72;
	}

	/* Generate event start procedure */
	parameters[0] = motor_id;
	parameters[1] = HS_SEARCH | START_MOVEMENT;
	status = asw_PUS_5_TM_report(EVENT_WHEEL_NOTIFICATION, 8, parameters);

	/* Read home sensor */
	ret = icu_fwa_gwa_drv_hs_state_read((uint32_t) motor_id, threshold,
			&new_hs_A, &new_hs_B);

	if (ret == 0) {
		/* First read of hs values */
		hs_A = new_hs_A;
		hs_B = new_hs_B;

		/* Cycling for max 370 times checking for transitions and errors */
		while (((i < 368) && (hs_errors == 0)
				&& (((hs_right > 0) && (hs_left > 0)) == FALSE))
		== TRUE) {

			/* Execute movement */
			ret = asw_PUS_8_1_8_1_execute_wp(motor_id,
					(FWA_CW_1_1_2 + pf_idx_offset + (i % 4)), &hs_errors);

			/* Continue only in case of no error */
			if (hs_errors == 0) {

				/* Wait end of movement */
				ret = asw_check_wheels_end_movement(motor_id);

				/* Continue only in case of no error */
				if (ret == 0) {

					/* Deenergize motor */
					ret = icu_fwa_gwa_drv_deenergize();
					icu_fwa_gwa_drv_stop();

					/* Continue only in case of no error */
					if (ret == 0) {

						/* Wait 2 second before read hs */
						rtems_status = rtems_task_wake_after(2000);

						/* Read hs */
						ret = icu_fwa_gwa_drv_hs_state_read((uint32_t) motor_id,
								threshold, &new_hs_A, &new_hs_B);
						/* Continue only in case of no error */
						if (ret == 0) {

							/* check transition */
							if (((hs_A ^ new_hs_A) == 1)
									&& ((hs_B ^ new_hs_B) == 0)
									&& (hs_left_found == FALSE)) {
								hs_left = i;
								hs_left_found = TRUE;
							}

							if (((hs_A ^ new_hs_A) == 0)
									&& ((hs_B ^ new_hs_B) == 1)
									&& (hs_right_found == FALSE)) {
								hs_right = i;
								hs_right_found = TRUE;
							}

							hs_A = new_hs_A;
							hs_B = new_hs_B;

						} else {
							hs_errors = HS_READ_PH;
						}

					} else {
						hs_errors = DEENERGIZE_PH;
					}
				} else {
					hs_errors = PROFILE_EXEC_PH;
				}
			} else {
				hs_errors = PROFILE_EXEC_PH;
			}

			i++;
		} // end while

	} else {
		hs_errors = HS_READ_PH;
	}

	/* Continue only in case of no error during execution */
	if (((hs_left_found == TRUE)&& (hs_right_found == TRUE) && (hs_errors == 0))== TRUE) {
		/*
		 * Calculate the zero position and the distance between current and zero position. floor() not used.
		 * starting movement from a Step1...index 0 means that transition is detected at Step2
		 */
		/* Set hs_search_value */
		hs_center_search = ((hs_right + hs_left) % 360) / 2;

		switch (hs_center_search % 4) {
			case 0:
			hs_zero_pos = hs_center_search + 3;
			break;
			case 1:
			hs_zero_pos = hs_center_search + 2;
			break;
			case 2:
			hs_zero_pos = hs_center_search + 1;
			break;
			case 3:
			hs_zero_pos = hs_center_search + 4;
			break;
			default:
			break;
		}

		/* Calculate distance between current and zero position and number of 4 CCW profile to be executed */
		dist = (int16_t) (i - hs_zero_pos);

		/* Calculate offset to be applied to one step movements */
		start_offset = (i + 1) % 4;

		/* Move wheel to zero position only in case distance is different from zero*/
		if (dist > 0) {

			while (((j != dist) && (hs_errors == 0)) == TRUE) {
				/* Execute movement */
				ret = asw_PUS_8_1_8_1_execute_wp(motor_id, (FWA_CW_1_1_2 + pf_idx_offset + ((j + start_offset) % 4)), &hs_errors);
				if (hs_errors == 0) {
					/* Wait end of movement */
					ret = asw_check_wheels_end_movement(motor_id);
					if (ret == 0) {
						/* Deenergize motor */
						ret = icu_fwa_gwa_drv_deenergize();
						icu_fwa_gwa_drv_stop();
						/* Check errors during deenergize */
						if (ret != 0) {
							hs_errors = DEENERGIZE_PH;
						}
					} else {
						hs_errors = PROFILE_EXEC_PH;
					}
				}
				j++;
			}
		} else {
			if (dist < 0) {
				/* Change sign */
				dist = -dist;

				while (((j != dist) && (hs_errors == 0)) == TRUE) {
					/* Check profile to be executed */
					switch ((start_offset + j) % 4) {
						case 0:
						ret = asw_PUS_8_1_8_1_execute_wp(motor_id, (FWA_C_CW_1_1_4 + pf_idx_offset), &hs_errors);
						break;
						case 1:
						ret = asw_PUS_8_1_8_1_execute_wp(motor_id, (FWA_C_CW_1_4_3 + pf_idx_offset), &hs_errors);
						break;
						case 2:
						ret = asw_PUS_8_1_8_1_execute_wp(motor_id, (FWA_C_CW_1_3_2 + pf_idx_offset), &hs_errors);
						break;
						case 3:
						ret = asw_PUS_8_1_8_1_execute_wp(motor_id, (FWA_C_CW_1_2_1 + pf_idx_offset), &hs_errors);
						break;
						default:
						break;
					}

					if (hs_errors == 0) {
						/* Wait end of movement */
						ret = asw_check_wheels_end_movement(motor_id);
						if (ret == 0) {
							/* Deenergize motor */
							ret = icu_fwa_gwa_drv_deenergize();
							icu_fwa_gwa_drv_stop();
							/* Check errors during deenergize */
							if (ret != 0) {
								hs_errors = DEENERGIZE_PH;
							}
						} else {
							hs_errors = PROFILE_EXEC_PH;
						}
					}

					j++;
				}
			}
		}

	} else {
		hs_errors = HS_NOT_FOUND;
	}

	if (hs_errors == 0) {
		if (motor_id == 0) {
			parID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET)
							| 1U;
			par_index = asw_PUS_3_ID_to_not_mon_DB_idx(parID);
		} else {
			parID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET)
							| 1U;
			par_index = asw_PUS_3_ID_to_not_mon_DB_idx(parID);
		}

		/* Update DB values */
		icu_sem_lock(not_mon_db_sem_Id, 5);
		db_param[par_index].current_value = 0;
		icu_sem_unlock(not_mon_db_sem_Id);

		/* Transmit notification */
		parameters[0] = motor_id;
		parameters[1] = HS_SEARCH | END_MOVEMENT;
		status = asw_PUS_5_TM_report(EVENT_WHEEL_NOTIFICATION, 8, parameters);
	}

	*errors = hs_errors;

	return (ret);
}

uint32_t asw_PUS_8_1_8_8_hs_check(uint8_t motor_id, uint8_t *errors) {
	icu_status_code status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint32_t ret = 0;
	uint8_t i = 0;
	bool_t hs_A = FALSE, hs_B = FALSE, new_hs_A = FALSE, new_hs_B = FALSE,
			hs_right_found = FALSE, hs_left_found = FALSE;
	uint8_t threshold = 0;
	uint8_t offset = 0;
	uint8_t hs_right = 0;
	uint8_t hs_left = 0;
	uint8_t hs_center_check = 0;
	uint8_t parameters[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t pf_idx_offset = 0;
	uint16_t parID = 0;
	uint16_t par_index = 0;
	uint32_t hs_reads = 0;
	uint8_t hc_errors = 0;

	/* Get threshold value */
	threshold = wheel_tc.TC_pkt_data[5];
	/* 0 FWA, 1 GWA */
	if (motor_id == 0) {
		pf_idx_offset = 0;
	} else {
		pf_idx_offset = GWA_CW_72;
	}
	/* Operations */
	parameters[0] = motor_id;
	parameters[1] = HS_CHECK | START_MOVEMENT;
	status = asw_PUS_5_TM_report(EVENT_WHEEL_NOTIFICATION, 8, parameters);

	/* First read of hs */
	ret = icu_fwa_gwa_drv_hs_state_read((uint32_t) motor_id, threshold,
			&new_hs_A, &new_hs_B);

	if (ret == 0) {

		hs_A = new_hs_A;
		hs_B = new_hs_B;

		/* Execute 16 steps and read hs values */
		while (((i < 16) && (hc_errors == 0)) == TRUE) {
			/* Execute movement */
			ret = asw_PUS_8_1_8_1_execute_wp(motor_id,
					(FWA_CW_1_1_2 + pf_idx_offset + (i % 4)), &hc_errors);

			/* Continue only in case of no error */
			if (hc_errors == 0) {
				/* Wait end of movement */
				ret = asw_check_wheels_end_movement(motor_id);

				/* Continue only in case of no error */
				if (ret == 0) {
					/* Deenergize motor */
					ret = icu_fwa_gwa_drv_deenergize();
					icu_fwa_gwa_drv_stop();
					/* Continue only in case of no error */
					if (ret == 0) {
						/* Wait 2 second before read hs */
						rtems_status = rtems_task_wake_after(2000);

						/* Read hs */
						ret = icu_fwa_gwa_drv_hs_state_read((uint32_t) motor_id,
								threshold, &new_hs_A, &new_hs_B);
						/* Continue only in case of no error */
						if (ret == 0) {
							/*
							 * Update hs_reads variable is updated only when HS is found
							 * Value hs_reads is an I/O parameter and is set to zero in wheel_task
							 */
							if ((new_hs_A == TRUE)&& (i < 16)){
							hs_reads = hs_reads | (new_hs_A << i);
						}
						if ((new_hs_B == TRUE)&& (i < 16)) {
							hs_reads = hs_reads | (new_hs_B << (i + 16));
						}

						/* check transition */
						if (((hs_A ^ new_hs_A) == 1) && ((hs_B ^ new_hs_B) == 0)
								&& (hs_left_found == FALSE)) {
							hs_left = i;
							hs_left_found = TRUE;
						}

						if (((hs_A ^ new_hs_A) == 0) && ((hs_B ^ new_hs_B) == 1)
								&& (hs_right_found == FALSE)) {
							hs_right = i;
							hs_right_found = TRUE;
						}
						hs_A = new_hs_A;
						hs_B = new_hs_B;
					} else {
						hc_errors = HS_READ_PH;
					}
				} else {
					hc_errors = DEENERGIZE_PH;
				}
			} else {
				hc_errors = PROFILE_EXEC_PH;
			}
		}

			i++;
		}
	} else {
		hc_errors = HS_READ_PH;
	}

	/* Home sensor is found generate event and put wheel in zero position */
	if (((hs_left_found == TRUE)&& (hs_right_found == TRUE))== TRUE) {
		i = 0;

		/* Calculate offset */
		hs_center_check = (hs_right + hs_left) / 2;
		if (hs_center_check > hs_center_search) {
			offset = hs_center_check - hs_center_search;
		} else {
			offset = hs_center_search - hs_center_check;
		}

		/* Move to zero position. Possible combination:
		 * hs_center == 15 move (+4 one time);
		 * 12 <= hs _center < 15 no movement;
		 * 8 <= hs_center < 12 move (-4 one time);
		 * 4 <= hs_center < 8 move (-4 two times);
		 * hs_center <= 4 move (-4 three times);
		 */
		i = 0;
		if (hs_center_check == 15) {
			ret = asw_PUS_8_1_8_1_execute_wp(motor_id, FWA_CW_4 + pf_idx_offset, &hc_errors);
			/* Continue only in case of no error */
			if (hc_errors == 0) {
				/* Wait end of movement */
				ret = asw_check_wheels_end_movement(motor_id);
				/* Continue only in case of no error */
				if (ret == 0) {
					/* Deenergize motor */
					ret = icu_fwa_gwa_drv_deenergize();
					icu_fwa_gwa_drv_stop();
					if (ret != 0) {
						hc_errors = DEENERGIZE_PH;
					}
				} else {
					hc_errors = PROFILE_EXEC_PH;
				}
			} else {
				hc_errors = PROFILE_EXEC_PH;
			}
		} else if (((hs_center_check < 12) && (hs_center_check >= 8)) == TRUE) {
			ret = asw_PUS_8_1_8_1_execute_wp(motor_id, FWA_C_CW_4 + pf_idx_offset, &hc_errors);
			/* Continue only in case of no error */
			if (hc_errors == 0) {
				/* Wait end of movement */
				ret = asw_check_wheels_end_movement(motor_id);
				/* Continue only in case of no error */
				if (ret == 0) {
					/* Deenergize motor */
					ret = icu_fwa_gwa_drv_deenergize();
					icu_fwa_gwa_drv_stop();
					if (ret != 0) {
						hc_errors = DEENERGIZE_PH;
					}
				} else {
					hc_errors = PROFILE_EXEC_PH;
				}
			} else {
				hc_errors = PROFILE_EXEC_PH;
			}
		} else if (((hs_center_check < 8) && (hs_center_check >= 4)) == TRUE) {
			while (((hc_errors == 0) && (i < 2)) == TRUE) {
				ret = asw_PUS_8_1_8_1_execute_wp(motor_id, FWA_C_CW_4 + pf_idx_offset, &hc_errors);
				/* Continue only in case of no error */
				if (hc_errors == 0) {
					/* Wait end of movement */
					ret = asw_check_wheels_end_movement(motor_id);
					/* Continue only in case of no error */
					if (ret == 0) {
						/* Deenergize motor */
						ret = icu_fwa_gwa_drv_deenergize();
						icu_fwa_gwa_drv_stop();
						if (ret != 0) {
							hc_errors = DEENERGIZE_PH;
						}
					} else {
						hc_errors = PROFILE_EXEC_PH;
					}
				} else {
					hc_errors = PROFILE_EXEC_PH;
				}
				i++;
			}
		} else {
			if (hs_center_check <= 4) {
				while (((hc_errors == 0) && (i < 3)) == TRUE) {
					ret = asw_PUS_8_1_8_1_execute_wp(motor_id, FWA_C_CW_4 + pf_idx_offset, &hc_errors);
					/* Continue only in case of no error */
					if (hc_errors == 0) {
						/* Wait end of movement */
						ret = asw_check_wheels_end_movement(motor_id);
						/* Continue only in case of no error */
						if (ret == 0) {
							/* Deenergize motor */
							ret = icu_fwa_gwa_drv_deenergize();
							icu_fwa_gwa_drv_stop();
							if (ret != 0) {
								hc_errors = DEENERGIZE_PH;
							}
						} else {
							hc_errors = PROFILE_EXEC_PH;
						}
					} else {
						hc_errors = PROFILE_EXEC_PH;
					}
					i++;
				}
			}
		}

		/* Generate notification offset */
		parameters[0] = motor_id;
		parameters[1] = HS_CHECK | END_MOVEMENT | HS_OFFSET;
		memcpy(&parameters[2], &offset, sizeof(uint16_t));
		memcpy(&parameters[4], &hs_reads, sizeof(uint32_t));
		status = asw_PUS_5_TM_report(EVENT_WHEEL_NOTIFICATION,
		8, parameters);

	} else {
		hc_errors = HS_NOT_FOUND;
	}

	if (hc_errors == 0) {

		if (motor_id == 0) {
			parID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET)
							| 1U;
			par_index = asw_PUS_3_ID_to_not_mon_DB_idx(parID);
		} else {
			parID =
					((uint16_t) SW_ASW << PARAM_SW_OFFSET)
							| ((uint16_t) PARAM_NOT_MONITORED
									<< PARAM_MONIT_FLAG_OFFSET)
							| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
							| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET)
							| 1U;
			par_index = asw_PUS_3_ID_to_not_mon_DB_idx(parID);
		}
		/* Update DB Value */
		icu_sem_lock(not_mon_db_sem_Id, 5);
		db_param[par_index].current_value = 0;
		icu_sem_unlock(not_mon_db_sem_Id);
	}

	/* Copy error */
	*errors = hc_errors;

	return (ret);
}

uint32_t asw_PUS_8_1_8_9_conn_check(uint8_t motor_id) {
	uint32_t ret = 0;

	/* Check Motor and select a first step of a profile */
	if (motor_id == 0) {
		ret = icu_fwa_gwa_drv_energize((uint32_t) motor_id,
				(int8_t) (((*(uint32_t*) wheel_profiles.fwa_cw_72.address)
						& 0xFF00U) >> 8U),
				(int8_t) ((*(uint32_t*) wheel_profiles.fwa_cw_72.address)
						& 0xFFU));
	} else {
		ret = icu_fwa_gwa_drv_energize((uint32_t) motor_id,
				(int8_t) (((*(uint32_t*) wheel_profiles.gwa_cw_72.address)
						& 0xFF00U) >> 8),
				(int8_t) ((*(uint32_t*) wheel_profiles.gwa_cw_72.address)
						& 0xFFU));
	}

	return (ret);
}

icu_status_code asw_PUS_8_1_8_x_move_wheels(telecommand_t tc) {
	icu_status_code icu_status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint8_t profile_idx = 0;
	uint8_t is_delayed = 0;
	uint8_t errors[2] = { 0, 0 };
	uint32_t funct_id = 0;
	uint32_t ret = 0;
	icu_led_drv_status_t led_status;
	uint16_t size_in_bytes = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	memcpy(&funct_id, &tc.TC_pkt_data[0], sizeof(uint32_t));

	/* Acquire led status */
	ret = icu_led_drv_get_status(&led_status);

	/* Check parameters and Subunits status */
	if (tc.TC_pkt_data[4] > 1) {
		/* motor_id parameter out of bounds */
		errors[0] = 1;
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
		icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if ((wheel_flag != 0) || (ret != 0)
			|| ((led_status.status & 0x1U) != 0)) {
		/* wheel movement already activated or Calibration Unit is on */
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_ICU_STATUS_ERROR, 0, NULL );
		icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/* Copy received TC in another structure */
		memcpy(&wheel_tc, &tc, sizeof(telecommand_t));

		switch (funct_id) {
		case 0x80001U: //execute_wp
			profile_idx = tc.TC_pkt_data[5];
			is_delayed = tc.TC_pkt_data[6];

			if (size_in_bytes != 7) {
				icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
				icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
						NULL );
			} else if (is_delayed > 1) {
				/* is_delayed parameter out of bounds */
				errors[0] = 3;
				icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
				icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
						NULL );
			} else if (profile_idx >= MAX_PROFILES_NUMBER) {
				/* Profile index not exists */
				errors[0] = 2;
				icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
				icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
						NULL );
			} else if ((((tc.TC_pkt_data[4] == 0) && (profile_idx >= GWA_CW_72))
					|| ((tc.TC_pkt_data[4] == 1) && (profile_idx < GWA_CW_72)))
					== TRUE) {
				/* Profile index not applicable to the selected wheel */
				errors[0] = 4;
				icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors[0]);
				icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
						NULL );
			} else {
				wheel_flag |= WP_IN_EXECUTION;
			}
			break;
		case 0x80006U: //hs_search
			if (size_in_bytes != 5) {
				icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
				icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
						NULL );
			} else {
				wheel_flag |= HS_SEARCH;
			}
			break;
		case 0x80008U: //hs_check
			if (size_in_bytes != 5) {
				icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
				icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
						NULL );
			} else {
				wheel_flag |= HS_CHECK;
			}
			break;
		case 0x80009U: //wheel_conn_check
			if (size_in_bytes != 5) {
				icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
						tc.pkt_seq.all_bits, tc.data_field.sourceId,
						TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
				icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0,
						NULL );
			} else {
				wheel_flag |= CHECK_CONNECTION;
			}
			break;
		default:
			break;
		}

		/* Execute action only in case a wheel operation was accepted by ASW */
		if (wheel_flag != 0) {
			if (is_delayed == 0) {
				rtems_status = rtems_task_restart(task_wp_exec_id, 0);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					errors[0] = ((1U << 10U) & 0xFF00U) >> 8U;
					errors[1] = (1U << 10U) & 0xFFU;
					icu_status = asw_PUS_5_TM_report(EVENT_TASK_NOT_STARTED, 2,
							errors);
				}
			} else {
				/* Set wheel_flag in order to start a timer at synch w/o dw reception */
				wheel_flag |= WP_TO_BE_STARTED;
			}
		}
	}

	/* Stop profile timer */
	rtems_status = rtems_timer_cancel(wheel_timer_id);

	return (icu_status);
}

uint32_t asw_check_wheels_end_movement(uint8_t motor_id) {
	uint32_t err_status = 0, end_movement_status = 0;
	uint32_t ret = 0;
	uint8_t check_end = 0;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint16_t parID_act = 0;
	uint16_t par_index_act = 0;

	while (check_end == 0) {

		ret = icu_intmngr_fpga_get_module_pending(FPGA_IRQ3_MOTOR_PROFILE_END,
				&end_movement_status);
		ret = icu_intmngr_fpga_get_module_pending(FPGA_IRQ3_MOTOR_ERR,
				&err_status);

		/* Wait in case of no events */
		if (((end_movement_status == 0) && (err_status == 0)) == TRUE) {
			/* Wait 2 seconds */
			rtems_status = rtems_task_wake_after(2000);
		} else {
			/* Exit WHILE cycle and clear isr registers */
			ret = icu_intmngr_fpga_clear_module_pending(
					FPGA_IRQ3_MOTOR_PROFILE_END,
					FPGA_IRQ3_MOTOR_PROFILE_END_CLEAR_ALL);
			ret = icu_intmngr_fpga_clear_module_pending(FPGA_IRQ3_MOTOR_ERR,
					FPGA_IRQ3_MOTOR_ERR_CLEAR_ALL);

			/* Return errors */
			ret = err_status;

			check_end = 1;
		}
	}

	if (motor_id == 0) {
		parID_act = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)

		| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)

		| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET) | 2U;
		par_index_act = asw_PUS_3_ID_to_not_mon_DB_idx(parID_act);
	} else {
		parID_act = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)

		| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
				| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
				| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET) | 2U;
		par_index_act = asw_PUS_3_ID_to_not_mon_DB_idx(parID_act);
	}

	/* Update DB values */
	icu_sem_lock(not_mon_db_sem_Id, 5);
	db_param[par_index_act].current_value = 0;
	icu_sem_unlock(not_mon_db_sem_Id);

	return (ret);
}

wheel_profile_t asw_select_profile(uint8_t profile_idx) {
	wheel_profile_t wheel_pf = { 0, 0, 0 };

	switch (profile_idx) {
	case FWA_CW_72:
		memcpy(&wheel_pf, &wheel_profiles.fwa_cw_72, sizeof(wheel_profile_t));
		break;
	case FWA_C_CW_72:
		memcpy(&wheel_pf, &wheel_profiles.fwa_c_cw_72, sizeof(wheel_profile_t));
		break;
	case FWA_CW_144:
		memcpy(&wheel_pf, &wheel_profiles.fwa_cw_144, sizeof(wheel_profile_t));
		break;
	case FWA_C_CW_144:
		memcpy(&wheel_pf, &wheel_profiles.fwa_c_cw_144,
				sizeof(wheel_profile_t));
		break;
	case FWA_CW_1_1_2:
		memcpy(&wheel_pf, &wheel_profiles.fwa_cw_1_1_2,
				sizeof(wheel_profile_t));
		break;
	case FWA_CW_1_2_3:
		memcpy(&wheel_pf, &wheel_profiles.fwa_cw_1_2_3,
				sizeof(wheel_profile_t));
		break;
	case FWA_CW_1_3_4:
		memcpy(&wheel_pf, &wheel_profiles.fwa_cw_1_3_4,
				sizeof(wheel_profile_t));
		break;
	case FWA_CW_1_4_1:
		memcpy(&wheel_pf, &wheel_profiles.fwa_cw_1_4_1,
				sizeof(wheel_profile_t));
		break;
	case FWA_C_CW_1_1_4:
		memcpy(&wheel_pf, &wheel_profiles.fwa_c_cw_1_1_4,
				sizeof(wheel_profile_t));
		break;
	case FWA_C_CW_1_4_3:
		memcpy(&wheel_pf, &wheel_profiles.fwa_c_cw_1_4_3,
				sizeof(wheel_profile_t));
		break;
	case FWA_C_CW_1_3_2:
		memcpy(&wheel_pf, &wheel_profiles.fwa_c_cw_1_3_2,
				sizeof(wheel_profile_t));
		break;
	case FWA_C_CW_1_2_1:
		memcpy(&wheel_pf, &wheel_profiles.fwa_c_cw_1_2_1,
				sizeof(wheel_profile_t));
		break;
	case FWA_CW_4:
		memcpy(&wheel_pf, &wheel_profiles.fwa_cw_4, sizeof(wheel_profile_t));
		break;
	case FWA_C_CW_4:
		memcpy(&wheel_pf, &wheel_profiles.fwa_c_cw_4, sizeof(wheel_profile_t));
		break;
	case FWA_TO_ZERO:
		memcpy(&wheel_pf, &wheel_profiles.fwa_to_zero, sizeof(wheel_profile_t));
		break;
	case FWA_TO_START:
		memcpy(&wheel_pf, &wheel_profiles.fwa_to_start,
				sizeof(wheel_profile_t));
		break;
	case GWA_CW_72:
		memcpy(&wheel_pf, &wheel_profiles.gwa_cw_72, sizeof(wheel_profile_t));
		break;
	case GWA_C_CW_72:
		memcpy(&wheel_pf, &wheel_profiles.gwa_c_cw_72, sizeof(wheel_profile_t));
		break;
	case GWA_CW_144:
		memcpy(&wheel_pf, &wheel_profiles.gwa_cw_144, sizeof(wheel_profile_t));
		break;
	case GWA_C_CW_144:
		memcpy(&wheel_pf, &wheel_profiles.gwa_c_cw_144,
				sizeof(wheel_profile_t));
		break;
	case GWA_CW_1_1_2:
		memcpy(&wheel_pf, &wheel_profiles.gwa_cw_1_1_2,
				sizeof(wheel_profile_t));
		break;
	case GWA_CW_1_2_3:
		memcpy(&wheel_pf, &wheel_profiles.gwa_cw_1_2_3,
				sizeof(wheel_profile_t));
		break;
	case GWA_CW_1_3_4:
		memcpy(&wheel_pf, &wheel_profiles.gwa_cw_1_3_4,
				sizeof(wheel_profile_t));
		break;
	case GWA_CW_1_4_1:
		memcpy(&wheel_pf, &wheel_profiles.gwa_cw_1_4_1,
				sizeof(wheel_profile_t));
		break;
	case GWA_C_CW_1_1_4:
		memcpy(&wheel_pf, &wheel_profiles.gwa_c_cw_1_1_4,
				sizeof(wheel_profile_t));
		break;
	case GWA_C_CW_1_4_3:
		memcpy(&wheel_pf, &wheel_profiles.gwa_c_cw_1_4_3,
				sizeof(wheel_profile_t));
		break;
	case GWA_C_CW_1_3_2:
		memcpy(&wheel_pf, &wheel_profiles.gwa_c_cw_1_3_2,
				sizeof(wheel_profile_t));
		break;
	case GWA_C_CW_1_2_1:
		memcpy(&wheel_pf, &wheel_profiles.gwa_c_cw_1_2_1,
				sizeof(wheel_profile_t));
		break;
	case GWA_CW_4:
		memcpy(&wheel_pf, &wheel_profiles.gwa_cw_4, sizeof(wheel_profile_t));
		break;
	case GWA_C_CW_4:
		memcpy(&wheel_pf, &wheel_profiles.gwa_c_cw_4, sizeof(wheel_profile_t));
		break;
	case GWA_TO_ZERO:
		memcpy(&wheel_pf, &wheel_profiles.gwa_to_zero, sizeof(wheel_profile_t));
		break;
	case GWA_TO_START:
		memcpy(&wheel_pf, &wheel_profiles.gwa_to_start,
				sizeof(wheel_profile_t));
		break;
	default:
		break;
	}

	return (wheel_pf);
}

void asw_monitor_wheels(void) {
	/* Check motor phase values when a wheel profile is running */
	return;
}
