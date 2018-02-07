/**
 * \file	PUS_Service_8_1.h
 *
 * \brief	This file contains declaration of the SW components usied to manage the PUS 8,1 commands not included in other sections.
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	16-05-2017
 *
 */

#ifndef PUS_SERVICE_8_1_H_
#define PUS_SERVICE_8_1_H_

/**
 *  \ingroup PUS_8_1_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to set the planning ID parameter.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 2 arguments:
 * 					[FunctionID [4 bytes]: must be 0x20006,
 *  				 planning ID [4 bytes]: planning ID value to be used in TM packets]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_2_6_set_planning_ID(telecommand_t tc);

/**
 * \brief Function used to set RT addresses of the DPUs. Based on parameters received it will set the 1553 RT address of DPU units.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 3 arguments:
 * 					[FunctionID [4 bytes]: must be 0x20003,
 *  				 DPU1 unit [1 byte]: 0 = disable (RT address = 0), 1 = nominal unit on( RT address = 1), 2 = redundant unit on (RT address = 2),
 * 					 DPU2 unit [1 byte]: 0 = disable (RT address = 0), 1 = nominal unit on (RT address = 3), 2 = redundant unit on (RT address = 4)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_2_3_setDPUsAddress(telecommand_t tc);

/**
 * \brief Function used to set thresholds of configurable parameters.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 3 arguments:
 *  				[FunctionID [4 bytes]: must be 0x20008,
 * 					 number_of_parameters [1 byte]: number of parameters]
 * 				  For each parameters 3 arguments are needed:
 * 					[parameterID [2 bytes]: parameter to be setted,
 * 					 lower_threshold [2 bytes],
 * 					 upper threshold [2 bytes]]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_2_8_set_thresholds(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_1_H_ */
