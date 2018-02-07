/**
 * \file	PUS_Service_5.h
 *
 * \brief	Definition of PUS Service 5
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Aug 6, 2015
 */

#ifndef PUS_SERVICE_5_H_
#define PUS_SERVICE_5_H_

#include "PUS_5_DB_def.h"
#include "tm.h"
#include "ICU_config.h"
#include "tc_header.h"

/**
 *  \ingroup PUS_5_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief This function create the TM packet of the event and enqueue it in the appropriate queue based on the priority of the event.
 * \param[in] event_idx the ID of the event, from EID_LIST in PUS_5_DB_def.h
 * \param[in] parameter_length the length of the parameter array, in Bytes
 * \param[in] parameter_list the array of parameters (Bytes)
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_5_TM_report(uint16_t event_idx,
		uint8_t parameter_length, const uint8_t *parameter_list);

/**
 * \brief This function enables the list of event passed as argument.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_5_5_enable_event_gen(telecommand_t tc);

/**
 * \brief This function disables the list of event passed as argument.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_5_6_disable_event_gen(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_5_H_ */
