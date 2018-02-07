/**
 * \file	tm_acq_manager.h
 *
 * \brief	This file defines the public interface of the TM Acquisition driver
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	1-march-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */

#ifndef ICU_TM_ACQ_MANAGER_H
#define ICU_TM_ACQ_MANAGER_H


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/**
 *  @addtogroup DSW_TM_MANAGER TM Manager
 *  @{
 */

/**
 *  @defgroup DSW_TM_MANAGER_DRV TM Manager
 *  @{
 */

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/


/**
 *  @defgroup DSW_TM_MANAGER_DRV_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the TM Acquisition Driver functions
 */

/* The first 6 are the same as SPI driver */
#define 	ICU_TM_ACQ_SPI_ERROR_DEV_ID_ERR		 		(1UL<<1)
#define 	ICU_TM_ACQ_SPI_ERROR_RD_PARITY_ERR	 		(1UL<<2)
#define 	ICU_TM_ACQ_SPI_ERROR_TX_OVERFLOW	 		(1UL<<5)
#define 	ICU_TM_ACQ_SPI_ERROR_APB_ERR		 		(1UL<<6)
#define 	ICU_TM_ACQ_SPI_ERROR_INVALID_INPUT_PARAM  	(1UL<<8)
#define 	ICU_TM_ACQ_SPI_ERROR_TIME_OUT 		 		(1UL<<9)

#define 	ICU_TM_ACQ_ERROR_MODE 				 		(1UL<<16)
#define 	ICU_TM_ACQ_ERROR_MNG_IS_RUNNING 	 		(1UL<<17)
#define 	ICU_TM_ACQ_ERROR_ID_IS_OUT_OF_LIMITS 		(1UL<<18)
#define 	ICU_TM_ACQ_ERROR_MNG_IS_NOT_RUNNING  		(1UL<<19)
#define 	ICU_TM_ACQ_ERROR_TIME_OUT  					(1UL<<20)
#define 	ICU_TM_ACQ_ERROR_INDEX_IS_OUT_OF_LIMITS 	(1UL<<21)
#define 	ICU_TM_ACQ_ERROR_NUMBER_OF_TM_IDS  			(1UL<<22)
#define 	ICU_TM_ACQ_ERROR_NULL_PARAMETER  			(1UL<<23)
#define 	ICU_TM_ACQ_ERROR_ASYNC_EXECUTION			(1UL<<24)
#define 	ICU_TM_ACQ_ERROR_ASYNC_TIMEOUT				(1UL<<25)
#define 	ICU_TM_ACQ_ERROR_TM_TABLE					(1UL<<26)


/// @cond ALTERNATIVE_ERROR_CODES
#define 	TM_ACQ_SPI_ERROR_DEV_ID_ERR		 		ICU_TM_ACQ_SPI_ERROR_DEV_ID_ERR
#define 	TM_ACQ_SPI_ERROR_RD_PARITY_ERR	 		ICU_TM_ACQ_SPI_ERROR_RD_PARITY_ERR
#define 	TM_ACQ_SPI_ERROR_TX_OVERFLOW	 		ICU_TM_ACQ_SPI_ERROR_TX_OVERFLOW
#define 	TM_ACQ_SPI_ERROR_APB_ERR		 		ICU_TM_ACQ_SPI_ERROR_APB_ERR
#define 	TM_ACQ_SPI_ERROR_INVALID_INPUT_PARAM  	ICU_TM_ACQ_SPI_ERROR_INVALID_INPUT_PARAM
#define 	TM_ACQ_SPI_ERROR_TIME_OUT 		 		ICU_TM_ACQ_SPI_ERROR_TIME_OUT

#define 	TM_ACQ_ERROR_MODE 				 		ICU_TM_ACQ_ERROR_MODE
#define 	TM_ACQ_ERROR_MNG_IS_RUNNING 	 		ICU_TM_ACQ_ERROR_MNG_IS_RUNNING
#define 	TM_ACQ_ERROR_ID_IS_OUT_OF_LIMITS 		ICU_TM_ACQ_ERROR_ID_IS_OUT_OF_LIMITS
#define 	TM_ACQ_ERROR_MNG_IS_NOT_RUNNING  		ICU_TM_ACQ_ERROR_MNG_IS_NOT_RUNNING
#define 	TM_ACQ_ERROR_TIME_OUT  					ICU_TM_ACQ_ERROR_TIME_OUT
#define 	TM_ACQ_ERROR_INDEX_IS_OUT_OF_LIMITS 	ICU_TM_ACQ_ERROR_INDEX_IS_OUT_OF_LIMITS
#define 	TM_ACQ_ERROR_NUMBER_OF_TM_IDS  			ICU_TM_ACQ_ERROR_NUMBER_OF_TM_IDS
#define 	TM_ACQ_ERROR_NULL_PARAMETER  			ICU_TM_ACQ_ERROR_NULL_PARAMETER
#define 	TM_ACQ_ERROR_ASYNC_EXECUTION			ICU_TM_ACQ_ERROR_ASYNC_EXECUTION
#define 	TM_ACQ_ERROR_ASYNC_TIMEOUT				ICU_TM_ACQ_ERROR_ASYNC_TIMEOUT
#define 	TM_ACQ_ERROR_TM_TABLE					ICU_TM_ACQ_ERROR_TM_TABLE
/// @endcond

/**
 * @}
 */

/**
 *  @defgroup DSW_TM_MANAGER_DRV_TM_IDS TM IDs
 *  @{
 *  \brief	TM IDs
 */

/* TM IDs */
#define NI_SA_TMP_1_CRYO		(0x00F78140UL)
#define NI_SA_TMP_2_CRYO 		(0x00F70141UL)
#define NI_SA_TMP_3_CRYO		(0x00F70142UL)
#define NI_SA_TMP_4_CRYO		(0x00F78143UL)
#define NI_SA_TMP_5_CRYO		(0x00F70144UL)
#define NI_SA_TMP_6_CRYO		(0x00F78145UL)
#define NI_CSS_TMP_1_CRYO		(0x00F70136UL)
#define NI_CSS_TMP_2_CRYO		(0x00F78137UL)
#define NI_SSS_TMP_1_CRYO		(0x00F70148UL)
#define NI_SSS_TMP_2_CRYO		(0x00F78149UL)
#define NI_FWA_TMP_CRYO			(0x00F7812AUL)
#define NI_GWA_TMP_CRYO			(0x00F7012BUL)

#define NI_SA_TMP_1_FR			(0x00F78120UL)
#define NI_SA_TMP_2_FR			(0x00F70121UL)
#define NI_SA_TMP_3_FR			(0x00F70122UL)
#define NI_SA_TMP_4_FR			(0x00F78123UL)
#define NI_SA_TMP_5_FR			(0x00F70124UL)
#define NI_SA_TMP_6_FR			(0x00F78125UL)
#define NI_CSS_TMP_1_FR			(0x00F78126UL)
#define NI_CSS_TMP_2_FR			(0x00F70127UL)
#define NI_SSS_TMP_1_FR			(0x00F70128UL)
#define NI_SSS_TMP_2_FR			(0x00F78129UL)
#define NI_FWA_TMP_FR			(0x00F7812AUL)
#define NI_GWA_TMP_FR			(0x00F7012BUL)

#define FWA_MOT_PHA_I			(0x08F7822FUL)
#define FWA_MOT_PHB_I			(0x08F7023FUL)
#define GWA_MOT_PHA_I			(0x08F7824FUL)
#define GWA_MOT_PHB_I			(0x08F7025FUL)

#define NI_CU_LED_V_1			(0x30F7026FUL)
#define NI_CU_LED_V_2			(0x30F7827FUL)
#define NI_CU_LED_V_3			(0x30F7828FUL)
#define NI_CU_LED_V_4			(0x30F7029FUL)
#define NI_CU_LED_V_5			(0x30F702AFUL)

#define ICU_5V					(0x08F782BFUL)
#define ICU_5V_CURRENT			(0x08F702CFUL)
#define ICU_CDPU_TMP			(0x18F782DFUL)
#define ICU_LVPS_TMP			(0x18F782EFUL)

#define ASM_TSM1_GAIN			(0x08F7020FUL)
#define ASM_TSM1_OFFSET			(0x08F7821FUL)
#define SSS_CHAIN_CRYO_OFFSET	(0x00F7814CUL)
#define SSS_CRYO_CHAIN_GAIN		(0x00F7014DUL)
#define CSS_CRYO_CHAIN_OFFSET 	(0x00F7813EUL)
#define CSS_CRYO_CHAIN_GAIN		(0x00F7013FUL)

#define CMD_DAS_ALARMS_READ_REQ (0x40CA0000UL)
#define CMD_MOT_HS_READ_REQ 	(0x40C80000UL)

#define OBT_COARSE_HIGH			(0x60800000UL)
#define OBT_COARSE_LOW			(0x60810000UL)
#define OBT_FINE_HIGH			(0x60820000UL)
#define OBT_FINE_LOW			(0x60830000UL)
#define END_OF_TABLE			(0x00000000UL)

/**
 * @}
 */


/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 *  @defgroup DSW_TM_MANAGER_DRV_TYPES Public structures
 *  @{
 *  \brief	Public structures provided by the TM Acquisition Driver
 */
/**
 * \struct	icu_tm_acq_manager_conf_t
 *
 * \brief	Structure used to configure the TM Acquisition Driver
 */
typedef struct
{
	uint16_t settling_time_0; // time unit: 1 us
	uint16_t settling_time_1; // time unit: 1 us
	uint16_t settling_time_2; // time unit: 1 us
	uint16_t settling_time_3; // time unit: 1 us
	bool_t sync_acq_disable;
	uint32_t sync_acq_timeout; // time unit: 100 ns
	bool_t cyclic_mode_enable;
	uint16_t cyclic_period; // time unit: 1 ms
}
icu_tm_acq_manager_conf_t;


/**
 * \struct	icu_tm_acq_manager_status_t
 *
 * \brief	Structure with different information about the status of the TM
 * 			Acquisition Driver
 */
typedef struct
{
	bool_t sync_tm_disable;
	bool_t cyclic_mode;
	bool_t tm_buffer_data_available;
	bool_t tm_buffer_toggle_bit;
	bool_t tm_buffer_lock;
	bool_t running_state;
}
icu_tm_acq_manager_status_t;


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
 *  @defgroup DSW_TM_MANAGER_DRV_API Public functions
 *  @{
 *  \brief	Public functions provided by the TM Acquisition Driver
 */

/**
 * \brief	This function initializes the TM HK Acquisition system
 *
 * \param[in] config	Structure with the configuration
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_TM_ACQ_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_TM_ACQ_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_TM_ACQ_SPI_ERROR_APB_ERR
 * 				- ICU_TM_ACQ_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_TM_ACQ_SPI_ERROR_TIME_OUT
 * 				- ICU_TM_ACQ_ERROR_TIME_OUT
 * 				- ICU_TM_ACQ_ERROR_NULL_PARAMETER
 */
uint32_t icu_tm_acq_mng_initialize (const icu_tm_acq_manager_conf_t* config);

/**
 * \brief	This function loads the whole TM ID Table and writes the EOT
 * 			into the end of the table. This function will add a EOT identifier
 * 			after the user TMIDs, to guarantee that EOT	is always reached.
 *          The set of valid TMIDs are defined in the .h file,
 *          and reflect table 4.7.1-2 from the HW/SW ICD.
 *
 * \param[in] number_of_tms Number of TM IDs to load
 * \param[in] tm_ids	An user array with TM IDs to copy into the table
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_INDEX_IS_OUT_OF_LIMITS
 * 				- ICU_TM_ACQ_ERROR_NULL_PARAMETER
 * 				- ICU_TM_ACQ_ERROR_MNG_IS_RUNNING
 */
uint32_t icu_tm_acq_mng_load_whole_tm_id_table(const uint32_t number_of_tms,
		const uint32_t tm_ids[]);

/**
 * \brief	This function sets the acquisition mode to one shot. The TM
 * 			Acquisition manager has to be in stop state
 *
  * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_MNG_IS_RUNNING
 */
uint32_t icu_tm_acq_set_mode_one_shot(void);

/**
 * \brief	This function sets the acquisition mode to cyclic. The TM
 * 			Acquisition manager has to be in stop state
 *
 * \param[in] acquisition_cycle	Minimum cycle time, in msec, for each cycle
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_MNG_IS_RUNNING
 */
uint32_t icu_tm_acq_set_mode_cyclic(const uint16_t acquisition_cycle);

/**
 * \brief	This function enables the synchronization of acquisitions and sets the
 *          timeout of the synchronization process.
 *
 * \param[in] sync_acq_timeout	Maximum amount of time that the TMID manager
 *            will wait for a synchronization pulse when processing a synchronous analog TMID.
 *            LSB is 100ns. Range is [1, 2^20-1] LSBs.
 *
 *            Note: This time out procedure is used for LED TMs, where the acquisition
 *            must be done when the current PWM cycle high. The LED clock frequency is 4.882KHz.
 *            Thus a reasonable value is at least a couple of PWM clock cycle:
 *            aprox 410 us (i.e. 4100 LSBs).
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_TIME_OUT
 * 				- ICU_TM_ACQ_ERROR_MNG_IS_RUNNING
 */
uint32_t icu_tm_acq_mng_enable_synchronization(const uint32_t sync_acq_timeout);

/**
 * \brief	This function disables the synchronization of acquisitions
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_MNG_IS_RUNNING
 */
uint32_t icu_tm_acq_mng_disable_synchronization(void);

/**
 * \brief	This function starts the table TM acquisition. The table can be
 * 			cyclic or one-shot
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_MNG_IS_RUNNING
 */
uint32_t icu_tm_acq_mng_start_tm_acq(void);

/**
 * \brief	This function stops the cyclic TM acquisition at the end of
 * 			the current table. It does not affect one-shot or asynchronous
 * 			acquisition
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_MNG_IS_NOT_RUNNING
 */
uint32_t icu_tm_acq_mng_stop_tm_acq(void);

/**
 * \brief	This function reads the inactive buffer that contains the TM from
 * 			the previous cycle. The function locks the buffer before reading and
 *			unlocks after reading. FPGA buffer is accessed in 32-bit read instructions.
 *          Acquired TM values are returned including the error bit (bit 15) as reported by FPGA.
 *          In addition, if error bit is set in one or more TM values (except for OBT-related TM IDs,
 *          namely OBT_COARSE_HIGH, OBT_COARSE_LOW, OBT_FINE_HIGH, OBT_FINE_LOW, in which bit 15 is not
 *          an error bit), the function returns the error code ICU_TM_ACQ_ERROR_TM_TABLE.
 *
 * \param[in] number_of_ids	Defines the number of TM to retrieve,
 * 				range in [1, 255]
 * \param[out] p_tm_values	Pointer to a user memory space where the TM values
 * 				are copied
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_NUMBER_OF_TM_IDS
 * 				- ICU_TM_ACQ_ERROR_NULL_PARAMETER
 *              - ICU_TM_ACQ_ERROR_TM_TABLE
 */
uint32_t icu_tm_acq_mng_get_tms(const uint32_t number_of_ids, uint16_t p_tm_values[]);

/**
 * \brief	This function gets the TM manager status
 *
 * \param[out] p_tm_acq_status	Pointer to structure where TM status is provided
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_TM_ACQ_ERROR_NULL_PARAMETER
 */
uint32_t icu_tm_acq_mng_get_status (icu_tm_acq_manager_status_t * p_tm_acq_status);

/**
 * \brief	This function performs a single TM ID asynchronous execution when
 * 			the current TMID, if any, finishes. It waits internally for the
 * 			period of time provided as parameter plus the estimated time a TMID
 * 			needs to finish (worst case).
 *          Acquired TM value is returned including the error bit (bit 15)
 *          as reported by FPGA.
 *          In addition, if error bit is set in TM value,
 *          the function returns the error code ICU_TM_ACQ_ERROR_ASYNC_EXECUTION.
 *			NOTE: The following TM IDs are not supported by FPGA in asynchronous execution:
 *          OBT_COARSE_HIGH, OBT_COARSE_LOW, OBT_FINE_HIGH, OBT_FINE_LOW.
 *          If any of these TM IDs is used as input, the function will return the error
 *          code ICU_TM_ACQ_ERROR_ASYNC_EXECUTION.
 *
 * \param[in] tm_id Identifier of the TM
 * \param[in] delay_ms	Defines the amount of milliseconds (integer) to wait
 * 				associated with the TMID until its result is ready. However,
 * 				a higher wait will be executed when taking into account the
 * 				current TMID in execution, if any.
 *
 * \param[out] acquired_tm	Contains the result of the TMID
 * 				acquisition
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 						- ICU_TM_ACQ_ERROR_ASYNC_EXECUTION
 * 						- ICU_TM_ACQ_ERROR_ASYNC_TIMEOUT
 * 						- ICU_TM_ACQ_ERROR_NULL_PARAMETER
 */
uint32_t icu_tm_acq_mng_get_asynch_tm(const uint32_t tm_id,
		const uint32_t delay_ms, uint16_t * acquired_tm);



/**
 * \brief	This function reads the value of last TM ID asynchronous acquisition.
 *          It shall be called from Interrupt Handler Routine associated to TM
 *          asynchronous acquisition finish, if this interrupt is enabled.
 *          Acquired TM value is returned including the error bit (bit 15)
 *          as reported by FPGA.
 *          In addition, if error bit is set in TM value,
 *          the function returns the error code ICU_TM_ACQ_ERROR_ASYNC_EXECUTION.
 *
 * \param[out] acquired_tm	Contains the result of the last TMID acquisition
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 						- ICU_TM_ACQ_ERROR_NULL_PARAMETER
 */
uint32_t icu_tm_acq_mng_read_asynch_tm_last_value(uint16_t* acquired_tm);


/**
 * \brief	This function performs a reset of the TM Manager FPGA module.
 *
 * \return	void
 */
void icu_tm_acq_mng_reset(void);

/**
 * @}
 */

/**
 * @}
 */
/**
 * @}
 */



#endif // ICU_TM_ACQ_MANAGER_H
