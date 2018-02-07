/**
 * \file	PUS_Service_8_DCU.h
 *
 * \brief	PUS Service 8: TCs and internal commands for DCUs
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Jun 22, 2017
 */

#ifndef PUS_SERVICE_8_DCU_H_
#define PUS_SERVICE_8_DCU_H_

#include "../../rtems_configuration.h"

/**
 *  \ingroup PUS_8_DCU_FUNCTIONS
 *  \{
 */

/**
 * \brief This function implements the DCU_PWONOFF telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 DCU number (1 B),
 * 					 SCE ON/OFF (1 B)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of failure.
 */
icu_status_code asw_PUS_8_1_6_1_DCU_PWONOFF(telecommand_t tc);

/**
 * \brief This function implements the DCU_SYNC telecommand.
 * \param[in] tc : the TC array,  from which we extract data:
 * 						[DPU number (1 B)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of failure.
 */
icu_status_code asw_PUS_8_1_6_7_DCU_SYNC(telecommand_t tc);

/**
 * \brief This function implements the DCU_RST telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 DCU number (1 B),
 * 					 DCU reset mode (1 B)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of failure.
 */
icu_status_code asw_PUS_8_1_6_8_DCU_RST(telecommand_t tc);

/**
 * \brief This function implements the DCU_SHSK telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 SU-ROM Image (1 B)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of failure.
 */
icu_status_code asw_PUS_8_1_6_11_DCU_SHSK(telecommand_t tc);

/**
 * \}
 */

/**
 *  \ingroup PUS_8_DCU_INTERNAL_FUNCTIONS
 *  \{
 */

/**
 * \}
 */

#endif /* PUS_SERVICE_8_DCU_H_ */
