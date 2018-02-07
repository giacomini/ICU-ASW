/**
 * \file    PUS_Service_8_1_DPU_BSW.h
 *
 * \brief	Definition of types and functions used by ICU-ASW to manage DPU BSW.
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 * Created: 	19-10-2017
 */

#ifndef PUS_SERVICE_8_1_DPU_BSW_H_
#define PUS_SERVICE_8_1_DPU_BSW_H_

/**
 * \brief This function implements the DPU BSW command that load the ASW image from EEPROM to RAM.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_11_1_load_ASW_img(telecommand_t tc);

/**
 * \brief This function implements the DPU BSW command that starts the ASW.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_11_3_jump_to_ASW(telecommand_t tc);

/**
 * \brief This function implements the DPU BSW command that manages the FDIR of the BSW.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_8_1_11_7_BSW_FDIR(telecommand_t tc);


#endif /* PUS_SERVICE_8_1_DPU_BSW_H_ */
