/**
 * \file	rtems_configuration.h
 *
 * \brief	This file contains declaration of the ICU-ASW running tasks
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	29-01-2016
 *
 */

#ifndef RTEMS_CONFIGURATION_H_
#define RTEMS_CONFIGURATION_H_

#include "ICU_config.h"
#include "PUS_6_DB_def.h"
#include "PUS_8_DB_def.h"
#include "PUS_8_TM_HK_def.h"
#include "PUS_services/interface/PUS_Service_1.h"
#include "PUS_services/interface/PUS_Service_3.h"
#include "PUS_services/interface/PUS_Service_3_diag.h"
#include "PUS_services/interface/PUS_Service_5.h"
#include "PUS_services/interface/PUS_Service_6.h"
#include "PUS_services/interface/PUS_Service_8_1_DPU_BSW.h"
#include "PUS_services/interface/PUS_Service_8_DCU.h"
#include "PUS_services/interface/PUS_Service_8_DPU_config.h"
#include "PUS_services/interface/PUS_Service_8_CPU.h"
#include "PUS_services/interface/PUS_Service_8_SCE.h"
#include "PUS_services/interface/PUS_Service_8_1.h"
#include "PUS_services/interface/PUS_Service_8_1_FDIR.h"
#include "PUS_services/interface/PUS_Service_8_1_fwa_gwa.h"
#include "PUS_services/interface/PUS_Service_8_1_watchdog.h"
#include "PUS_services/interface/PUS_Service_8_1_heaters.h"
#include "PUS_services/interface/PUS_Service_8_1_leds.h"
#include "PUS_services/interface/PUS_Service_8_1_transitions.h"
#include "PUS_services/interface/PUS_Service_9.h"
#include "PUS_services/interface/PUS_Service_17.h"
#include "ext_and_data_structures/interface/external_functions.h"
#include "management_1553/interface/BC_Configuration.h"
#include "management_1553/interface/RT_Configuration.h"
#include "management_1553/interface/milbus.h"
#include "telecommand/interface/tc_header.h"
#include "telemetry/interface/tm.h"
#include "interr_isr/interface/icu_interr.h"
#include <public/asw_runtime_init.h>
#include <public/edac_mng.h>
#include <public/eeprom_mng.h>
#include <public/file_manager.h>
#include <public/fpga_int_manager.h>
#include <public/fpga_timers.h>
#include <public/fwa_gwa_motors.h>
#include <public/heater_drv.h>
#include <public/led_drv.h>
#include <public/mdpa_int_manager.h>
#include <public/mdpa_timers.h>
#include <public/mil_std_1553_bc_drv.h>
#include <public/mil_std_1553_common_mdpa_reg_mem.h>
#include <public/mil_std_1553_rt_drv.h>
#include <public/obt.h>
#include <public/reset.h>
#include <public/spi_drv.h>
#include <public/tm_acq_manager.h>
#include <public/tm_acq_manager_reg_mem.h>
#include <public/watchdog.h>
#include <public/active_wait.h>
#include <rtems/system.h>
#include <rtems/score/interr.h>
#include <rtems/rtems/interr.h>
#include <rtems/fatal.h>
#include <rtems.h>

/* Definition of the doxygen structure */

/**
 *  \defgroup ICU-ASW_TASKS Rtems Configuration
 *  \{
 *  \brief RTEMS tasks, configurations and global variables.
 */
/**  \defgroup ICU-ASW_TASKS_CONSTANTS Constants */
/**  \defgroup ICU-ASW_TASKS_VARIABLES Global variables */
/**  \defgroup ICU-ASW_TASKS_FUNCTIONS functions */
/**  \defgroup ICU-ASW_TASKS_OPTIONS Rtems options */
/**
 * \}
 */
/**
 *  \defgroup TM_HANDLER Telemetry Handler
 *  \{
 *  \brief Data types, structure and functions used to handle TM packets.
 */
/**  \defgroup TM_HANDLER_CONSTANTS Constants */
/**  \defgroup TM_HANDLER_TYPES Types */
/**  \defgroup TM_HANDLER_VARIABLES Variables */
/**  \defgroup TM_HANDLER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup TC_HANDLER Telecommand Handler
 *  \{
 *  \brief Telecommands formatting and execution
 */
/**  \defgroup TC_HANDLER_CONSTANTS Constants */
/**  \defgroup TC_HANDLER_TYPES Types */
/**  \defgroup TC_HANDLER_VARIABLES Variables */
/**  \defgroup TC_HANDLER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup ISR_MANAGER Interrupts Handler
 *  \{
 *  \brief Interrupts functions
 */
/**  \defgroup ISR_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup EXT_FUNCT_MANAGER DSW external functions
 *  \{
 *  \brief External functions and ICU-ASW utilities
 */
/**  \defgroup EXT_FUNCT_EXT_FUNCTIONS External_Functions */
/**  \defgroup EXT_FUNCT_UTILITIES Utilities */
/**
 * \}
 */
/**
 *  \defgroup DB_MANAGER Database Handler
 *  \{
 *  \brief Parameters DB manager
 */
/**  \defgroup DB_MANAGER_TYPES Types */
/**  \defgroup DB_MANAGER_CONSTANTS Constants */
/**  \defgroup DB_MANAGER_VARIABLES Variables */
/**  \defgroup DB_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_LIBRARY PUS Library
 *  \{
 */
/**
 *  \defgroup PUS_1_MANAGER PUS Service 1
 *  \{
 *  \brief PUS Service 1 management
 */
/**  \defgroup PUS_1_CONSTANTS Constants */
/**  \defgroup PUS_1_FUNCTIONS Functions */
/**  \defgroup PUS_1_PUBLIC_FUNCTION Public functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_3_MANAGER PUS Service 3
 *  \{
 *  \brief PUS Service 3 management
 */
/** \defgroup PUS_3_MANAGER_FUNCTIONS Functions */
/** \defgroup PUS_3_DIAG_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_5_MANAGER PUS Service 5
 *  \{
 *  \brief PUS Service 5 management
 */
/** \defgroup PUS_5_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_6_MANAGER PUS Service 6
 *  \{
 *  \brief PUS Service 6 management
 */
/** \defgroup PUS_6_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_SERVICE_8_1_MANAGER PUS Service 8
 *  \{
 *  \brief PUS Service 8,1 manager
 */
/**
 *  \defgroup WATCHDOG_MANAGER PUS Service 8,1 - Watchdog
 *  \{
 *  \brief Function to handle watchdog functionalities
 */
/**  \defgroup WATCHDOG_MANAGER_CONSTANTS Constants */
/**  \defgroup WATCHDOG_MANAGER_VARIABLES Variables */
/**  \defgroup WATCHDOG_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_CPU_MANAGER PUS Service 8,1 - CPU manager
 *  \{
 *  \brief PUS Service 8: commands toward CPUs - both TCs and internal commands
 */
/**  \defgroup PUS_8_CPU_FUNCTIONS Public Functions */
/**  \defgroup PUS_8_CPU_INTERNAL_FUNCTIONS Private Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_DCU_MANAGER PUS Service 8,1 - DCU manager
 *  \{
 *  \brief Definition of telecommands and internal commands toward DCUs.
 */
/**  \defgroup PUS_8_DCU_FUNCTIONS Public functions */
/**  \defgroup PUS_8_DCU_INTERNAL_FUNCTIONS Private Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_DPU_HL_MANAGER PUS Service 8,1 - DPU configuration
 *  \{
 *  \brief PUS Service 8 management: high level commands toward DPUs
 */
/**  \defgroup PUS_8_DPU_HL_FUNCTIONS Public Functions */
/**  \defgroup PUS_8_DPU_HL_INTERNAL_FUNCTIONS Private Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_1_FDIR_MANAGER PUS Service 8,1 - FDIR
 *  \{
 *  \brief Functions to initialize FDIR structures and handle PUS 8,1 telecommands.
 */
/**  \defgroup PUS_8_1_FDIR_MANAGER_CONSTANTS Constants */
/**  \defgroup PUS_8_1_FDIR_MANAGER_TYPES Types */
/**  \defgroup PUS_8_1_FDIR_MANAGER_VARIABLES Variables */
/**  \defgroup PUS_8_1_FDIR_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup HEATER_MANAGER PUS Service 8,1 - Heaters
 *  \{
 *  \brief Function to handle heaters functionalities
 */
/**  \defgroup HEATER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup LEDS_MANAGER PUS Service 8,1 - Led
 *  \{
 *  \brief Function to handle leds functionalities
 */
/**  \defgroup LEDS_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_SCE_MANAGER PUS Service 8,1 - SCE
 *  \{
 *  \brief PUS Service 8 management: commands toward SCE/EEF
 */
/**  \defgroup PUS_8_SCE_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_1_SETTINGS PUS Service 8,1 - Settings command
 *  \{
 *  \brief Functions to handle all PUS 8,1 commands not included in other sections
 */
/**  \defgroup PUS_8_1_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_SERVICE_8_1_TRANSISIONS PUS Service 8,1 - ICU State Transition
 *  \{
 *  \brief Function to handle instrument transition and state
 */
/**  \defgroup PUS_8_1_TRANSITION_VARIABLES Variables */
/**  \defgroup PUS_8_1_TRANSITION_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup FWA_GWA_MANAGER PUS Service 8,1 - FWA and GWA
 *  \{
 *  \brief Function to handle filter wheel and grism wheel functionalities
 */
/**  \defgroup FWA_GWA_CONSTANTS Constants */
/**  \defgroup FWA_GWA_TYPES Types */
/**  \defgroup FWA_GWA_VARIABLES Variables */
/**  \defgroup FWA_GWA_FUNCTIONS Functions */
/**
 * \}
 */
/**
 * \}
 */
/**
 *  \defgroup PUS_9_MANAGER PUS Service 9
 *  \{
 *  \brief PUS Service 9 management
 */
/** \defgroup PUS_9_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_17_MANAGER PUS Service 17
 *  \{
 *  \brief PUS Service 17 management
 */
/** \defgroup PUS_17_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */

/**
 *  \defgroup PUS_LIBRARY_SUPPORT PUS Library support
 *  \{
 *  \brief PUS Services Library support
 */
/**
 *  \defgroup ICU_CONFIG ICU Configuration
 *  \{
 *  \brief ICU-ASW state and error
 */
/**  \defgroup ICU_CONFIG_CONSTANTS Constants */
/**  \defgroup ICU_CONFIG_TYPES Types */
/**  \defgroup ICU_CONFIG_VARIABLES Variables */
/**
 * \}
 */
/**
 *  \defgroup PUS_3_SUPPORT_MANAGER PUS Service 3 support
 *  \{
 *  \brief Definition of PUS Service 3 ancillary variables and functions.
 */
/**  \defgroup PUS_3_SUPPORT_CONSTANTS Constants */
/**  \defgroup PUS_3_SUPPORT_TYPES Types */
/**  \defgroup PUS_3_SUPPORT_VARIABLES Variables */
/**  \defgroup PUS_3_SUPPORT_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_5_SUPPORT_MANAGER PUS Service 5 support
 *  \{
 *  \brief Definition of PUS Service 5 ancillary variables and functions.
 */
/**  \defgroup PUS_5_SUPPORT_CONSTANTS Constants */
/**  \defgroup PUS_5_SUPPORT_TYPES Types */
/**  \defgroup PUS_5_SUPPORT_VARIABLES Variables */
/**  \defgroup PUS_5_SUPPORT_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_6_SUPPORT_MANAGER PUS Service 6 support
 *  \{
 *  \brief Definition of PUS Service 6 ancillary variables and functions.
 */
/**  \defgroup PUS_6_SUPPORT_CONSTANTS Constants */
/**  \defgroup PUS_6_SUPPORT_TYPES Types */
/**  \defgroup PUS_6_SUPPORT_VARIABLES Variables */
/**  \defgroup PUS_6_SUPPORT_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_SUPPORT_MANAGER PUS Service 8 support
 *  \{
 *  \brief Definition of PUS Service 8 ancillary variables and functions.
 */
/**  \defgroup PUS_8_SUPPORT_CONSTANTS Constants */
/**
 *  \defgroup PUS_8_SUPPORT_TYPES Types
 *  \{
 *  \brief ICU-DPU Communication support types
 */
/**  \defgroup PUS_8_SUPPORT_DPU_TO_ICU DPU_to_ICU */
/**  \defgroup PUS_8_SUPPORT_ICU_TO_DPU ICU_to_DPU */
/**
 * \}
 */
/**  \defgroup PUS_8_SUPPORT_VARIABLES Variables */
/**  \defgroup PUS_8_SUPPORT_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup PUS_8_DPU_HK_MANAGER PUS Service 8 DPU_HK support
 *  \{
 *  \brief Interpreters for DPUs HK tables - from 1553 messages to ICU database
 */
/** \defgroup DPU_HK_MANAGER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 * \}
 */
/**
 * \}
 */
/**
 *  \defgroup MILBUS_MANAGER 1553 MILBUS Handler
 *  \{
 */
/** \defgroup MILBUS_MANAGER_CONSTANTS Constants */
/** \defgroup MILBUS_MANAGER_TYPES Types */
/**
 *  \defgroup BC_HANDLER 1553 Bus Controller
 *  \{
 *  \brief Configuration of the Bus Controller Manager
 */
/**  \defgroup BC_HANDLER_CONSTANTS Constants */
/**  \defgroup BC_HANDLER_TYPES Types */
/**  \defgroup BC_HANDLER_VARIABLES Variables */
/**  \defgroup BC_HANDLER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 *  \defgroup RT_HANDLER Remote Terminal
 *  \{
 *  \brief Configuration of the Remote Terminal manager.
 */
/**  \defgroup RT_HANDLER_CONSTANTS Constants */
/**  \defgroup RT_HANDLER_VARIABLES Variables */
/**  \defgroup RT_HANDLER_FUNCTIONS Functions */
/**
 * \}
 */
/**
 * \}
 */

/* End Definition of the doxygen structure */

/**
 *  \ingroup ICU-ASW_TASKS_CONSTANTS
 *  \{
 */

/* Definition of masks for queues_with_data variable */
#define QUEUE_5_4_DATA_IN 0x1000U //!< Mask to set PUS 5,4 bit in the variable queues_with_data variable
#define QUEUE_5_3_DATA_IN 0x800U //!< Mask to set PUS 5,3 bit in the variable queues_with_data variable
#define QUEUE_5_2_DATA_IN 0x400U //!< Mask to set PUS 5,2 bit in the variable queues_with_data variable
#define QUEUE_5_1_DATA_IN 0x200U //!< Mask to set PUS 5,1 bit in the variable queues_with_data variable
#define QUEUE_PUS_1_DATA_IN 0x100U //!< Mask to set PUS Service 1,x bit in the variable queues_with_data variable
#define QUEUE_PUS_3_DATA_IN 0x80U //!< Mask to set PUS Service 3,* (except periodic) bit in the variable queues_with_data variable
#define QUEUE_DIAG_0_DATA_IN 0x40U //!< Mask to set HK DIAG 1 bit in the variable queues_with_data variable
#define QUEUE_DIAG_1_DATA_IN 0x20U //!< Mask to set HK DIAG 2 bit in the variable queues_with_data variable
#define QUEUE_HK_4_DATA_IN 0x10U //!< Mask to set HK 4 bit in the variable queues_with_data variable
#define QUEUE_HK_3_DATA_IN 0x8U //!< Mask to set HK 3 bit in the variable queues_with_data variable
#define QUEUE_HK_2_DATA_IN 0x4U //!< Mask to set HK 2 bit in the variable queues_with_data variable
#define QUEUE_HK_1_DATA_IN 0x2U //!< Mask to set HK 1 bit in the variable queues_with_data variable
#define QUEUE_M_DUMP_DATA_IN 0x1U //!< Mask to set Memory Dump bit in the variable queues_with_data variable
/* ASW version data (8 bit) */
#define ASW_VER 0x0U //!< ASW version (3 bits)
#define ASW_SUBVER 0x1U //!< ASW subversion (5 bits)
#define ASW_VERSION ((ASW_VER << 5U) | ASW_SUBVER) //!< ASW_version HK parameter (8 bits) composed by version and subversion
/* Masks of RTEMS and DSW components initialized by ASW */
#define ASW_TIMER_MASK 0x1FU //!< Mask of RTEMS timers used by ASW. To be compared with timers_init variable during initialization
#define ASW_SEMAPHORE_MASK 0x1FFU //!< Mask of RTEMS semaphores managed by ASW. To be compared with sems_init variable during initialization
#define ASW_TASKS_MASK 0xFFFU //!< Mask of RTEMS tasks created by ASW. To be compared with tasks_init variable during initialization
#define ASW_DSW_COMPONENTS_MASK 0x1FFFU //!< Mask of drivers components used by ASW. To be compared with dsw_comp_init variable during initialization
#define ASW_QUEUES_MASK 0x1FFFU //!< Mask of RTEMS queues handled by ASW. To be compared with queues_init variable during initialization
/* ASW reset causes */
#define ASW_RESET_BY_TC (1U << 5U) //!< Reset generated by TC transition_to_boot
#define ASW_RESET_RAM_EDAC_DOUBLE_ERR (1U << 6U) //!< Reset generated by MDPA EDAC double error
#define ASW_RESET_MDPA_EDAC_DOUBLE_ERR (1U << 7U) //!< Reset generated by MDPA EDAC double error
#define ASW_RESET_RTEMS_FATAL_ERROR (1U << 8U) //!< Reset generated after an RTEMS fatal error
#define ASW_RESET_INIT_FAILURE (1U << 9U) //!< Reset generated if there is a failure during initialization of ASW components

/**
 * \}
 */

/**
 *  \ingroup ICU-ASW_TASKS_VARIABLES
 *  \{
 */
/**
 * \brief RTEMS's tasks Ids
 */
extern rtems_id task_rt_id, task_bc_id, task_tc_id, task_exec_id, task_atr_id,
		task_tm_id, task_idle_id, task_fdir_id, task_tm_acq_id, taskTC_6_5_Id,
		task_wp_exec_id, task_monitor_id;

/**
 * \brief RTEMS's Semaphore Ids used for external functions and global resources
 */
extern rtems_id fm_sem_id, spi_sem_id, queue_sem_id, mem_reg_sem_Id,
		not_mon_db_sem_Id, monit_db_sem_Id, TM_cnt_sem_Id, cmd_ver_tab_sem_Id,
		send_list_sem_id;

/**
 * \brief RTEMS id assigned to the telemetry queues
 */
extern rtems_id ev_5_4_queue_id, ev_5_3_queue_id, ev_5_2_queue_id,
		ev_5_1_queue_id, pus_1_queue_id, hk_4_queue_id, hk_3_queue_id,
		hk_2_queue_id, hk_1_queue_id, diag_0_queue_id, diag_1_queue_id,
		mem_dump_queue_id, pus_3_queue_id;

/**
 *\brief RTEMS IDs assigned to timers
 */
extern rtems_id wheel_timer_id, tc_timer_id, timer_no_wd_id, dpu1_tc_timer_id,
		dpu2_tc_timer_id;

/**
 *  \brief Variable used to manage telemetry queues
 */
extern uint16_t queues_with_data;

/**
 * \brief Manage RT parameters
 */
extern Handler_1553_t handler_1553;

/**
 * \brief CPU load counter
 */
extern uint32_t count_idle;

/**
 *\brief Variable used to manage enable or disable OBT synch
 */
extern bool_t obt_synch_enable;

/**
 * \brief Variable used to store task's TCB pointers
 */
extern rtems_tcb* thread_ctrl[12];

/**
 * \brief Starting address of the death report.
 */
extern uint32_t dr_addr;

/**
 * \brief Starting address of the health report. The first element is the reset source (4 Byte).
 */
extern uint32_t health_addr;
/**
 * \}
 */

/* functions */

/**
 *  \ingroup ICU-ASW_TASKS_FUNCTIONS Functions
 *  \{
 */

/**
 * \brief Entry Point of the ICU ASW. Configure and check the instruments status
 * and prepare all tasks used in the ASW.
 * \return void
 */
rtems_task Init(rtems_task_argument arg);

/**
 * \brief Task to handle RT 1553 Process
 * \return void
 */
rtems_task asw_task_rt(rtems_task_argument arg);

/**
 * \brief Task to handle BC 1553 Process
 * \return void
 */
rtems_task asw_task_bc(rtems_task_argument arg);

/**
 * \brief Task to manage TC
 * \return void
 */
rtems_task asw_task_tc_manager(rtems_task_argument arg);

/**
 * \brief Task to execute TC
 * \return void
 */
rtems_task asw_task_tc_exec(rtems_task_argument arg);

/**
 * \brief Task used to create next telemetry packet and next ATR to be
 * transmitted
 * \return void
 */
rtems_task asw_task_atr(rtems_task_argument arg);

/**
 * \brief Task that manage the telemetry acquisition
 * \return void
 */
rtems_task asw_task_tm(rtems_task_argument arg);

/**
 * \brief Task that substitute the RTEMS Idle task. Memory scrubbing and CPU
 * load estimation are done here.
 * \return void
 */
rtems_task asw_task_idle(rtems_task_argument arg);

/**
 * \brief Task that will perform recovery actions in case of errors
 * \return void
 */
rtems_task asw_task_fdir(rtems_task_argument arg);

/**
 * \brief Task that will perform telemetry acquisition of ICU and DAS parameters
 */
rtems_task asw_task_tm_acq(rtems_task_argument arg);

/**
 * \brief Task that will perform system monitoring
 */
rtems_task asw_task_monitor(rtems_task_argument arg);

/**
 * \brief Function called by RTEMS when a new task is created. It is used to save the TCB pointer of every task.
 * \param[out] current_task Pointer to the TCB of the current task
 * \param[out] new_task Pointer to the TCB of the new task
 * \return TRUE if successfully executed, FALSE otherwise
 */

boolean asw_myCreate(rtems_tcb *current_task, rtems_tcb *new_task);

/**
 * \brief Function called by RTEMS during a task control switch. It is used to estimate the CPU load by calculation of IDLE Task running time.
 * \param current_task Pointer to the TCB of the task that is swapped out
 * \param heir_task Pointer to the TCB of the task that is swapped in
 * \return None
 */
rtems_extension asw_mySwitch(rtems_tcb *current_task, rtems_tcb *heir_task);

/**
 * \brief Function used to create the health report at the end of ICU ASW startup procedure.
 * \param[in] tasks_init Mask of tasks created and started. Bits set to 1 means that task is successfully initialized.
 * bit[0..11] asw_task_rt, asw_task_bc, asw_task_tc, asw_task_atr, asw_task_tm, asw_task_idle, asw_task_exec, asw_task_fdir, asw_task_tm_acq, asw_taskTC_6_5, asw_task_wp_exec, task_monitor.
 * bit[12..15] Not used
 * \param[in] sems_init Mask of semaphores initialized by ICU_ASW. Bits set to 1 means that semaphore is successfully initialized.
 * bit[0..8] fm_sem, spi_sem, queue_sem, memreg, not_mon_db, mon_db_sem, TM_cnt_sem, cmd_ver_tab_sem, send_list_sem.
 * bit[9..15] Not used
 * \param[in] queues_init Mask of queues initialized by ICU_ASW. Bits set to 1 means that queue is successfully initialized.
 * bit[0..12] ev_5_4, ev_5_3, ev_5_2, ev_5_1, pus_1, pus_3, hk_4, hk_3, hk_2, hk_1, diag_1, diag_0, mem_dump.
 * bit[13..15] Not used
 * \param[in] timers_init Mask of timers initialized by ICU_ASW. Bits set to 1 means that timer is successfully initialized.
 * bit[0..4] wheel_timer, tc_timer, timer_no_wd, dpu1_tc_timer_id, dpu2_tc_timer_id.
 * bit[5..7] Not used
 * \param[in] dsw_comp_init Mask of driver components initialized by ICU_ASW. Bit set to 1 means that is successfully initialized (OBT is not included).
 * bit[0..12]	rt, bc, eeprom, MDPA_timer, FPGA_timer, watchdog, intmngr_fpga, intmngr_mdpa, spi, leds, heaters, tm_acq_mng and fwa_gwa.
 * bit[13..15] Not used
 * \param[in] rt_sub_tx_init 1553 RT Transmit subaddress correctly initialized (characterize, set buffer and get pointers operations successfully executed).
 * \param[in] rt_sub_rx_init 1553 RT Receive subaddress correctly initialized (characterize, set buffer and get pointers operations successfully executed).
 * \param[in] rt_mc_init 1553 RT Mode codes successfully initialized.
 * \return none
 */
void asw_create_health_report(uint16_t tasks_init, uint16_t sems_init,
		uint16_t queues_init, uint8_t timers_init, uint16_t dsw_comp_init,
		uint32_t rt_sub_tx_init, uint32_t rt_sub_rx_init, uint32_t rt_mc_init);

/**
 * \brief Function used to create the death report before force the reset.
 * \return none
 */
void asw_create_death_report(void);
/**
 * \}
 */

/**
 *  \ingroup ICU-ASW_TASKS_OPTIONS Rtems options
 *  \{
 */
#define CONFIGURE_MAXIMUM_TIMERS 5U //!< Maximum number of timers that could be used by the ICU ASW
#define CONFIGURE_MAXIMUM_TASKS 13U //!< Maximum number of tasks that could be used by the ICU ASW
#define CONFIGURE_MAXIMUM_MESSAGE_QUEUES 13U //!< Maximum number of queues that could be used by the ICU ASW
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_HAS_OWN_APP_SAFE_STATE_HANDLER //!< ICU_ASW overwrites the RTEMS default app_safe_state_handler() function
#define CONFIGURE_EXTRA_TASK_STACKS (48U * RTEMS_MINIMUM_STACK_SIZE) //!< Extra stack allocated for tasks
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_MAXIMUM_SEMAPHORES 9U //!< Maximum number of semaphores that could be used by the ICU ASW
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_MICROSECONDS_PER_TICK 1000U //!< Ticks time. It is sized as 1 millisecond
#define CONFIGURE_TICKS_PER_TIMESLICE 10U //!< Tasks timeslice assigned to the task. It is sized as 10 milliseconds
#define CONFIGURE_MAXIMUM_USER_EXTENSIONS 1U //!< Maximum number of extensions that could be used by the ICU ASW

/**
 * \}
 */

#endif /*RTEMS_CONFIGURATION_H_*/
