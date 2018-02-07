/**
 * \file	PUS_Service_17.h
 *
 * \brief	PUS Service 17 management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Dec 6, 2016
 */
#ifndef PUS_SERVICE_17_H_
#define PUS_SERVICE_17_H_

#include "tc_header.h"
#include "ICU_config.h"

/**
 *  \ingroup PUS_17_MANAGER_FUNCTIONS
 *  \{
 */
/**
 * \brief Function used to check connection. Return a TM(1,7) packet.
 * \param[in] tc TC sent by the spacecraft.
 * \return status ICU_SUCCESSFULL if ok, an error code otherwise.
 */
icu_status_code asw_PUS_17_1_connection_test(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_17_H_ */
