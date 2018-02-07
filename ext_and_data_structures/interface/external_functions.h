/**
 * \file	external_functions.h
 *
 * \brief	This file contains declaration of utilities functions and of all functions
 * required by Drivers
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	March 14, 2016
 *
 */

#ifndef EXTERNAL_FUNCTIONS_H_
#define EXTERNAL_FUNCTIONS_H_

/**
 *  \ingroup EXT_FUNCT_EXT_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to disable interrupts
 * \return none
 */
void icu_disable_irqs(void);

/**
 * \brief Function used to enable interrupts
 * \return none
 */
void icu_enable_irqs(void);

/**
 * \brief Function used to enable semaphore for file management drivers
 * \return none
 */
void icu_fm_mutex_lock(void);

/**
 * \brief Function used to disable semaphore for file management drivers
 * \return none
 */
void icu_fm_mutex_unlock(void);

/**
 * \brief Function used to enable semaphore for spi drivers
 * \return none
 */
void icu_spi_ctrl_mutex_lock(void);

/**
 * \brief Function used to disable semaphore for spi drivers
 * \return none
 */
void icu_spi_ctrl_mutex_unlock(void);

/**
 * \brief Function that uses rtems_task_wake_after() functionalities instead of
 * sleep()
 * \param[in] microsecs microsecs number of microseconds to wait
 */
void icu_fwa_gwa_drv_wait_usecs(uint32_t microsecs);

/**
 * \}
 */

/**
 *  \ingroup EXT_FUNCT_UTILITIES
 *  \{
 */

/**
 * \brief Function used to enable ICU-ASW-defined semaphore.
 * All semaphore have the RTEMS_WAIT option enabled.
 * \param[in] sem_id: the ID of the semaphore (rtems_id)
 * \param[in] timeOut: the timeout (rtems_interval)
 */
void icu_sem_lock(rtems_id sem_id, rtems_interval timeOut);

/**
 * \brief Function used to release a ICU-ASW-defined semaphore.
 * \param[in] sem_id: the ID of the semaphore (rtems_id)
 */
void icu_sem_unlock(rtems_id sem_id);

/**
 * \}
 */

#endif /* EXTERNAL_FUNCTIONS_H_ */
