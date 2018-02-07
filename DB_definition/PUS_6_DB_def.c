/**
 * \file	PUS_6_DB_def.c
 *
 * \brief	PUS Service 6 auxiliary variables and functions
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Nov 4, 2016
 */

#include "../rtems_configuration.h"

void asw_DPU_MEMREG_tab_interpreter(uint8_t rt_addr, uint8_t subaddress,
		uint8_t length, const uint16_t *stream) {
	uint8_t DPU_idx = 0;
	uint8_t table_error = 0;
	uint8_t header_size = 0, i = 0;
	uint16_t buff_idx = 0;

	if (rt_addr == (uint8_t)dpu1_active_rt) {
		DPU_idx = 0;
	} else {
		DPU_idx = 1;
	}

	if (subaddress == 1) {
		buff_idx = 0;
		header_size = 9;
	} else {
		buff_idx = 2
				* (DPU_MEMREG_TAB_1_MAX_DW
						+ (DPU_MEMREG_TAB_2_16_MAX_DW * (subaddress - 2)));
		header_size = 5;
	}

	if (subaddress == 1) {
		icu_sem_lock(mem_reg_sem_Id, 5);
		if ((DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_ICU_counter != stream[1])
				|| (DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_mem_ID != stream[5])
				|| (DPU_MEMREG_DUMP_tab[DPU_idx].DPU_MDUMP_requested_size
						!= stream[6])
				|| (DPU_MEMREG_DUMP_tab[DPU_idx].MDUMP_start_address
						!= (((uint32_t)stream[7] << 16) | (uint32_t)stream[8]))) {
			/* FIXME do something - table are not right! */
			table_error = 1;
		}
		icu_sem_unlock(mem_reg_sem_Id);
	}

	if (table_error == 0) {

		for (i = 0; i < (uint8_t)(length - header_size); i++) {
			DPU_MEMREG_DUMP_tab[DPU_idx].memory_data[buff_idx + (uint16_t)(i * 2)] =
					(uint8_t) ((stream[i] & 0xFF00U) >> 8U);
			DPU_MEMREG_DUMP_tab[DPU_idx].memory_data[buff_idx + (uint16_t)((i * 2) + 1)] =
					(uint8_t) (stream[i] & 0x00FFU);
		}
		DPU_MEMREG_DUMP_tab[DPU_idx].effective_mem_length = (length
				- header_size) * 2;
		icu_sem_unlock(mem_reg_sem_Id);

	}

	return;
}
