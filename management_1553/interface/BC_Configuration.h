/**
 * \file	BC_Configuration.h
 *
 * \brief	This file manages the configuration of the BC manager
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: 01/feb/2016
 */

#ifndef BC_CONFIGURATION_H_
#define BC_CONFIGURATION_H_

#include "milbus.h"
#include "ICU_config.h"

/**
 *  \ingroup BC_HANDLER_CONSTANTS
 *  \{
 */

/*Define RT address enabled*/
#define RT_BROADCAST_1553 31U //!< 1553 RT Address used for Broadcast messages
#define RT_DPU_1_NOM 1U //!< DPU_1 Nominal Unit RT Address
#define RT_DPU_1_RED 2U //!< DPU_1 Redundant Unit RT Address
#define	RT_DPU_2_NOM 3U //!< DPU_2 Nominal Unit RT Address
#define RT_DPU_2_RED 4U //!< DPU_2 Redundant Unit RT Address

/*Define transmit/receive bit*/
#define TRANSMIT_BIT_1553 1U //!< 1553 Command Word Transmit bit
#define RECEIVE_BIT_1553 0U //!< 1553 Command Word Receive bit

/* Maximum number of messages for sendlist and sendlist number */
#define MAX_NUMBER_OF_SENDLIST_MSGS 20U //!< Maximum number of messages to be inserted in a a sendlist supported by drivers
#define MAX_NUMBER_OF_SENDLIST 10U //!< Maximum number of sendlists supported by drivers

/* Number of communication frame */
#define COMMUNICATION_FRAME_NUMBER 60U //!< Number of communication frames

/* Clear all sendlists value */
#define CLEAR_BUS_PROFILE 255U //!< Value used to clear the 1553 BC Bus profile
/* Maximum number of BC messages */
#define MAX_BC_MESSAGES 175U //!< Maximum number of BC messages

/* Static messages index */
#define SYNCH_WITHOUT_DW_MSG_INDEX 0 //!< Index of the Synch without DW message
#define SYNCH_WITH_DW_START_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 1U) //!< Index of the first Synch with DW messages
#define SYNCH_WITH_DW_LAST_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 59U) //!< Index of the last Synch with DW messages

/* Memory load messages index */
#define MEM_LOAD_FIRST_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 60U) //!< Index of the first memory load messages (16 messages).

/* CMD message index */
#define CMD_MSG_INDEX_DPU_1 (SYNCH_WITHOUT_DW_MSG_INDEX + 76U) //!< Index of Command messages for DPU 1.
#define CMD_MSG_INDEX_DPU_2 (SYNCH_WITHOUT_DW_MSG_INDEX + 77U) //!< Index of Command messages for DPU 2.

/* ASW_CMD_VER or BSW_CMD_VER_1 */
#define CMD_VER_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 77U) //!< Index of ASW CMD_VER_TAB or BSW_CMD_VER_1 message. Add RT address to calculate indexes.

/* Time messages */
#define TIME_RCV_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 82U) //!< Index of the Broadcast Time message
#define TIME_RETRIEVE_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 82U) //!< Index of the Retrieve Time message. Add RT address to calculate indexes.

/* ASW_MONITOR_TAB or BSW_HK2 */
#define MON_TAB_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 86U) //!< Index of ASW_MON_TAB or BSW_HK2 message. Add RT address to calculate index

/* ASW_STATUS_TAB or BSW_HK1 */
#define STATUS_TAB_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 90U) //!< Index of ASW_STATUS_TAB or BSW_HK1 message. Add RT address to calculate index

/* ASW_ERROR_TAB or BSW_ERROR */
#define ERROR_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 94U) //!< Index of DPU ERROR_TAB messages. Add RT address to calculate index

/* ASW_DPU_HK */
#define HSK_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 98U) //!< Index of DPU_HSK_TAB message. Add RT address to calculate index

/* DPUs DCUs, SCEs and Memory Dump */
#define CPU_1_DCU_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 103U) //!< Index of the CPU#1 DCU#1 HK message. Add RT address to calculate index
#define CPU_1_SCE_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 111U) //!< Index of the CPU#1 SCE#1 HK message. Add RT address to calculate index
#define MEM_DUMP_MSG_INDEX_CPU1 (SYNCH_WITHOUT_DW_MSG_INDEX + 103U) //!< Index of the first memory dump message (max 16 messages) for CPU 1.

#define CPU_2_DCU_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 119U) //!< Index of the CPU#2 DCU#1 HK message. Add RT address to calculate index
#define CPU_2_SCE_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 127U) //!< Index of the CPU#2 SCE#1 HK message. Add RT address to calculate index
#define MEM_DUMP_MSG_INDEX_CPU2 (SYNCH_WITHOUT_DW_MSG_INDEX + 119U) //!< Index of the first memory dump message (max 16 messages) for CPU 2.

#define CPU_3_DCU_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 135U) //!< Index of the CPU#3 DCU#1 HK message. Add RT address to calculate index
#define CPU_3_SCE_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 143U) //!< Index of the CPU#3 SCE#1 HK message. Add RT address to calculate index
#define MEM_DUMP_MSG_INDEX_CPU3 (SYNCH_WITHOUT_DW_MSG_INDEX + 135U) //!< Index of the first memory dump message (max 16 messages) for CPU 3.

#define CPU_4_DCU_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 151U) //!< Index of the CPU#4 DCU#1 HK message. Add RT address to calculate index
#define CPU_4_SCE_1_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 159U) //!< Index of the CPU#4 SCE#1 HK message. Add RT address to calculate index
#define MEM_DUMP_MSG_INDEX_CPU4 (SYNCH_WITHOUT_DW_MSG_INDEX + 151U) //!< Index of the first memory dump message (max 16 messages) for CPU 4.

/* ASW_DBB_DRB TAB */
#define DBB_DRB_HSK_TAB_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 166U) //!< Index of DBB_DRB_HSK_TAB message. Add RT address to calculate index

/* BSW_CMD_VER_2 */
#define CMD_VER_2_MSG_INDEX (SYNCH_WITHOUT_DW_MSG_INDEX + 170U) //!< Index of BSW_CMD_VER_2 message. Add RT address to calculate index


/* Gap between messages inside a sendlist */
#define MSG_INTERGAP_DELAY 370U //!< Gap between messages in the sendlist
/* Value to assign 32 data word in a command word */
#define CMD_WORD_32_DW 0U //!< Value to create a 1553 message with 32 Data Word
/* Size of OBT TM packet */
#define DPU_OBT_TM_PKT_DW_SIZE 10U //!< Number of Data word used by DPUs for Time messages
#define MAX_MEM_DUMP_LOAD_MSGS 16U //!< Maximum number of messages for memory load and memory dump

/* Communication Frames used by ICU ASW */

/* CF used for Synch without DW */
#define SYNCH_WITHOUT_DW_CF 0U //!< Communication Frame used for Synch without DW message
/* CF used for Memory load */
#define BASE_MEM_LOAD_CF 1U //!< Communication Frame used for Memory Load messages (must add DPU rt_address to this value)
/* CF used for DPU commands */
#define BASE_COMMAND_CF 6U //!< Communication Frame used for commanding messages (must add DPU rt_address to this value)
/* CFs 21 to 24 */
#define BASE_CMD_VER_1_CF 20U //!< Communication Frame used for ASW and BSW_CMD_VER_1 Command Verification messages (must add DPU rt_address to this value)
/* CF used for Time distribution/retrieval */
#define TIME_DISTRIBUTION_CF 35U //!< Communication Frame used for time distribution and retrieve messages (must add DPU rt_address to this value)
/* CFs 40 to 43 */
#define BASE_HK_CF 39U //!< Communication Frame used to retrieve BSW HKs, DPU_STATUS_TAb, ASW_ERROR_TAB, DPU_MONITOR TAB and DPU_HSK_TAB (must add DPU rt_address to this value)
/* CFs 44 to 47 */
#define ASW_DCU_SCE_HSK_TAB_CF 43U //!< Communication Frame used to retrieve DCUs and SCEs HK tables (must add DPU rt_address to this value)
/* CFs 48 to 51 */
#define BASE_MEM_DUMP_CF 47U //!< Communication Frame used for Memory Dump messages (must add DPU rt_address to this value)
/* CFs 52 to 55 */
#define DPU_DBB_DRB_STATUS_TAB_CF 51U //!< Communication Frame used to retrieve DBB_DRB status table (must add DPU rt_address to this value)
/* CFs 56 to 59 */
#define CMD_VER_2_CF 55U //!< Communication Frame used for BSW command verification 2 (must add DPU rt_address to this value)
#define DPU_DEATH_REP_TAB_CF 55U //!< Communication Frame used to retrieve death report table (must add DPU rt_address to this value)
#define	DPU_BOOT_REP_TAB_CF 55U //!< Communication Frame used to retrieve boot report table (must add DPU rt_address to this value)
/**
 * \brief SubAddresses used by ICU-ASW
 */
/* DPU memory Load/Dump SAs */
#define	DPU_CPU_START_SA 1U //!< First SubAddress used by memory dump/load commands
#define DPU_CPU_LAST_SA 16U //!< Last SubAddress used by memory dump/load commands
/* DPU HK SAs */
#define DCU_HSK_TAB_START_SA 1U //!< First SubAddress used by DCU HK tables
#define DCU_HSK_TAB_LAST_SA 8U //!< Last SubAddress used by DCU HK tables
#define SCE_TAB_START_SA 9U //!< First SubAddress used by SCE HK tables
#define SCE_TAB_LAST_SA 16U //!< Last SubAddress used by SCE HK tables
/* SA 17 */
#define BSW_CMDVER1_AND_ASW_CMDVER_SA 17U //!< SubAddress used to retrieve BSW Command Verification1 and ASW Command Verification table
/* SA 18 */
#define BSW_HSK1_AND_ASW_STATUS_TAB_SA 18U //!< SubAddress used to retrieve BSW HK1 and ASW_STATUS table
#define COMMAND_DPU_SA 18U //!< SubAddress used to transmit commands to DPUs
/* SA 19 */
#define BSW_CMD_VERIFICATION_2_SA 19U //!< SubAddress used to retrieve BSW Command Verification 2 table
#define PROC_PARAM_TAB_SA 19U //!< SubAddress used to transmit the Processing Parameter table
/* SA 20 */
#define BSW_EVENT_REP_AND_ASW_ERROR_SA 20U //!< SubAddress used to retrieve BSW Boot event abd ASW_ERROR tables
#define SYS_CONFIG_TAB_SA 20U //!< SubAddress used to transmit the Sys Config table
/* SA 21 */
#define BSW_HSK2_AND_ASW_MON_TAB_SA 21U //!< SubAddress used to retrieve BSW HK2 and ASW_MONITOR table
#define DITHER_TAB_SA 21U //!< SubAddress used to transmit the Dither Config table
/* SA 22 */
#define ASW_DPU_DEATH_REP_TAB_SA 22U //!< SubAddress used to retrieve death report table
#define ASW_DPU_BOOT_REP_TAB_SA 22U //!< SubAddress used to retrieve boot report table
#define ASW_DPU_DBB_DRB_STATUS_TAB_SA 22U //!< SubAddress used to retrieve DBB_DRB status table
/* SA 23 */
#define ASW_DPU_HSK_TAB_SA 23U //!< SubAddress used to retrieve DPU HK table
/* SA 29 */
#define TIME_MSG_SA 29U //!< SubAddress used for Time messages
/* SA 30 */
#define WRAP_AROUND_SA 30U //!< SubAddress used for wrap-around operations
/* SA 31 */
#define MODE_CODES_SA 31U //!< SubAddress used for mode codes

/**
 * \enum DPU_HK_TABLES_GTAB GTAB_tables
 * \brief List of DPU HK tables to be acquired using GTAB by ICU-ASW
 */
enum {
	SCE1_HK_TAB = 0, SCE2_HK_TAB, SCE3_HK_TAB, DBB_DRB_TAB,
};

/**
 * \}
 */

/**
 *  \ingroup BC_HANDLER_TYPES
 *  \{
 */
/**
 * \brief Data types used to manage communication with DPU.
 * These types must contain the index of messages to be sent for each communication frame.
 */
typedef uint8_t BC_Profile_1553[COMMUNICATION_FRAME_NUMBER][MAX_NUMBER_OF_SENDLIST_MSGS];

/**
 * \}
 */

/**
 *  \ingroup BC_HANDLER_VARIABLES
 *  \{
 */
/**
 * \brief Array used to store data contained in the exchanged sendlist. Each index of the array corresponds to a sendlist.
 */
extern SendListData ptr_sendlist[MAX_NUMBER_OF_SENDLIST];

/**
 * \brief Global Variables used to manage ICU -> DPUs communication
 */

/* Matrix containing index of messages for each communication frame */
extern BC_Profile_1553 bus_profile;

/* Arrays containing number of messages for each communication frame */
extern uint8_t bus_profile_msg_counter_0[COMMUNICATION_FRAME_NUMBER];
extern uint8_t bus_profile_msg_counter_1[COMMUNICATION_FRAME_NUMBER];
extern uint8_t *bus_profile_msg_counter;

extern uint8_t init_bus_profile_msg_counter_0[COMMUNICATION_FRAME_NUMBER];
extern uint8_t init_bus_profile_msg_counter_1[COMMUNICATION_FRAME_NUMBER];
extern uint8_t *init_bus_profile_msg_counter;

/**
 * \brief Variable used to store BC 1553 messages
 */
extern ExchangeMsg_t dpu_1553_msg_chA[MAX_BC_MESSAGES];
extern ExchangeMsg_t dpu_1553_msg_chB[MAX_BC_MESSAGES];
extern ExchangeMsg_t *dpu_1553_msg;

/**
 * Each index correspond to a SendList (0..9). Values are the position indexes
 * of the last insert messages.
 */
extern uint8_t list_handler[MAX_NUMBER_OF_SENDLIST];

/**
 * \brief Flag to be used to create 1553 BC messages
 */
extern uint8_t flag_bc_msgs;

/**
 * \brief Active DPUs Remote Terminal address
 */
extern uint16_t dpu1_active_rt, dpu2_active_rt;

/**
 * \brief DPUs software status
 */
extern uint8_t dpu1_status;
extern uint8_t dpu2_status;

/**
 * \brief SCEs overall status (can be IDLE = 1 or EXPOSING = 2)
 */
extern uint8_t dpu1_sce_status;
extern uint8_t dpu2_sce_status;

/**
 * \brief MMU transmission status (can be "no transmission" = 0 or "transmission" = 1)
 */
extern uint8_t dpu1_mmu_transm_status;
extern uint8_t dpu2_mmu_transm_status;

/**
 * \brief Counters for GTAB tables
 */
extern uint8_t dpu1_next_gtab;
extern uint8_t dpu2_next_gtab;

/**
 * \brief Status of DPUs
 */
extern uint8_t dpu1_tc_running;
extern uint8_t dpu2_tc_running;

/**
 * \}
 */

/**
 *  \ingroup BC_HANDLER_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to configure and start milbus 1553 BC Core.
 * \return 0 if successfully executed, an error otherwise.
 */
uint8_t asw_configure_bc(void);

/**
 * \brief Function used to read messages of a send list
 * \param[in] sl_index: Index of a send list that must be read
 * \return status of the exchanged messages of the sendlist.
 * Returns ICU_SUCCESSFUL if success, error code otherwise.
 */
icu_status_code asw_read_sendlist(uint8_t sl_index);

/**
 * \brief Function used to create a new 1553 message and insert it in the first available position
 * \param[in] delay time to wait after exchange of the new message
 * \param[in] msg_data Data to be send to the Remote Terminal
 * \param[in] cf Communication Frame where the message will be send
 * \param[in] rt_addr Remote Terminal address of the destination unit
 * \param[in] tr_bit Transmit/receive bit. 0 means receive, 1 transmit
 * \param[in] sub_addr Destination subaddress
 * \param[in] word_count Number of data word to be send/receive
 * \param[in] bc_msg_index Index of message to create inside dpu_1553_msg structure.
 * \return status of the message. Returns ICU_SUCCESSFUL if success, error code otherwise.
 */
icu_status_code asw_create_msg_to_DPU(uint8_t delay, uint16_t msg_data[],
		const uint8_t cf, uint16_t rt_addr, const uint16_t tr_bit,
		const uint16_t sub_addr, uint16_t word_count, const uint16_t bc_msg_index);

/**
 * \brief Function used to generate a sendlist
 * \param[in] cf Communication frame number to generate sendlist
 * \return ICU_SUCCESSFUL if ok. Errors could be ICU_WRONG_PARAMETER and ICU_BC_1553_ERROR
 */
icu_status_code asw_prepare_sendlist(uint8_t cf);

/**
 * \brief Function used to select and send a sendlist
 * \param[in] sl_index Communication frame number to transmit sendlist
 * \return Returns ICU_SUCCESSFUL if success, error code otherwise.
 */
icu_status_code asw_transmit_sendlist(uint8_t sl_index);

/**
 * \brief Function used to handle messages contained in the sendlist
 * \param[in] sl_index Index of a send list that must be read
 * \return none.
 */
void asw_sendlist_interpreter(uint8_t sl_index);

/**
 * \brief Function used to reset BC Profile. It will reset non-active structure. Changes will be applied swapping active structures.
 * \param[in] cf communication frame to be cleared. If it values 255 all communication frames will be cleared except synchronize messages.
 * \return ICU_SUCCESSFUL if ok, ICU_WRONG_PARAMETER otherwise.
 */
icu_status_code asw_clear_bus_profile(uint8_t cf);

/**
 * \brief Function used to create and update the 1553 BC profile. It will generate profile for both DPUs. Switching profile operation will be done after execution of this function.
 * \return None
 */
void asw_prepare_bus_profile(void);

/**
 * \brief Function used to create 1553 bus controller profile to be used when ICU does not know which sw is actually running on the DPU
 * (e.g., after the DPU has been powered on but the BSW has not been detected yet, or a DPU performed an autonomous reboot).
 * \param[in] rt_address Remote Terminal address of the DPU
 * \return None
 */
void asw_dpu_undef_bc_profile(uint16_t rt_address);

/**
 * \brief Function used to create 1553 bus controller profile to be used when boot software runs on a DPU.
 * \param[in] rt_address Remote Terminal address of the DPU
 * \return None
 */
void asw_dpu_bsw_bc_profile(uint16_t rt_address);

/**
 * \brief Function used to create 1553 bus controller profile to be used when application software runs on a DPU.
 * \param[in] rt_address Remote Terminal address of the DPU
 * \return None
 */
void asw_dpu_asw_bc_profile(uint16_t rt_address);

/**
 * \brief Function used to update 1553 BC in order to send 1553 messages to DPUs. It will update also variable in order to point to next table to be acquired.
 * \param[in] gtab_hk_table DPU HK tables to be acquired.
 * \param[in] rt_addr RT address of DPU units to be commanded.
 * \return none
 */
void asw_SCE_and_DBB_DRB_msgs_in_sl(
		const uint32_t gtab_hk_table, uint16_t rt_addr);
/**
 * \brief Function used to command GTAB to DPUs if there is not another TC running.
 * \param[in] dpu_unit DPU unit. 1 for DPU 1, 2 for DPU 2.
 * \return None
 */
void asw_command_GTAB_to_DPUs(uint8_t dpu_unit);

/**
 * \brief Function used to initialize all DPU messages. It is called during BC initialization to create messages for both 1553 channels (A and B).
 * \return Returns 0 if successfull, number of errors otherwise.
 */
uint8_t asw_create_fixed_dpu_msgs(void);

/**
 * \}
 */

#endif /* BC_CONFIGURATION_H_ */
