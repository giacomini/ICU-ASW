/**
 * \file	PUS_Service_8_SCE.h
 *
 * \brief	PUS Service 8: commands toward SCE-EEF
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 22, 2015
 */
#ifndef PUS_SERVICE_8_SCE_H_
#define PUS_SERVICE_8_SCE_H_

#include "PUS_8_DB_def.h"
#include "ICU_config.h"
#include "tc_header.h"

/**
 *  \ingroup PUS_8_SCE_FUNCTIONS
 *  \{
 */

/**
 * \brief This functions implements the NISC_0401 TC: send SCE_IPCEXP command.
 * It prepares the command table and send it to the selected DPU/DPUs and DCU/DCUs.
 * \param[in] tc : the TC array, from which data is extracted:
 * 					[DPU number (2 B) - DPU = 0 (broadcast); 1; 2
 * 					 DCU number (2 B) - DCU = 0 (broadcast); 1 ... 8
 * 					 reset frame # (2 B)
 *  				 droplines 1 # (2 B)
 *   				 groups # (2 B)
 *    				 number of reads per group (2 B)
 *     				 intergroup DropLines2 (2 B)
 *     				 intergroup DropLines3 (1 B)
 *     				 exposure configuration (2 B)
 *  				 IPC start Y (2 B)
 * 					 IPC step Y (2 B)
 *  				 IPC start X (2 B)
 * 					 IPC step X (2 B)
 * 					 IPC reset voltage]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_4_1_SCE_IPCEXP(telecommand_t tc);

/**
 * \brief This functions implements the NISC_0402 TC: send SCE_KTCEXP command.
 * It prepares the command table and send it to the selected DPU/DPUs and DCU/DCUs.
 * \param[in] tc : the TC array, from which data is extracted:
 * 					[DPU number (1 B)
 * 					 DCU number (1 B) - DCU = 0 (broadcast); 1 ... 8
 * 					 reset frame # (2 B)
 *  				 droplines 1 # (2 B)
 *   				 number of KTC iterations (1 B)
 *     				 exposure configuration (2 B)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */

icu_status_code asw_PUS_8_1_4_2_SCE_KTCEXP(telecommand_t tc);

/**
 * \brief This functions implements the NISC_0403 TC: send SCE_EXP command.
 * It prepares the command tab and send it to the selected DPU/DPUs and DCU/DCUs.
 * \param[in] tc : the TC array, from which data is extracted:
 * 					[DPU number (1 B)
 * 					 DCU number (1 B) - DCU = 0 (broadcast); 1 ... 8
 * 					 reset frame # (2 B)
 *  				 droplines 1 # (2 B)
 *   				 groups # (2 B)
 *    				 reads per group (2 B)
 *     				 intergroup DropLines2 (2 B)
 *     				 intergroup DropLines3 (1 B)
 *     				 exposure configuration (2 B)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_4_3_SCE_EXP(telecommand_t tc);


/**
 * \brief This functions implements the NISC_0404 TC: send SCE_FMODE command.
 * It prepares the command tab and send it to the selected DPU/DPUs and DCU/DCUs.
 * \param[in] tc : the TC array, from which data is extracted:
 * 					[DPU number (1 B) - 0. 1. 2
 * 					 DCU number (1 B) - DCU = 0 ... 8
 * 					 SCA mode (2 B)
 * 					 simulated pattern mode (1 B) - 0 / 1
 * 					 simulated pattern mode Ramp Frame to Frame (2 B)
 * 					 simulated pattern mode first frame pixel start (2 B)
 * 					 preamp Gain (2 B)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_4_4_SCE_FMODE(telecommand_t tc);

/**
 * \brief This functions implements the NISC_0407 TC: send SCE_BOOT command.
 * It prepares the command tab and send it to the selected DPU/DPUs and DCU/DCUs.
 * \param[in] tc : the TC array, from which data is extracted:
 * 					[DPU number (1 B)
 * 					 DCU number (1 B) - DCU = 1 ... 8 (no broadcast)
 * 					 boot mode (1 B): 1 = hard SCE boot; 2 = soft SCE boot]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_4_7_SCE_BOOT(telecommand_t tc);

/**
 * \brief This functions implements the NISC_0408 TC: send SCE_ALIVE command.
 * It prepares the command tab and send it to the selected DPU/DPUs and DCU/DCUs.
 * \param[in] tc : the TC array, from which data is extracted:
 * 					[DPU number (1 B)
 * 					 DCU number (1 B) - DCU = 1 ... 8 (no broadcast)]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_4_8_SCE_ALIVE(telecommand_t tc);

/**
 * \brief This functions implements the NISC_0409 TC: send SCE_SHSK command.
 * It prepares the command tab and send it to the selected DPU/DPUs and DCU/DCUs.
 * \param[in] tc : the TC array, from which data is extracted:
 * 					[DPU number (1 B)
 * 					 xx (2 B): 0 = stop scanner; xx = scanner period in seconds]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_4_9_SCE_SHSK(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_SCE_H_ */
