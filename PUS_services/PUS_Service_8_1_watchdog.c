/**
 * \file	PUS_Service_8_1_watchdog.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	Apr 04, 2017
 *
 */

#include "../rtems_configuration.h"

bool_t wd_activation = FALSE;

icu_status_code asw_PUS_8_1_2_7_watchdog_en_dis(telecommand_t tc) {
	icu_status_code icu_status = ICU_SUCCESSFUL;
	uint8_t en_dis = 0;
	uint16_t time_to_int = 0;
	uint16_t from_int_to_res = 0;
	uint8_t errors = 0;
	uint16_t size_in_bytes = 0;
	uint32_t ret = 0, rst_mask = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;

	memcpy(&en_dis, &tc.TC_pkt_data[4], sizeof(uint8_t));
	memcpy(&time_to_int, &tc.TC_pkt_data[5], sizeof(uint16_t));
	memcpy(&from_int_to_res, &tc.TC_pkt_data[7], sizeof(uint16_t));

	/* Check parameters */
	if (size_in_bytes != 9) {
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (en_dis > 1) {
		/* en_dis parameter out of bounds */
		errors = 1;
		icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
		icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else if (en_dis == 0) {
		/* Disable watchdog count */
		icu_watchdog_stop_count();

		/* Disable interrupt WD1 */
		ret = icu_intmngr_fpga_set_module_mask(FPGA_IRQ2_WD_T1,
				FPGA_IRQ2_WD_T1_DISABLE_ALL);

		/* Mask WD in the reset mask */
		rst_mask = icu_reset_get_mask();
		ret = icu_reset_set_mask(rst_mask & ~RESET_WATCHDOG_RST_SOURCE);
		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			icu_status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	} else {
		/* Check interval parameter values */
		if (time_to_int < WDG_TO_INT_MIN_INTERVAL) {
			errors = 2;
			icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
			icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else if (from_int_to_res < WDG_TO_RES_MIN_INTERVAL) {
			errors = 3;
			icu_status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId,
					TM_1_8_TC_PARAM_OUT_OF_BOUNDS, 1, &errors);
			icu_status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
		} else {
			/* Set parameters */
			icu_watchdog_set_interval_to_interrupt(time_to_int);
			icu_watchdog_set_interval_from_interrupt_to_reset(from_int_to_res);

			/* Enable interrupt WD1 */
			ret = icu_intmngr_fpga_set_module_mask(FPGA_IRQ2_WD_T1,
					FPGA_IRQ2_WD_T1_ENABLE_ALL);

			/* Unmask WD in the reset mask */
			rst_mask = icu_reset_get_mask();
			ret = icu_reset_set_mask(rst_mask | RESET_WATCHDOG_RST_SOURCE);

			/* Start WD counter at next synch message*/
			wd_activation = TRUE;

			if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
				icu_status = asw_PUS_1_7_TM_TC_exec_ok(
						tc.pkt_id.all_bits, tc.pkt_seq.all_bits,
						tc.data_field.sourceId);
			}
		}
	}

	return (icu_status);
}
