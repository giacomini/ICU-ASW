/**
 * \file	tm.c
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 * \author	Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 * Created: 	5-05-2015
 *
 */
#include "../rtems_configuration.h"

/*
 * global variables initialization
 */

uint16_t TM_ss_counter[APID_NUMBER - ICU_ASW_APID] = { 0 };

uint16_t last_three_das_values[DAS_PARAM_NM - 6][3] = { { 0, 0, 0 } };

/**********************************************************************************
 * ANCILLARY FUNCTIONS
 **********************************************************************************/
/*
 * function for serialization of TM_packet_t structure
 * at this level, still missing the packet_error_control dataword
 */
void asw_serialize_TM(TM_packet_t *TM_pack, uint8_t *buffer, uint16_t length) {
	uint32_t coarseTime = 0, fineTime = 0;
	bool_t alreadyRead;
	uint16_t APID = (TM_pack->TM_packet_ID) & 0x07FFU;
	/*
	 * grouping flag: shall be
	 * 		11 - stand-alone packet
	 * 		01 - first packet of a group
	 * 		00 - continuation packet
	 * 		10 - last packet of a group
	 */
	uint16_t grouping_flag = 3;
	/* to keep record of the writing position in 'buffer'*/
	uint16_t pos = 0, i = 0;

	icu_obt_latch_obt();
	icu_obt_read_latched_obt(&coarseTime, &fineTime, &alreadyRead);
	TM_pack->TM_OBT_fine_time = (uint16_t) ((fineTime & 0xFFFF0000) >> 16);

	/* Assign TM packet general counter */
	TM_pack->TM_packet_seq_control = (grouping_flag << TM_GROUPING_FLAG_OFFSET)
			| (TM_ss_counter[APID - ICU_ASW_APID] << TM_SSCOUNT_OFFSET);

	/* increase the TM packet general counter */
	icu_sem_lock(TM_cnt_sem_Id, 5);
	TM_ss_counter[APID - ICU_ASW_APID] =
			(TM_ss_counter[APID - ICU_ASW_APID] + 1) % TM_MAX_SEQ_COUNTER;
	icu_sem_unlock(TM_cnt_sem_Id);

	// packet header
	memcpy(&buffer[pos], &(TM_pack->TM_packet_ID),
			sizeof(TM_pack->TM_packet_ID));
	pos += (uint16_t) sizeof(TM_pack->TM_packet_ID);

	memcpy(&buffer[pos], &(TM_pack->TM_packet_seq_control),
			sizeof(TM_pack->TM_packet_seq_control));
	pos += (uint16_t) sizeof(TM_pack->TM_packet_seq_control);

	memcpy(&buffer[pos], &(TM_pack->TM_packet_length),
			sizeof(TM_pack->TM_packet_length));
	pos += (uint16_t) sizeof(TM_pack->TM_packet_length);

	// data source
	memcpy(&buffer[pos], &(TM_pack->TM_PUS_version),
			sizeof(TM_pack->TM_PUS_version));
	pos += (uint16_t) sizeof(TM_pack->TM_PUS_version);

	memcpy(&buffer[pos], &(TM_pack->TM_service_type),
			sizeof(TM_pack->TM_service_type));
	pos += (uint16_t) sizeof(TM_pack->TM_service_type);

	memcpy(&buffer[pos], &(TM_pack->TM_service_subtype),
			sizeof(TM_pack->TM_service_subtype));
	pos += (uint16_t) sizeof(TM_pack->TM_service_subtype);

	memcpy(&buffer[pos], &(TM_pack->TM_destination_ID),
			sizeof(TM_pack->TM_destination_ID));
	pos += (uint16_t) sizeof(TM_pack->TM_destination_ID);

	memcpy(&buffer[pos], &coarseTime, sizeof(uint32_t));
	pos += (uint16_t) sizeof(uint32_t);

	memcpy(&buffer[pos], &(TM_pack->TM_OBT_fine_time), sizeof(uint16_t));
	pos += (uint16_t) sizeof(uint16_t);

	/* data */
	for (i = 0; i < length; i++) {
		memcpy(&buffer[pos], &(TM_pack->TM_data_source[0]) + i,
				sizeof(uint16_t));
		pos += (uint16_t) sizeof(uint16_t);
	}

	return;
}

void asw_PUS_TM_set_data_header(TM_packet_t *TM_pack, uint8_t type,
		uint8_t sub_type) {

	TM_pack->TM_PUS_version = 16; /* fixed: 00010000 */
	TM_pack->TM_service_type = type;
	TM_pack->TM_service_subtype = sub_type;
	/* counters are global variables, passed as argument to avoid switch on sub-type */
	TM_pack->TM_destination_ID = 1; //TBC

	return;
}

void asw_from_das_IDs_to_ASW_Idxs(uint32_t das_ids[DAS_PARAM_NM],
		uint8_t icu_db_idx[DAS_PARAM_NM], uint8_t nr_par) {
	uint8_t i = 0;
	uint16_t par_ID = 0;

	const uint16_t parID_asw_icu_icu_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_gwa_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_GWA << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_fwa_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_FWA << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_cu_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_CU << PARAM_SUB_SYSTEM_OFFSET) | 0U;
	const uint16_t parID_asw_icu_tc_monit = ((uint16_t) SW_ASW
			<< PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_TC << PARAM_SUB_SYSTEM_OFFSET) | 0U;

	for (i = 0; i < nr_par; i++) {
		switch (das_ids[i]) {
		/* parameters in TC (thermal control) DB section */
		case NI_SA_TMP_1_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 1);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_2_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 2);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_3_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 3);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_4_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 4);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_5_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 5);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_6_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 6);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CSS_TMP_1_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 7);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CSS_TMP_2_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 8);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SSS_TMP_1_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 9);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SSS_TMP_2_CRYO:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 10);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_FWA_TMP_CRYO: // Also case NI_FWA_TMP_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 11);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_GWA_TMP_CRYO: // Also case NI_GWA_TMP_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 12);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_1_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 13);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_2_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 14);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_3_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 15);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_4_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 16);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_5_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 17);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SA_TMP_6_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 18);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CSS_TMP_1_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 19);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CSS_TMP_2_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 20);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SSS_TMP_1_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 21);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_SSS_TMP_2_FR:
			par_ID = parID_asw_icu_tc_monit | ((uint16_t) 22);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case FWA_MOT_PHA_I:
			par_ID = parID_asw_icu_fwa_monit | ((uint16_t) 1);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case FWA_MOT_PHB_I:
			par_ID = parID_asw_icu_fwa_monit | ((uint16_t) 2);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case GWA_MOT_PHA_I:
			par_ID = parID_asw_icu_gwa_monit | ((uint16_t) 1);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case GWA_MOT_PHB_I:
			par_ID = parID_asw_icu_gwa_monit | ((uint16_t) 2);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CU_LED_V_1:
			par_ID = parID_asw_icu_cu_monit | ((uint16_t) 1);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CU_LED_V_2:
			par_ID = parID_asw_icu_cu_monit | ((uint16_t) 2);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CU_LED_V_3:
			par_ID = parID_asw_icu_cu_monit | ((uint16_t) 3);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CU_LED_V_4:
			par_ID = parID_asw_icu_cu_monit | ((uint16_t) 4);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case NI_CU_LED_V_5:
			par_ID = parID_asw_icu_cu_monit | ((uint16_t) 5);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case ICU_5V:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 3);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case ICU_5V_CURRENT:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 4);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case ICU_CDPU_TMP:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 1);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case ICU_LVPS_TMP:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 2);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case ASM_TSM1_GAIN:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 5);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case ASM_TSM1_OFFSET:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 6);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case SSS_CHAIN_CRYO_OFFSET:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 7);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case SSS_CRYO_CHAIN_GAIN:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 8);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case CSS_CRYO_CHAIN_OFFSET:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 9);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case CSS_CRYO_CHAIN_GAIN:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 10);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case CMD_DAS_ALARMS_READ_REQ:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 11);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case CMD_MOT_HS_READ_REQ:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 12);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
			/*
			 * FIXME: 32-bit parameters!
			 */
		case OBT_COARSE_HIGH:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 13);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case OBT_COARSE_LOW:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 13);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case OBT_FINE_HIGH:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 14);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		case OBT_FINE_LOW:
			par_ID = parID_asw_icu_icu_monit | ((uint16_t) 14);
			icu_db_idx[i] = asw_PUS_3_ID_to_mon_DB_idx(par_ID);
			break;
		default:
			break;
		}
	}

	while (i < DAS_PARAM_NM) {
		icu_db_idx[i] = 255;
		i++;
	}
}

void asw_from_ASW_IDs_to_das_IDs(uint16_t icu_ids[DAS_PARAM_NM],
		uint32_t das_ids[DAS_PARAM_NM], uint8_t nr_par) {
	uint8_t i = 0;
	uint8_t j = 0;
	uint16_t index = 0;
	uint16_t obt_fine_id = 0;
	uint16_t obt_coarse_id = 0;

	obt_coarse_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 13U;
	obt_fine_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 14U;
	/*
	 * nr_par <= DAS_PARAM_NM due to size!
	 */
	for (i = 0; i < DAS_PARAM_NM; i++) {
		das_ids[i] = 255;
	}

	/*
	 * NOTE: OBT fine/coarse HIGH/LOW bytes from DAS are linked to a single par_ID
	 */
	icu_sem_lock(monit_db_sem_Id, 5);
	j = 0;
	for (i = 0; i < nr_par; i++) {
		if (icu_ids[i] == obt_coarse_id) {
			das_ids[j] = OBT_COARSE_HIGH;
			das_ids[j + 1] = OBT_COARSE_LOW;
			j += 2;
		} else if (icu_ids[i] == obt_fine_id) {
			das_ids[j] = OBT_FINE_HIGH;
			das_ids[j + 1] = OBT_FINE_LOW;
			j += 2;
		} else {
			index = asw_PUS_3_ID_to_mon_DB_idx(icu_ids[i]);
			das_ids[j] = monitored_db_param[index].DAS_idx;
			j++;
		}
	}
	icu_sem_unlock(monit_db_sem_Id);

	return;
}

void asw_copy_data_in_das_db(uint8_t acq_par_ids[DAS_PARAM_NM],
		uint16_t values[DAS_PARAM_NM], uint8_t nr_par, uint8_t cycle_number) {
	uint16_t cpu_load_par_idx = 0;
	uint8_t i = 0;
	uint8_t mask = 0;
	uint8_t n_fa = 0;
	uint16_t n_par = 0;
	uint16_t SID = 0;
	uint16_t par_index = 0;
	uint16_t par_pos = 0;
	uint16_t starting_pos = 0;
	uint8_t k = 0;
	uint8_t fa_check = 1;
	uint8_t fa_mask = 0;
	uint8_t pos_found = 0;
	uint8_t das_posit = 0;

	uint16_t parID = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_NOT_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 19U;

	cpu_load_par_idx = asw_PUS_3_ID_to_not_mon_DB_idx(parID);

	/* Copy CPU load */
	icu_sem_lock(not_mon_db_sem_Id, 5);
	db_param[cpu_load_par_idx].current_value = cpu_load;
	icu_sem_unlock(not_mon_db_sem_Id);

	/*
	 * check if there are diagnostic packets enabled
	 */
	if (TM_DIAG_report_flag[0] > 0) {

		mask = 1U << TM_DIAG_FLAG_ICU_PAR_OFFSET;
		for (i = 0; i < TM_DIAG_MAX_SIMULT_REPORT_NUMB; i++) {
			if (((uint8_t) (diag_sampling_space[i][2] & mask)
					>> TM_DIAG_FLAG_ICU_PAR_OFFSET) == 0) {
				/* SID contains ICU parameters */
				SID = (uint16_t) ((uint16_t) diag_sampling_space[i][0] << 8U)
						| (uint16_t) diag_sampling_space[i][1];
				if (asw_PUS_3_check_DIAG_rep_stat(SID) == 1) {
					if ((diag_sampling_space[i][2] & 1U) == 0) {
						/*
						 * SID is enabled, but the storage is not ongoing. Check if the DIAG packet has been
						 * created, if yes start again the collection
						 */
						if ((diag_sampling_space[i][2] & (1U << 1U)) > 0) {
							diag_sampling_space[i][2] |= 1U;
							diag_sampling_space[i][2] &= ~(1U << 1U);
							for (n_fa = 0;
									n_fa
											< (uint8_t) diag_SID[0][SID - 1].fixed_array_number;
									n_fa++) {
								diag_sampling_space[i][2] &=
										~((uint8_t) 1U
												<< (uint8_t) (TM_DIAG_FLAG_FA1_COMPL_OFFSET
														+ n_fa));
							}
						}
					}

					if ((diag_sampling_space[i][2] & 1U) == 1) {
						/* SID is enabled and the data storage is ongoing */
						for (n_fa = 0;
								n_fa
										< (uint8_t) diag_SID[0][SID - 1].fixed_array_number;
								n_fa++) {
							/* check if this fa is complete or not */
							fa_mask = 1U
									<< (TM_DIAG_FLAG_FA1_COMPL_OFFSET + n_fa);
							if ((diag_sampling_space[i][2] & fa_mask) == 0) {
								/* position where we write the current DAS value */
								par_pos =
										(uint16_t) ((uint16_t) diag_sampling_space[i][7
												+ (n_fa * 4)] << 8U)
												| (uint16_t) diag_sampling_space[i][8
														+ (n_fa * 4)];
								for (n_par = 0;
										n_par
												< diag_SID[0][SID - 1].os_param_number[n_fa];
										n_par++) {
									/*retrieve parameters description */
									/*
									 * check monitored table?! FIXME
									 */
									/*
									 * find the relation between parameter index in diag_SID and values in DAS
									 * monitored_db_param[acq_par_ids[i]].current_value = values[i]
									 */
									par_index =
											diag_SID[0][SID - 1].DIAG_data[n_fa][n_par].DB_index;
									/*
									 * retrieve DAS idx
									 */
									k = 0;
									pos_found = 0;
									while ((k < DAS_PARAM_NM)
											&& (pos_found == 0)) {
										if (acq_par_ids[k]
												== (uint8_t) par_index) {
											das_posit = k;
											pos_found = 1;
										}
										k++;
									}
									memcpy(&(diag_sampling_space[i][par_pos]),
											&(values[das_posit]),
											sizeof(uint16_t));
									par_pos += 2;
								}
								/* update position in storage area */
								memcpy(
										&(diag_sampling_space[i][7 + (n_fa * 4)]),
										&par_pos, sizeof(uint16_t));
								/*
								 * check if collection of this fa is finished; if yes, set the flag to zero
								 */
								starting_pos =
										(uint16_t) ((uint16_t) diag_sampling_space[i][5
												+ (n_fa * 4)] << 8U)
												| (uint16_t) diag_sampling_space[i][6
														+ (n_fa * 4)];
								if (((par_pos - starting_pos) / 2)
										>= (diag_SID[0][SID - 1].os_repetition_number[n_fa]
												* diag_SID[0][SID - 1].os_param_number[n_fa])) {
									/* collection is finished */
									diag_sampling_space[i][2] =
											diag_sampling_space[i][2]
													| ((uint8_t) 1U
															<< (uint8_t) (TM_DIAG_FLAG_FA1_COMPL_OFFSET
																	+ n_fa));
									diag_sampling_space[i][7 + (n_fa * 4)] =
											diag_sampling_space[i][5
													+ (n_fa * 4)];
									diag_sampling_space[i][8 + (n_fa * 4)] =
											diag_sampling_space[i][6
													+ (n_fa * 4)];
								}
							}
							if (((n_fa < 4)
									&& ((TM_DIAG_FLAG_FA1_COMPL_OFFSET + n_fa)
											>= 0)
									&& ((TM_DIAG_FLAG_FA1_COMPL_OFFSET + n_fa)
											< (8
													* sizeof(diag_sampling_space[i][2]))))
									== TRUE) {
								fa_check =
										fa_check
												* (diag_sampling_space[i][2]
														>> (TM_DIAG_FLAG_FA1_COMPL_OFFSET
																+ n_fa));
							}
						}
						/*
						 * check if all FA have finished the collection
						 */
						if (fa_check == 1) {
							diag_sampling_space[i][2] &= ~1U;
						}
					} /* end if collection ongoing */

				} /* end if enabled */
			} /* end if SID with ICU parameters */
		} /* end for number of simultaneous DIAG packets */
	}

	/* Copy analogic data in the matrix */
	for (i = 0; i < (uint8_t) (nr_par - 6); i++) {
		if (cycle_number < 3) {
			last_three_das_values[i][cycle_number] = values[i];
		}
		/* reuse values array to store the median values */
		values[i] = asw_calculate_median(i);
	}

	icu_sem_lock(monit_db_sem_Id, 5);
	/* Copy median values, digital data and OBT in DB */
	for (i = 0; i < (uint8_t) (nr_par - 4); i++) {
		monitored_db_param[acq_par_ids[i]].current_value = values[i];
	}
	icu_sem_unlock(monit_db_sem_Id);

	/* save OBT */
	icu_sem_lock(not_mon_db_sem_Id, 5);
	db_long_curr_value[DB_DAS_TABLE_OBT_COARSE_IDX][0] = 0;
	db_long_curr_value[DB_DAS_TABLE_OBT_COARSE_IDX][1] =
			((uint32_t) values[n_par - 4] << 16U)
					| (uint32_t) values[n_par - 3];
	db_long_curr_value[DB_DAS_TABLE_OBT_FINE_IDX][0] = 0;
	db_long_curr_value[DB_DAS_TABLE_OBT_FINE_IDX][1] = ((uint32_t) values[n_par
			- 2] << 16U) | (uint32_t) values[n_par - 1];
	icu_sem_unlock(not_mon_db_sem_Id);

	return;
}

uint16_t asw_calculate_median(uint8_t idx) {
	uint16_t median = 0;

	/* x1 == x2 */
	if ((last_three_das_values[idx][0] == last_three_das_values[idx][1])
			|| (last_three_das_values[idx][0] == last_three_das_values[idx][2])) {
		median = last_three_das_values[idx][0];
	}
	/* x2 == x3 */
	else if (last_three_das_values[idx][1] == last_three_das_values[idx][2]) {
		median = last_three_das_values[idx][1];
	}
	/* x1 < x2 */
	else if (last_three_das_values[idx][0] < last_three_das_values[idx][1]) {
		/* x2 <= x3 */
		if (last_three_das_values[idx][1] <= last_three_das_values[idx][2]) {
			median = last_three_das_values[idx][1];
		} else {
			/* x2 <= x3 and x1 < x3 */
			if (last_three_das_values[idx][0]
					<= last_three_das_values[idx][2]) {
				median = last_three_das_values[idx][2];
			} else {
				/* x2 <= x3 and x1 >= x3 */
				median = last_three_das_values[idx][0];
			}
		}
	} else {
		/* x1 > x2  is implicit */
		/* x1 <= x3 */
		if (last_three_das_values[idx][0] <= last_three_das_values[idx][2]) {
			median = last_three_das_values[idx][0];
		} else {
			/* x1 > x3 and x2 < x3 */
			if (last_three_das_values[idx][1] < last_three_das_values[idx][2]) {
				median = last_three_das_values[idx][2];
			} else {
				/* x1 > x3 and x2 >= x3 */
				median = last_three_das_values[idx][1];
			}
		}
	}

	return (median);
}

void asw_monitor_dpu_errors(void) {
	return;
}

void asw_monitor_das_alarms(void) {
	uint16_t par_idx = 0;
	uint16_t par_id = 0;
	uint16_t par_value = 0;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;

	/* Acquire idx of parameter inside datapool */
	par_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 11U;
	par_idx = asw_PUS_3_ID_to_mon_DB_idx(par_id);

	/* Read last value acquired */
	icu_sem_lock(monit_db_sem_Id, 5);
	par_value = monitored_db_param[par_idx].current_value;
	icu_sem_unlock(monit_db_sem_Id);

	/* Check if it is call FDIR task*/
	if (par_value != icu_settings.das_alarm_reg) {
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0, EVENT_DAS_WARNING);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1, 6);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2, 0);
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3, (uint32_t) ((par_id << 16) | par_value));
		rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_4, (uint32_t) (icu_settings.das_alarm_reg << 16));
		rtems_status = rtems_task_resume(task_fdir_id);
		if (rtems_status != RTEMS_SUCCESSFUL) {
			rtems_error_report(NON_FATAL_ERROR, rtems_status);
		}
	}

	return;
}

void asw_monitor_cdpu_tmp(void) {
	uint16_t par_idx = 0;
	uint16_t par_id = 0;
	uint16_t par_value = 0;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint8_t errors[4] = { 0 };

	/* Acquire idx of parameter inside datapool */
	par_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 1;
	par_idx = asw_PUS_3_ID_to_mon_DB_idx(par_id);

	/* Read last value acquired */
	icu_sem_lock(monit_db_sem_Id, 5);
	par_value = monitored_db_param[par_idx].current_value;
	icu_sem_unlock(monit_db_sem_Id);

	/* Check if par is over thresholds */
	if (par_value >= icu_settings.cdpu_temp_t1) {
		/* Parameter is over T1 */
		if (par_value < icu_settings.cdpu_temp_t2) {
			/* Parameter is over T1 -- call FDIR task */
			if (par_value < icu_settings.cdpu_temp_t2) {
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_0, EVENT_TEMPERATURE_WARNING);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_1, 6);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_2, 0);
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_3,
						(uint32_t) ((par_id << 16) | par_value));
				rtems_status = rtems_task_set_note(task_fdir_id,
						RTEMS_NOTEPAD_4,
						(uint32_t) (icu_settings.cdpu_temp_t1 << 16));
				rtems_status = rtems_task_resume(task_fdir_id);
				if (rtems_status != RTEMS_SUCCESSFUL) {
					rtems_error_report(NON_FATAL_ERROR, rtems_status);
				}
			} else {
				/* Parameter is over T2 -- notify Spacecraft*/
				memcpy(&errors[0], &par_id, sizeof(uint16_t));
				memcpy(&errors[2], &par_value, sizeof(uint16_t));
				memcpy(&errors[4], &icu_settings.cdpu_temp_t2,
						sizeof(uint16_t));
				icu_status = asw_PUS_5_TM_report(EVENT_TEMPERATURE_ALARM, 6,
						errors);
			}
		}
	}
	return;
}

void asw_monitor_lvps_tmp(void) {
	uint16_t par_idx = 0;
	uint16_t par_id = 0;
	uint16_t par_value = 0;
	icu_status_code icu_status = ICU_SUCCESSFUL;
	rtems_status_code rtems_status = RTEMS_SUCCESSFUL;
	uint8_t errors[6] = { 0 };

	/* Acquire idx of parameter inside datapool */
	par_id = ((uint16_t) SW_ASW << PARAM_SW_OFFSET)
			| ((uint16_t) PARAM_MONITORED << PARAM_MONIT_FLAG_OFFSET)
			| ((uint16_t) SYS_ICU << PARAM_SYSTEM_OFFSET)
			| ((uint16_t) SUBSYS_ICU << PARAM_SUB_SYSTEM_OFFSET) | 2;
	par_idx = asw_PUS_3_ID_to_mon_DB_idx(par_id);

	/* Read last value acquired */
	icu_sem_lock(monit_db_sem_Id, 5);
	par_value = monitored_db_param[par_idx].current_value;
	icu_sem_unlock(monit_db_sem_Id);

	/* Check if par is over thresholds */
	if (par_value >= icu_settings.lvps_temp_t1) {
		/* Parameter is over T1 -- call FDIR task */
		if (par_value < icu_settings.lvps_temp_t2) {
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_0,
					EVENT_TEMPERATURE_WARNING);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_1,
					6);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_2,
					0);
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_3,
					(uint32_t) ((par_id << 16) | par_value));
			rtems_status = rtems_task_set_note(task_fdir_id, RTEMS_NOTEPAD_4,
					(uint32_t) (icu_settings.lvps_temp_t1 << 16));
			rtems_status = rtems_task_resume(task_fdir_id);
			if (rtems_status != RTEMS_SUCCESSFUL) {
				rtems_error_report(NON_FATAL_ERROR, rtems_status);
			}
		} else {
			/* Parameter is over T2 -- notify Spacecraft*/
			memcpy(&errors[0], &par_id, sizeof(uint16_t));
			memcpy(&errors[2], &par_value, sizeof(uint16_t));
			memcpy(&errors[4], &icu_settings.lvps_temp_t2, sizeof(uint16_t));
			icu_status = asw_PUS_5_TM_report(EVENT_TEMPERATURE_ALARM, 6,
					errors);
		}
	}

	return;
}
