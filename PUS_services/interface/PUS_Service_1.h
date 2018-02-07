/**
 * \file	PUS_Service_1.H
 *
 * \brief	PUS Service 1 management
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Sep 11, 2015
 */

#ifndef PUS_SERVICE_1_H_
#define PUS_SERVICE_1_H_

#include "tm.h"
#include "ICU_config.h"
#include <rtems.h>

/**
 *  \ingroup PUS_1_CONSTANTS
 *  \{
 */
enum {
	TM_1_8_ICU_STATUS_ERROR = 10U, //!< ICU status not compatible with TC
	TM_1_8_TC_PARAM_SIZE_ERROR, //!< size of TC parameters is not as expected
	TM_1_8_TC_PARAM_OUT_OF_BOUNDS, //!< TC parameter is out of bounds
	TM_1_8_BC_1553_RW_ERROR, //!< Error on 1553 bus communication with DPUs (from DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes)
	TM_1_8_PUS3_REPORT_SIZE_ERROR, //!< the requested size for new SID is too big
	TM_1_8_PUS3_TM_PKT_NOT_CREATED, //!< TM packet was not created
	TM_1_8_PUS3_TOO_MANY_HK_REP, //!< maximum number of HK SID reached
	TM_1_8_PUS3_TOO_MANY_DIAG_REP, //!< maximum number of DIAG SID reached
	TM_1_8_PUS3_TOO_MANY_DIAG_ON, //!< too many diagnostic packets enable
	TM_1_8_PUS6_DPU_CRC_ERROR, //!< The CRC of data dumped from DPUs is not correct
	TM_1_8_PUS6_DPU_DUMP_LGHT_ERROR, //!< The received data from DPU dump is not as expected
	TM_1_8_PUS6_EEPROM_READ_ERROR, //!< error from EEPROM reading
	TM_1_8_PUS6_TM_PKT_NOT_CREATED, //!< TM(6,6/10) packet was not created
	TM_1_8_DPU_CMD_ERROR, //!< Error on DPU command execution (as reported by CMD_VER_TAB)
	TM_1_8_DPU_BSW_CMD_ERROR, //!< Error on DPU BSW command execution (as reported by CMD_VER_TAB_1/2)
	TM_1_8_EXEC_ERROR, //!< Error during execution of a telecommand
	TM_1_8_LAST_ENTRY //!< Last entry, it is not and error
} TM_error_code_list;

#define PUS_1_MAX_ERROR_PARAM_LENGTH 15U // TBCCCCCCCCCC !!!!!!!!  FIXME
/**
 * \}
 */

/**
 *  \ingroup PUS_1_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to create PUS 1,1 or 1,7 packet
 * \param[in] TC_packet_ID TC packet ID data
 * \param[in] TC_seq_control TC sequence control data
 * \param[in] dest_ID TC destination ID data
 * \param[in] sub_type PUS subtype
 * \return ICU_SUCCESSFUL if OK, an error code in case of an error.
 */
icu_status_code asw_PUS_1_success(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint8_t sub_type);

/**
 * \brief Function used to create PUS 1,2 or 1,8 packet
 * \param[in] TC_packet_ID TC packet ID data
 * \param[in] TC_seq_control TC sequence control data
 * \param[in] dest_ID TC destination ID data
 * \param[in] error_code the error
 * \param[in] param_length parameters size in bytes
 * \param[in] param_list array containing the parameters to be passed
 * \param[in] sub_type PUS subtype
 * \return ICU_SUCCESSFUL if OK, an error code in case of an error.
 */
icu_status_code asw_PUS_1_failure(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint16_t error_code,
		uint8_t param_length, const uint8_t *param_list, uint8_t sub_type);
/**
 * \}
 */

/**
 *  \ingroup PUS_1_PUBLIC_FUNCTION
 *  \{
 */

/**
 * \brief This function generates the TM(1,1) packet.
 * \param[in] TC_packet_ID TC packet ID data
 * \param[in] TC_seq_control TC sequence control data
 * \param[in] dest_ID TC destination ID data
 * \return ICU_SUCCESSFUL if OK, an error code in case of an error.
 */
icu_status_code asw_PUS_1_1_TM_TC_accept_ok(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID);

/**
 * \brief This function generates the TM(1,2) packet.
 * \param[in] TC_packet_ID TC packet ID data
 * \param[in] TC_seq_control TC sequence control data
 * \param[in] dest_ID TC destination ID data
 * \param[in] error_code the error
 * \param[in] param_length parameters size in bytes
 * \param[in] param_list array containing the parameters to be passed
 * \return: ICU_SUCCESSFUL if OK, an error code in case of an error.
 */
icu_status_code asw_PUS_1_2_TM_TC_accept_fail(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint16_t error_code,
		uint8_t param_length, const uint8_t *param_list);

/**
 * \brief This function generates the TM(1,7) packet.
 * \param[in] TC_packet_ID TC packet ID data
 * \param[in] TC_seq_control TC sequence control data
 * \param[in] dest_ID TC destination ID data
 * \return ICU_SUCCESSFUL if OK, an error code in case of an error.
 */
icu_status_code asw_PUS_1_7_TM_TC_exec_ok(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID);

/**
 * \brief This function generates the TM(1,8) packet.
 * \param[in] TC_packet_ID TC packet ID data
 * \param[in] TC_seq_control TC sequence control data
 * \param[in] dest_ID TC destination ID data
 * \param[in] error_code the error
 * \param[in] param_length parameters size in bytes
 * \param[in] param_list array containing the parameters to be passed
 * \return: ICU_SUCCESSFUL if OK, an error code in case of an error.
 */
icu_status_code asw_PUS_1_8_TM_TC_exec_fail(uint16_t TC_packet_ID,
		uint16_t TC_seq_control, uint8_t dest_ID, uint16_t error_code,
		uint8_t param_length, const uint8_t *param_list);

/**
 * \}
 */

#endif /* PUS_SERVICE_1_H_ */
