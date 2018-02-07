/**
 * \file 	mil_std_1553_bc_drv.h
 *
 * \brief	This file declares the public interface of the 1553 BC Driver
 *
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	25-august-2015
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2015, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef ICU_MIL_STD_1553_BC_DRV_H
#define ICU_MIL_STD_1553_BC_DRV_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>
#include <public/mil_std_1553_bc_reg_mem.h>

/**
 *  @defgroup DSW_MILBUS_1553 MILBUS 1553
 *  @{
 */

/**
 *  @defgroup DSW_MILBUS_1553_BC BC
 *  @{
 * \brief	This DSW-BC Driver can be used from different tasks or execution
 * 			lines, due to the driver uses shared resources inside of its
 * 			functions, all those functions that modify these critical sections
 * 			shall be called using a mutual exclusion mechanism (mutex,
 * 			semaphore...etc). We have to differentiate between two types of
 * 			shared resources:
 * 			- COMMON SHARED VARIABLES: Those shared variables that are
 * 			used in common by all the Send Lists handled by the Driver. This
 * 			type of shared variable always need mutual exclusion.
 * 			- INDIVIDUAL SHARED VARIABLES: Those shared variables that
 * 			are used just for a specific index of Send List, then it shall be
 * 			necessary using mutual exclusion just when the same Send List Index
 * 			is used by different tasks, but not for different Send List Index (
 * 			e.g the number of messages counter for a Send List).
 *
 *			Those functions that need some of these types of protection are
 *			indicated below.
 */
/**
 *  @defgroup DSW_MILBUS_1553_BC_DRV Bus Controller
 *  @{
 */


/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_MILBUS_1553_BC_DRV_CONST Constants
 *  @{
 */

/**
 * Max length of Data Words for a Command Word
 */
#define MAX_16W_DATA_WORDS_1553_COMMAND (32UL)

/**
 * 1 word16 (FLAGS) + 1 word16 (COMMAND WORD) + 1 word16 (INTER MSG TIMING) +
 * 1 word16 (STATUS WORD) + 32 word16 (DATA WORDS) = 36
 */
#define EXCHANGE_MSG_WORD_16_SIZE 36U

/**
 * Max number of messages (Send List entries) per Send List
 */
#define MAX_NUMBER_OF_MSG_PER_SEND_LIST 64U



/** Configuration value for the time out of 14 microseconds*/
#define 		BC_TIME_OUT_14_MICROSECONDS (0UL) //!< BC_TIME_OUT_14_MICROSECONDS

/** Configuration value for the time out of 31 microseconds*/
#define 		BC_TIME_OUT_31_MICROSECONDS (1UL) //!< BC_TIME_OUT_31_MICROSECONDS


/** Value to define a Send List in FREE State */
#define	SEND_LIST_FREE  0xAAAAU        //!< SEND_LIST_FREE
/** Value to define a Send List in READY State */
#define	SEND_LIST_READY  0xBBBBU       //!< SEND_LIST_READY
/** Value to define a Send List in IN EXECUTION State */
#define	SEND_LIST_IN_EXECUTION  0xCCCCU//!< SEND_LIST_IN_EXECUTION
/** Value to define a Send List in EXCHANGED State */
#define	SEND_LIST_EXCHANGED  0xDDDDU    //!< SEND_LIST_EXCHANGED
/** Value to define a Send List in INVALID State */
#define	SEND_LIST_INVALID  0xFFFFU    //!< SEND_LIST_INVALID


/**
 * @}
 */


/**
 *  @defgroup DSW_MILBUS_1553_BC_DRV_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the BC Driver functions
 */


#define 	ICU_BC_OPERATION_SUCCESSFUL  (0UL)                        //!< BC_OPERATION_SUCCESSFUL
#define 	ICU_BC_ERROR_MEM_AREA_IS_NOT_ALIGNED  (1UL)          //!< BC_ERROR_MEM_AREA_IS_NOT_ALIGNED
#define 	ICU_BC_ERROR_MEM_AREA_IS_OUT_OF_LIMITS  (1UL<<1)        //!< BC_ERROR_MEM_AREA_IS_OUT_OF_LIMITS
#define 	ICU_BC_ERROR_CORE_IS_ALREADY_STARTED  (1UL<<2)          //!< BC_ERROR_CORE_IS_ALREADY_STARTED
#define 	ICU_BC_ERROR_CORE_IS_ALREADY_STOPPED  (1UL<<3)          //!< BC_ERROR_CORE_IS_ALREADY_STOPPED
#define 	ICU_BC_ERROR_INVALID_DATA_PARAMETER  (1UL<<4)           //!< BC_ERROR_DATA_IGNORED
#define 	ICU_BC_ERROR_FLAGS  (1UL<<6)                            //!< BC_ERROR_FLAGS
#define 	ICU_BC_ERROR_MSG_INDEX_IS_OUT_OF_LIMITS  (1UL<<7)       //!< BC_ERROR_MSG_INDEX_IS_OUT_OF_LIMITS
#define 	ICU_BC_ERROR_SEND_LIST_NON_FREE  (1UL<<8)               //!< BC_ERROR_SEND_LIST_NON_FREE
#define 	ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS  (1UL<<9)//!< BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
#define 	ICU_BC_ERROR_SEND_LIST_ENTRY_NON_FREE  (1UL<<10)           //!< BC_ERROR_SEND_LIST_ENTRY_NON_FREE
#define 	ICU_BC_ERROR_CORE_IS_EXCHANGING_SEND_LIST  (1UL<<11)    //!< BC_ERROR_CORE_IS_EXCHANGING_SEND_LIST
#define 	ICU_BC_ERROR_SEND_LIST_IS_NON_READY  (1UL<<12)          //!< BC_ERROR_SEND_LIST_IS_NON_READY
#define 	ICU_BC_ERROR_SEND_LIST_LOCKED  (1UL<<13)                //!< BC_ERROR_SEND_LIST_LOCKED
#define 	ICU_BC_ERROR_SEND_LIST_TO_SEND_HAS_GAPS  (1UL<<15)      //!< BC_ERROR_SEND_LIST_TO_SEND_HAS_GAPS
#define 	ICU_BC_ERROR_CONF_TIME_OUT_VALUE  (1UL<<16)  			//!< BC_ERROR_CONF_TIME_OUT_VALUE
#define 	ICU_BC_ERROR_NULL_PARAMETER  (1UL<<17) 					//!< BC_ERROR_NULL_PARAMETER



/// @cond ALTERNATIVE_ERROR_CODES
#define 	BC_OPERATION_SUCCESSFUL  ICU_BC_OPERATION_SUCCESSFUL
#define 	BC_ERROR_MEM_AREA_IS_NOT_ALIGNED  ICU_BC_ERROR_MEM_AREA_IS_NOT_ALIGNED
#define 	BC_ERROR_MEM_AREA_IS_OUT_OF_LIMITS  ICU_BC_ERROR_MEM_AREA_IS_OUT_OF_LIMITS
#define 	BC_ERROR_CORE_IS_ALREADY_STARTED  ICU_BC_ERROR_CORE_IS_ALREADY_STARTED
#define 	BC_ERROR_CORE_IS_ALREADY_STOPPED  ICU_BC_ERROR_CORE_IS_ALREADY_STOPPED
#define 	BC_ERROR_INVALID_DATA_PARAMETER  ICU_BC_ERROR_INVALID_DATA_PARAMETER
#define 	BC_ERROR_FLAGS  ICU_BC_ERROR_FLAGS
#define 	BC_ERROR_MSG_INDEX_IS_OUT_OF_LIMITS  ICU_BC_ERROR_MSG_INDEX_IS_OUT_OF_LIMITS
#define 	BC_ERROR_SEND_LIST_NON_FREE  ICU_BC_ERROR_SEND_LIST_NON_FREE
#define 	BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS  ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
#define 	BC_ERROR_SEND_LIST_ENTRY_NON_FREE  ICU_BC_ERROR_SEND_LIST_ENTRY_NON_FREE
#define 	BC_ERROR_CORE_IS_EXCHANGING_SEND_LIST  ICU_BC_ERROR_CORE_IS_EXCHANGING_SEND_LIST
#define 	BC_ERROR_SEND_LIST_IS_NON_READY  ICU_BC_ERROR_SEND_LIST_IS_NON_READY
#define 	BC_ERROR_SEND_LIST_LOCKED  ICU_BC_ERROR_SEND_LIST_LOCKED
#define 	BC_ERROR_SEND_LIST_TO_SEND_HAS_GAPS  ICU_BC_ERROR_SEND_LIST_TO_SEND_HAS_GAPS
#define 	BC_ERROR_CONF_TIME_OUT_VALUE  ICU_BC_ERROR_CONF_TIME_OUT_VALUE
#define 	BC_ERROR_NULL_PARAMETER  ICU_BC_ERROR_NULL_PARAMETER
/// @endcond
/**
 * @}
 */


/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 *  @defgroup DSW_MILBUS_1553_BC_DRV_FLAGS Flags
 *  @{
 *  \brief	Flags used to build BC messages
 */

/**
 * \enum	BCFlag_t
 *
 * \brief	Flags for a message of a Send List
 */
typedef enum
{
	/** The message will be sent again if its transmission had an error*/
	BC_FLAG_REPEAT_ON_ERROR = (1<<0),//!< BC_FLAG_REPEAT_ON_ERROR
	/** The message will be sent through the BUS ID B (BUS ID 0 by default)*/
	BC_FLAG_BUS_ID_B = (1<<2),       //!< BC_FLAG_BUS_ID_B
	/** The BC stops on EXCERR*/
	BC_FLAG_STOP_WHEN_ERROR = (1<<3) //!< BC_FLAG_STOP_WHEN_ERROR
}
BCFlag_t;


/**
 * @}
 */

/**
 *  @defgroup DSW_MILBUS_1553_BC_DRV_TYPES	Structures
 *  @{
 *  \brief	Structures used by the BC Driver
 */
/**
 * \union	ExchangeMsg_t
 *
 * \brief	Format of a message (Send List entry) of a Send List
 */
typedef union {

    uint16_t all_words[EXCHANGE_MSG_WORD_16_SIZE];
    struct
    {
    	/**
    	 * FLAGS\n:
    	 *	bit 0 (LSB) REPEAT-ON-ERROR: 1 repeat on error capability activated for the
    	 *	the message, 0 this feature is not chosen for the message
    	 *	bit 1 BUSID: Bus Identifier (0 Bus A, 1 Bus B)
    	 */
    	uint16_t flags;
    	/**
    	 * Time elapsed between the end of the message and the beginning of the next
    	 * one, if the message is the last one of the SendList, then this value
    	 * indicates the time elapsed between
    	 */
    	uint16_t inter_msg_timing;
    	/**
    	 * Command Word which defines the type of message to send
    	 */
    	CommandWord_t command_word;
    	/**
    	 * Status Word
    	 */
    	StatusWord_t status_word;
    	/**
    	 * Data of the message
    	 */
    	uint16_t data[MAX_16W_DATA_WORDS_1553_COMMAND];
    };

}
ExchangeMsg_t;


/**
 * \struct	SendList_t
 *
 * \brief	This structure defines a complete Send List
 */
typedef struct
{
	ExchangeMsg_t message[MAX_NUMBER_OF_MSG_PER_SEND_LIST];
}
SendList_t;
/**
 * @}
 */


/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/


/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_MILBUS_1553_BC_DRV_API Public Functions
 *  @{
 *  \brief	Public functions provided by the BC Driver
 */


/**
 * \brief	This function is used to initialize the BC Driver and allocate the
 * 			RAM address where the Data and Program Area of the 1553 BC Core will
 * 			be placed.
 *
 * \details	This function uses COMMON SHARED VARIABLES
 *
 * \param[in]	mem_area_address RAM address where the Memory Area of the BC
 * 				Core shall be allocated. This area must be aligned to 512KBytes
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_BC_ERROR_MEM_AREA_IS_NOT_ALIGNED
 *				- ICU_BC_ERROR_MEM_AREA_IS_OUT_OF_LIMITS
 */
uint32_t icu_mil_std_1553_bc_drv_init(const uint32_t mem_area_address);

/**
 * \brief	This function is used to configure the behavior of the 1553 BC
 * 			Core
 *
 * \param[in] mask_irq_errors	It is used to mask or not the IRQ Errors
 * \param[in] time_out	time-out value (0->14 usec or 1->31 usec)
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_CONF_TIME_OUT_VALUE
 */
uint32_t icu_mil_std_1553_bc_drv_configure(const bool_t mask_irq_errors,
		const uint8_t time_out);



/**
 * \brief	This function stops the 1553 BC Core before to execute the next
 * 			Instruction Block
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_CORE_IS_ALREADY_STOPPED
 */
uint32_t icu_mil_std_1553_bc_drv_stop(void);


/**
 * \brief	This function is used to build a message structure that can
 * 			afterwards be added to a Send List. The command word will be used
 * 			to check If data words are needed (Rx message), and then the
 * 			function will copy only the amount of DWs needed (not necessarily
 * 			all 32 DWs).
 *
 * \param[in] flags	Different flags required by the 1553 BC Core to exchange
 * 				the message. The available flags are defined and described by
 * 				the ::BCFlag_t	enumerate type
 * \param[in] inter_msg_timing	time to wait after the exchange of a message.
 * 				Time unit for this parameter is 2 microsecs
 * \param[in] cmd_word	Command Word of the message
 * \param[in] data	array of Data Words to exchange; If the Command Word is a
 * 				Transmit Command, this field shall be NULL
 * \param[out] new_msg	structure where the message will be built
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_FLAGS
 * 				- ICU_BC_ERROR_INVALID_DATA_PARAMETER
 */
uint32_t icu_mil_std_1553_bc_drv_create_msg(const uint8_t flags,
		const uint16_t inter_msg_timing, const CommandWord_t cmd_word,
		const uint16_t data[], ExchangeMsg_t * new_msg);

/**
 * \brief	This function transfers a new msg to a Send List. The data is
 * 			copied (including potential data words), and therefore the
 * 			ExchangeMsg_t structure is afterwards free to be used with a new
 * 			message. The user is responsible of filling all gaps in the Send
 * 			List structure, i.e. to avoid an uninitialized message index in
 * 			between 2 initialized indexes. The Send List must be in FREE state.
 *
 * \details	This function uses INDIVIDUAL SHARED VARIABLES
 *
 * \param[in] send_list_index	Index of the Send List where the message will
 * 				be added
 *
 * \param[in] msg_index	the index, inside of the Send List, where the message will
 * 				be copied
  * \param[in] new_msg	the message to add into the Send List
 *
 * \return	The function returns 0 if was copied successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_MSG_INDEX_IS_OUT_OF_LIMITS
 * 				- ICU_BC_ERROR_SEND_LIST_NON_FREE
 * 				- ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
 * 				- ICU_BC_ERROR_NULL_PARAMETER
 */
uint32_t icu_mil_std_1553_bc_drv_add_msg_to_send_list(const uint32_t send_list_index,
		const uint8_t msg_index,const ExchangeMsg_t * new_msg);


/**
 * \brief	This function deploys the contents of a Send List into de Core
 * 			Command and Data Areas. Each Send List has a fixed pre-configured
 * 			address space in these areas, so there is no need for the user to
 * 			configure it. This function sets afterwards the Send List in READY
 * 			state, where it can afterwards be exchanged. It also configures the
 * 			parameters related to the Send List as a whole: whether an
 * 			interruption will occur at the end of the last slot (i.e. after the
 * 			WAIT coming after the last message transmission), and the
 * 			identification of the next Send List in case automatic transition
 * 			is required.
 *
 * \details	This function uses several INIDIVIDUAL SHARED VARIABLES
 *
 * \param[in]	send_list_index	Index of the Send List to deploy
 *
 * \param[in]	enable_irq_on_end_list	If TRUE, the ItSynch IRQ ill be
 * 				triggered at the end of this Send List execution
 *
 * \param[in]	next_send_list_index	If it contains a valid Send List index,
 * 				the last instruction of the current Send List will jump to the
 * 				index-referred Send List. If -1 or invalid Send List index,
 * 				the Core will stop after execution of this Send List.
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_SEND_LIST_TO_SEND_HAS_GAPS
 * 				- ICU_BC_ERROR_SEND_LIST_IS_NON_FREE
 * 				- ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
 */
uint32_t icu_mil_std_1553_bc_drv_set_ready_send_list(const uint32_t send_list_index,
		const bool_t enable_irq_on_end_list, const int32_t next_send_list_index);


/**
 * \brief	This function starts the 1553 BC Core which will execute the program
 * 			stored into the program area, identified by the Send List
 * 			identifier. It shall check that the referred Send List is already
 * 			deployed.
 *
 * \details	This function uses COMMON SHARED VARIABLES
 *
 * \param[in] send_list_index The index of the Send List to exchange
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_CORE_IS_EXCHANGING_SEND_LIST
 * 				- ICU_BC_ERROR_SEND_LIST_IS_NON_READY
 * 				- ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
 */
uint32_t icu_mil_std_1553_bc_drv_execute_send_list(const uint32_t send_list_index);

/**
 * \brief	This function gets a Send List which has been exchanged completely
 * 			and the number of the messages that the Send List contains.
 *
 * \param[in] send_list_index	index of the Send List
 * \param[out] p_send_list
 * \param[out] number_of_msgs	number of messages of the Send List
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_SEND_LIST_LOCKED
 * 				- ICU_BC_ERROR_NULL_PARAMETER
 * 				- ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
 */
uint32_t  icu_mil_std_1553_bc_drv_get_send_list(const uint32_t send_list_index,
		SendList_t * p_send_list,uint32_t * number_of_msgs);

/**
 * \brief 	This function modifies the state of the referred Send List in the
			SW driver, and releases and cleans its associated Send List
			descriptor. The Send List will afterwards be at Free state and new
			messages could then be added.
 *
 * \note 	If the Send List selected to free is in a FREE state but under
 * 			construction, this function cleans all messages previously added
 *
 * \details	This function uses INDIVIDUAL SHARED VARIABLES
 *
 * \param[in] send_list_index	index of the Send List
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_SEND_LIST_LOCKED
 * 				- ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
 */
uint32_t icu_mil_std_1553_bc_drv_free_send_list(const uint32_t send_list_index);

/**
 * \brief	This function checks if the 1553 Core is running
 *
 * \return	TRUE if the 1553 core is running, FALSE if not
 */
bool_t icu_mil_std_1553_bc_drv_is_core_running(void);

/**
 * \brief	This function provides an identifier of the state the referred Send
			List is in. In case of invalid parameter (index) the returned value
			will indicate it.

 * \param[in] send_list_index	Index of the Send List
 *
 * \return	The state of the Send List (SEND_LIST_FREE, SEND_LIST_READY,
 * 			SEND_LIST_IN_EXECUTION, SEND_LIST_EXCHANGED, SEND_LIST_INVALID)
 */
uint16_t icu_mil_std_1553_bc_get_send_list_state(const uint32_t send_list_index);


/**
 * \brief	This function provides the control word associated to the exchange of
 * 			specified Send List.
 *
 * \param[in] send_list_index	Index of the Send List
 * \param[out]	control_word	control word associated to the exchanged Send
 * 					List
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_BC_ERROR_SEND_LIST_INDEX_IS_OUT_OF_LIMITS
 */
uint32_t icu_mil_std_1553_bc_get_sendlist_control_word(const uint32_t send_list_index, uint16_t* control_word);


/**
 * \brief	This function returns the value of the NOMINAL INTERRUPT REGISTER
 * 			of the 1553 Core
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_bc_drv_get_nominal_irq_reg_value(void);

/**
 * \brief	This function returns the value of the ERRORL INTERRUPT REGISTER
 * 			of the 1553 Core
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_bc_drv_get_error_irq_reg_value(void);

/**
 * \brief	This function returns the value of the RESET INTERRUPT REGISTER
 * 			of the 1553 Core
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_bc_drv_get_reset_irq_reg_value(void);

/**
 * @}
 */

/**
 * @}
 */
/**
 * @}
 */
/**
 * @}
 */


#endif // ICU_MIL_STD_1553_BC_DRV_H
