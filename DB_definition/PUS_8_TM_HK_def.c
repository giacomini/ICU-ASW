/*
 * PUS_8_TM_HK_def.c
 *
 *  Created on: Nov 3, 2015
 *      Author: D. Bonino
 */

#include "PUS_8_TM_HK_def.h"

/*
 * Function to fill HSK tables from DPUs into local structure
 */
void asw_DPU_HSK_table_interpreter(const uint16_t *stream, uint8_t rt_addr) {
	uint16_t db_start_idx = 0;
	uint8_t tab_idx = 0;
	uint16_t i = 0, obt_index = 0;

	/* set starting point on the DB */
	if (rt_addr == (uint8_t) dpu1_active_rt) {
		tab_idx = 0;
		db_start_idx = DB_PAR_IDX_ASW_DPU1_DPU_OFFSET;
		obt_index = DB_DPU1_HSK_OBT_IDX;
	} else {
		tab_idx = 1;
		db_start_idx = DB_PAR_IDX_ASW_DPU2_DPU_OFFSET;
		obt_index = DB_DPU2_HSK_OBT_IDX;
	}

	if (stream[1] == tab_counter[tab_idx][DPU_HSK_TAB_ID]) {
		/* fixme table not updated */
	} else {
		tab_counter[tab_idx][DPU_HSK_TAB_ID] = stream[1];
		if (obt_index < DB_LONG_PARAM_NUMBER) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			/* OBT */
			db_long_curr_value[obt_index][0] = stream[2]; /* 2^31 - 2^16 */
			db_long_curr_value[obt_index][1] = ((uint32_t) stream[3] << 16U)
					| (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */

			for (i = 0; i < (DB_ASW_DPU_DPU_HSK_PARAM_NUMBER - 1); i++) {
				db_param[db_start_idx + (uint16_t) (1 + i)].current_value =
						stream[5 + i];
			}
			icu_sem_unlock(not_mon_db_sem_Id);
		}
	}

	return;
}

void asw_DCU_HSK_table_interpreter(const uint16_t *stream, uint8_t rt_addr) {
	uint16_t table_id, db_idx = 0;
	uint8_t i = 0, tab_idx = 0;
	uint16_t obt_index = 0, fifo_err_idx = 0;

	if (rt_addr == (uint8_t) dpu2_active_rt) {
		tab_idx = 1;
	}

	table_id = stream[0];

	if (stream[1] == tab_counter[tab_idx][table_id]) {
		/* fixme table not updated */
	} else {
		tab_counter[tab_idx][table_id] = stream[1];
		/* set starting point on the DB */
		if (rt_addr == (uint8_t) dpu1_active_rt) {
			db_idx = DB_PAR_IDX_ASW_DPU1_DCU_OFFSET
					+ ((table_id - DCU1_HSK_TAB_ID)
							* DB_ASW_DPU_DCU_PARAM_NUMBER);
			obt_index = DB_DPU1_DCU1_HSK_OBT_IDX
					+ (2 * (table_id - DCU1_HSK_TAB_ID));
			fifo_err_idx = DB_DPU1_DCU1_FIFO_ERR_CNT_IDX
					+ (2 * (table_id - DCU1_HSK_TAB_ID));
		} else {
			db_idx = DB_PAR_IDX_ASW_DPU2_DCU_OFFSET
					+ ((table_id - DCU1_HSK_TAB_ID)
							* DB_ASW_DPU_DCU_PARAM_NUMBER);
			obt_index = DB_DPU2_DCU1_HSK_OBT_IDX
					+ (2 * (table_id - DCU1_HSK_TAB_ID));
			fifo_err_idx = DB_DPU2_DCU1_FIFO_ERR_CNT_IDX
					+ (2 * (table_id - DCU1_HSK_TAB_ID));

		}

		if (((obt_index < DB_LONG_PARAM_NUMBER)
				&& (db_idx < DB_NOT_MONITORED_PARAM_NUMB)&& (fifo_err_idx < DB_LONG_PARAM_NUMBER))== TRUE) {
					icu_sem_lock(not_mon_db_sem_Id, 5);
					/* OBT */
					db_long_curr_value[obt_index][0] = stream[2]; /* 2^31 - 2^16 */
					db_long_curr_value[obt_index][1] = ((uint32_t) stream[3]
					<< 16U) | (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */
					db_idx++;

					db_param[db_idx].current_value = stream[5];
					db_idx++;
					db_param[db_idx].current_value = stream[6];
					db_idx++;

					db_long_curr_value[fifo_err_idx][0] = 0;
					db_long_curr_value[fifo_err_idx][1] =
					((uint32_t) stream[7] << 16U)
					| (uint32_t) stream[8];
					db_idx++;

					db_param[db_idx].current_value = stream[9];
					db_idx++;
					db_param[db_idx].current_value = (uint8_t) (stream[10] & 0x00FFU);
					db_idx++;
					db_param[db_idx].current_value = (uint8_t) ((stream[10] & 0xFF00U)
					>> 8U);
					db_idx++;

					db_param[db_idx].current_value = stream[11];
					db_idx++;
					db_param[db_idx].current_value = stream[12];
					db_idx++;

					/* a free line in the table */

					for (i = 0; i < (DB_ASW_DPU_DCU_PARAM_NUMBER - 9); i++) {
						db_param[db_idx].current_value = stream[14 + i];
						db_idx++;
					}
					icu_sem_unlock(not_mon_db_sem_Id);
				}
			}

	return;
}

void asw_DPU_SCE_HSK_t1_interpreter(const uint16_t *stream, uint8_t rt_address) {
	uint16_t table_id, db_idx = 0, tmp = 0;
	uint8_t tab_idx = 0;
	uint16_t obt_index = 0, i = 0;

	if (rt_address == (uint8_t) dpu2_active_rt) {
		tab_idx = 1;
	}

	table_id = stream[0];

	if (stream[1] == tab_counter[tab_idx][table_id]) {
		/* fixme table not updated */
	} else {
		tab_counter[tab_idx][table_id] = stream[1];
		tmp = (table_id - SCE1_HSK_TAB1_ID) / 3;
		/* set starting point on the DB */
		if (rt_address == (uint8_t) dpu1_active_rt) {
			db_idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET
					+ (tmp * DB_ASW_DPU_SCE_PARAM_NUMBER);
			obt_index = DB_DPU1_SCE1_HSK1_OBT_IDX + tmp;
		} else {
			db_idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET
					+ (tmp * DB_ASW_DPU_SCE_PARAM_NUMBER);
			obt_index = DB_DPU2_SCE1_HSK1_OBT_IDX + tmp;
		}

		if (((obt_index < DB_LONG_PARAM_NUMBER)
				&& (db_idx < DB_NOT_MONITORED_PARAM_NUMB))== TRUE) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			/* OBT */
			db_long_curr_value[obt_index][0] = stream[2]; /* 2^31 - 2^16 */
			db_long_curr_value[obt_index][1] = ((uint32_t) stream[3] << 16U)
					| (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */
			db_idx++;

			for (i = 0; i < (DB_ASW_DPU_SCE_HSK1_PAR_NUM - 1); i++) {
				db_param[db_idx + i].current_value = stream[5 + i];
			}
			icu_sem_unlock(not_mon_db_sem_Id);
		}
	}
	return;
}

void asw_DPU_SCE_HSK_t2_interpreter(const uint16_t *stream, uint8_t rt_address) {
	uint16_t table_id, db_idx = 0, tmp = 0;
	uint8_t i = 0, tab_idx = 0;
	uint16_t obt_index = 0;

	if (rt_address == (uint8_t) dpu2_active_rt) {
		tab_idx = 1;
	}

	table_id = stream[0];

	if (stream[1] == tab_counter[tab_idx][table_id]) {
		/* fixme table not updated */
	} else {
		tab_counter[tab_idx][table_id] = stream[1];
		tmp = (table_id - SCE1_HSK_TAB2_ID) / 3;
		/* set starting point on the DB */
		if (rt_address == (uint8_t) dpu1_active_rt) {
			db_idx = DB_PAR_IDX_ASW_DPU1_SCE_OFFSET
					+ DB_ASW_DPU_SCE_HSK1_PAR_NUM
					+ (tmp * DB_ASW_DPU_SCE_PARAM_NUMBER);
			obt_index = DB_DPU1_SCE1_HSK2_OBT_IDX + tmp;
		} else {
			db_idx = DB_PAR_IDX_ASW_DPU2_SCE_OFFSET
					+ DB_ASW_DPU_SCE_HSK1_PAR_NUM
					+ (tmp * DB_ASW_DPU_SCE_PARAM_NUMBER);
			obt_index = DB_DPU2_SCE1_HSK2_OBT_IDX + tmp;
		}

		if (((obt_index < DB_LONG_PARAM_NUMBER)
				&& (db_idx < DB_NOT_MONITORED_PARAM_NUMB))== TRUE) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			/* OBT */
			db_long_curr_value[obt_index][0] = stream[2]; /* 2^31 - 2^16 */
			db_long_curr_value[obt_index][1] = ((uint32_t) stream[3] << 16U)
					| (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */
			db_idx++;

			for (i = 0; i < 5; i++) {
				db_param[db_idx].current_value = stream[5 + i];
				db_idx++;
			}

			/* Instrument mode (low byte) = param #35; filter/detector ID (high byte) = param #36 */
			db_param[db_idx].current_value = (uint8_t) (stream[10] & 0x00FFU);
			db_idx++;

			db_param[db_idx].current_value = (uint8_t) ((stream[10] & 0xFF00U)
					>> 8U);
			db_idx++;

			for (i = 0; i < (DB_ASW_DPU_SCE_HSK2_PAR_NUM - 8); i++) {
				db_param[db_idx].current_value = stream[11 + i];
				db_idx++;
			}
			icu_sem_unlock(not_mon_db_sem_Id);
		}
	}
	return;
}

void asw_DPU_SCE_HSK_t3_interpreter(const uint16_t *stream, uint8_t rt_address) {
	uint16_t table_id, db_idx = 0, tmp = 0;
	uint8_t tab_idx = 0;
	uint16_t obt_index = 0, i = 0;

	if (rt_address == (uint8_t) dpu2_active_rt) {
		tab_idx = 1;
	}

	table_id = stream[0];

	if (stream[1] == tab_counter[tab_idx][table_id]) {
		/* fixme table not updated */
	} else {
		tab_counter[tab_idx][table_id] = stream[1];
		tmp = (table_id - SCE1_HSK_TAB3_ID) / 3;
		/* set starting point on the DB */
		if (rt_address == (uint8_t) dpu1_active_rt) {
			db_idx =
					DB_PAR_IDX_ASW_DPU1_SCE_OFFSET
							+ (DB_ASW_DPU_SCE_HSK1_PAR_NUM
									+ DB_ASW_DPU_SCE_HSK2_PAR_NUM)
							+ (tmp * DB_ASW_DPU_SCE_PARAM_NUMBER);
			obt_index = DB_DPU1_SCE1_HSK3_OBT_IDX + tmp;
		} else {
			db_idx =
					DB_PAR_IDX_ASW_DPU2_SCE_OFFSET
							+ (DB_ASW_DPU_SCE_HSK1_PAR_NUM
									+ DB_ASW_DPU_SCE_HSK2_PAR_NUM)
							+ (tmp * DB_ASW_DPU_SCE_PARAM_NUMBER);
			obt_index = DB_DPU2_SCE1_HSK3_OBT_IDX + tmp;
		}

		if (((obt_index < DB_LONG_PARAM_NUMBER)
				&& (db_idx < DB_NOT_MONITORED_PARAM_NUMB))== TRUE) {
			icu_sem_lock(not_mon_db_sem_Id, 5);
			/* OBT */
			db_long_curr_value[obt_index][0] = stream[2]; /* 2^31 - 2^16 */
			db_long_curr_value[obt_index][1] = ((uint32_t) stream[3] << 16U)
					| (uint32_t) stream[4]; /* 2^8 - 2^0 - 2^-8 - 2^-16 */
			db_idx++;

			for (i = 0; i < (DB_ASW_DPU_SCE_HSK3_PAR_NUM - 1); i++) {
				db_param[db_idx + i].current_value = stream[5 + i];
			}
			icu_sem_unlock(not_mon_db_sem_Id);
		}
	}
	return;
}

void asw_DPU_BSW_CPU_DRB_interpreter(const uint16_t *stream, uint8_t rt_address) {
	uint16_t i = 0, db_idx = 0;
	uint16_t tab_idx = 0, table_id = 0;
	uint16_t dpu_drb_obt = 0;
	uint16_t dpu_drb_sw_ver = 0, dpu_drb_sc_stat = 0;
	uint16_t dpu_drb_eeprom_stat = 0, dpu_drb_eeprom_err = 0;

	if (rt_address == (uint8_t) dpu1_active_rt) {
		db_idx = DB_PARAM_INDEX_BSW_DPU1_OFFSET;
		dpu_drb_obt = DB_DPU1_BSW_CPU_DRB_OBT_IDX;
		dpu_drb_sw_ver = DB_DPU1_BSW_CPU_DRB_SW_VERS_IDX;
		dpu_drb_sc_stat = DB_DPU1_BSW_CPU_DRB_SC_STAT_IDX;
		dpu_drb_eeprom_stat = DB_DPU1_BSW_CPU_DRB_MEMSTAT_IDX;
		dpu_drb_eeprom_err = DB_DPU1_BSW_CPU_DRB_MEMERR_IDX;
	} else {
		tab_idx = 1;
		db_idx = DB_PARAM_INDEX_BSW_DPU2_OFFSET;
		dpu_drb_obt = DB_DPU2_BSW_CPU_DRB_OBT_IDX;
		dpu_drb_sw_ver = DB_DPU2_BSW_CPU_DRB_SW_VERS_IDX;
		dpu_drb_sc_stat = DB_DPU2_BSW_CPU_DRB_SC_STAT_IDX;
		dpu_drb_eeprom_stat = DB_DPU2_BSW_CPU_DRB_MEMSTAT_IDX;
		dpu_drb_eeprom_err = DB_DPU2_BSW_CPU_DRB_MEMERR_IDX;
	}

//	table_id = (stream[0] & 0xFF00U) >> 8;
//
//	if (stream[1] == tab_counter[tab_idx][table_id]) {
//		/* fixme table not updated */
//	} else {
//		tab_counter[tab_idx][table_id] = stream[1];
	table_id = (stream[8] >> 14U) & 0x0003U;

	if (((table_id < BSW_MAX_TABLE_NUMBER) && (table_id != 0)) == TRUE) {
		if (stream[1] == bsw_tab_counter[tab_idx][table_id]) {
			/* fixme table not updated */
		} else {
			bsw_tab_counter[tab_idx][table_id] = stream[1];
			if (((dpu_drb_obt < DB_LONG_PARAM_NUMBER)
					&& (db_idx < DB_NOT_MONITORED_PARAM_NUMB)&& (dpu_drb_sw_ver < DB_LONG_PARAM_NUMBER) && (dpu_drb_sc_stat < DB_LONG_PARAM_NUMBER) && (dpu_drb_eeprom_stat < DB_LONG_PARAM_NUMBER) && (dpu_drb_eeprom_err < DB_LONG_PARAM_NUMBER))== TRUE) {
						icu_sem_lock(not_mon_db_sem_Id, 5);
						/* OBT */
						db_long_curr_value[dpu_drb_obt][0] = ((uint32_t) stream[2] << 16U)
						| (uint32_t) stream[3];
						db_long_curr_value[dpu_drb_obt][1] = ((uint32_t) stream[4] << 16U)
						| (uint32_t) stream[5];
						db_idx++;

						/* SW version */
						db_long_curr_value[dpu_drb_sw_ver][0] = 0;
						db_long_curr_value[dpu_drb_sw_ver][1] =
						((uint32_t) stream[6] << 16U) | (uint32_t) stream[7];
						db_idx++;

						/*
						 * unpack DW 8
						 */
						/* DW 8: HK packet ID */
						db_param[db_idx].current_value = (uint8_t) ((stream[8] >> 14U) & 0x0003U);
						db_idx++;

						/* DW 8: watchdog enable/disable */
						db_param[db_idx].current_value = (uint8_t) ((stream[8] >> 12U) & 0x0003U);
						db_idx++;

						/* DW 8: SW DW 1 */
						db_param[db_idx].current_value = (uint8_t) ((stream[8] >> 10U) & 0x0003U);
						db_idx++;

						/* DW 8: SDRAM RESYNCH */
						db_param[db_idx].current_value = (uint8_t) ((stream[8] >> 8U) & 0x0003U);
						db_idx++;

						/* DW 8: Memory scrub */
						db_param[db_idx].current_value = (uint8_t) ((stream[8] >> 6U) & 0x0003U);
						db_idx++;

						/* DW 8: CPU RESYNCH */
						db_param[db_idx].current_value = (uint8_t) ((stream[8] >> 4U) & 0x0003U);
						db_idx++;

						/* DW 8: SW DW 2*/
						db_param[db_idx].current_value = (uint8_t) (stream[8] & 0x000FU);
						db_idx++;

						/* SYS controller Status Bit Register */
						db_long_curr_value[dpu_drb_sc_stat][0] = 0;
						db_long_curr_value[dpu_drb_sc_stat][1] = ((uint32_t) stream[9]
						<< 16U) | (uint32_t) stream[10];
						db_idx++;

						/* EEPROM ECC Status Register */
						db_long_curr_value[dpu_drb_eeprom_stat][0] = 0;
						db_long_curr_value[dpu_drb_eeprom_stat][1] = ((uint32_t) stream[11]
						<< 16U) | (uint32_t) stream[12];
						db_idx++;

						/* EEPROM ECC Status Register */
						db_long_curr_value[dpu_drb_eeprom_err][0] = 0;
						db_long_curr_value[dpu_drb_eeprom_err][1] = ((uint32_t) stream[13]
						<< 16U) | (uint32_t) stream[14];
						db_idx++;

						for (i = 0; i < 11; i++) {
							db_param[db_idx].current_value = stream[15 + i];
							db_idx++;
						}

						icu_sem_unlock(not_mon_db_sem_Id);
					}
				}
			}
	return;
}

void asw_DPU_BSW_DCU_interpreter(const uint16_t *stream, uint8_t rt_address) {
	uint16_t i = 0, db_idx = 0;
	uint16_t tab_idx = 0, table_id = 0;
	uint16_t dcu_obt = 0;
	uint16_t dcu_sw_ver = 0;

	if (rt_address == (uint8_t) dpu1_active_rt) {
		db_idx = DB_PARAM_INDEX_BSW_DPU1_OFFSET
				+ DB_BSW_DPU_DPU_DRB_PARAM_NUMBER;
		dcu_obt = DB_DPU1_BSW_DCU_OBT_IDX;
		dcu_sw_ver = DB_DPU1_BSW_DCU_SW_VERS_IDX;
	} else {
		tab_idx = 1;
		db_idx = DB_PARAM_INDEX_BSW_DPU2_OFFSET
				+ DB_BSW_DPU_DPU_DRB_PARAM_NUMBER;
		dcu_obt = DB_DPU2_BSW_DCU_OBT_IDX;
		dcu_sw_ver = DB_DPU2_BSW_DCU_SW_VERS_IDX;
	}

//	table_id = (stream[0] & 0xFF00U) >> 8;
//	if (stream[1] == tab_counter[tab_idx][table_id]) {
//		/* fixme table not updated */
//	} else {
//		tab_counter[tab_idx][table_id] = stream[1];

	table_id = (stream[8] >> 14U) & 0x3U;

	if (((table_id < BSW_MAX_TABLE_NUMBER) && (table_id != 0)) == TRUE) {
		if (stream[1] == bsw_tab_counter[tab_idx][table_id]) {
			/* fixme table not updated */
		} else {
			bsw_tab_counter[tab_idx][table_id] = stream[1];
			if (((dcu_obt < DB_LONG_PARAM_NUMBER)
					&& (dcu_sw_ver < DB_LONG_PARAM_NUMBER)
					&& (db_idx < DB_NOT_MONITORED_PARAM_NUMB))== TRUE) {
				icu_sem_lock(not_mon_db_sem_Id, 5);
				/* OBT */
				db_long_curr_value[dcu_obt][0] = ((uint32_t) stream[2] << 16U)
						| (uint32_t) stream[3];
				db_long_curr_value[dcu_obt][1] = ((uint32_t) stream[4] << 16U)
						| (uint32_t) stream[5];
				db_idx++;

				/* SW version */
				db_long_curr_value[dcu_sw_ver][0] = 0;
				db_long_curr_value[dcu_sw_ver][1] =
						((uint32_t) stream[6] << 16U) | (uint32_t) stream[7];
				db_idx++;

				/* DW 8 */
				db_param[db_idx].current_value = (uint8_t) ((stream[8] >> 14U)
						& 0x0003U);
				db_idx++;

				for (i = 0; i < 16; i++) {
					db_param[db_idx].current_value = stream[9 + i];
					db_idx++;
				}

				icu_sem_unlock(not_mon_db_sem_Id);
			}
		}
	}
	return;
}

void asw_monitor_DPUs_ASW_error_tab(void) {
	/*
	 * Check DPUs_ASW_errors_tab and report errors to ground.
	 */
	return;
}
