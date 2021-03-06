/**
 * \file	PUS_Service_9.c
 *
 * \brief	PUS Service 9 management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 * \author  Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Dec 6, 2016
 */

#include "../rtems_configuration.h"

icu_status_code asw_PUS_9_130_dis_time_synch(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	/*
	 * check the parameters size
	 */
	if (size_in_bytes != 0) {
		/* prepare TM(1,8) - TC failure */
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {

		/*
		 * disable the synchronization
		 */
		obt_synch_enable = FALSE;

		/*
		 * everything was ok, generate a TM(1,7)
		 */
		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	}

	return (status);
}

icu_status_code asw_PUS_9_131_en_time_synch(telecommand_t tc) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint16_t size_in_bytes = 0;

	size_in_bytes = (tc.TC_pkt_length - 6) + 1;
	/*
	 * check the parameters size
	 */
	if (size_in_bytes != 0) {
		/* prepare TM(1,8) - TC failure */
		status = asw_PUS_1_8_TM_TC_exec_fail(tc.pkt_id.all_bits,
				tc.pkt_seq.all_bits, tc.data_field.sourceId,
				TM_1_8_TC_PARAM_SIZE_ERROR, 0, NULL );
		status = asw_PUS_5_TM_report(EVENT_CMD_NOT_EXECUTED, 0, NULL );
	} else {
		/*
		 * enable the synchronization
		 */
		obt_synch_enable = TRUE;

		/*
		 * everything was ok, generate a TM(1,7)
		 */
		if ((tc.data_field.ack & PUS_1_7_GEN_FLAG) > 0) {
			status = asw_PUS_1_7_TM_TC_exec_ok(tc.pkt_id.all_bits,
					tc.pkt_seq.all_bits, tc.data_field.sourceId);
		}
	}
	return (status);
}
