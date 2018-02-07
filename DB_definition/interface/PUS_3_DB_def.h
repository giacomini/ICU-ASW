/**
 * \file	PUS_3_DB_def.h
 *
 * \brief	Definition of PUS 3 ancillary variables and functions.
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Apr 24, 2015
 */

#ifndef PUS_DB_DEF_H_
#define PUS_DB_DEF_H_

#include "db_type_def.h"
#include "tm.h"

/**
 *  \ingroup PUS_3_SUPPORT_CONSTANTS
 *  \{
 */
/**
 * constants for both HK and DIAG packets
 */
#define TM_QUEUE_1_MAX_COLLECTION_INT 5U /* seconds */
#define TM_QUEUE_2_MAX_COLLECTION_INT 30U /* seconds */

/* 1006 Bytes - 2 Bytes for SID number */
#define TM_3_MAX_DATA_LENGTH_BYTE 1004U // Bytes
#define TM_REPORT_MAX_PARAM_NUMBER 502U //!< TM_3_MAX_DATA_LENGTH_BYTE / 2

/**
 * constants for HK packets
 */

/*
 *  TBChecked, Confirmed, Agreed...
 */
#define TM_MAX_REPORT_NUMBER 10U //32U
#define TM_MAX_ON_DEMAND_REPORT_NUMBER 32U

/**
 * constants for diagnostic packets
 */
#define TM_DIAG_MAX_REPORT_NUMBER 10U
#define TM_DIAG_MAX_SIMULT_REPORT_NUMB 4U //!< TM_DIAG_MAX_SIMULTANEOUS_REPORT_NUMB
#define TM_DIAG_MAX_SINGLE_PARAM_NUMBER 10U
#define TM_DIAG_MAX_FIXED_ARR_NUMBER 2U
#define TM_DIAG_MAX_REPETITION_NUMBER 480U
#define TM_DIAG_MAX_OVERSAMP_PAR_NUMB 10U //!< TM_DIAG_MAX_OVERSAMPLED_PARAM_NUMBER
#define TM_DIAG_STORED_DATA_HEAD_OFFSET (5U + (4U * (TM_DIAG_MAX_FIXED_ARR_NUMBER))) //!< TM_DIAG_STORED_DATA_HEADER_OFFSET

#define TM_DIAG_FLAG_PKT_WRITTEN_OFFSET 1U
#define TM_DIAG_FLAG_ONE_SHOT_OFFSET    2U
#define TM_DIAG_FLAG_ICU_PAR_OFFSET     3U
#define TM_DIAG_FLAG_FA1_COMPL_OFFSET   4U
#define TM_DIAG_FLAG_FA2_COMPL_OFFSET   5U

enum {
	TM_HK_SID_ICU_DPU_STATUS_IDX = 0,
	TM_HK_SID_DPU_DCU_HK_IDX,
	TM_HK_SID_SCE_TAB1_HK_IDX,
	TM_HK_SID_SCE_TAB2_HK_IDX,
	TM_HK_SID_SCE_TAB3_HK_IDX,
	TM_HK_SID_DBB_DRB_STATUS_IDX,
	TM_HK_SID_DPU_BSW_IDX,
	TM_HK_SID_NUMBER,
} TM_HK_harcoded_SID;

enum {
	TM_DIAG_SID_FWA_IDX = 0U,
	TM_DIAG_SID_GWA_IDX = 1U,
	TM_DIAG_SID_CU1_IDX = 2U,
	TM_DIAG_SID_CU2_IDX = 3U,
	TM_DIAG_SID_CU3_IDX = 4U,
	TM_DIAG_SID_CU4_IDX = 5U,
	TM_DIAG_SID_CU5_IDX = 6U,
	TM_DIAG_SID_NUMBER = 7U,
} TM_DIAG_harcoded_SID;

/**
 * \}
 */

/**
 *  \ingroup PUS_3_SUPPORT_TYPES
 *  \{
 */

/**
 * \struct TM_SID_DATA_t
 * \brief description of a parameter in TM packet data source: table (monitored or not),
 *  index inside that table
 */
typedef struct {
	/* param_ID */
	uint16_t DB_index :15;
	uint16_t DB_table :1; /* 1: monitored; 0: not monitored */
} TM_SID_data_t;

/**
 * \struct HK_SID_t HK_SID_t
 * \brief SID structure: description of a SID: SID number, collection interval,
 *  number of parameters, data structure, counter for generation
 *  NOTE: SID number starts from 1.
 */
typedef struct {
	uint16_t SID_number;
	uint16_t collection_interval;
	uint16_t param_number;
	uint16_t generation_cnt;
	TM_SID_data_t SID_data[TM_REPORT_MAX_PARAM_NUMBER];
} HK_SID_t;

/**
 * \struct DIAG_SID_t
 * \brief DIAG SID structure: description of a TM diagnostic SID: SID number, collection interval,
 *  number of parameters, data structure, counter for generation
 *  NOTE: SID number starts from 1.
 */
typedef struct {
	uint16_t SID_number;
	uint16_t collection_interval;
	uint16_t generation_cnt;
	uint16_t param_number;
	TM_SID_data_t SID_data[TM_DIAG_MAX_SINGLE_PARAM_NUMBER];
	uint16_t fixed_array_number;
	uint16_t os_param_number[TM_DIAG_MAX_FIXED_ARR_NUMBER];
	uint16_t os_repetition_number[TM_DIAG_MAX_FIXED_ARR_NUMBER];
	uint16_t fa_param_system; /* 0: ICU; 1: DPUs */
	uint16_t fa_length[TM_DIAG_MAX_FIXED_ARR_NUMBER];
	TM_SID_data_t DIAG_data[TM_DIAG_MAX_FIXED_ARR_NUMBER][TM_DIAG_MAX_OVERSAMP_PAR_NUMB];
} DIAG_SID_t;

/**
 * \}
 */

/**
 *  \ingroup PUS_3_SUPPORT_VARIABLES
 *  \{
 */
extern HK_SID_t SID[APID_NUMBER - ICU_ASW_APID][TM_MAX_REPORT_NUMBER];
extern DIAG_SID_t diag_SID[APID_NUMBER - ICU_ASW_APID][TM_DIAG_MAX_REPORT_NUMBER];

/**
 * \brief enable/disable flag: TM_report_flag=(bit_0, ..., bit_n-1)
 * SID_1 = bit_n-1  ..... SID_n = bit_0
 */
extern uint32_t TM_report_flag[APID_NUMBER - ICU_ASW_APID];
extern uint32_t TM_DIAG_report_flag[APID_NUMBER - ICU_ASW_APID];
/**
 * \brief existing/not existing flag (hardcoded or user-created): to avoid overwriting
 *  TM_report_exists_flag=(bit_0, ..., bit_n-1)
 *  SID_1 = bit_n-1  ..... SID_n = bit_0
 */
extern uint32_t TM_report_exists_flag[APID_NUMBER - ICU_ASW_APID];
extern uint32_t TM_DIAG_report_exists_flag[APID_NUMBER - ICU_ASW_APID];
/**
 * \brief Variable foreseen to keep trace of the actual number of report - eventually
 *  not ordered!
 */
extern uint8_t TM_report_number[APID_NUMBER - ICU_ASW_APID];
extern uint8_t TM_DIAG_report_number[APID_NUMBER - ICU_ASW_APID];

/**
 * \brief Storing space for diagnostic packets.
 * First two bytes of each row contain the SID of the collected data.
 * If zero, the row is free.
 */
extern uint8_t diag_sampling_space[TM_DIAG_MAX_SIMULT_REPORT_NUMB][TM_DIAG_STORED_DATA_HEAD_OFFSET
		+ TM_3_MAX_DATA_LENGTH_BYTE];
/**
 * \brief Flag for the diagnostic data storage:
 * bit n = 0 --> data in storing for SID n
 * bit n = 1 --> data storage finished for SID n
 */
extern uint8_t diag_data_sampling_status;
/**
 * \brief Flag for the diagnostic packet preparation:
 * bit n = 0 --> TM(3.26) not prepared for SID n
 * bit n = 1 --> TM(3.26) prepared for SID n
 */
extern uint8_t diag_data_3_26_packet_status;
/**
 * \}
 */

/**
 *  \ingroup PUS_3_SUPPORT_FUNCTIONS
 *  \{
 */
/**
 * \brief Function for the initialization of PUS 3 TM reports.
 *
 * This function initializes the structures containing the monitored and non-monitored parameters.
 * The monitored DB contains parameters read from DAS.
 * \return none
 */
void asw_PUS_3_initialize_HK_reports(void);

/**
 * \brief Function for the initialization of the flags (existence and enabling).
 * \param[in] sid The SID number of the HK packet to be enabled
 * \return none
 */
void asw_PUS_3_enable_HK_report_flag(uint16_t sid);

/**
 * \brief Function used to disable generation of an HK packet.
 * \param[in] sid The SID number of the HK packet to be disabled
 * \return none
 */
void asw_PUS_3_disable_HK_rep_flag(uint16_t sid);

/**
 * \brief Function for the initialization of PUS 3 diagnostic reports.
 *
 * This function initializes the structures containing the parameters and the fixed arrays.
 * \return none
 */
void asw_PUS_3_init_DIAG_reports(void);

/**
 * \brief Function for the initialization of the flags (existence and enabling) of diagnostic system.
 * \param[in] sid The SID number of the DIAG packet to be enabled
 * \return none
 */
void asw_PUS_3_enable_DIAG_rep_flag(uint16_t sid);

/**
 * \brief Function used to disable generation of an DIAG packet through the enabling flag.
 * \param[in] sid The SID number of the DIAG packet to be disabled
 * \return none
 */
void asw_PUS_3_disable_DIAG_rep_flag(uint16_t sid);

/**
 * \brief Function used to check if a DIAG packet is enabled or not.
 * \param[in] sid The SID number of the DIAG packet to be checked
 * \return status (0: disabled, 1: enabled)
 */
uint8_t asw_PUS_3_check_DIAG_rep_stat(uint16_t sid);

/**
 * \brief Evaluate the DB index from parameter ID.
 *
 * This function evaluates the index inside the monitored DB table (essentially parameters read from DAS).
 * \param[in] param_ID parameter ID
 * \return index in the monitored DB
 */
uint16_t asw_PUS_3_ID_to_mon_DB_idx(uint16_t param_ID);

/**
 * \brief Evaluate the DB index from parameter ID.
 *
 * This function evaluates the index inside the not-monitored DB table.
 * \param[in] param_ID parameter ID
 * \return index in the not-monitored DB
 */
uint16_t asw_PUS_3_ID_to_not_mon_DB_idx(uint16_t param_ID);

/**
 * \}
 */

/*
 * Private functions
 */
void asw_PUS_3_initialize_HK_SID_1(void);
void asw_PUS_3_initialize_HK_SID_2(void);
void asw_PUS_3_initialize_HK_SID_3(void);
void asw_PUS_3_initialize_HK_SID_4(void);
void asw_PUS_3_initialize_HK_SID_5(void);
void asw_PUS_3_initialize_HK_SID_6(void);
void asw_PUS_3_initialize_HK_SID_7(void);
uint16_t asw_bitCount(uint32_t value);

#endif /* PUS_DB_DEF_H_ */
