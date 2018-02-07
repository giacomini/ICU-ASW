/**
 * \file	PUS_Service_8_1_fwa_gwa.h
 *
 * \brief	This file contains declaration of components used by ICU_ASW to manage wheels.
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	20-04-2017
 *
 */

#ifndef PUS_SERVICE_8_1_FWA_GWA_H_
#define PUS_SERVICE_8_1_FWA_GWA_H_

#include "ICU_config.h"
#include "tc_header.h"

/**
 *  \ingroup FWA_GWA_CONSTANTS
 *  \{
 */

/* FDL EEPROM memory address */
#define FDL_1_ADDRESS 0X103FE000U //!< EEPROM address of the nominal file directory list
#define FDL_2_ADDRESS 0X103FE800U //!< EEPROM address of the redundant file directory list

/* First RAM address to be used to store wheel profiles */
#define WPF_BASE_RAM_ADDRESS 0x40610000U //!< First address in RAM used to store wheel profiles

/* Profile size */
#define WHEELS_PFS_SIZE_IN_RAM 0x80000U //!< Size in byte of data area of the profiles loaded in RAM (524.288 Bytes)

/* Mask to be applied to "wheel_flag" variable */
#define WP_IN_EXECUTION 0X1U //!< Wheel profile execution is running
#define HS_SEARCH 0X2U //!< Home sensor search procedure is running
#define HS_CHECK 0X4U //!< Home sensor check procedure is running
#define CHECK_CONNECTION 0X8U //!< Check connection procedure is running
#define WP_TO_BE_STARTED 0X10U //!< Wheel profile to be started after 500ms from beginning of major frame
#define START_MOVEMENT 0x20U //!< Notify wheel start movement
#define HS_OFFSET 0x40U //!< Notify the offset after a home sensor check procedure
#define END_MOVEMENT 0x80U //!< Notify end of movement

/* Cmd execution phases */
#define ENERGIZE_PH 0x1U //!< Error during Energize
#define PROFILE_EXEC_PH 0x2U //!< Error during during Profile execution
#define DEENERGIZE_PH 0x4U //!< Error during Deenergize
#define HS_READ_PH 0x8U //!< Error during acquisition of Home Sensor value
#define HS_NOT_FOUND 0x10U //!< Home sensor not found
/**
 * \}
 */

/**
 *  \ingroup FWA_GWA_TYPES
 *  \{
 */

/**
 * \struct wheel_profile_t
 * \brief Structure defined for manage wheel profile. For each profile there are address in RAM, number of degrees and estimated execution time.
 */
typedef struct {
	uint32_t address;
	int16_t degree;
	uint32_t execution_time;
} wheel_profile_t;

/**
 * \struct wheel_profile_manager_t
 * \brief Structure defined to handle all wheel profiles.
 */
typedef struct {
	wheel_profile_t fwa_cw_72;
	wheel_profile_t fwa_c_cw_72;
	wheel_profile_t fwa_cw_144;
	wheel_profile_t fwa_c_cw_144;
	wheel_profile_t fwa_cw_1_1_2;
	wheel_profile_t fwa_cw_1_2_3;
	wheel_profile_t fwa_cw_1_3_4;
	wheel_profile_t fwa_cw_1_4_1;
	wheel_profile_t fwa_c_cw_1_1_4;
	wheel_profile_t fwa_c_cw_1_4_3;
	wheel_profile_t fwa_c_cw_1_3_2;
	wheel_profile_t fwa_c_cw_1_2_1;
	wheel_profile_t fwa_cw_4;
	wheel_profile_t fwa_c_cw_4;
	wheel_profile_t fwa_to_zero;
	wheel_profile_t fwa_to_start;
	wheel_profile_t gwa_cw_72;
	wheel_profile_t gwa_c_cw_72;
	wheel_profile_t gwa_cw_144;
	wheel_profile_t gwa_c_cw_144;
	wheel_profile_t gwa_cw_1_1_2;
	wheel_profile_t gwa_cw_1_2_3;
	wheel_profile_t gwa_cw_1_3_4;
	wheel_profile_t gwa_cw_1_4_1;
	wheel_profile_t gwa_c_cw_1_1_4;
	wheel_profile_t gwa_c_cw_1_4_3;
	wheel_profile_t gwa_c_cw_1_3_2;
	wheel_profile_t gwa_c_cw_1_2_1;
	wheel_profile_t gwa_cw_4;
	wheel_profile_t gwa_c_cw_4;
	wheel_profile_t gwa_to_zero;
	wheel_profile_t gwa_to_start;
} wheel_profile_manager_t;

/**
 * \enum PROFILE_LIST_INDEX
 * \brief List of all ICU wheel profiles.
 */
typedef enum {
	FWA_CW_72 = 0U,
	FWA_C_CW_72,
	FWA_CW_144,
	FWA_C_CW_144,
	FWA_CW_1_1_2,
	FWA_CW_1_2_3,
	FWA_CW_1_3_4,
	FWA_CW_1_4_1,
	FWA_C_CW_1_1_4,
	FWA_C_CW_1_4_3,
	FWA_C_CW_1_3_2,
	FWA_C_CW_1_2_1,
	FWA_CW_4,
	FWA_C_CW_4,
	FWA_TO_ZERO,
	FWA_TO_START,
	GWA_CW_72, /* 16 */
	GWA_C_CW_72,
	GWA_CW_144,
	GWA_C_CW_144,
	GWA_CW_1_1_2,
	GWA_CW_1_2_3,
	GWA_CW_1_3_4,
	GWA_CW_1_4_1,
	GWA_C_CW_1_1_4,
	GWA_C_CW_1_4_3,
	GWA_C_CW_1_3_2,
	GWA_C_CW_1_2_1,
	GWA_CW_4,
	GWA_C_CW_4,
	GWA_TO_ZERO,
	GWA_TO_START,
	MAX_PROFILES_NUMBER, /* 32 */
} PROFILE_LIST_INDEX;

/**
 * \}
 */

/**
 *  \ingroup FWA_GWA_VARIABLES
 *  \{
 */

/*Variable used to lock memory area for wheel profiles*/
extern uint32_t pf_area[WHEELS_PFS_SIZE_IN_RAM / 4];

/* Variable containing the wheel profile addresses in RAM */
extern wheel_profile_manager_t wheel_profiles;

/* Flag used by the ICU ASW to know the state of the wheels */
extern uint8_t wheel_flag;

/* Variable used to store home sensor center position */
extern uint16_t hs_center_search;

/**
 * \}
 */

/**
 *  \ingroup FWA_GWA_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to copy wheels profile from EEPROM to RAM
 * \param[out] fwa_gwa_single_error Variable used to store information concerning FWA nominal profile download. Bits set to 1 show error during deploy.
 * \param[out] fwa_gwa_double_error Variable used to store information concerning FWA redundant profile download. Bits set to 1 show error during deploy.
 * \param[out] fdl_error Variable used to store errors during deploy of the FDL files. A 0 means that the operation was successfully executed, 1 means error.
 * Bit 0: FDL 1 not deployed
 * Bit 1: FDL 1 CRC error
 * Bit 2: FDL 2 not deployed
 * Bit 3: FDL 2 CRC error
 * bit 4..7: 0
 * \return void
 */
void asw_store_wheels_profile_in_RAM(uint16_t *fwa_gwa_single_error,
		uint16_t *fwa_gwa_double_error, uint8_t *fdl_error);

/**
 * \brief Function used to load in RAM a file containing a single profile from EEPROM
 * \param[in] eeprom_address Array containing EEPROM address of the file to be deployed.
 * \param[in] ram_address Address in RAM where profile must be loaded.
 * \param[in] bit_shift Bit to be shifted in case of error.
 * \param[out] single_error Variable used to store information concerning FWA nominal profile download. Bits set to 1 show error during deploy.
 * \param[out] double_error Variable used to store information concerning GWA nominal profile download. Bits set to 1 show error during deploy.
 * \return 0 if successfully executed, error code otherwise.
 */
uint32_t asw_dump_EEPROM_file(uint32_t eeprom_address[], uint32_t ram_address,
		uint8_t bit_shift, uint16_t *single_error, uint16_t *double_error);

/**
 * \brief Function used to deploy File Directory List from EEPROM
 * \param[out] profile_addresses Array containing the eeprom address of the header of the wheel profile OBFs.
 * \param[out] errors Variable used to store errors during deploy of the FDL files. A 0 means that the operation was successfully executed, 1 means error.
 * Bit 0: FDL 1 CRC error
 * Bit 1: FDL 2 CRC error
 * bit 2..7: Not Used
 * \return void
 */
void asw_deploy_FDL(uint32_t profile_addresses[], uint8_t *errors);

/**
 * \brief High level function used to manage wheel TCs. It will choose operation to be done based on functionID received in the TC Data area.
 * It will check that motorId value is correct and that CU is off and restart asw_task_wheel_exec.
 * In case of functionID 0x80001(execute profile) check also a synch parameter. If it is on wheel movement will be postponed to 500ms after reception of 1553 synch w/o DW Mode Code
 * \param[in] tc TC sent by SpaceCraft.
 * \return ICU_SUCCESSFUL if successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_8_x_move_wheels(telecommand_t tc);

/**
 * \brief Function that execute a wheel profile address.
 * \param[in] motor_id [1 byte]: Motor to be move. 0 for FWA, 1 for GWA.
 * \param[in] profile_idx [1 byte]: Profile index to be executed.
 * \param[out] errors Phase of errors used to generate event packet. For this function maybe ENERGIZE_PH or PROFILE_EXEC_PH
 * \return 0 if successfully executed, a DSW error code otherwise.
 */
uint32_t asw_PUS_8_1_8_1_execute_wp(uint8_t motor_id, uint8_t profile_idx, uint8_t *errors);

/**
 * \brief Function that implements the procedure to search the home sensor.
 * \param[in] motor_id [1 byte]: Motor to be moved. 0 for FWA, 1 for GWA.
 * \param[out] errors Phase of errors used to generate event packet. For this function maybe ENERGIZE_PH or PROFILE_EXEC_PH
 * \return 0 if successfully executed, a DSW error code otherwise.
 */
uint32_t asw_PUS_8_1_8_6_hs_search(uint8_t motor_id, uint8_t *errors);

/**
 * \brief Function that implements the procedure to home sensor check.
 * \param[in] motor_id [1 byte]: Motor to be moved. 0 for FWA, 1 for GWA.
 * \param[out] errors Phase of errors used to generate event packet. For this function maybe ENERGIZE_PH or PROFILE_EXEC_PH
 * \return 0 if successfully executed, a DSW error code otherwise.
 */
uint32_t asw_PUS_8_1_8_8_hs_check(uint8_t motor_id, uint8_t *errors);

/**
 * \brief Function that implements the procedure to check wheel connection.
 * \param[in] motor_id [1 byte]: Motor connection to be checked. 0 for FWA, 1 for GWA.
 * \return 0 if successfully executed, an error code otherwise.
 */
uint32_t asw_PUS_8_1_8_9_conn_check(uint8_t motor_id);

/**
 * \brief Task used to handle wheels profiles execution
 * \return status of task
 */
rtems_task asw_task_wheel_exec(rtems_task_argument argument);

/**
 * \brief Function used to check the end of a profile execution.
 * \param[in] motor_id Motor connection to be checked. 0 for FWA, 1 for GWA.
 * \return 0 if profile is successfully executed, 1 in case of error.
 */
uint32_t asw_check_wheels_end_movement(uint8_t motor_id);

/**
 * \brief Function used to select a wheel profile.
 * \param[in] profile_idx Wheel profile index to be loaded.
 * \return The wheel profile selected.
 */
wheel_profile_t asw_select_profile(uint8_t profile_idx);

/**
 * \brief Function used to do monitoring of FWA and GWA parameters
 * \return none
 */
void asw_monitor_wheels(void);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_1_FWA_GWA_H_ */
