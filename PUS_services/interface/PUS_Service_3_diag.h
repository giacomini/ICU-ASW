/**
 * \file	PUS_Service_3_diag.h
 *
 * \brief	PUS Service 3: diagnostic services
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Jun 9, 2017
 */

#ifndef PUS_SERVICE_3_DIAG_H_
#define PUS_SERVICE_3_DIAG_H_

#include "../../rtems_configuration.h"

/**
 *  \ingroup PUS_3_DIAG_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief This function implements the PUS Service 3.2: create a new DIAG report.
 * The report generation is disabled.
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 			  [APID (2 B),
 *      	  SID: number of reports (2 B),
 * 			  collection interval (2 B),
 * 			  parameters number (2 B),
 *  		  parameters IDs array (par_num * 2 B),
 *  		  number of Fixed Arrays (NFA) (2 B),
 *  		  [number of repeted sampling (NREP) (2 B),
 *  		   number of parameters to be oversampled (NPAR) (2B),
 *  		   IDs of parameters to be oversampled (NPAR * 2 B)] * NFA
 *  		  ]
 * \return status
 */
icu_status_code asw_PUS_3_2_set_new_DIAG_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.4: clear the content of an
 * existing DIAG report.
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 					[APID (2 B),
 * 					 NSID - number of reports to clear (1 B)
 *					 SID - array of reports ID (NSID * 2 B)]
 * \return status
 */
icu_status_code asw_PUS_3_4_clear_DIAG_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.7: enable the generation of
 * an existing DIAG report.
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 					[APID (2 B),
 * 					 NSID - number of reports to enable (1 B)
 *					 SID - array of reports ID (NSID * 2 B)]
 * \return status
 */
icu_status_code asw_PUS_3_7_enable_DIAG_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.8: disable the generation
 * of an existing DIAG report.
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 					[APID (2 B),
 * 					 NSID - number of reports to disable (1 B)
 *					 SID - array of reports ID (NSID * 2 B)]
 * \return status
 */
icu_status_code asw_PUS_3_8_disable_DIAG_report(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.11: request of a DIAG packet
 * containing a packet description
 * If more than a SID is requested, several TM(3,10) packets will be generated (one for each SID).
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 					[APID (2 B),
 * 					 SID (2 b)]
 * \return status
 */
icu_status_code asw_PUS_3_11_get_DIAG_rep_def(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.12: produce the DIAG packet
 * requested by TC(3.11).
 * Not requested directly through TC.
 * \param[in] APID     2 Bytes
 * \param[in] SID_number report number (2B)
 * \return status
 */
icu_status_code asw_PUS_3_12_TM_DIAG_rep_def(uint16_t APID,
		uint16_t SID_number);

/**
 * \brief This function implements the PUS Service 3.25: produce a list of TM
 * packets. Not requested directly through TC.
 * NOTE: the following checks must be done before calling this function:
 *  1 APID_OUT_OF_BOUNDS
 *  2 SID_OUT_OF_BOUNDS
 *  3 SID_NOT_EXISTING
 *  4 SID_NOT_ENABLED
 * \param[in] APID 			apid number (2B)
 * \param[in] SID_number	sid number (2B)
 * \return status
 */
icu_status_code asw_PUS_3_26_TM_DIAG_report(uint16_t APID, uint16_t SID_number);

/**
 * \brief This function implements the PUS Service 3.131: set the frequencies of
 * array of SID.
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 					[APID (2 B),
 * 					 NSID - number of reports  (1 B)
 *					 [SID - array of reports ID (2 B),
 *					 collection interval (2B)] * NSID]
 * \return status
 */
icu_status_code asw_PUS_3_131_set_DIAG_freq(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.137: request of a TM(3,138)
 * packet with the list of enabled SID.
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 					[APID (2 B)]
 * \return status
 */
icu_status_code asw_PUS_3_137_get_enabled_DIAG(telecommand_t tc);

/**
 * \brief This function implements the PUS Service 3.138: produce the DIAG packet
 * requested by TC(3.137).
 * Not requested directly through TC.
 * \param[in] APID   2 Bytes
 * \return status
 */
icu_status_code asw_PUS_3_138_TM_DIAG_en_rep(uint16_t APID);

/**
 * \brief This function implements the PUS Service 3.140: produce one occurrence of an
 * existing TM packet.
 *
 * \param[in] tc : the received telecommand, from which data is extracted:
 * 					[NSID - number of reports  (1 B)
 *					 [APID (2 B), SID (2 B)] * NSID]
 * \return status
 */
icu_status_code asw_PUS_3_141_get_DIAG_onDemand(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_3_DIAG_H_ */
