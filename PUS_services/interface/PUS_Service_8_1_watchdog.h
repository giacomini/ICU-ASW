/**
 * \file	PUS_Service_8_1_watchdog.h
 *
 * \brief	This file contains declaration of the ICU-ASW objects used to handle watchdog.
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	27-04-2017
 *
 */

#ifndef PUS_SERVICE_8_1_WATCHDOG_H_
#define PUS_SERVICE_8_1_WATCHDOG_H_

#include "ICU_config.h"
#include "tc_header.h"

/**
 *  \ingroup WATCHDOG_MANAGER_CONSTANTS
 *  \{
 */

#define WDG_TO_INT_MIN_INTERVAL 25U //!< Minimum interval (in ms) acceptable for watchdog to interrupt interval.
#define WDG_TO_RES_MIN_INTERVAL 60000U //!< Minimum interval (in ms) acceptable for watchdog to reset interval.
/**
 * \}
 */

/**
 *  \ingroup WATCHDOG_MANAGER_VARIABLES
 *  \{
 */

/* Variable used to start WD count in the 1553 isr */
extern bool_t wd_activation;

/**
 * \}
 */

/**
 *  \ingroup WATCHDOG_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to enable or disable the watchdog.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 4 arguments:
 * 					[FunctionID [4 bytes]: must be 0x20007;
 * 					 enable_disable [1 byte]: 0 = disable, 1 = enable;
 * 					 time_to_interrupt [2 bytes]: It is used only in case of an enable command and must be greater than WDG_TO_INT_MIN_INTERVAL value (25 ms);
 * 					 time_to_reset [2 bytes]: It is used only in case of an enable command and must be greater than WDG_TO_RES_MIN_INTERVAL value (60000 ms)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_2_7_watchdog_en_dis(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_1_WATCHDOG_H_ */
