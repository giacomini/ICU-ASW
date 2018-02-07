/**
 * \file	PUS_8_TM_HK_def.h
 *
 * \brief	Interpreters for the HK tables of DPUs
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 *  Created on: May 24, 2017
 */

#ifndef PUS_8_TM_HK_DEF_H_
#define PUS_8_TM_HK_DEF_H_

#include "../../rtems_configuration.h"
#include "db_type_def.h"

/*
 * on demand
 * DPU_BOOT_REP_Tab / ASW_Death_Rep_Tab / SCE_BOOT_REP_TAB
 *
 * DCU#_HSK_Tab / SCE#_TLM_Tab(1,2,3,4)
 */

/**
 *  \ingroup DPU_HK_MANAGER_FUNCTIONS
 *  \{
 */

/**
 * \brief This function copy the DPU HSK table into local data pool
 * \param[in] stream: a DW array (16 bit) from DPU 1553
 * \param[in] rt_addr (1 B): the rt_address identifying the DPU
 * \return none
 */
void asw_DPU_HSK_table_interpreter(const uint16_t *stream, uint8_t rt_addr);

/**
 * \brief This function copy one DCU HSK table into local data pool
 * \param[in] stream: a DW array (16 bit) from DPU 1553
 * \param[in] rt_addr (1 B): the rt_address identifying the DPU
 * \return none
 */
void asw_DCU_HSK_table_interpreter(const uint16_t *stream, uint8_t rt_addr);

/**
 * \brief This function copy the DPU SCE HSK TAB1 into local data pool
 * \param[in] stream: a DW array (16 bit) from DPU 1553
 * \param[in] rt_address (1 B): the rt_address identifying the DPU
 * \return none
 */
void asw_DPU_SCE_HSK_t1_interpreter(const uint16_t *stream, uint8_t rt_address);

/**
 * \brief This function copy the DPU SCE HSK TAB2 into local data pool
 * \param[in] stream: a DW array (16 bit) from DPU 1553
 * \param[in] rt_address (1 B): the rt_address identifying the DPU
 * \return none
 */
void asw_DPU_SCE_HSK_t2_interpreter(const uint16_t *stream, uint8_t rt_address);

/**
 * \brief This function copy the DPU SCE HSK TAB3 into local data pool
 * \param[in] stream: a DW array (16 bit) from DPU 1553
 * \param[in] rt_address (1 B): the rt_address identifying the DPU
 * \return none
 */
void asw_DPU_SCE_HSK_t3_interpreter(const uint16_t *stream, uint8_t rt_address);

/**
 * \brief This function copy the DPU BSW CPU-DRB table into local data pool
 * \param[in] stream: a DW array (16 bit) from DPU 1553
 * \param[in] rt_address (1 B): the rt_address identifying the DPU
 * \return none
 */
void asw_DPU_BSW_CPU_DRB_interpreter(const uint16_t *stream, uint8_t rt_address);

/**
 * \brief This function copy the DPU BSW DCU table into local data pool
 * \param[in] stream: a DW array (16 bit) from DPU 1553
 * \param[in] rt_address (1 B): the rt_address identifying the DPU
 * \return none
 */
void asw_DPU_BSW_DCU_interpreter(const uint16_t *stream, uint8_t rt_address);

/**
 * \brief Function used to check errors reported by DPUs.
 * \return none
 */
void asw_monitor_DPUs_ASW_error_tab(void);
/**
 * \}
 */

#endif /* PUS_8_TM_HK_DEF_H_ */
