/**
 * \file	fwa_gwa_motors.h
 *
 * \brief	This file defines the public interface of the FWA & GWA Motors
 * 			driver
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	10-march-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcalá.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */


#ifndef ICU_FWA_GWA_MOTORS_H
#define ICU_FWA_GWA_MOTORS_H



/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>
#include <public/fpga_addresses.h>

/**
 *  @addtogroup DSW_MOTORS FWA GWA Motors
 *  @{
 */

/**
 *  @defgroup DSW_FWA_GWA_MOTORS FWA GWA Motors
 *  @{
 */

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/* Return error codes */

/**
 *  @defgroup DSW_FWA_GWA_MOTORS_ERROR_CODES Error Codes
 *  @{
 *  \brief	Error codes returned by the FWA & GWA Motor driver functions
 */
/* The first 6 are the same as SPI driver */
#define 	ICU_MOTOR_SPI_ERROR_DEV_ID_ERR	 		(1UL<<1)
#define 	ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR		(1UL<<2)
#define 	ICU_MOTOR_SPI_ERROR_TX_OVERFLOW	 		(1UL<<5)
#define 	ICU_MOTOR_SPI_ERROR_APB_ERROR			(1UL<<6)
#define 	ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM (1UL<<8)
#define 	ICU_MOTOR_SPI_ERROR_TIME_OUT 		 	(1UL<<9)
#define 	ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE	 	(1UL<<10)

#define 	ICU_MOTOR_ERROR_MOTOR_ID  				(1UL<<16)
#define 	ICU_MOTOR_ERROR_MICROSTEP_NUMBER  		(1UL<<17)
#define 	ICU_MOTOR_ERROR_PROFILE_IS_RUNNING  	(1UL<<18)
#define 	ICU_MOTOR_ERROR_DRV_IS_NOT_ON  			(1UL<<19)
#define 	ICU_MOTOR_ERROR_NULL_PARAMETER  		(1UL<<20)
#define 	ICU_MOTOR_ERROR_TM_ASYNCH_ACQUISITION  	(1UL<<21)
#define 	ICU_MOTOR_ERROR_PHASES_VALUE  			(1UL<<22)
#define 	ICU_MOTOR_ERROR_HS_DRIVER_OUTPUT  		(1UL<<23)
#define 	ICU_MOTOR_ERROR_HS_TM_ACQ_EXECUTION		(1UL<<24)
#define 	ICU_MOTOR_ERROR_HS_TM_ACQ_TIMEOUT		(1UL<<25)
#define 	ICU_MOTOR_ERROR_PROFILE_NOT_ALIGNED		(1UL<<26)
#define 	ICU_MOTOR_ERROR_INVALID_INPUT_PARAM		(1UL<<27)


/// @cond ALTERNATIVE_ERROR_CODES
/*Alternative error code values*/
#define 	MOTOR_SPI_ERROR_DEV_ID_ERR	ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
#define 	MOTOR_SPI_ERROR_RD_PARITY_ERR	ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
#define 	MOTOR_SPI_ERROR_TX_OVERFLOW	ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
#define 	MOTOR_SPI_ERROR_APB_ERROR	ICU_MOTOR_SPI_ERROR_APB_ERROR
#define 	MOTOR_SPI_ERROR_INVALID_INPUT_PARAM	ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
#define 	MOTOR_SPI_ERROR_TIME_OUT	ICU_MOTOR_SPI_ERROR_TIME_OUT
#define 	MOTOR_SPI_ERROR_COMM_IN_COURSE	ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE

#define 	MOTOR_ERROR_MOTOR_ID	ICU_MOTOR_ERROR_MOTOR_ID
#define 	MOTOR_ERROR_MICROSTEP_NUMBER	ICU_MOTOR_ERROR_MICROSTEP_NUMBER
#define 	MOTOR_ERROR_PROFILE_IS_RUNNING	ICU_MOTOR_ERROR_PROFILE_IS_RUNNING
#define 	MOTOR_ERROR_DRV_IS_NOT_ON	ICU_MOTOR_ERROR_DRV_IS_NOT_ON
#define 	MOTOR_ERROR_NULL_PARAMETER	ICU_MOTOR_ERROR_NULL_PARAMETER
#define 	MOTOR_ERROR_TM_ASYNCH_ACQUISITION	ICU_MOTOR_ERROR_TM_ASYNCH_ACQUISITION
#define 	MOTOR_ERROR_PHASES_VALUE	ICU_MOTOR_ERROR_PHASES_VALUE
#define 	MOTOR_ERROR_HS_DRIVER_OUTPUT	ICU_MOTOR_ERROR_HS_DRIVER_OUTPUT
#define 	MOTOR_ERROR_HS_TM_ACQ_EXECUTION	ICU_MOTOR_ERROR_HS_TM_ACQ_EXECUTION
#define 	MOTOR_ERROR_HS_TM_ACQ_TIMEOUT	ICU_MOTOR_ERROR_HS_TM_ACQ_TIMEOUT
#define 	MOTOR_ERROR_PROFILE_NOT_ALIGNED	ICU_MOTOR_ERROR_PROFILE_NOT_ALIGNED
#define 	MOTOR_ERROR_INVALID_INPUT_PARAM	ICU_MOTOR_ERROR_INVALID_INPUT_PARAM
/// @endcond

/**
 * @}
 */
/**
 *  @defgroup DSW_FWA_GWA_MOTORS_CONST Constants
 *  @{
 */

/** FWA MOTOR ID */
#define 	FWA_MOTOR_ID  (0UL)

/** GWA MOTOR ID */
#define 	GWA_MOTOR_ID  (1UL)

/* profile_exec_status */
#define 	MOTOR_PROFILE_IDLE  			0x0U
#define 	MOTOR_PROFILE_EXEC_IN_PROGRESS  0x1U

/* drv_exec_status */
#define 	MOTOR_DRV_SAFE_STATE  		0x0U
#define 	MOTOR_DRV_CONTROL_DISABLE  	0x1U
#define 	MOTOR_DRV_CONTROL_ON  		0x2U

/**
 * @}
 */

/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 *  @defgroup DSW_FWA_GWA_MOTORS_TYPES Structures
 *  @{
 *  \brief	Different structures handled by the FWA & GWA Motors Driver
 */

/**
 * \struct	icu_fwa_gwa_microstep_t
 * \brief
 */
typedef struct
{
	uint16_t	microstep_time;
	int8_t		phase_a;
	int8_t		phase_b;
}
icu_fwa_gwa_microstep_t;

/**
 * \struct	icu_fwa_gwa_status_t
 * \brief	Different information about the status of the Motors module
 */
typedef struct
{
	uint32_t	profile_exec_status;
	uint32_t 	drv_exec_status;
}
icu_fwa_gwa_status_t;

/**
 * \struct	icu_fwa_gwa_phase_status_t
 * \brief	Different information about the phase status
 */
typedef struct
{
	int8_t 		direct_phase_a_current_output;
	int8_t 		direct_phase_b_current_output;
	uint16_t 	direct_microstep_exec_time;
	int8_t 		direct_microstep_exec_phase_a_current;
	int8_t 		direct_microstep_exec_phase_b_current;
}
icu_fwa_gwa_phase_status_t;

/**
 * \struct	icu_fwa_gwa_errors_t
 * \brief	Different information about the possible errors produced into the
 * 			Motors module
 */
typedef struct
{
	bool_t spw_sequence;
	bool_t spw_escape;
	bool_t spw_parity;
	bool_t spw_credit;
	bool_t spw_disconnect;
	bool_t spw_eep_detected;
	bool_t unexpected_profile_data_received;
	bool_t profile_fifo_edac_uncorrectable;
	bool_t profile_fifo_overflow;
	bool_t lut_edac_uncorrectable;
	bool_t size_error;
	bool_t profile_fifo_underflow;
}
icu_fwa_gwa_errors_t;

/**
 * @}
 */

/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_FWA_GWA_MOTORS_API Public Functions
 *  @{
 *  \brief	Public functions provided by the FWA & GWA Motors Driver
 */


/**
 * \brief	This function gets the fwa_gwa status, which can be used for
 * 			monitoring the current configuration and the execution status
 * 			("in_progress" or "idle", used to detect by polling the end of a
 * 			profile execution)
 *
 * \param[out] status	status of the motor driver
 *
 * \param[out] phase_status	Information status about phases of the motors
 *
 * \param[out] number_of_microsteps_executed	Number of steps executed since
 * 				the last profile execution start command
 *
 * \param[out] last_microstep_in_execution	Current microstep in execution
 * 				(last microstep executed if idle)
 *
 * \param[out] execution_errors Errors detected in profile execution
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_MOTOR_ERROR_NULL_PARAMETER
 */
uint32_t icu_fwa_gwa_drv_get_status (icu_fwa_gwa_status_t * status,
										icu_fwa_gwa_phase_status_t  * phase_status,
										uint16_t * number_of_microsteps_executed,
										icu_fwa_gwa_microstep_t * last_microstep_in_execution,
										icu_fwa_gwa_errors_t * execution_errors);

/**
 * \brief	This function initializes the complete FWA/GWA system and the
 * 			SpW channel. It sends SPI commands to MOTOR_CONFx with the constant
 * 			values declared in HW/SW ICD  (0x01510340, 0x01538340, 0x01558100,
 * 			0x01570100 respectively).
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_MOTOR_SPI_ERROR_APB_ERROR
 * 				- ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_MOTOR_SPI_ERROR_TIME_OUT
 * 				- ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE
 */
uint32_t icu_fwa_gwa_drv_initialize(void);

/**
 * \brief	This function resets the complete FWA/GWA system and the SpW channel
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 				- ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_MOTOR_SPI_ERROR_APB_ERROR
 * 				- ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_MOTOR_SPI_ERROR_TIME_OUT
 * 				- ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE
 */
uint32_t icu_fwa_gwa_drv_reset(void);

/**
 * \brief	This function sets the requested energization level to selected
 * 			motor according to the energization procedure. It includes several waiting steps
 *          made by ASW with RTOS means by means of external wait function.
 *
 *          Note: The user can set the parameters phase A and phase B both to 0 mA.
 *          This choice is not operationally correct (no step state starts with both coils set to 0),
 *          but it is allowed by the DSW in order to allow SW integration tests.
 *          Therefore, when the user calls energize with phase A & B set to 0, the function returns
 *          an error code MOTOR_ERROR_PHASES_VALUE, but the energization process is actually properly
 *          executed and the user can now execute a profile.
 *
 *          Note: If on the energization call, the ASW receives error code MOTOR_ERROR_PHASES_VALUE,
 *          it should trigger the motor protection FDIR, unless the protection is already disabled
 *          or the initial phases A & B are set to 0 mA (see previous note and FDIR use case description).
 *
 *
 * \param[in] motor_id	identifier of the motor to be energized
 * \param[in] energization_phase_a	Value on phase A, LSB is 1.65 mA, parameter range is
 *                                  therefore aprox [-211, 210] mA
 * \param[in] energization_phase_b	Value on phase B. See phase A parameter definition.
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_MOTOR_OP_SUCCESSFUL
 * 				- ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_MOTOR_SPI_ERROR_APB_ERROR
 * 				- ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_MOTOR_SPI_ERROR_TIME_OUT
 * 				- ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE
 *				- ICU_MOTOR_ERROR_MOTOR_ID
 *				- ICU_MOTOR_ERROR_PROFILE_IS_RUNNING
 *				- ICU_MOTOR_ERROR_TM_ASYNCH_ACQUISITION
 * 				- ICU_MOTOR_ERROR_PHASES_VALUE
 */
uint32_t icu_fwa_gwa_drv_energize (const uint32_t motor_id, int8_t energization_phase_a, int8_t energization_phase_b);

/**
 * \brief	This function starts the execution of a complete profile. This is a
 * 			"fire&forget" function. After the profile is launched, the execution
 * 			is performed  by the SpW DMA channel and FPGA, without SW
 * 			intervention. The user must synchronize with the end of the
 * 			execution in one of two ways: activating the "end-of-profile"
 * 			interrupt; or by polling the field MOTOR_CTRL_STATUS.
 * 			ProfileExecCtrl_Status (when 0, then execution is over). The
 * 			microstep buffer provided by the user can be used as the actual
 * 			packet pointer in the SpW interface (i.e. the user must not modify
 * 			the buffer contents during the profile execution).
 *          In order to execute a profile, motor has to be previously energized.
 *
 * \param[in] microstep_number	Number of microsteps in the profile
 * \param[in] p_microsteps	Address in RAM where the microsteps are stored
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_MOTOR_ERROR_NULL_PARAMETER
 *				- ICU_MOTOR_ERROR_DRV_IS_NOT_ON
 *				- ICU_MOTOR_ERROR_MICROSTEP_NUMBER
 *				- ICU_MOTOR_ERROR_PROFILE_IS_RUNNING
 */
uint32_t icu_fwa_gwa_drv_execute_profile (const uint16_t microstep_number,
		const icu_fwa_gwa_microstep_t * p_microsteps);

/**
 * \brief	This function stops the current profile execution and resets the
 * 			SpW channel.
 *
 * \return	void
 */
void icu_fwa_gwa_drv_stop(void);

/**
 * \brief	This function executes a unique microstep, without using the SpW
 * 			channel. The HW driver must not be in execution. This function
 * 			includes the energizing procedure.
 *
 * \param[in] motor_id	Identifier of the motor to execute profile
 * \param[in] microstep	Microstep information
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_MOTOR_SPI_ERROR_APB_ERROR
 * 				- ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_MOTOR_SPI_ERROR_TIME_OUT
 * 				- ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE
 *				- ICU_MOTOR_ERROR_MOTOR_ID
 *				- ICU_MOTOR_ERROR_PROFILE_IS_RUNNING
 */
uint32_t icu_fwa_gwa_drv_execute_single_microstep (const uint32_t motor_id, icu_fwa_gwa_microstep_t* microstep);


/**
 * \brief	This function de-energizes motor, following the appropriate
 * 			 procedure.
 *
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_MOTOR_SPI_ERROR_APB_ERROR
 * 				- ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_MOTOR_SPI_ERROR_TIME_OUT
 * 				- ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE
 */
uint32_t icu_fwa_gwa_drv_deenergize (void);


/**
 * \brief	This function sends a SPI command to enable or disable the Motor
 * 			Current Protection mechanism. In general, it should be used only on
 * 			FDIR recovery action in order to disable the protection mechanism
 * 			(i.e. do_enable = FALSE)
 *
 * \param[in] do_enable	enable (TRUE) or disable (FALSE) the Motor Current
 * 			Protection mechanism.
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_MOTOR_SPI_ERROR_APB_ERROR
 * 				- ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_MOTOR_SPI_ERROR_TIME_OUT
 * 				- ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE
 */
uint32_t icu_fwa_gwa_drv_enable_current_protection (const bool_t do_enable);



/**
 * \brief	This function loads two LUT tables (corresponding to phase A and
 * 			phase B) into FPGA in order to perform driver motor calibration.
 * 			It also enables/disables the use of these LUTs.
 *
 * \param[in] phase_a_lut_data	Array containing 256 values to be applied for
 * 			calibration in motor driver phase A (only 10 LSBs are used in each
 * 			value)
 * \param[in] phase_b_lut_data	Array containing 256 values to be applied for
 * 			calibration in motor driver phase B (only 10 LSBs are used in each
 * 			value)
 * \param[in] enable_luts	enable (TRUE) or disable (FALSE) the use of LUTs..
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_MOTOR_ERROR_NULL_PARAMETER
 *				- ICU_MOTOR_ERROR_PROFILE_IS_RUNNING
 */
uint32_t icu_fwa_gwa_drv_load_lut_tables(const uint16_t phase_a_lut_data[],
					const uint16_t phase_b_lut_data[], bool_t enable_luts);


/**
 * \brief	This function performs a reading of the two digital Home Sensor
 * 			signals that indicate the Home Sensor state. It will connect HS
 * 			driver to motor, switch on HS driver, set detection threshold,
 * 			enable detection, read both HS input signals, disable detection,
 * 			switch off HS driver and disconnect HS driver from motor.
 *
 *           Note: The HS system is required to be able to produce a 0.5 mV
 *           marginal signal that can be then detected without false positives.
 *           That corresponds to a threshold SW parameter equal to the maximum (255).
 *           However, for future potential aging of the subcomponents, where a lower
 *           voltage signal might be produced, the ICU design allows the user to reduce
 *           the threshold (for instance to 0.2 mV, i.e. threshold param=105)
 *           in order to be able to detect the signal, although with the risk
 *           of detecting false positives.
 *
 * \param[in] motor_id	Identifier of the motor from which Home Sensor state
 * 				will be read
 * \param[in] detection_threshold: Detection threshold to be applied in HS
 * 				detection (1 LSB = 0.0019 mV)
 * \param[out] hs_input_1 (out): TRUE if hs_input1 digital signal equals 1,
 * 				FALSE if hs_input1 digital signal equals 0
 * \param[out] hs_input_2 (out): TRUE if hs_input2 digital signal equals 1,
 * 				FALSE if hs_input2 digital signal equals 0
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 * 				- ICU_MOTOR_SPI_ERROR_DEV_ID_ERR
 * 				- ICU_MOTOR_SPI_ERROR_RD_PARITY_ERR
 * 				- ICU_MOTOR_SPI_ERROR_TX_OVERFLOW
 * 				- ICU_MOTOR_SPI_ERROR_APB_ERROR
 * 				- ICU_MOTOR_SPI_ERROR_INVALID_INPUT_PARAM
 * 				- ICU_MOTOR_SPI_ERROR_TIME_OUT
 * 				- ICU_MOTOR_SPI_ERROR_COMM_IN_COURSE
 *				- ICU_MOTOR_ERROR_MOTOR_ID
 *				- ICU_MOTOR_ERROR_PROFILE_IS_RUNNING
 *				- ICU_MOTOR_ERROR_NULL_PARAMETER
 *				- ICU_MOTOR_ERROR_HS_TM_ACQ
 *				- ICU_MOTOR_ERROR_HS_DRIVER_OUTPUT
 */
uint32_t icu_fwa_gwa_drv_hs_state_read (const uint32_t motor_id,
											uint8_t detection_threshold,
											bool_t* hs_input_1,
											bool_t* hs_input_2
										);


/**
 * \brief	This function provides the state of motor HW alarms.
 *
 * \param[out] vref_prot	VREF protection alarm (TRUE if alarm is active,
 * 				 FALSE otherwise)
 * \param[out] motor_pwm_prot_ph_a	Motor phase B PWM protection alarm
 * 				(TRUE if alarm is active, FALSE otherwise)
 * \param[out] motor_pwm_prot_ph_b	Motor phase A PWM protection alarm
 * 				(TRUE if alarm is active, FALSE otherwise)
 * \param[out] motor_driver	Motor driver alarm (TRUE if alarm is active,
 * 				FALSE otherwise)
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_MOTOR_ERROR_NULL_PARAMETER
 */
uint32_t icu_fwa_gwa_drv_get_hw_alarms(bool_t* vref_prot,
										bool_t* motor_pwm_prot_ph_a,
										bool_t* motor_pwm_prot_ph_b,
										bool_t* motor_driver
										);


/**
 * \brief	This function configures the frequency of Home Sensor clock.
 * 			This frequency will be calculated by HW according to the formula:
 *               F_home_sensor = F_system/((Value+1)*2)
 *          being "Value" the function input parameter (range [0..0x1FF]
 * 			and F_system = 40 MHz
 *
 *          NOTE: Reset value = 0x42 -> Reset HS clock freq = 298.5 KHz
 *
 * \param[in] value	Value to be set according to formula
 *
 * \return	The function returns 0 if was executed successfully, an error code
 * 			if some error happened. The error codes returned by this function
 * 			can be the following:
 *				- ICU_MOTOR_ERROR_INVALID_INPUT_PARAM
 *              - ICU_MOTOR_ERROR_PROFILE_IS_RUNNING
 */
uint32_t icu_fwa_gwa_drv_hs_config_frequency( const uint32_t value );



/**
 * @}
 */
/**
 * @}
 */

/**
 * @}
 */

#endif // ICU_FWA_GWA_MOTORS_H
