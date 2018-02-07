/**
 *  @file
 *  interr.h
 *
 *  @brief Definitions regarding the User Application possibility to obtain information on internal,
 *         external, fatal and/or non-fatal errors.
 *
 *  Project: RTEMS - Real-Time Executive for Multiprocessor Systems. Partial Modifications by RTEMS Improvement Project (Edisoft S.A.)
 *
 *  COPYRIGHT (c) 1989-2006.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  Version | Date        | Name         | Change history
 *  $Rev: 0000000 $ | $Date: 0000-00-00 00:00:00 +0000 (ABC, 00 Abc 0000) $| $Author: abcdefghijkl $ | original version
 *
 **/

/**
 *  @addtogroup RTEMS_API RTEMS API
 *  @{
 */

#ifndef _RTEMS_RTEMS_ERR_H
#define _RTEMS_RTEMS_ERR_H

/**
 *  @defgroup RTEMS_API_ERROR Error Handler
 *
 *  @brief This handler encapsulates functionality which provides the foundation
 *          for error management accessible from the User Application API.
 *  @{
 */

#include <rtems/score/interr.h>

    /**
     * @brief Macro to get __FILE__ and __LINE__ without need of user input - User Application
     */
    #define rtems_error_report(type, error_code) \
            _Error_Report( \
                          type , \
                          ERROR_DETECTOR_APPLICATION , \
                          (uint8_t *) __FILE__ , \
                          __LINE__ , \
                          error_code \
                          )


    /**
     * @brief Give API access to Internal_errors_Source.
     */
    typedef Internal_errors_Source rtems_internal_error_type;

    /**
     * @brief Give API access to Internal_errors_Core_list.
     */
    typedef Internal_errors_Core_list rtems_internal_error_code;

    /**
     * @brief Give API access to Error_Message_Type.
     */
    typedef Error_Message_Type rtems_error_message_type;

    /**
     * @brief Give API access to Error_Message.
     */
    typedef Error_Message rtems_error_message;


    /**
     * @brief Get a previous non-fatal error message by negative index.
     *
     * @param[in]  offset        The offset of the message to obtain.
     * @param[out] error_message The message structure as returned by the system.
     *
     * @return  returns RTEMS_INVALID_ADDRESS if error_message is not correctly set.
     *          returns RTEMS_SUCCESSFUL if the error is successfully reported to the system.
     *          returns a different error code if a call to _Error_Message_Get_Index results in error (check this function's definition for return values).
     */
    rtems_status_code rtems_error_get_latest_non_fatal_by_offset(
                                                                 int32_t offset ,
                                                                 rtems_error_message *error_message
                                                                 );


    /**
     * @brief Get a previous fatal error message by negative index.
     *
     * @param[in]  offset        The offset of the message to obtain.
     * @param[out] error_message The message structure as returned by the system.
     *
     * @return  returns RTEMS_INVALID_ADDRESS if error_message is not correctly set.
     *          returns RTEMS_SUCCESSFUL if the error is successfully reported to the system.
     *          returns a different error code if a call to _Error_Message_Get_Index results in error (check this function's definition for return values).    *
     */
    rtems_status_code rtems_error_get_latest_fatal_by_offset(
                                                             int32_t offset ,
                                                             rtems_error_message *error_message
                                                             );


#endif
/* end of include file */

/**
 *  @}
 */

/**
 *  @}
 */
