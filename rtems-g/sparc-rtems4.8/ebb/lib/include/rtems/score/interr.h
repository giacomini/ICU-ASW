/**
 *  @file
 *  interr.h
 *
 *  @brief contains constants and prototypes related
 *  to the Internal Error Handler.
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
 *  179     | 17/09/2008  | hsilva       | original version
 *  602     | 17/11/2008  | mcoutinho    | IPR 65
 *  3509    | 19/06/2009  | mcoutinho    | IPR 528
 *  3706    | 09/07/2009  | jsousa       | IPR 153
 *  4406    | 21/09/2009  | mcoutinho    | IPR 605
 *  4443    | 21/09/2009  | mcoutinho    | IPR 768
 *  4944    | 23/10/2009  | mcoutinho    | IPR 819
 *  5099    | 28/10/2009  | mcoutinho    | IPR 553
 *  5273    | 01/11/2009  | mcoutinho    | IPR 843
 *  5317    | 02/11/2009  | mcoutinho    | IPR 831
 *  6356    | 02/03/2010  | mcoutinho    | IPR 1935
 *  8184    | 15/06/2010  | mcoutinho    | IPR 451
 *  9872    | 18/03/2011  | aconstantino | SPR 2819
 *  10173   | 18/05/2011  | hsilva       | SPR 2967 - Questionable no return attribute for rtems_shutdown_executive()
 *  $Rev: 12218 $ | $Date: 2013-11-19 16:05:49 +0000 (Tue, 19 Nov 2013) $| $Author: hsilva $ | SW-FMECA_5_6_7, 2_3_18 and 17
 *
 **/

/**
 *  @addtogroup SUPER_CORE Super Core
 *  @{
 */

#ifndef _RTEMS_SCORE_INTERR_H
#define _RTEMS_SCORE_INTERR_H

/**
 *  @defgroup ScoreIntErr Internal Error Handler
 *
 *   @brief This handler encapsulates functionality which provides the foundation
 *  Semaphore services used in all of the APIs supported by RTEMS.
 */
/**@{*/

#include <rtems/score/types.h>
#include <rtems/system.h>
#include <rtems/rtems/status.h>

#ifdef __cplusplus
extern "C"
{
#endif

   /**
    *  @brief Macro to get __FILE__ and __LINE__ without need of user input - System
    */
   #define _Error_Report_Internal(type, error_code) \
           _Error_Report( \
                         type , \
                         ERROR_DETECTOR_SYSTEM , \
                         (uint8_t *) RTEMSFILENAME , \
                         __LINE__ , \
                         error_code \
                         )

   /**
    *  @brief lists the possible sources from which an error
    *  can be reported.
    */
   typedef enum
   {
      INTERNAL_ERROR_CORE ,
      INTERNAL_ERROR_RTEMS_API ,
      INTERNAL_ERROR_INTERRUPTION_NOT_HANDLED
   } Internal_errors_Source;

   /**
    *  @brief list of errors which are generated internally by the executive core.
    */
   typedef enum
   {
      INTERNAL_ERROR_NO_CONFIGURATION_TABLE = 0 ,
      INTERNAL_ERROR_NO_CPU_TABLE = 1 ,
      INTERNAL_ERROR_INVALID_WORKSPACE_ADDRESS = 2 ,
      INTERNAL_ERROR_TOO_LITTLE_WORKSPACE = 3 ,
      INTERNAL_ERROR_WORKSPACE_ALLOCATION = 4 ,
      INTERNAL_ERROR_INTERRUPT_STACK_TOO_SMALL = 5 ,
      INTERNAL_ERROR_THREAD_EXITTED = 6 ,
      INTERNAL_ERROR_INCONSISTENT_MP_INFORMATION = 7 ,
      INTERNAL_ERROR_INVALID_NODE = 8 ,
      INTERNAL_ERROR_NO_MPCI = 9 ,
      INTERNAL_ERROR_BAD_PACKET = 10 ,
      INTERNAL_ERROR_OUT_OF_PACKETS = 11 ,
      INTERNAL_ERROR_OUT_OF_GLOBAL_OBJECTS = 12 ,
      INTERNAL_ERROR_OUT_OF_PROXIES = 13 ,
      INTERNAL_ERROR_INVALID_GLOBAL_ID = 14 ,
      INTERNAL_ERROR_BAD_STACK_HOOK = 15 ,
      INTERNAL_ERROR_BAD_ATTRIBUTES = 16 ,
      INTERNAL_ERROR_INVALID_MESSAGE_SUBMIT_TYPE = 17 ,
      INTERNAL_ERROR_TOO_MANY_INITIAL_EXTENSIONS = 18 ,
      INTERNAL_ERROR_INVALID_MICROSECONDS_PER_TICK = 19 ,
      INTERNAL_ERROR_INVALID_TICKS_PER_TIMESLICE = 20 ,
      INTERNAL_ERROR_TOO_MANY_DEVICE_DRIVERS = 21 ,
      INTERNAL_ERROR_NO_DEVICE_DRIVER_TABLE = 22 ,
      INTERNAL_ERROR_NO_USER_EXTENSION_TABLE = 23 ,
      INTERNAL_ERROR_NO_API_CONFIGURATION_TABLE = 24 ,
      INTERNAL_ERROR_INVALID_NUMBER_TASKS = 25 ,
      INTERNAL_ERROR_INVALID_NUMBER_INITIALIZATION_TASKS = 26 ,
      INTERNAL_ERROR_NO_USER_INITIALIZATION_TASKS_TABLE = 27 ,
      INTERNAL_ERROR_NOT_ENOUGH_MEMORY = 28 ,
      INTERNAL_ERROR_WRONG_ENVIRONMENT = 29 ,
      INTERNAL_ERROR_INVALID_EVENT_SYNC_STATE = 30 ,
      INTERNAL_ERROR_INVALID_PENDING_MESSAGE = 31 ,
      INTERNAL_ERROR_INVALID_OBJECT_INDEX = 32 ,
      INTERNAL_ERROR_INVALID_OBJECT_LOCATION = 33 ,
      INTERNAL_ERROR_INVALID_LOCK_NESTING_BEHAVIOR = 34 ,
      INTERNAL_ERROR_INVALID_CORE_MUTEX_ATTRIBUTE = 35 ,
      INTERNAL_ERROR_INVALID_THREAD_QUEUE_STATE = 36 ,
      INTERNAL_ERROR_INVALID_THREAD_QUEUE_DISCIPLINE = 37 ,
      INTERNAL_ERROR_INVALID_THREAD_BUDGET_ALGORITHM = 38 ,
      INTERNAL_ERROR_INVALID_WATCHDOG_DIRECTION = 39 ,
      INTERNAL_ERROR_INVALID_WATCHDOG_STATE = 40 ,
      INTERNAL_ERROR_INVALID_MUTEX_NEST_COUNT = 41 ,
      INTERNAL_ERROR_INVALID_HEAP_ALIGNMENT = 42 ,
      INTERNAL_ERROR_SHUTDOWN_WHEN_NOT_UP_OR_NOT_APP_SAFE = 43 ,
      INTERNAL_ERROR_INVALID_DEADLINE_STATE = 44 ,
      INTERNAL_ERROR_TASK_STACK_UNDERFLOW = 45 ,
      INTERNAL_ERROR_TASK_STACK_OVERFLOW = 46
   } Internal_errors_Core_list;

   /**
    *  @brief holds the fatal error information.
    */
   typedef struct
   {
      /**
       * @brief This is the source of the error.
       */
      Internal_errors_Source the_source;
      /**
       * @brief This indicates if the error is internal of external.
       */
      boolean is_internal;
      /**
       * @brief This is the error code.
       */
      uint32_t the_error;
   } Internal_errors_Information;

   /**
    * @brief Error message structure.
    *
    * TODO: reuse the structure from rtems/rtems/types.h, something which is currently not possible
    *       due to circular dependencies.
    */
   typedef struct
      {
         /*
          * @brief This field is the year, A.D.
          */
         uint32_t year;
         /*
          * @brief This field is the month, 1 -> 12
          */
         uint32_t month;
         /*
          * @brief This field is the day, 1 -> 31
          */
         uint32_t day;
         /*
          * @brief This field is the hour, 0 -> 23
          */
         uint32_t hour;
         /*
          * @brief This field is the minute, 0 -> 59
          */
         uint32_t minute;
         /*
          * @brief This field is the second, 0 -> 59
          */
         uint32_t second;
         /*
          * @brief This field is the elapsed ticks between secs
          */
         uint32_t ticks;
      } _rtems_time_of_day;

   /**
    * @brief list of error message types generated by the System or the User Application
    */
   typedef enum
   {
      /**
       * @brief API error not returned to the User Application but logged internally
       */
      FATAL_ERROR_API_INTERNAL = 0 ,
      /**
       * @brief Super API error not returned to the User Application but logged internally
       */
      FATAL_ERROR_SUPER_API_INTERNAL = 1 ,
      /**
        * @brief Super Core error not returned to the User Application but logged internally
        */
      FATAL_ERROR_SUPER_CORE_INTERNAL = 2 ,
      /**
       * @brief Hardware error returned to the User Application
       */
      FATAL_ERROR_HARDWARE = 3 ,
      /**
       * @brief Device driver error returned to the User Application
       */
      FATAL_ERROR_DEVICE_DRIVER = 4 ,
      /**
       * @brief Non-fatal error - unspecified - returned to the User Application
       */
      NON_FATAL_ERROR = 5
   } Error_Message_Type;

   /**
    * @brief Define who detected the error.
    */
   typedef enum
   {
      /**
       * @brief Identify the error source as the User Application.
       */
      ERROR_DETECTOR_APPLICATION = 0 ,
      /**
       * @brief Identify the error source as the System.
       */
      ERROR_DETECTOR_SYSTEM = 1
   } Error_Message_Detector;

   /**
    * @brief Keep track of where the error occurred: file and line.
    */
   typedef struct
   {
      /**
       * @brief Identifies the line where the error occurred.
       */
      uint16_t line;
      /**
       * @brief Pointer to the start of the source filename.
       */
      uint8_t *filename;
   } Error_Message_Location;

   typedef struct
   {
      /**
       * @brief Error message type: either obtained from Error_Message_Type or from Internal_errors_Source, as explained below
       */
      Error_Message_Type type;
      /**
       * @brief Define who detected the error.
       */
      Error_Message_Detector detector;
      /**
       * @brief Keep track of where the error occurred: file and line.
       */
      Error_Message_Location Location;
      /**
       * @brief Specify the time of occurrence of the error.
       */
      _rtems_time_of_day TOD;
      /**
       * @brief 1 means the clock might was set previous to reporting an error.
       *        0 means otherwise.
       */
      uint8_t clock_is_up;
      /**
       * @brief The effective message code: recommended: 0-127 used by System; 128-255 used by User Application.
       */
      uint8_t error_code;
      /**
       * @brief 1 means the translation table for error_code is rtems_internal_error_code.
       *        0 means the translation table for error_code is rtems_status_code,
       *        as defined in cpukit/rtems/include/rtems/rtems/status.h.
       *        Also 1 means the translation table for type is rtems_internal_error_type.
       *        0 means the translation table for type is rtems_error_message_type.
       */
      uint8_t is_internal;
   } Error_Message;

   /**
    * @brief The error message control structure - base for fatal and non-fatal errors.
    */
   typedef struct
   {
      /**
       * @brief Where to save the next message at; where to start when looking for previous messages.
       */
      uint8_t index;
      /**
       * @brief Indicates ring buffer for message reporting as full: used for index calculation, as the buffer is circular.
       */
      uint8_t is_full;
   } Error_Message_Control;

   /**
    * @brief Error message control structure - array for fatal errors.
    */
   typedef struct
   {
      /**
       * @brief Check the structure definition.
       */
      Error_Message_Control Control;
      /**
       * @brief Effectively holds all the messages.
       */
      Error_Message Message[100];
   } Error_Fatal_Message_Control;

   /**
    * @brief Global error message control structure declaration - fatal error.
    */
   extern Error_Fatal_Message_Control _Error_Fatal_Message;

   /**
    * @brief Error message control structure - array for non-fatal errors.
    */
   typedef struct
   {
      /**
       * @brief Check the structure definition.
       */
      Error_Message_Control Control;
      /**
       * @brief Effectively holds all the messages.
       */
      Error_Message Message[200];
   } Error_Non_Fatal_Message_Control;

   /**
    * @brief Global error message control structure declaration - non-fatal error.
    */
   extern Error_Non_Fatal_Message_Control _Error_Non_Fatal_Message;

   /**
    *  @brief store the error information when a fatal error occurs
    */
   extern Internal_errors_Information Internal_errors_What_happened;

   /**
    *  @brief announce that an internal error occurred
    *
    *  This routine will invoke the fatal error handler supplied by the user,
    *  followed by the the default one provided by the executive.  The default
    *  error handler assumes no hardware is present to help inform the user
    *  of the problem.  Halt stores the error code in a known register,
    *  disables interrupts, and halts the CPU.  If the CPU does not have a
    *  halt instruction, it will loop to itself.
    *
    *  @param[in] the_source what subsystem the error originated in
    *  @param[in] is_internal if the error was internally generated
    *  @param[in] the_error fatal error status code
    *
    *  @note The the_error is not necessarily a directive status code.
    */
   void _Internal_error_Occurred(
                                 Internal_errors_Source the_source ,
                                 boolean is_internal ,
                                 uint32_t the_error
                                 );

   /**
    *  @brief handles the application safe state
    *
    *  This routine is invoked when a error occurs.
    *  Care must be taken when invoking RTEMS directives inside this function.
    *  note that an RTEMS error could have occurred and RTEMS might be corrupted.
    *  For example, the system could not have been properly initalized because not
    *  enough memory was allocated.
    *
    *  @param[in] the_source what subsystem the error originated in
    *  @param[in] is_internal if the error was internally generated
    *  @param[in] the_error fatal error status code
    *
    *  @return  returns TRUE if the error was corrected and FALSE otherwise
    *
    *  @note The the_error is not necessarily a directive status code.
    *  @note This function does not necessarily have to return. It can restart
    *    the system, delete/restart the calling thread, enter a polling mode, etc
    */
   boolean app_safe_state_handler(
                                  Internal_errors_Source the_source ,
                                  boolean is_internal ,
                                  uint32_t the_error
                                  );

   /**
    * @brief Initialize the control structures for fatal and non-fatal error reporting.
    *
    * @return  returns RTEMS_SUCCESSFUL always.
    */
   rtems_status_code _Error_Control_Initialize(void);

   /**
    * @brief Report a System or User Application error and keep it in a circular error ring for further access.
    *
    * @param[in] type            The type of error to report.
    * @param[in] detector        The detector of the error (usually automatically calculated by macro).
    * @param[in] filename        The filename where the error was detected (usually automatically calculated by macro).
    * @param[in] filename_length The length of @a filename (usually automatically calculated by macro).
    * @param[in] line            The line where the error was detected (usually automatically calculated by macro).
    * @param[in] error_code      The error code to keep in the error message structure.
    *
    * @return  returns RTEMS_NOT_DEFINED is the system is not configured to report fatal errors and an attempt in that sense is made.
    *          returns RTEMS_NOT_DEFINED is the system is not configured to report non-fatal errors and an attempt in that sense is made.
    *          returns RTEMS_INTERNAL_ERROR if the type of error that is reported is not know to the system.
    *          returns RTEMS_SUCCESSFUL if the error is successfully reported to the system.
    */
   rtems_status_code _Error_Report(
                                   Error_Message_Type type ,
                                   Error_Message_Detector detector ,
                                   uint8_t *filename ,
                                   uint16_t line ,
                                   uint32_t error_code
                                   );

   /**
    * @brief Calculate a message index based on the ring structure, its size and the user requested offset.
    *
    * @param[in]  control
    * @param[in]  control_size
    * @param[in]  requested_offset
    * @param[out] message_index
    *
    * @return  returns RTEMS_INVALID_ADDRESS if @a control is not correctly set.
    *          returns RTEMS_NOT_DEFINED if the message ring is empty.
    *          returns RTEMS_NOT_DEFINED if trying to check if the message ring is empty results in error.
    *          returns RTEMS_INVALID_NUMBER if @requested_offset is not negative.
    *          returns RTEMS_NOT_DEFINED is the message expected to calculate the index for is not yet defined.
    *          returns RTEMS_SUCCESSFUL if the error is successfully reported to the system.
    */
   rtems_status_code _Error_Message_Get_Index(
                                              Error_Message_Control *control ,
                                              uint32_t control_size ,
                                              int32_t requested_offset ,
                                              int32_t *message_index
                                              );

   /**
    *  @brief initialize the Error Handler
    *
    *  This routine initializes the error handler. The error message circular
    *  rings are initialized (index set to 0 and indication of empty set to true).
    */
   void _Error_Handler_initialization(void);


#ifdef __cplusplus
}
#endif

/**@}*/

#endif
/* end of include file */

/**  
 *  @}
 */
