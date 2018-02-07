/**
 * \file	PUS_Service_6.h
 *
 * \brief	PUS Service 6 management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Mar 3, 2016
 */

#ifndef PUS_SERVICE_6_H_
#define PUS_SERVICE_6_H_

#include "PUS_6_DB_def.h"
#include "ICU_config.h"
#include "tc_header.h"
#include <rtems.h>
/**
 *  \ingroup PUS_6_MANAGER_FUNCTIONS
 *  \{
 */
/**
 * \brief This function implements the PUS Service 6.2: load memory at a given starting address.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_6_2_load_memory(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 6.5: dump memory from a given starting address.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error. */
icu_status_code asw_PUS_6_5_dump_memory(telecommand_t tc);

/**
 * \brief This function prepares the TM(6,6) in response of TC 6.5. More than one TM packet can be produced.
 * \param[in] APID APID number
 * \param[in] memory_ID Memory ID
 * \param[in] start_address Memory start address
 * \param[in] length size of data (in Bytes)
 * \param[in] mem_data data array
 * \return 0 if is successfully executed, an error code from RTEMS_LIST in case of an error.
 */
uint32_t asw_PUS_6_6_TM_mem_dump_report(uint16_t APID,
		uint16_t memory_ID, uint32_t start_address, uint32_t length,
		const uint8_t *mem_data);

/**
 * \brief This function implements the PUS Service 6.9: check a memory area returning the ISO checksum.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_6_9_check_memory(telecommand_t tc);

/**
 * \brief This function prepares the TM(6,10) in response of TC 6.9.
 * \param[in] APID APID number
 * \param[in] memory_ID Memory ID
 * \param[in] start_address Memory start address
 * \param[in] length size of data (in Bytes)
 * \param[in] checksum the value of ISO checksum
 * \return SUCCESS (0) if is successfully executed, an error code from RTEMS_LIST in case of an error.
 */
uint32_t asw_PUS_6_10_TM_mem_check_rep(uint16_t APID,
		uint16_t memory_ID, uint32_t start_address, uint32_t length,
		uint16_t checksum);

/**
 * \brief Task used for dumping the DPU memory
 * \return status of task
 */
rtems_task asw_taskTC_6_5(rtems_task_argument argument);
/**
 * \}
 */

#endif /* PUS_SERVICE_6_H_ */
