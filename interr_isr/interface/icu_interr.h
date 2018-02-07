/**
 * \file	icu_interr.h
 *
 * \brief	This file contains declaration all ICU_ASW isr
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	15-03-2017
 *
 */

#ifndef ICU_INTERR_H_
#define ICU_INTERR_H_

#include "../../rtems_configuration.h"

/**
 *  \ingroup ISR_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief 1553 RT Interrupt handler
 * \param[in] vector
 * \return void
 */
rtems_task asw_handler_mil_1553(rtems_vector_number vector);

/**
 * \brief EDAC Double Error handler. It is used for trap 0x20 and 0x09.
 * \param[in] vector
 * \return void
 */
rtems_task asw_handler_edac_double_error(rtems_vector_number vector);

/**
 * \brief ISR used to handle LEON_TRAP_TYPE(10) and to manage TM Acquisition, watchdog and memories access failures.
 * \param[in] vector
 * \return void
 */
rtems_task asw_fpga_irq2_handler_irq(rtems_vector_number vector);

/**
 * \brief ISR used to start the execution of the wheel profile
 * \param[in] id Rtems object ID
 * \param[in] argument none
 * \return none
 */
rtems_timer_service_routine asw_wheels_start_movement(rtems_id id, void *argument);

/**
 * \brief ISR used to handle a timeout expiration during a tc execution
 * \param[in] id Rtems object ID
 * \param[in] argument The telecommand packet
 * \return none
 */
rtems_timer_service_routine asw_tc_timeout_isr(rtems_id id, void *argument);

/**
 * \brief ISR used to handle basic operation in case of watchdog error.
 * \param[in] id Rtems object ID
 * \param[in] argument none
 * \return none
 */
rtems_timer_service_routine asw_no_wd_isr(rtems_id id, void *argument);

/**
 * \brief Function used to monitor and clean masked interrupt (all IRQ7 and some irq of IRQ10)
 * \return none
 */
void asw_monitor_masked_irq(void);

/**
 * \}
 */

#endif /* ICU_INTERR_H_ */
