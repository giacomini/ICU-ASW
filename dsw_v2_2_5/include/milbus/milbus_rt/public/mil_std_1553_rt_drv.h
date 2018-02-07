/**
 * \file	mil_std_1553_rt_drv.h
 *
 * \brief	This file defines the public interface of the RT driver
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	20-march-2015
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2015, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef MIL_STD_1553_RT_DRV_H
#define MIL_STD_1553_RT_DRV_H

/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>
#include <public/mil_std_1553_rt_reg_mem.h>

/**
 *  @addtogroup DSW_MILBUS_1553 MILBUS 1553
 *  @{
 */
/**
 *  @defgroup DSW_MILBUS_1553_RT RT
 *  @{
 */
/**
 *  @defgroup DSW_MILBUS_1553_RT_DRV Remote Terminal
 *  @{
 */


/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/**
 *  @defgroup DSW_MILBUS_1553_RT_CONST Constants
 *  @{
 */

#define RT_SUBADDRESS_BITMAP_SIZE (64UL)


/**
 * @}
 */


/**
 *  @defgroup DSW_MILBUS_1553_RT_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes reported by all the functions
 * 			of the RT Driver
 */

	/**
	 * There is no error, the operation was done successfully
	 */
#define 	ICU_RT_OPERATION_SUCCESSFUL  (0UL)
	/**
	 * The Memory Area where the Core shall work is not aligned to 512 KBytes
	 */
#define 	ICU_RT_ERROR_MEM_AREA_IS_NOT_ALIGNED  (1UL)
	/**
	 * The Memory Area is out of the RAM Limits (from 0x40000000 to 0x407FFFFF)
	 */
#define 	ICU_RT_ERROR_MEM_AREA_IS_OUT_OF_LIMITS  (1UL<<1)
	/**
	 * The RT Core is already started
	 */
#define 	ICU_RT_ERROR_CORE_IS_ALREADY_STARTED  (1UL<<2)
	/**
	 * The RT Core is already stopped
	 */
#define 	ICU_RT_ERROR_CORE_IS_ALREADY_STOPPED  (1UL<<3)
	/**
	 * The Mode Code id is out of the available limits (0...31)
	 */
#define 	ICU_RT_ERROR_MODE_CODE_ID_IS_OUT_OF_LIMITS  (1UL<<4)
	/**
	 * The RT Core is running and the user can't change anything related with
	 * the configuration
	 */
#define 	ICU_RT_ERROR_CORE_IS_RUNNING  (1UL<<5)
	/**
	 * The Subaddress is out of the limits (1...30). 0  Broadcast 31  Mode
	 * Codes
	 */
#define 	ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS  (1UL<<6)
	/**
	 * The Mode Code id is not available. Any reserved Mode Codes are available
	 */
#define 	ICU_RT_ERROR_MODE_CODE_ID_IS_NOT_AVAILABLE  (1UL<<7)
	/**
	 * The blocks selected are locked by other Subaddress
	 */
#define 	ICU_RT_ERROR_BLOCKS_LOCKED_BY_OTHER_SA  (1UL<<8)
	/**
	 * The blocks is out of the available blocks
	 */
#define 	ICU_RT_ERROR_BLOCKS_OUT_OF_LIMITS  (1UL<<9)
	/**
	 * The block selected plus the size for the buffer is out of the limits
	 */
#define 	ICU_RT_ERROR_BLOCK_SIZE_OUT_OF_LIMITS  (1UL<<10)
	/**
	 * The index of the Command Entries is out of the limits (Maximum1920)
	 */
#define 	ICU_RT_ERROR_MSG_INDEX_OUT_OF_LIMITS  (1UL<<11)
	/**
	 * The number of messages to copy is out of the limits (Maximum1920)
	 */
#define 	ICU_RT_ERROR_NUMBER_OF_MSGS_OUT_OF_LIMITS  (1UL<<12)
	/**
	 * One of the parameters of the function is NULL
	 */
#define 	ICU_RT_ERROR_NULL_PARAMETER  (1UL<<13)
	/**
	 * The operation is not valid over the type of buffer
	 */
#define 	ICU_RT_ERROR_INVALID_BUFFER_MODE  (1UL<<14)
	/**
	 * The last command word index is out of limits (due to there is no msgs)
	 */
#define 	ICU_RT_ERROR_LAST_CMD_INDEX_IS_OUT_LIMITS  (1UL<<15)
	/**
	 * Invalid selected Blocks
	 */
#define 	ICU_RT_ERROR_BLOCKS_INVALID  (1UL<<16)


/// @cond ALTERNATIVE_ERROR_CODES
#define 	RT_OPERATION_SUCCESSFUL 	ICU_RT_OPERATION_SUCCESSFUL
#define 	RT_ERROR_MEM_AREA_IS_NOT_ALIGNED  	ICU_RT_ERROR_MEM_AREA_IS_NOT_ALIGNED
#define 	RT_ERROR_MEM_AREA_IS_OUT_OF_LIMITS  	ICU_RT_ERROR_MEM_AREA_IS_OUT_OF_LIMITS
#define 	RT_ERROR_CORE_IS_ALREADY_STARTED 	ICU_RT_ERROR_CORE_IS_ALREADY_STARTED
#define 	RT_ERROR_CORE_IS_ALREADY_STOPPED  	ICU_RT_ERROR_CORE_IS_ALREADY_STOPPED
#define 	RT_ERROR_MODE_CODE_ID_IS_OUT_OF_LIMITS  	ICU_RT_ERROR_MODE_CODE_ID_IS_OUT_OF_LIMITS
#define 	RT_ERROR_CORE_IS_RUNNING  	ICU_RT_ERROR_CORE_IS_RUNNING
#define 	RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS  	ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
#define 	RT_ERROR_MODE_CODE_ID_IS_NOT_AVAILABLE  	ICU_RT_ERROR_MODE_CODE_ID_IS_NOT_AVAILABLE
#define 	RT_ERROR_BLOCKS_LOCKED_BY_OTHER_SA  	ICU_RT_ERROR_BLOCKS_LOCKED_BY_OTHER_SA
#define 	RT_ERROR_BLOCKS_OUT_OF_LIMITS  	ICU_RT_ERROR_BLOCKS_OUT_OF_LIMITS
#define 	RT_ERROR_BLOCK_SIZE_OUT_OF_LIMITS  	ICU_RT_ERROR_BLOCK_SIZE_OUT_OF_LIMITS
#define 	RT_ERROR_MSG_INDEX_OUT_OF_LIMITS  	ICU_RT_ERROR_MSG_INDEX_OUT_OF_LIMITS
#define 	RT_ERROR_NUMBER_OF_MSGS_OUT_OF_LIMITS  	ICU_RT_ERROR_NUMBER_OF_MSGS_OUT_OF_LIMITS
#define 	RT_ERROR_NULL_PARAMETER  	ICU_RT_ERROR_NULL_PARAMETER
#define 	RT_ERROR_INVALID_BUFFER_MODE  	ICU_RT_ERROR_INVALID_BUFFER_MODE
#define 	RT_ERROR_LAST_CMD_INDEX_IS_OUT_LIMITS  	ICU_RT_ERROR_LAST_CMD_INDEX_IS_OUT_LIMITS
#define 	RT_ERROR_BLOCKS_INVALID  	ICU_RT_ERROR_BLOCKS_INVALID
/// @endcond


/**
 * @}
 */

/**
 *  @defgroup DSW_MILBUS_1553_RT_MODE_CODES Available Mode Codes
 *  @{
 *  \brief	Applicable mode codes
 */


#define	TX_DYNAMIC_BUS_CONTROL  0U
#define	TX_SYNCHRONIZE_WITHOUT_DATA  1U
#define	TX_TRANSMIT_STATUS_WORD  2U
#define	TX_INITIALIZE_SELF_TEST  3U
#define	TX_TRANSMITER_SHUTDOWN  4U
#define	TX_OVERRIDE_TRANSMITTER  5U
#define	TX_INHIBIT_TERMINAL_FLAG_BIT  6U
#define	TX_OVERRIDE_INHIBIT_TERMINAL_FLAG_BIT  7U
#define	TX_RESET_RT  8U
#define	TX_TRANSMIT_VECTOR_WORD  16U
#define	RX_SYNCHRONIZE_WITH_DATA  17U
#define	TX_TRANSMIT_LAST_COMMAND  18U
#define	TX_TRANSMIT_BIT_WORD  19U
#define	RX_SELECTED_TRANSMITER  20U
#define	RX_OVERRIDE_SELECTED_TRANSMITER  21U


/**
 * @}
 */

/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/

/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_MILBUS_1553_RT_API Public functions
 *  @{
 *  \brief	Public functions provided by the RT Driver
 */

/**
 * \brief	This function is used to initialize the RT Driver and allocate the
 * 			RAM address where the Data and Command Area of the 1553 RT Core will
 * 			be placed
 *
 * \param[in]	mem_area_address	RAM address where the Memory Area of the RT
 * 				Core shall be allocated
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_MEM_AREA_IS_NOT_ALIGNED
 * 				- ICU_RT_ERROR_MEM_AREA_IS_OUT_OF_LIMITS
 */
uint32_t icu_mil_std_1553_rt_drv_init(const uint32_t mem_area_address);

/**
 * \brief	This function is used to configure the behavior of the 1553 RT
 * 			Core
 *
 * \param[in]	broad_cast_mode_enable	Enable the Broadcast Mode
 * \param[in]	mask_synch_irq	It is used to mask or not the IRQ Synch
 * \param[in]	mask_trok_irq	It is used to mask or not the IRQ TROK
 * \param[in] 	mask_irq_errors	It is used to mask or not the IRQ Errors
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_CORE_IS_RUNNING
 */
uint32_t icu_mil_std_1553_rt_drv_configure(const bool_t broad_cast_mode_enable,
		const bool_t mask_synch_irq, const bool_t mask_trok_irq,
		const bool_t mask_irq_errors);

/**
 * \brief	This function resets the RT Core
 */
void icu_mil_std_1553_rt_drv_reset(void);

/**
 * \brief	This function starts the RT Core
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_CORE_IS_ALREADY_STARTED
 */
uint32_t icu_mil_std_1553_rt_drv_start(void);

/**
 * \brief	This function stops the RT Core
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_CORE_IS_ALREADY_STOPPED
 */
uint32_t icu_mil_std_1553_rt_drv_stop(void);

/**
 * \brief	This function is used to change the mode code configuration.
 *          NOTE: Mode codes TX_SYNCHRONIZE_WITHOUT_DATA and RX_SYNCHRONIZE_WITH_DATA will
 *          cause a Nominal Milbus interrupt with ItSync bit set to 1 if mask_synch_irq has been
 *          set to FALSE in function icu_mil_std_1553_rt_drv_configure().
 *          In addition, any mode code will cause a Nominal Milbus interrupt with ItTrok bit set to 1
 *          if mask_trok_irq has been set to FALSE in function icu_mil_std_1553_rt_drv_configure()
 *          and trok_enable input parameter is set to TRUE.
 *
 * \param[in] mode_code_id	The mode code to set the configuration
 * \param[in] legal	TRUE if it is enable, FALSE disable
 * \param[in] trok_enable	TRUE if it is enable, FALSE disable
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_MODE_CODE_ID_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_CORE_IS_RUNNING
 * 				- ICU_RT_ERROR_MODE_CODE_ID_IS_NOT_AVAILABLE
 */
uint32_t icu_mil_std_1553_rt_drv_mode_code_characterize(const uint8_t mode_code_id,
								const bool_t legal, const bool_t trok_enable);

/**
 * \brief	This function is used to change characterize configuration of a
 * 			subaddress
 *
 * \param[in] subaddress	The subaddress to set the configuration
 * \param[in] tx_rx	TRUE if it is to transmit, FALSE to receive
 * \param[in] legal TRUE if it is enable, FALSE disable
 * \param[in] trok_enable TRUE if it is enable, FALSE disable
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_CORE_IS_RUNNING
 */
uint32_t icu_mil_std_1553_rt_drv_subaddress_characterize(const uint8_t subaddress,
		const bool_t tx_rx,const bool_t legal,const bool_t trok_enable);

/**
 * \brief	This function is used to allocate a single buffer for a specific
 * 			subaddress. The buffer is one block long, and therefore ADUPD bit is
 * 			set to 0 (no ItSwitch will be generated).
 *
 * \param[in] subaddress	Subaddress associated to the buffer
 * \param[in] tx_rx	if the buffer is for Tx(TRUE) or Rx(FALSE)
 * \param[in] buffer_start_block	The block inside of the Data Area where the
 * 				 buffer shall start
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_BLOCKS_LOCKED_BY_OTHER_SA
 * 				- ICU_RT_ERROR_BLOCKS_INVALID
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_CORE_IS_RUNNING
 */
uint32_t icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(const uint8_t subaddress,
		const bool_t tx_rx, const uint16_t buffer_start_block);

/**
 * \brief	This function is used to allocate a linear buffer for a specific
 * 			subaddress. The buffer will be several blocks long in size, and
 * 			therefore ADUPD bit is set to 1 (to allow new messages be stored in
 * 			consecutive blocks). If the buffer size is one block only, this
 * 			function will reject the configuration request (use single buffer
 * 			instead).
 *          NOTE: User is responsible for allocating a number of blocks larger than
 *          the maximum needed for messages Tx/Rx in the worst case.
 *          If buffer capacity is not large enough, when buffer gets full it
 *          will cause buffer rollover and a non-maskable Nominal Milbus interrupt
 *          with ItSwitch bit set to 1.
 *
 * \param[in] subaddress	Subaddress associated to the buffer
 * \param[in] tx_rx	if the buffer is for Tx(TRUE) or Rx(FALSE)
 * \param[in] buffer_start_block	The block inside of the Data Area where the
 * 				 buffer shall start
 * \param[in] buffer_blocks_size	Size (in blocks) for the new buffer; range
 * 				is [2, 1022]
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_BLOCKS_LOCKED_BY_OTHER_SA
 * 				- ICU_RT_ERROR_BLOCKS_INVALID
 * 				- ICU_RT_ERROR_BLOCK_SIZE_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_CORE_IS_RUNNING
 */
uint32_t icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(const uint8_t subaddress,
		const bool_t tx_rx, const uint16_t buffer_start_block, const uint16_t buffer_blocks_size);

/**
 * \brief	This function is used to allocate a double buffer for a specific
 * 			subaddress. This function reserves 2 consecutive blocks that will
 * 			be configured in the HW similarly to Single Buffer type, but they
 * 			will be managed by the SW driver as Double Buffer
 *
 * \param[in]	subaddress	Subaddress associated to the buffer
 * \param[in]	tx_rx	If the buffer is for Tx(TRUE) or Rx (FALSE)
 * \param[in]	buffer_start_block	The block inside of the Data Area where the
 * 				 buffer shall start
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_BLOCKS_LOCKED_BY_OTHER_SA
 * 				- ICU_RT_ERROR_BLOCKS_INVALID
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_CORE_IS_RUNNING
 */
uint32_t icu_mil_std_1553_rt_drv_set_double_buffer_subaddress(const uint8_t subaddress,
		const bool_t tx_rx, const uint16_t buffer_start_block);

/**
 * \brief	This function gets from the Data area the Data Word itself
 * 			associated with a specific Mode Code ID. The address where this
 * 			word is placed is fixed but not provided to the user.
 *
 * \param[in]	mode_code_id Identifier of the mode code
 * \param[out]	p_data_word User variable where the function writers the Data
 * 				Word
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_MODE_CODE_ID_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_MODE_CODE_ID_IS_NOT_AVAILABLE
 * 				- ICU_RT_ERROR_NULL_PARAMETER
 */
uint32_t icu_mil_std_1553_rt_drv_get_mode_code_data_word(const uint8_t mode_code_id,
													uint16_t * p_data_word);

/**
 * \brief	This function sets the Data Word associated with a specific Mode
 * 			Code Id
 *
 * \param[in] mode_code_id	Identifier of the Mode Code
 * \param[in] data_word	data word to be associated to the Mode Code
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_MODE_CODE_ID_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_MODE_CODE_ID_IS_NOT_AVAILABLE
 */
uint32_t icu_mil_std_1553_rt_drv_set_mode_code_data_word(const uint8_t mode_code_id,const uint16_t  data_word);

/**
 * \brief	This function provides the RAM address of the buffer associated to a
 * 			specific subaddress and Tx/Rx when in single buffer type mode. It
 * 			is user responsibility to access the area preventing conflicts with
 * 			other Core accesses.
 *
 * \param[in] subaddress	Subaddress associated to the buffer
 * \param[in] tx_rx	If the buffer is for Tx(TRUE) or Rx(FALSE)
 * \param[out] p_buffer_address	Output parameter. Full RAM addres of the desired
 * 				 buffer
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_NULL_PARAMETER
 */
uint32_t icu_mil_std_1553_rt_drv_get_single_buffer_addr(const uint8_t subaddress,
															const bool_t tx_rx,
															volatile uint32_t ** p_buffer_address);

/**
 * \brief	This function provides the RAM address of the buffer associated to a
 * 			specific subaddress and Tx/Rx when in linear buffer type mode, and
 * 			taking into account the data block number, as provided in the
 * 			Information Word for incoming messages. The data block number
 * 			should be set to 0 if the beginning of the full single buffer is
 * 			desired (first block when  multiple blocks are reserved). It is
 * 			user responsibility to access the area preventing conflicts with
 * 			other Core accesses.
 *
 * \param[in] subaddress	Subaddress associated to the buffer
 * \param[in] tx_rx	If the buffer is for Tx(TRUE) or Rx(FALSE)
 * \param[in] data_block_number	Index of the block inside the buffer where the
 * 				desired data is allocated; 0 if not meaningful or beginning of
 * 				buffer is desired
 * \param[out] p_buffer_address	Output parameter. Full RAM address of the
 * 				desired buffer
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_BLOCKS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_INVALID_BUFFER_MODE
 * 				- ICU_RT_ERROR_NULL_PARAMETER
 */
uint32_t icu_mil_std_1553_rt_drv_get_linear_buffer_addr(const uint8_t subaddress,
							const bool_t tx_rx, const uint16_t data_block_number,
							volatile uint32_t ** p_buffer_address);

/**
 * \brief	This function sets the field Current Block to 0, inside the
 * 			corresponding Indirection Table Word. Thus new incoming messages of
 * 			this subaddress-Tx/Rx will overwrite previous contents from the
 * 			start of the buffer. It is only meaningful in cases with Linear
 * 			Buffer type and long buffers (more than one block);
 * 			otherwise, this function does not have effect.

 * \param[in] subaddress	Subaddress associated to the buffer
 * \param[in] tx_rx	If the buffer is for Tx(TRUE) or Rx(FALSE)
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_INVALID_BUFFER_MODE
 */
uint32_t icu_mil_std_1553_rt_drv_clear_linear_buffer(const uint8_t subaddress,
														const bool_t tx_rx);

/**
 * \brief	This function gets the maximum buffer size (in blocks) of
 * 			subadddress
 *
 * \param[in] subaddress	Subaddress associated to the buffer
 * \param[in] tx_rx	If the buffer is for Tx(TRUE) or Rx(FALSE)
 * \param[out] blocks_size	Reference to user variable where the size will be
 * 				stored
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_NULL_PARAMETER
 */
uint32_t icu_mil_std_1553_rt_drv_get_max_blocks_size_of_buffer(const uint8_t subaddress,
									const bool_t tx_rx, uint32_t * blocks_size);

/**
 * \brief	This function provides the RAM address of the inactive buffer
 * 			associated to a specific subaddress and Tx/Rx when in double buffer
 * 			type mode. The buffer is assumed to be 1-block long. It is user
 * 			responsibility to access the area preventing conflicts with other
 * 			Core accesses (i.e. not using this address after the buffer becomes
 * 			active).
 *
 * \param[in]	subaddress	Subaddress associated to the buffer
 * \param[in]	tx_rx	If the buffer is for Tx(TRUE) or Rx(FALSE)
 * \param[out]	p_buffer_address	Full RAM address of the desired buffer
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_INVALID_BUFFER_MODE
 * 				- ICU_RT_ERROR_NULL_PARAMETER
 */
uint32_t icu_mil_std_1553_rt_drv_get_double_buffer_inactive_addr(const uint8_t subaddress,
																const bool_t tx_rx,
																volatile uint32_t ** p_buffer_address);


/**
 * \brief	This function switches the active and inactive buffers when in
 * 			double buffer type mode. The driver modifies the Core configuration
 * 			so that the new active buffer will be used upon incoming messages of
 * 			that subaddress-Tx/Rx type. The driver will also update any internal
 * 			structures to reflect such new state.
 *
 * \param[in]	subaddress	Subaddress associated to the buffer
 * \param[in]	tx_rx	If the buffer is for Tx(TRUE) or Rx(FALSE)
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_SUBADDRESS_IS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_INVALID_BUFFER_MODE
 */
uint32_t icu_mil_std_1553_rt_drv_switch_double_buffer_active_buffer(const uint8_t subaddress, const bool_t tx_rx);

/**
 * \brief	This function copies to a user buffer the descriptors of all the
 * 			commands received since the last query with this function. It starts
 * 			from the last unread message (identified by the parameter msgIndex),
 * 			and it moves until the last received command from the 1553 Core is
 * 			reached, or the user descriptor buffer is full. Finally, it updates
 * 			the msgIndex to allow a future user request starting from the next
 * 			unread message.
 *
 * \details	The correct behavior of this function is restricted to 1919 commands
 * 			between two consecutive queries
 *
 * \param[in,out]	msg_index	Index of the first message; After the execution
 * 					the value of the index will be the next value after the last
 * 					message
 * \param[out]	msg_buffer	Array of entries, given by the user, where the
 * 					message information  will be stored
 * \param[in,out]	max_number_of_msgs	Maximum number of messages descriptors
 * 					to read; after the execution the parameter will contain
 * 					the number of messages finally read; The maximum number of
 * 					commands is 1919
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_RT_ERROR_MSG_INDEX_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_NUMBER_OF_MSGS_OUT_OF_LIMITS
 * 				- ICU_RT_ERROR_LAST_CMD_INDEX_IS_OUT_LIMITS
 * 				- ICU_RT_ERROR_NULL_PARAMETER
 */
uint32_t icu_mil_std_1553_rt_drv_get_last_msgs(uint16_t * msg_index, RTCmdWordTableEntry_t * msg_buffer, uint16_t * max_number_of_msgs);

/**
 * \brief	This function returns the value of the NOMINAL INTERRUPT REGISTER
 * 			of the 1553 Core
 *
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_rt_drv_get_nominal_irq_reg_value(void);

/**
 * \brief	This function returns the value of the ERROR INTERRUPT REGISTER
 * 			of the 1553 Core
 *
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_rt_drv_get_error_irq_reg_value(void);

/**
 * \brief	This function returns the value of the RESET INTERRUPT REGISTER
 * 			of the 1553 Core
 *
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_rt_drv_get_reset_irq_reg_value(void);

/**
 * \brief	This function returns the value of the CONFIGURATION REGISTER
 * 			of the 1553 Core
 *
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_rt_drv_get_configuration_reg_value(void);

/**
 * \brief	This function returns the value of the COMMAND STATUS REGISTER
 * 			of the 1553 Core
 *
 * \return	The value of the register
 */
uint32_t icu_mil_std_1553_rt_drv_get_command_status_reg_value(void);


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

#endif // MIL_STD_1553_RT_DRV_H
