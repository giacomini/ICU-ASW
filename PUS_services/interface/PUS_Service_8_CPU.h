/**
 * \file	PUS_Service_8_DCU.h
 *
 * \brief	PUS Service 8: commands toward CPUs
 *
 * \author  Donata Bonino, <bonino_at_oato.inaf.it>
 *
 * \internal
 * Created on: May 17, 2017
 */

#ifndef PUS_SERVICE_8_CPU_H_
#define PUS_SERVICE_8_CPU_H_

/**
 *  \ingroup PUS_8_CPU_FUNCTIONS
 *  \{
 */

/**
 * \brief This function implements the CPU_RBT telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 SU-ROM Image (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_1_CPU_RBT(telecommand_t tc);

/**
 * \brief This function implements the CPU_SST telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 ASW mode (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_2_CPU_SST(telecommand_t tc);

/**
 * \brief This function implements the CPU_STM telecommand,
 * always broadcasted to both DPUs.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[master DPU (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_3_CPU_STM(telecommand_t tc);

/**
 * \brief This function implements the CPU_STDM telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 DCU mask (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_4_CPU_STDM(telecommand_t tc);

/**
 * \brief This function implements the CPU_SHSK telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 HSK mode (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_5_DRB_SHSK(telecommand_t tc);

/**
 * \brief This function implements the CPU_NOP telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_11_CPU_NOP(telecommand_t tc);

/**
 * \brief This function implements the CPU_RSTC telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[counter to be cleared (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_12_CPU_RSTC(telecommand_t tc);

/**
 * \brief This function implements the CPU_WDG telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 EOE_WD flag (1 B),
 * 					 EODP_WD flag (1 B),
 * 					 1553_WD flag (1 B),
 *  				 CPU HW RB_WD flag (1 B),
 *  				 CPU HW WD cycle time (2 B), TBC
 *  				 CPU memory scrubbing flag (1 B),
 *  				 CPU TRC flag (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_14_CPU_WDG(telecommand_t tc);

/**
 * \brief This function implements the CPU_MTST telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 device path (1 B),
 * 					 low word w/r pattern (2 B),
 * 					 high word w/r pattern (2 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_15_CPU_MTST(telecommand_t tc);

/**
 * \brief This function implements the CPU_MBE telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_16_CPU_MBE(telecommand_t tc);

/**
 * \brief This function implements the CPU_DABT telecommand.
 * \param[in] tc : the TC array, which has no data - always broadcasted
 * \return status
 */
icu_status_code asw_PUS_8_1_5_18_CPU_DABT(telecommand_t tc);

/**
 * \brief This function implements the DRB_CSPW telecommand.
 * \param[in] tc : the TC array, from which we extract data:
 * 					[DPU number (1 B),
 * 					 SPaceWire to select (1 B)]
 * \return status
 */
icu_status_code asw_PUS_8_1_5_20_DRB_CSPW(telecommand_t tc);

/**
 * \}
 */

/**
 *  \ingroup PUS_8_CPU_INTERNAL_FUNCTIONS
 *  \{
 */

/**
 * \brief This function implements the CPU_GTAB command.
 * \param[in] rt_address:   the DPU address
 * \param[in] sa9_16_table: 1 = SCE_HSK_TAB1, 2 = SCE_HSK_TAB2, 3 = SCE_HSK_TAB3
 * \param[in] sa22_table: 1 = DBB_DRB_STATUS_TAB, 2 = ASW_BOOT_REP_TAB, 3 = ASW_DEATH_REP_TAB
 * \return status
 */
icu_status_code asw_CPU_GTAB(uint16_t rt_address, uint8_t sa9_16_table,
		uint8_t sa22_table);
/**
 * \}
 */

#endif /* PUS_SERVICE_8_CPU_H_ */
