/**
 * \file	PUS_Service_8_DPU_config.h
 *
 * \brief	PUS Service 8: commands toward DPUs
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 2, 2015
 */

#ifndef PUS_SERVICE_8_DPU_CONFIG_H_
#define PUS_SERVICE_8_DPU_CONFIG_H_

#include "PUS_8_DB_def.h"
#include "ICU_config.h"
#include <rtems/rtems/tasks.h>
#include "tc_header.h"

/**
 *  \ingroup PUS_8_DPU_HL_FUNCTIONS
 *  \{
 */

/*
 * DPU control commands - type 8, subtype 1; function ID 3,*
 */

/**
 * \brief This function implements the NISC_0314 TC: send DITH_CONFIG_TAB.
 * It stores locally the values in a global structure, chooses the DPMEM pointer
 * and send it to both DPUs on SA 21.
 * Note: the configuration tables are the same for both DPUs.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_3_14_DPU_tx_dither(telecommand_t tc);

/**
 * \brief This function implements the NISC_0315 TC: send PROC_PARAM_TAB.
 * It stores locally the values in a global structure and send it to DPU/DPUs on SA 19.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_3_15_DPU_tx_procPar(telecommand_t tc);

/**
 * This functions implements the NISC_0303 TC: send SYS_CONFIG-TAB.
 * It stores values in a global structure on ICU ASW for further use and send them to both DPUs.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_3_3_DPU_tx_sysConf(telecommand_t tc);

/**
 * \}
 */

/*
 * Auxiliary functions
 */
/**
 *  \ingroup PUS_8_DPU_HL_INTERNAL_FUNCTIONS
 *  \{
 */

void asw_set_dither_table_values(dither_config_table_t *dither_table,
		uint8_t exp_number, uint8_t DPMEM, uint16_t dith_time,
		const uint8_t *exp_group_number, const uint8_t *exp_framePerGroup,
		const uint16_t *exp_dl1, const uint16_t *exp_dl2,
		const uint16_t *exp_instr_mode);
void asw_set_proc_param_table_values(proc_param_table_t *proc_par_tab,
		uint16_t proc_mode, const uint8_t *Chi2_pix_thrhold, const uint8_t *RON,
		uint16_t ffstat_box_size, const uint16_t *ffstat_box_Start_coord,
		uint16_t compr_bck_size, uint16_t compr_FSbits, uint16_t compr_FSmax,
		uint16_t compr_prescal, int16_t det_final_ima_offset,
		uint8_t raw_lines_number);
void asw_set_sys_config_table_values(sys_config_table_t *sys_conf_tab,
		uint8_t exp_WD, uint8_t dith_WD, uint8_t det_WD, uint8_t bus_WD,
		uint16_t TRP_err_WD, uint16_t DRB_EDAC_enab, uint16_t DBB_EDAC_enab,
		uint16_t DC_DC_thresh, uint16_t DCU_thresh, uint16_t DBB_thresh,
		uint16_t CPU_thresh, uint16_t temp_thresh_cnt);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_DPU_CONFIG_H_ */
