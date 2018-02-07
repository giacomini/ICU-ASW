/**
 * \file	PUS_Service_3.h
 *
 * \brief	PUS Service 3
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Apr 30, 2015
 */

#ifndef PUS_SERVICE_3_H_
#define PUS_SERVICE_3_H_

#include "PUS_3_DB_def.h"
#include "tm.h"
#include "ICU_config.h"
#include "tc_header.h"

/**
 *  \ingroup PUS_3_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief This function implements the PUS Service 3.1: create a new HK report.
 * The report generation is disabled.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_1_set_new_HK_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.3: clear the content of an
 * existing HK report.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_3_clear_HK_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.5: enable the generation of
 * an existing HK report.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_5_enable_HK_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.6: disable the generation
 * of an existing HK report.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_6_disable_HK_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.9: request of a TM packet
 * containing a packet description
 * If more than a SID is requested, several TM(3,10) packets will be generated (one for each SID).
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_9_get_HK_report_def(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.10: produce the TM packet
 * requested by TC(3.9).
 * Not requested directly through TC.
 * \param[in] APID APID number
 * \param[in] SID_number Report ID
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_10_TM_HK_report_def(uint16_t APID,
		uint16_t SID_number);

/**
 * \brief This function implements the PUS Service 3.25: produce a list of TM
 * packets. Not requested directly through TC.
 * NOTE: the following checks must be done before calling this function:
 *  1 APID_OUT_OF_BOUNDS
 *  2 SID_OUT_OF_BOUNDS
 *  3 SID_NOT_EXISTING
 *  4 SID_NOT_ENABLED
 * \param[in] APID APID number
 * \param[in] SID_number Report ID
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_25_TM_HK_report(uint16_t APID, uint16_t SID_number);

/**
 * \brief This function implements the PUS Service 3.130: set the frequencies of
 *array of SID.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_130_set_HK_frequency(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.135: request of a TM(3,136)
 * packet with the list of enabled SID.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_135_get_enabled_HK(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.136: produce the TM packet
 * requested by TC(3.135).
 * Not requested directly through TC.
 * \param[in] APID APID number
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_136_TM_HK_active_rep(uint16_t APID);

/**
 * \brief This function implements the PUS Service 3.140: produce one occurrence of an
 * existing TM packet.
 * \param[in] tc TC sent by the spacecraft.
 * \return ICU_SUCCESSFUL if is successfully executed, an error code in case of an error.
 */
icu_status_code asw_PUS_3_140_get_HK_onDemand(telecommand_t tc);

/**
 * \}
 */

/*
 * private functions
 */
int16_t asw_absval(int16_t i);

#endif /* PUS_SERVICE_3_H_ */
