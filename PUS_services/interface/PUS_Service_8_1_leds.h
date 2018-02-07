/**
 * \file	PUS_Service_8_1_leds.h
 *
 * \brief	This file contains declaration of the SW components usied to manage the calibration unit
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	27-04-2017
 *
 */

#ifndef PUS_SERVICE_8_1_LEDS_H_
#define PUS_SERVICE_8_1_LEDS_H_

/**
 *  \ingroup LEDS_FUNCTIONS
 *  \{
 */

/**
 * \brief Function used to power on/off a led.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 4 arguments:
 * 					[FunctionID [4 bytes]: must be 0x90001;
 * 					 led id [1 byte]: must be a value between zero( LED 1) and 4 (LED 5).
 * 					 power level [1 byte]: must be a value between zero and 255. Zero means that selected led must be powered off.
 * 					 duty cycle [2 bytes]: must be value between 1 and 1023. It is ignored when power level value is zero]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_9_1_leds_on_off(telecommand_t tc);

/**
 * \brief Function used to reset leds functionalities.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain an argument:
 * 					[FunctionID [4 bytes]: must be 0x90002]
 * \return ICU_SUCCESSFUL if is successfully executed, an error code otherwise.
 */
icu_status_code asw_PUS_8_1_9_2_leds_reset(telecommand_t tc);

/**
 * \brief Function used to monitor leds voltage when CU is on
 * \return none
 */
void asw_monitor_leds(void);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_1_LEDS_H_ */
