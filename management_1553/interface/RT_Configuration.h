/**
 * \file	RT_Configuration.h
 *
 * \brief	This file manages the configuration of the RT manager
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: 28/gen/2016
 */

#ifndef RT_CONFIGURATION_H_
#define RT_CONFIGURATION_H_

#include "milbus.h"
#include "ICU_config.h"

/**
 *  \ingroup RT_HANDLER_CONSTANTS
 *  \{
 */

/**
 * \brief Size of message table received by RT used by ASW
 */
#define RT_MESSAGES_TO_BE_STORED 100U //!< Size of table used by ICU ASW to read 1553 messages received by Remote Terminal
/**
 * \brief Communication Frames used by ICU-ASW to be synchronized with S/C
 */
#define ATR_1_CF 8U //!< Communication frame when ICU ASW should restart task_ATR to create new TM block and ATR
#define ATR_2_CF 26U //!< Communication frame when ICU ASW should restart task_ATR to create new TM block and ATR
#define ATC_1_CF 18U //!< Communication frame when ICU ASW should check the reception of an ATC
#define ATC_2_CF 36U //!< Communication frame when ICU ASW should check the reception of an ATC
#define OBT_CF 34U //!< Communication frame when ICU ASW should read the OBT sent by spacecraft
#define DTD_CF 11U //!< Communication frame when ICU ASW should evaluate the DTD reading subaddress 27T
#define DPU_DUMP_CF	52U //!< Communication frame when ICU ASW should evaluate the memory dump packets received by DPUs

/**
 * \enum	ModeCode_t
 * \brief	This enum defines the available Mode Codes required by the
 * protocol REQ[EUCL-SVM-CDMS-BUS-REQ-0070]
 */
typedef enum {
	MC_SYNCHRONIZE_WITHOUT_DATA = 0x1U, //!< SYNCHRONIZE_WITHOUT_DATA Mode code
	MC_TRANSMIT_STATUS_WORD = 0x2U, //!< TRANSMIT_STATUS_WORD Mode code
	MC_TRANSMITTER_SHUTDOWN = 0x4U, //!< TRANSMITTER_SHUTDOWN Mode code
	MC_OVERRIDE_TRANSMITTER = 0x5U, //!< OVERRIDE_TRANSMITTER Mode code
	MC_RESET_RT = 0x8U, //!< RESET_RT Mode code
	MC_SYNCHRONIZE_WITH_DATA = 0x11U, //!< SYNCHRONIZE_WITH_DATA Mode code
	MC_TRANSMIT_LAST_COMMAND = 0x12U //!< TRANSMIT_LAST_COMMAND Mode code
} ModeCode_t;
/**
 * \}
 */

/**
 *  \ingroup RT_HANDLER_VARIABLES
 *  \{
 */

/*Declaration of pointers to subaddress buffer*/
/**
 * \brief Pointers to the 1553 RT Receive Subaddress enabled
 */
extern uint32_t *ptr_tm_reset, *ptr_tc_rx, *ptr_dtd, *ptr_atc, *ptr_time_rx,
		*ptr_wrap_around_rx;

/**
 * \brief Pointers to the 1553 RT Transmit Subaddress enabled
 */
extern uint32_t *ptr_pus_3, *ptr_evt_pus_1, *ptr_hk_4, *ptr_hk_3, *ptr_hk_2,
		*ptr_hk_1, *ptr_dump, *ptr_diag_1, *ptr_diag_0, *ptr_dtc, *ptr_atr,
		*ptr_time_tx, *ptr_wrap_around_tx;

/**
 * \brief Variable that contains index of the next position to read new command
 * received (icu_mil_std_1553_rt_drv_get_last_msgs)
 */
extern uint16_t msg_index;

/**
 * \brief Variable used to define maximum number to read from msg_buffer
 * (...rt_get_last_msgs function)
 */
extern uint16_t number_of_msg;

/**
 * \brief table to manage last messages received by RT 1553
 */
extern RTCmdWordTableEntry_t msg_buffer[100];

/**
 * \brief Variable used to enable/disable the reset procedure of the telemetry
 * transfer protocol
 */
extern bool_t tm_reset_active;

/**
 * \}
 */

/**
 *  \ingroup RT_HANDLER_FUNCTIONS
 *  \{
 */

/**
 * \brief Configuration of 1553 Remote Terminal Core.
 * \param[out] sa_rx_en Receive subaddresses activated
 * \param[out] sa_tx_en Transmit subaddresses activated
 * \param[out] mc_en Mode codes activated
 * \return 0 if successfully executed, an error code otherwise.
 */
uint8_t asw_configure_rt(uint32_t *sa_rx_en, uint32_t *sa_tx_en,
		uint32_t *mc_en);

/**
 * \brief Reset all the starting parameters used by RT 1553 Core
 */
void asw_reset_rt(void);

/**
 * \brief Function used to reset the TM protocol, an ATR with reset bit set to 1
 * will be created
 */
void asw_telemetry_reset(void);

/**
 * \brief Function used to check if a new DTD is received. In this case create
 * DTC, copy TC from SA 10R and update 1553 Handler parameters (DTC and DTD)
 * \param[in] data Variable where copy TC data
 * \return 0 if function completed without errors, 1 otherwise.
 */
uint32_t asw_check_new_DTD(uint8_t *data);

/**
 * \brief Function used to create a Command Word
 * \param[in] RT_addr Remote Terminal address
 * \param[in] tr_bit 1 = Tx message, 0 = Rcv message
 * \param[in] sub_addr Subaddress number. 31 = Mode Code
 * \param[in] word_count Number of data word or number of mode code.
 * \return Command word with input arguments
 */
CommandWord_t asw_set_cmd_word(uint16_t RT_addr, uint16_t tr_bit,
		uint16_t sub_addr, uint16_t word_count);

/**
 * \brief function used to create Data Descriptor data (DTD, DTC, ATR, ATC)
 * \param[in] size Number of data word
 * \param[in] qos_err Bit used to set QoS or Error
 * \param[in] reset Bit used to reset counters
 * \param[in] mode Bit used to set mode
 * \param[in] sa subaddress number
 * \param[in] counter Command or telemetry counter
 * \return Formatted Data Descriptor
 */
TransferDescriptorDataWord_t
asw_set_transfer_descriptor(uint32_t size, uint32_t qos_err, uint32_t reset,
		uint32_t mode, uint32_t sa, uint32_t counter);

/**
 * \brief Function used to create a new ATR
 * \return ATR block_counter value
 */
uint8_t asw_create_ATR_counter(void);

/**
 * \brief function used to update the last ATC received from S/C
 * \return 0 if OK, 1 otherwise
 */
uint32_t asw_read_ATC(void);

/**
 * \brief Function used to implement MIL-STD-1553B data wrap-around on SA 30
 * \return 0 if OK, 1 otherwise
 */
uint32_t asw_rt_wrap_around(void);

/**
 * \brief Function to verify messages received from S/C
 * \param[in] last_rcv_msgs list of command words
 * \param[in] number_of_msgs number of new messages
 * \return 0 if OK, 1 otherwise
 */
uint32_t asw_check_last_rcv_cmd_word(
		const RTCmdWordTableEntry_t *last_rcv_msgs, uint16_t number_of_msgs);

/**
 * \brief Function used to read OnBoard Time received on SA 29R, generate events in case of errors
 * \return none
 */
void asw_read_last_OBT_received(void);

/**
 * \}
 */

#endif /* RT_CONFIGURATION_H_ */
