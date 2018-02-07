/**
 * \file	PUS_Service_8_1_FDIR.h
 *
 * \brief	Definition of types and functions used by ICU-ASW to manage FDIR actions.
 *
 * \author  Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	27-06-2017
 */

#ifndef PUS_SERVICE_8_1_FDIR_H_
#define PUS_SERVICE_8_1_FDIR_H_

#include "ICU_config.h"
#include "tc_header.h"

/**
 *  \ingroup PUS_8_1_FDIR_MANAGER_CONSTANTS
 *  \{
 */
#define MAX_NUMBER_OF_FDIR 13U //!< Number of events with a related FDIR action
/**
 * \}
 */
/**
 *  \ingroup PUS_8_1_FDIR_MANAGER_TYPES
 *  \{
 */

/**
 * \struct fdir_t
 * \brief Structure to handle an fdir action. It contains recovery function and a flag enable/disable.
 */

typedef struct {
	void (*ptrf)(void);
	bool_t en_dis;
} fdir_t;

/**
 * \}
 */

/**
 *  \ingroup PUS_8_1_FDIR_MANAGER_VARIABLES
 *  \{
 */
fdir_t fdir_collection[MAX_NUMBER_OF_FDIR];
/**
 * \}
 */

/**
 *  \ingroup PUS_8_1_FDIR_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief Function to initialize FDIR action to default state (all enabled)
 */
void asw_initialize_fdir(void);

/**
 * \brief Function used to enable or disable FDIR operations.
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 2 arguments:
 * 					[FunctionID [4 bytes]: must be 0x20002;
 * 					 enable_disable [1 byte]: 0 = disable, 1 = enable]
 * \return ICU_SUCCESSFUL if execution was successful, an error code otherwise
 */
icu_status_code asw_PUS_8_1_2_2_set_fdir_mode(telecommand_t tc);

/**
 * \brief Function used to enable an FDIR
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 2 arguments:
 * 					[FunctionID [4 bytes]: must be 0x20004;
 * 					 fdirID [1 byte]: FDIR identifiers to be enabled]
 * \return ICU_SUCCESSFUL if execution was successful, an error code otherwise
 */
icu_status_code asw_PUS_8_1_2_4_enable_fdir(telecommand_t tc);

/**
 * \brief Function used to disable an FDIR
 * \param[in] tc: TC sent by the spacecraft. TC Data must contain 2 arguments:
 * 					[FunctionID [4 bytes]: must be 0x20005;
 * 					 fdirID [1 byte]: FDIR identifiers to be disabled]
 * \return ICU_SUCCESSFUL if execution was successful, an error code otherwise
 */
icu_status_code asw_PUS_8_1_2_5_disable_fdir(telecommand_t tc);

/**
 * \}
 */

#endif /* PUS_SERVICE_8_1_FDIR_H_ */
