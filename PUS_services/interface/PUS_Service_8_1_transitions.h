/**
 * \file	PUS_Service_8_1_transitions.h
 *
 * \brief	This file contains declaration of the SW components used to handle ICU_ASW state and transitions
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	10-05-2017
 *
 */

#ifndef PUS_SERVICE_8_1_TRANSISIONS_H_
#define PUS_SERVICE_8_1_TRANSISIONS_H_

#include "ICU_config.h"
#include "tc_header.h"

/**
 *  \ingroup PUS_8_1_TRANSITION_VARIABLES
 *  \{
 */

/**
 * \brief Global variable used to store the ICU's machine state. Default value is STARTUP.
 */
extern state_machine_t icu_state_machine;

/**
 * \}
 */

/**
 *  \ingroup PUS_8_1_TRANSITION_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to change ICU state to STARTUP.
 * \param[in] tc: TC sent by the spacecraft. TC Data must one argument:
 * 				  [function ID [4 bytes]: must be 0x10001]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_1_1_to_startup(telecommand_t tc);

/**
 * \brief Function used to change ICU state to STANDBY.
 * \param[in] tc: TC sent by the spacecraft. TC Data must one argument:
 *  				[function ID [4 bytes]: must be 0x10002]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_1_2_to_standby(telecommand_t tc);

/**
 * \brief Function used to change ICU state to IDLE.
 * \param[in] tc: TC sent by the spacecraft. TC Data must one argument:
 * 					[function ID [4 bytes]: must be 0x10003]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_1_3_to_idle(telecommand_t tc);

/**
 * \brief Function used to change ICU state to OBSERVATION.
 * \param[in] tc: TC sent by the spacecraft. TC Data must one argument:
 * 					[function ID [4 bytes]: must be 0x10004]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_1_4_to_observation(telecommand_t tc);

/**
 * \brief Function used to change ICU state to MANUAL.
 * \param[in] tc: TC sent by the spacecraft. TC Data must one argument:
 * 					[function ID [4 bytes]: must be 0x10005]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_1_5_to_manual(telecommand_t tc);

/**
 * \brief Function used to change ICU state to BOOT.
 * \param[in] tc: TC sent by the spacecraft. TC Data must one argument:
 * 					[function ID [4 bytes]: must be 0x10007]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_1_7_to_boot(telecommand_t tc);

/**
 * \brief Function used to change ICU state in SAFE state. (Currently not a PUS Service 8,1 TC).
 * \return none
 */
void asw_icu_to_safe(void);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_1_TRANSISIONS_H_ */
