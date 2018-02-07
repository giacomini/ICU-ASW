/**
 * \file	tm.h
 *
 * \brief	This file contains declaration of structures and functions for HK TM packet generation
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 * \author	Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 * Created: 	5-05-2015
 *
 */
#ifndef TM_H_
#define TM_H_

#include <rtems.h>
/*
 * global variables declaration
 * from EUCL-TAST-ICD-1-003 V.6 26/02/2016
 */

/**
 *  \ingroup TM_HANDLER_CONSTANTS
 *  \{
 */

#define TM_PKT_MAX_DATA_LENGTH_BYTES 1006U //!< Maximum size in bytes of data area for a TM PUS packet
#define TM_PKT_MAX_DATA_LENGTH_DW 503U  //!< Maximum number of 1553 Data words for data area of a PUS TM packet (1004 / 2)
#define DAS_PARAM_NM 49U //!< Number of ICU parameters that could be read by ICU (Max parameter number is 255)
#define TM_MAX_SEQ_COUNTER 0x4000U //!< Modulo to be applied to telemetry sequence counter.  Starts from 0 to (0x3FFF + 1)
//#define DAS_PARAM_NM 8U Used to test home search and home check procedure

/**
 * \brief Offsets for Packet_ID and Packet_sequence_control words in the TM Primary Header
 */
#define TM_VERSION_OFFSET 13U //!< Offset to evaluate TM_VERSION data
#define TM_TYPE_OFFSET 12U //!< Offset to evaluate TM_TYPE data
#define TM_DF_HEADER_FLAG_OFFSET 11U //!< Offset to evaluate TM_DF_HEADER_FLAG data
#define TM_GROUPING_FLAG_OFFSET 14U //!< Offset to evaluate TM_GROUPING_FLAG data
#define TM_SSCOUNT_OFFSET 0U //!< Offset to evaluate TM_SSCOUNT data
/**
 * \}
 */

/**
 *  \ingroup TM_HANDLER_TYPES
 *  \{
 */

/**
 * \enum	APID_LIST
 *
 * \brief	APID List definition
 */
typedef enum {
	ICU_ASW_APID = 0x520U, //!< Apid assigned to ICU_ASW
	APID_NUMBER, //!< Next Apid number
} APID_LIST;

/**
 * \struct	TM_packet_t
 *
 * \brief	Definition of PUS TM packet
 */
typedef struct {
	/* source packet header */
	uint16_t TM_packet_ID;
	uint16_t TM_packet_seq_control;
	uint16_t TM_packet_length;
	/* data field header */
	uint8_t TM_PUS_version;
	uint8_t TM_service_type;
	uint8_t TM_service_subtype;
	uint8_t TM_destination_ID;
	uint32_t TM_OBT_coarse_time;
	uint16_t TM_OBT_fine_time;
	/* data component, in DW */
	uint16_t TM_data_source[TM_PKT_MAX_DATA_LENGTH_DW];
	// .. spare to pad the all packet to an integer number of DW
	uint8_t *spare; /* in funzione della lunghezza dei dati... */
	uint16_t packet_error_control;
} TM_packet_t;
/**
 * \}
 */

/**
 *  \ingroup TM_HANDLER_VARIABLES
 *  \{
 */
/* global TM packets counter */
extern uint16_t TM_ss_counter[APID_NUMBER - ICU_ASW_APID];

/*
 * Matrix containing last three values of DAS analogical data
 * First index is equal to (number of ICU parameters - 2(n_of digital parameters) - 4(number of OBT parameters)
 */
extern uint16_t last_three_das_values[DAS_PARAM_NM - 6][3];

/**
 * \}
 */

/**
 *  \ingroup TM_HANDLER_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to create a byte stream starting from telemetry PUS packet
 * \param[in] TM_pack Telemetry packet in PUS format
 * \param[out] buffer data stream of the PUS telemetry packet
 * \param[in] length size in word of the data area of Telemetry packet
 */
void asw_serialize_TM(TM_packet_t *TM_pack, uint8_t *buffer, uint16_t length);

/**
 * \brief Function used to create header of a TM PUS packet
 * \param[out] TM_pack Telemetry packet in PUS format
 * \param[in] type PUS service type
 * \param[in] sub_type PUS subservice type
 * \return none
 */
void asw_PUS_TM_set_data_header(TM_packet_t *TM_pack, uint8_t type,
		uint8_t sub_type);

/**
 * \brief Function used to create an array containing the DB index of DAS TM data to be acquired.
 * \param[in] das_ids Array containg the DAS parameter TM IDs
 * \param[out] icu_db_idx Array that contains the DB index of the DAS parameters present in the das_ids array.
 * \param[in] nr_par Number of parameters that are contained in the structure
 * \return none
 */
void asw_from_das_IDs_to_ASW_Idxs(uint32_t das_ids[DAS_PARAM_NM],
		uint8_t icu_db_idx[DAS_PARAM_NM], uint8_t nr_par);

/**
 * \brief Function used to store data TM data acquired from DAS in the DB of parameters.
 * \param[in] acq_par_ids array containing the DB index of the acquired data
 * \param[in] values Data acquired during last TM cycle acquisition
 * \param[in] nr_par Number of the acquired parameters
 * \param[in] cycle_number Array position (2nd parameter of last_three_das_values matrix) where values will be copied.
 * \return none
 */
void asw_copy_data_in_das_db(uint8_t acq_par_ids[DAS_PARAM_NM],
		uint16_t values[DAS_PARAM_NM], uint8_t nr_par, uint8_t cycle_number);

/**
 * \brief Function used to create the DAS ID table to be used for TM acquisition starting from ICU parameter ID.
 * \param[in] icu_ids Array containing the ICU parameter ID to be translate
 * \param[out] das_ids Array containing the DAS parameter IDs starting from icu_ids array.
 * \param[in] nr_par Number of parameters
 * \return none
 */
void asw_from_ASW_IDs_to_das_IDs(uint16_t icu_ids[DAS_PARAM_NM],
		uint32_t das_ids[DAS_PARAM_NM], uint8_t nr_par);

/**
 * \brief Function used to calculate median of last three values
 * \param[in] idx Position inside matrix containing values
 * \return median
 */
uint16_t asw_calculate_median(uint8_t idx);

/**
 * \brief Function used to check status of CMD_DAS_ALARMS_READ_REQ register
 * \return none
 */
void asw_monitor_das_alarms(void);

/**
 * \brief Function used to monitor temperature of CDPU module
 * \return none
 */
void asw_monitor_cdpu_tmp(void);

/**
 * \brief Function used to check temperature of LVPS module
 * \return none
 */
void asw_monitor_lvps_tmp(void);

/**
 * \brief Function used to monitor errors on DPUs
 * \return none
 */
void asw_monitor_dpu_errors(void);

/**
 * \}
 */

#endif /* TM_H_ */
