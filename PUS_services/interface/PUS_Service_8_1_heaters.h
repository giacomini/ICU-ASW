/**
 * \file	PUS_Service_8_1_heaters.h
 *
 * \brief	This file contains declaration of software components used to handle heaters
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	27-04-2017
 *
 */

#ifndef PUS_SERVICE_8_1_HEATERS_H_
#define PUS_SERVICE_8_1_HEATERS_H_

/**
 *  \ingroup HEATER_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to power on/off the heaters.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 3 arguments:
 * 					[FunctionID [4 bytes]: must be 0xA0001;
 * 					 heater id [1 byte]: 0 for FPA, 1 for SA.
 * 					 power level [2 bytes]: must be a value between zero and 1023. Zero means that selected heater must be powered off]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_10_1_heater_on_off(telecommand_t tc);

/**
 * \brief Function used to reset heaters functionalities.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain an argument:
 * 				[FunctionID [4 bytes]: must be 0xA0002]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_10_2_heaters_reset(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_1_HEATERS_H_ */
