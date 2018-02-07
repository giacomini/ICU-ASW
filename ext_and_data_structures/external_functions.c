/**
 * \file	external_functions.c
 *
 * \brief	This file contains implementation of utilities functions and of all functions
 * required by Drivers
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	March 14, 2016
 *
 */

#include "../rtems_configuration.h"

void icu_disable_irqs(void) {
	rtems_interrupt_level level = 0;

	rtems_interrupt_disable(level);

	return;
}

void icu_enable_irqs(void) {
	rtems_interrupt_level level = 0;

	/* Disable isr to retrieve isr level */
	rtems_interrupt_disable(level);
	rtems_interrupt_enable(level);

	return;
}

void icu_fm_mutex_lock(void) {
	rtems_status_code status;

	status = rtems_semaphore_obtain(fm_sem_id, RTEMS_WAIT, 10);

	return;
}

void icu_fm_mutex_unlock(void) {
	rtems_status_code status;

	status = rtems_semaphore_release(fm_sem_id);

	return;
}

void icu_spi_ctrl_mutex_lock(void) {
	rtems_status_code status;

	status = rtems_semaphore_obtain(spi_sem_id, RTEMS_WAIT, 10);

	return;
}

void icu_spi_ctrl_mutex_unlock(void) {
	rtems_status_code status;

	status = rtems_semaphore_release(spi_sem_id);

	return;
}

void icu_fwa_gwa_drv_wait_usecs(uint32_t microsecs) {
	rtems_status_code status;
	/* Calculate milliseconds starting from microseconds */
	status = rtems_task_wake_after(microsecs / CONFIGURE_MICROSECONDS_PER_TICK);

	return;
}

/*
 * Utilities
 */
void icu_sem_lock(rtems_id sem_id, rtems_interval timeOut) {
	rtems_status_code status;
	status = rtems_semaphore_obtain(sem_id, RTEMS_WAIT, timeOut);

	return;
}

void icu_sem_unlock(rtems_id sem_id) {
	rtems_status_code status;
	status = rtems_semaphore_release(sem_id);

	return;
}
