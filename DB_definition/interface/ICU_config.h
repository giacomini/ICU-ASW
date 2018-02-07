/**
 * \file	ICU_config.h
 *
 * \brief	Configuration of ICU ASW
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: Oct 21, 2015
 */

#ifndef ICU_CONFIG_H_
#define ICU_CONFIG_H_

#include <public/basic_types.h>

/**
 *  \ingroup ICU_CONFIG_CONSTANTS
 *  \{
 */

/**
 * \enum icu_status_code
 */

typedef enum {
	ICU_SUCCESSFUL = 0U, //!< Operation successfully executed
	ICU_ILLEGAL_STATE, //!< Operation is forbidden in present ICU status
	ICU_WRONG_PARAMETER, //!< Parameter out of bounds/illegal number of parameters
	ICU_UNSATISFIED, //!< Missing parameters or ICU state not expected
	ICU_RTEMS_QUEUE_ERROR, //!> Error from RTEMS queue functions
	ICU_DPU_ILLEGAL_STATE, //!< Operation is forbidden in present DPU status
	ICU_DPU_TAB_NOT_UPDATED, //!< Asynchronous table not updated
	ICU_BC_1553_ERROR, //!< Error during creation of 1553 message or sendlist
	ICU_OBT_ERROR, //!< Error of the OBT values
	/* errors for TM(1,8) */
} icu_status_code;

/**
 * \enum state_machine_t ICU_state
 * \brief NISP state definition
 */
/* ASW states machine */
typedef enum {
	ICU_OFF = 0U, //!< ICU is off
	ICU_BOOT = 1U << 0U, //!< ICU Boot software is running
	ICU_STARTUP = 1U << 1U, //!< ICU Application software is running and it is in Startup mode
	ICU_STANDBY = 1U << 2U, //!< ICU Application software is running and it is in Standby mode
	ICU_IDLE = 1U << 3U, //!< ICU Application software is running and it is in Idle mode
	ICU_OBSERVATION = 1U << 4U, //!< ICU Application software is running and it is in Science mode
	ICU_MANUAL = 1U << 5U, //!< ICU Application software is running and it is in Manual mode
	ICU_SAFE = 1U << 6U, //!< ICU Application software is running and it is in Safe mode
} state_machine_t;

/**
 * \}
 */

/**
 * \ingroup ICU_CONFIG_TYPES Types
 * \{
 */

/**
 * \struct mem_dump_flag_t
 * \brief Format defined for memory dump operation vs DPUs.
 */
typedef struct {
		uint8_t dpu1_asw;
		uint8_t dpu1_bsw;
		uint8_t dpu2_asw;
		uint8_t dpu2_bsw;
} mem_dump_flag_t;

/**
 * \struct	ICU_settings_t
 *
 * \brief	Definition of thresholds and default values to be used during ICU-ASW initialization.
 * These values are stored and could be modified in EEPROM. ICU-ASW can apply new values only after a reboot.
 */
typedef struct {
	uint16_t leds_off;
	uint16_t leds_on;
	uint16_t leds_offset;
	uint16_t fwa_off;
	uint16_t fwa_on;
	uint16_t fwa_offset;
	uint16_t gwa_off;
	uint16_t gwa_on;
	uint16_t gwa_offset;
	uint16_t leds_max_duty_cycle;
	uint16_t leds_max_delay;
	uint16_t acq_st0;
	uint16_t acq_st1;
	uint16_t acq_st2;
	uint16_t acq_st3;
	uint16_t acq_sync_timeout;
	uint16_t cdpu_temp_t1;
	uint16_t cdpu_temp_t2;
	uint16_t lvps_temp_t1;
	uint16_t lvps_temp_t2;
	uint16_t das_alarm_reg;
	uint16_t wd_t1;
	uint16_t wd_t2;
} ICU_settings_t;

/**
 * \}
 */

/**
 *  \ingroup ICU_CONFIG_VARIABLES
 *  \{
 */

/**
 * \brief Variable use to store last evaluation of ICU CPU_load
 */
extern uint16_t cpu_load;

/**
 * \brief number of SA (from 1 to 16) to read after a MDUMP
 */
/*
 extern uint16_t MDUMP_data_length;
 */
/**
 * \brief memory dump flags
 */
extern mem_dump_flag_t mem_dump_active;

/**
 * \brief ICU default settings
 */
extern ICU_settings_t icu_settings;

/**
 * \}
 */

#endif /* ICU_CONFIG_H_ */
