/**
 * \file	PUS_Service_9.h
 *
 * \brief	PUS Service 9 management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Dec 6, 2016
 */

#ifndef PUS_SERVICE_9_H_
#define PUS_SERVICE_9_H_

#include "tc_header.h"
#include "ICU_config.h"

/**
 *  \ingroup PUS_9_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief Disable the synchronization with the master Time Information
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error. */
icu_status_code asw_PUS_9_130_dis_time_synch(telecommand_t tc);

/**
 * \brief Enable the synchronization with the master Time Information
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_9_131_en_time_synch(telecommand_t tc);
/**
 * \}
 */

#endif /* PUS_SERVICE_9_H_ */
