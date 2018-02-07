/**
 * \file	PUS_5_DB_def.h
 *
 * \brief	Definition of PUS 3 ancillary variables and functions.
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Sep 2, 2015
 */

#ifndef PUS_5_DB_DEF_H_
#define PUS_5_DB_DEF_H_

#include "db_type_def.h"
#include "tm.h"
#include "ICU_config.h"

/**
 *  \ingroup PUS_5_SUPPORT_CONSTANTS
 *  \{
 */
#define NI_ICU_EVENT_ID_MIN 256U //!< NI_ICU_EVENT_ID_MIN
#define NI_ICU_EVENT_ID_MAX 511U //!< NI_ICU_EVENT_ID_MAX
#define EVENT_MAX_PARAMETER_LENGTH 10U //!< EVENT_MAX_PARAMETER_LENGTH
/* total allowed events for NISP */
/*****************************************************************
 **** to be changed with the effective number of events!
 ****************************************************************/
#define MAX_EVENT_NUMBER 256U //!< MAX_EVENT_NUMBER
#define MAX_EVENT_FLAG_SIZE 8U //!< Number of elements of array of flags (256 / 32)
/**
 * \}
 */
/**
 *  \ingroup PUS_5_SUPPORT_TYPES
 *  \{
 */
/**
 * \enum EID_LIST
 * \brief List of all ICU events. This enumeration allows indexing in arrays.
 */
typedef enum {
	EVENT_TASK_NOT_STARTED,
	EVENT_EEPROM_ERROR,
	EVENT_EDAC_ERROR,
	EVENT_TM_ACQ_ERROR,
	EVENT_BC_1553_ERROR,
	EVENT_RT_1553_ERROR,
	EVENT_WATCHDOG_T1_EXPIRATION,
	EVENT_WHEEL_PROFILE_NOT_LOADED,
	EVENT_WHEELS_MOVEMENT_ERROR,
	EVENT_CU_WARNING,
	EVENT_WHEELS_WARNING,
	EVENT_TEMPERATURE_WARNING,
	EVENT_DAS_WARNING,
	EVENT_TEMPERATURE_ALARM,
	EVENT_WHEEL_NOTIFICATION,
	EVENT_ICU_SAFE_STATE,
	EVENT_DPU_TAB_ID_NOT_CORRECT,
	EVENT_NOT_NOMINAL_WPF_LOADED,
	EVENT_OBT_ERROR,
	EVENT_OBT_NOT_UPDATED,
	EVENT_SENDLIST_ERROR,
	EVENT_CMD_NOT_EXECUTED,
	EVENT_END_OF_EXPOSURE,
	EVENT_END_OF_MMU_TRANSMISSION,
	EVENT_STARTUP_COMPLETED,
	/* BSW events */
	/*
	 EVENT_BSW_BOOT_REPORT_SUCCESS,
	 EVENT_BSW_BOOT_REPORT_FAILURE,
	 EVENT_BSW_ERROR_TRAP,
	 EVENT_BSW_CORRECTABLE_EDAC_FAILURE,
	 EVENT_BSW_UNCORRECTABLE_EDAC_FAILURE,
	 */
	/* ASW general events */
	/*
	 EVENT_ASW_CORRECTABLE_EDAC_FAILURE,
	 EVENT_ASW_UNCORRECTABLE_EDAC_FAILURE,
	 */
	/*EVENT_ID_290 = 34,*//*To be enabled to use testService5()*/
	ICU_ASW_EVENT_NUMBER,
} EID_LIST;

/**
 * \struct event_tag_t
 * \brief Structure for the description of an event: ID, severity, "is error event" flag (y/n),
 * fields used to avoid massive repetitions of a single event.
 */

typedef struct {
	uint16_t event_ID;
	uint8_t event_priority;
	uint32_t last_transmission_seconds;
	uint32_t last_transmission_microseconds;
	uint32_t transm_delta_seconds; /* in seconds: TBC */
	uint32_t transm_delta_microseconds; /* in microseconds: TBC */
	uint16_t repetition_cnt;
	uint16_t repetition_maximum;
} event_tag_t;

/**
 * \}
 */

/**
 *  \ingroup PUS_5_SUPPORT_VARIABLES
 *  \{
 */
event_tag_t EID[ICU_ASW_EVENT_NUMBER];
uint32_t event_flag[MAX_EVENT_FLAG_SIZE];
/**
 * \}
 */

/**
 *  \ingroup PUS_5_SUPPORT_FUNCTIONS
 *  \{
 */
/**
 * \brief Function for the initialization of PUS 5 events.
 *
 * This function initializes the structures containing the events.
 * \return none
 */
void asw_PUS_5_initialize_EID(void);

/**
 * \brief Function for the initialization of the enable/disable flag.
 * \param[in] id The ICU eid index [see EID_LIST enumerator]
 * \return none
 */
void asw_PUS_5_enable_event(uint16_t id);

/**
 * \brief Function used to disable an event.
 * \param[in] id The ICU eid index [see EID_LIST enumerator]
 * \return none
 */
void asw_PUS_5_disable_event(uint16_t id);

/**
 * \brief Function to enable all dpu events and fdir.
 * \return none
 */
void asw_PUS_5_enable_dpu_events(void);

/**
 * \brief Function to disable all dpu events and fdir.
 * \return none
 */
void asw_PUS_5_disable_dpu_events(void);

/**
 * \}
 */

#endif /* PUS_5_DB_DEF_H_ */
