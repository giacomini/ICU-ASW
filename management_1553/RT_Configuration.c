/**
 * \file	RT_Configuration.c
 *
 * \brief	This file manages the configuration of the RT manager
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 *  Created on: 29/gen/2016
 */

#include "../rtems_configuration.h"
#include <public/hw_map.h>

uint32_t *ptr_tm_reset = NULL;
uint32_t *ptr_tc_rx = NULL;
uint32_t *ptr_dtd = NULL;
uint32_t *ptr_atc = NULL;
uint32_t *ptr_time_rx = NULL;
uint32_t *ptr_wrap_around_rx = NULL;
uint32_t *ptr_pus_3 = NULL;
uint32_t *ptr_evt_pus_1 = NULL;
uint32_t *ptr_hk_4 = NULL;
uint32_t *ptr_hk_3 = NULL;
uint32_t *ptr_hk_2 = NULL;
uint32_t *ptr_hk_1 = NULL;
uint32_t *ptr_dump = NULL;
uint32_t *ptr_diag_1 = NULL;
uint32_t *ptr_diag_0 = NULL;
uint32_t *ptr_dtc = NULL;
uint32_t *ptr_atr = NULL;
uint32_t *ptr_time_tx = NULL;
uint32_t *ptr_wrap_around_tx = NULL;

uint16_t msg_index = 0;
uint16_t number_of_msg = 0;

bool_t tm_reset_active = TRUE;

RTCmdWordTableEntry_t msg_buffer[RT_MESSAGES_TO_BE_STORED] = { { .cmd_word =
		{ 0 }, .ctrl_word = { 0 }, .info_word = { 0 } } };

uint8_t asw_configure_rt(uint32_t *sa_rx_en, uint32_t *sa_tx_en,
		uint32_t *mc_en) {
	uint8_t errors = 0;
	uint32_t ret = 0;
	volatile uint32_t *p_bypass = (uint32_t *) 0x80000150;
	volatile uint32_t *p_pending_mask = (uint32_t *) 0x80000100;

	// Init
	(*p_bypass) = 0x0;
	(*p_pending_mask) = 0x1C0;

	icu_mil_std_1553_rt_drv_reset();

	ret = icu_mil_std_1553_rt_drv_init(RAM_BASE_ADDRESS + 0x00500000);
	if (ret == 0) {
		ret = icu_mil_std_1553_rt_drv_configure(TRUE, FALSE, FALSE, FALSE);
		if (ret == 0) {
			/* Configure Subaddress */
			/* RX Start TM reset */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_TM_RESET,
					0, TRUE, FALSE);
			if (ret == 0) {
				*sa_rx_en = *sa_rx_en | ((uint32_t) 1U << SA_TM_RESET);
			} else {
				errors++;
			}

			/* RX TC */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_TC, 0,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_rx_en = *sa_rx_en | ((uint32_t) 1U << SA_TC);
			} else {
				errors++;
			}

			/* PUS 3 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_TM_PUS_3,
					1, TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_TM_PUS_3);
			} else {
				errors++;
			}

			/* Event an PUS 1 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(
					SA_EV_AND_PUS_1, 1, TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_EV_AND_PUS_1);
			} else {
				errors++;
			}

			/* HK 4 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_HK_4, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_HK_4);
			} else {
				errors++;
			}

			/* HK 3 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_HK_3, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_HK_3);
			} else {
				errors++;
			}

			/* HK 2 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_HK_2, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_HK_2);
			} else {
				errors++;
			}

			/* HK 1 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_HK_1, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_HK_1);
			} else {
				errors++;
			}

			/* MEM DUMP */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_DUMP, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_DUMP);
			} else {
				errors++;
			}

			/* DIAG 1 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_DIAG_1, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_DIAG_1);
			} else {
				errors++;
			}

			/* DIAG 0 */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_DIAG_0, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_DIAG_0);
			} else {
				errors++;
			}

			/* DTD and DTC */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_DTD_DTC, 0,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_rx_en = *sa_rx_en | ((uint32_t) 1U << SA_DTD_DTC);
			} else {
				errors++;
			}
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_DTD_DTC, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_DTD_DTC);
			} else {
				errors++;
			}

			/* ATR and ATC */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_ATR_ATC, 0,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_rx_en = *sa_rx_en | ((uint32_t) 1U << SA_ATR_ATC);
			} else {
				errors++;
			}
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_ATR_ATC, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_ATR_ATC);
			} else {
				errors++;
			}

			/* Time */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_TIME, 0,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_rx_en = *sa_rx_en | ((uint32_t) 1U << SA_TIME);
			} else {
				errors++;
			}
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(SA_TIME, 1,
					TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_TIME);
			} else {
				errors++;
			}

			/* Wrap around */
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(
					SA_WRAP_AROUND, 0, TRUE, FALSE);
			if (ret == 0) {
				*sa_rx_en = *sa_rx_en | ((uint32_t) 1U << SA_WRAP_AROUND);
			} else {
				errors++;
			}
			ret = icu_mil_std_1553_rt_drv_subaddress_characterize(
					SA_WRAP_AROUND, 1, TRUE, FALSE);
			if (ret == 0) {
				*sa_tx_en = *sa_tx_en | ((uint32_t) 1U << SA_WRAP_AROUND);
			} else {
				errors++;
			}

			if (errors == 0) {
				/* SAs 10R and from 11Tx to 19Tx use deep-subaddressing */
				/* TC */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_TC, 0, 0, 16);
				if (ret != 0) {
					errors++;
					*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_TC);
				}

				/* PUS 3 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_TM_PUS_3, 1, 0, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_TM_PUS_3);
				}

				/* Event and PUS 1 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_EV_AND_PUS_1, 1, 16, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_EV_AND_PUS_1);
				}

				/* HK 4 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_HK_4, 1, 32, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_4);
				}

				/* HK 3 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_HK_3, 1, 48, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_3);
				}

				/* HK 2 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_HK_2, 1, 64, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_2);
				}

				/* HK 1 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_HK_1, 1, 80, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_1);
				}

				/* Memory Dump */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_DUMP, 1, 96, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DUMP);
				}

				/* DIAG 1 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_DIAG_1, 1, 112, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DIAG_1);
				}

				/* Diag 0 */
				ret = icu_mil_std_1553_rt_drv_set_linear_buffer_subaddress(
						SA_DIAG_0, 1, 128, 16);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DIAG_0);
				}

				/* All other SAs use standard 1553 buffer size */
				/* TM reset */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_TM_RESET, 0, 16);
				if (ret != 0) {
					errors++;
					*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_TM_RESET);
				}

				/* DTD */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_DTD_DTC, 0, 17);
				if (ret != 0) {
					errors++;
					*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_DTD_DTC);
				}

				/* ATR */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_ATR_ATC, 0, 18);
				if (ret != 0) {
					errors++;
					*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_ATR_ATC);
				}

				/* Time */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_TIME, 0, 19);
				if (ret != 0) {
					errors++;
					*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_TIME);
				}

				/* Wrap-around */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_WRAP_AROUND, 0, 20);
				if (ret != 0) {
					errors++;
					*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_WRAP_AROUND);
				}

				/* DTC */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_DTD_DTC, 1, 145);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DTD_DTC);
				}

				/* ATC */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_ATR_ATC, 1, 146);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_ATR_ATC);
				}

				/* Time */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_TIME, 1, 147);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_TIME);
				}

				/* Wrap around */
				ret = icu_mil_std_1553_rt_drv_set_single_buffer_subaddress(
						SA_WRAP_AROUND, 1, 148);
				if (ret != 0) {
					errors++;
					*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_WRAP_AROUND);
				}

				if (errors == 0) {
					/* Get pointers of subaddress that use deep-subaddressing (10Rx and 11Tx-to-19Tx*/

					/* TC */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(SA_TC,
							0, 0, (volatile uint32_t **) &ptr_tc_rx);
					if (ret != 0) {
						errors++;
						*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_TC);
					}

					/* PUS 3 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_TM_PUS_3, 1, 0,
							(volatile uint32_t **) &ptr_pus_3);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_TM_PUS_3);
					}

					/* Event and PUS 1 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_EV_AND_PUS_1, 1, 0,
							(volatile uint32_t **) &ptr_evt_pus_1);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en
								& ~((uint32_t) 1U << SA_EV_AND_PUS_1);
					}

					/* HK 4 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_HK_4, 1, 0, (volatile uint32_t **) &ptr_hk_4);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_4);
					}

					/* HK 3 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_HK_3, 1, 0, (volatile uint32_t **) &ptr_hk_3);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_3);
					}

					/* HK 2 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_HK_2, 1, 0, (volatile uint32_t **) &ptr_hk_2);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_2);
					}

					/* HK 1 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_HK_1, 1, 0, (volatile uint32_t **) &ptr_hk_1);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_HK_1);
					}

					/* Memory Dump */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_DUMP, 1, 0, (volatile uint32_t **) &ptr_dump);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DUMP);
					}

					/* Diag 1 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_DIAG_1, 1, 0,
							(volatile uint32_t **) &ptr_diag_1);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DIAG_1);
					}

					/* Diag 2 */
					ret = icu_mil_std_1553_rt_drv_get_linear_buffer_addr(
							SA_DIAG_0, 1, 0,
							(volatile uint32_t **) &ptr_diag_0);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DIAG_0);
					}

					/* Get pointers of other configured SAs */

					/* TM protocol reset */
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_TM_RESET, 0,
							(volatile uint32_t **) &ptr_tm_reset);
					if (ret != 0) {
						errors++;
						*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_TM_RESET);
					}

					/* DTD and DTC */
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_DTD_DTC, 0, (volatile uint32_t **) &ptr_dtd);
					if (ret != 0) {
						errors++;
						*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_DTD_DTC);
					}
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_DTD_DTC, 1, (volatile uint32_t **) &ptr_dtc);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_DTD_DTC);
					}

					/* ATR and ATC */
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_ATR_ATC, 0, (volatile uint32_t **) &ptr_atc);
					if (ret != 0) {
						errors++;
						*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_ATR_ATC);
					}
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_ATR_ATC, 1, (volatile uint32_t **) &ptr_atr);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_ATR_ATC);
					}

					/* Time */
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_TIME, 0, (volatile uint32_t **) &ptr_time_rx);
					if (ret != 0) {
						errors++;
						*sa_rx_en = *sa_rx_en & ~((uint32_t) 1U << SA_TIME);
					}
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_TIME, 1, (volatile uint32_t **) &ptr_time_tx);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en & ~((uint32_t) 1U << SA_TIME);
					}

					/* Wrap around */
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_WRAP_AROUND, 0,
							(volatile uint32_t **) &ptr_wrap_around_rx);
					if (ret != 0) {
						errors++;
						*sa_rx_en = *sa_rx_en
								& ~((uint32_t) 1U << SA_WRAP_AROUND);
					}
					ret = icu_mil_std_1553_rt_drv_get_single_buffer_addr(
							SA_WRAP_AROUND, 1,
							(volatile uint32_t **) &ptr_wrap_around_tx);
					if (ret != 0) {
						errors++;
						*sa_tx_en = *sa_tx_en
								& ~((uint32_t) 1U << SA_WRAP_AROUND);
					}

					if (errors == 0) {
						/* Mode code supported by S/C */
						/* MC_SYNCHRONIZE_WITHOUT_DATA */
						ret = icu_mil_std_1553_rt_drv_mode_code_characterize(
								MC_SYNCHRONIZE_WITHOUT_DATA, TRUE, TRUE);
						if (ret == 0) {
							*mc_en = *mc_en
									| ((uint32_t) 1U
											<< MC_SYNCHRONIZE_WITHOUT_DATA);
						} else {
							errors++;
						}

						/* MC_TRANSMIT_STATUS_WORD */
						ret = icu_mil_std_1553_rt_drv_mode_code_characterize(
								MC_TRANSMIT_STATUS_WORD, TRUE, FALSE);
						if (ret == 0) {
							*mc_en =
									*mc_en
											| ((uint32_t) 1U
													<< MC_TRANSMIT_STATUS_WORD);
						} else {
							errors++;
						}

						/* MC_TRANSMITTER_SHUTDOWN */
						ret = icu_mil_std_1553_rt_drv_mode_code_characterize(
								MC_TRANSMITTER_SHUTDOWN, TRUE, FALSE);
						if (ret == 0) {
							*mc_en =
									*mc_en
											| ((uint32_t) 1U
													<< MC_TRANSMITTER_SHUTDOWN);
						} else {
							errors++;
						}

						/* MC_OVERRIDE_TRANSMITTER */
						ret = icu_mil_std_1553_rt_drv_mode_code_characterize(
								MC_OVERRIDE_TRANSMITTER, TRUE, FALSE);
						if (ret == 0) {
							*mc_en =
									*mc_en
											| ((uint32_t) 1U
													<< MC_OVERRIDE_TRANSMITTER);
						} else {
							errors++;
						}

						/* MC_RESET_RT */
						ret = icu_mil_std_1553_rt_drv_mode_code_characterize(
								MC_RESET_RT, TRUE, TRUE);
						if (ret == 0) {
							*mc_en = *mc_en | ((uint32_t) 1U << MC_RESET_RT);
						} else {
							errors++;
						}

						ret = icu_mil_std_1553_rt_drv_mode_code_characterize(
								MC_SYNCHRONIZE_WITH_DATA, TRUE, TRUE);
						if (ret == 0) {
							*mc_en =
									*mc_en
											| ((uint32_t) 1U
													<< MC_SYNCHRONIZE_WITH_DATA);
						} else {
							errors++;
						}

						ret = icu_mil_std_1553_rt_drv_mode_code_characterize(
								MC_TRANSMIT_LAST_COMMAND, TRUE, FALSE);
						if (ret == 0) {
							*mc_en =
									*mc_en
											| ((uint32_t) 1U
													<< MC_TRANSMIT_LAST_COMMAND);
						} else {
							errors++;
						}

						if (errors != 0) {
							errors = 1U << 5; // MODE CODES NOT INITIALIZED
						}
					} else {
						errors = 1U << 4; // NO POINTER TO SUBADDRESS
					}

				} else {
					errors = 1U << 3; // SA NOT BUFFERED
				}
			} else {
				errors = 1U << 2; // SA NOT CHARACTERIZED
			}
		} else {
			errors = 1U << 1; //NOT CONFIGURED
		}

	} else {
		errors = 1; //NOT INIT
	}

	asw_reset_rt();

	ret = icu_mil_std_1553_rt_drv_start();

	return (errors);
}

void asw_reset_rt(void) {
	uint32_t ret = 0;

	/*Set the initial values for the ATR*/
	handler_1553.ATR.all_bits = 0;

	/*Set the initial values for the DTD*/
	handler_1553.DTD.all_bits = 0;

	/*Set the initial values for the ATC*/
	handler_1553.ATC.all_bits = 0;

	/*Set the initial values for the DTC */
	handler_1553.DTC.all_bits = 0;

	handler_1553.current_comm_frame = 0;

	/*Set initial time*/
	handler_1553.time_message[0] = 0x2f;
	handler_1553.time_message[1] = 0;
	handler_1553.time_message[2] = 0;
	handler_1553.time_message[3] = 0;
	handler_1553.time_message[4] = 0;

	/*Set initial sendlist parameters*/
	handler_1553.last_sendlist_executed = 0;
	handler_1553.last_sendlist_processed = 0;
	handler_1553.last_sendlist_read = 0;
	handler_1553.last_msg_index_used = 0;

	/*Clear SAs 10R and 13T (due to deep-subaddressing)*/
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_TC, 0);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_TM_PUS_3, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_EV_AND_PUS_1, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_HK_4, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_HK_3, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_HK_2, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_HK_1, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_DUMP, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_DIAG_1, 1);
	ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_DIAG_0, 1);

	return;
}

void asw_telemetry_reset(void) {
	TransferDescriptorDataWord_t atr;

	tm_reset_active = TRUE;
	atr = asw_set_transfer_descriptor(0, 0, 1, 1, 0, 0);
	memcpy(ptr_atr, &atr, sizeof(uint32_t));

	return;
}

uint32_t asw_check_new_DTD(uint8_t *data) {
	uint32_t ret = 1;
	TransferDescriptorDataWord_t dtd = { 0 };

	/*read DTD from SA 27R*/
	memcpy(&dtd.all_bits, ptr_dtd, sizeof(uint32_t));

	if (dtd.all_bits != handler_1553.DTD.all_bits) {
		/*Check if there is a new TC*/
		if (((dtd.reset == 0) && (dtd.blockCount != handler_1553.DTD.blockCount))
				== TRUE) {

			/*Copy DTD in the SA of the DTC and uodate DTC counter*/
			memcpy(ptr_dtc, &dtd.all_bits, sizeof(uint32_t));
			handler_1553.DTC.all_bits = dtd.all_bits;

			/*Update last received DTD*/
			handler_1553.DTD.all_bits = dtd.all_bits;

			/*Copy TC from SA 10R only if size is OK*/
			if (((dtd.size >= MIL_1553_MIN_TC_PKT_LENGTH) && (dtd.size <= MIL_1553_MAX_TC_PKT_LENGTH)) == TRUE) {
				memcpy(data, ptr_tc_rx, dtd.size);

				/*Clear SA 10Rx*/
				ret = icu_mil_std_1553_rt_drv_clear_linear_buffer(SA_TC, 0);

				ret = 0;
			}
		} else {
			/* Always copy the new DTD in DTC*/
			memcpy(ptr_dtc, &dtd.all_bits, sizeof(uint32_t));
			handler_1553.DTD.all_bits = dtd.all_bits;
			handler_1553.DTC.all_bits = dtd.all_bits;
		}
	}

	return (ret);
}

CommandWord_t asw_set_cmd_word(uint16_t RT_addr, uint16_t tr_bit,
		uint16_t sub_addr, uint16_t word_count) {

	CommandWord_t cmd_word;

	cmd_word.rt_address = RT_addr;
	cmd_word.tx_rx = tr_bit;
	cmd_word.subaddress = sub_addr;
	cmd_word.data_word_count = word_count;

	return (cmd_word);
}

TransferDescriptorDataWord_t asw_set_transfer_descriptor(uint32_t size,
		uint32_t qos_err, uint32_t reset, uint32_t mode, uint32_t sa,
		uint32_t counter) {
	TransferDescriptorDataWord_t descriptor;

	descriptor.reserved = 0;
	descriptor.size = size;
	descriptor.QoS = qos_err;
	descriptor.reset = reset;
	descriptor.mode = mode;
	descriptor.SA = sa;
	descriptor.blockCount = counter;

	return (descriptor);
}

uint8_t asw_create_ATR_counter(void) {
	uint8_t atr = 0;

	atr = (handler_1553.ATR.blockCount + 1) % 256;
	if (atr == 0) {
		atr = 1;
	}
	return (atr);
}

uint32_t asw_read_ATC(void) {
	uint32_t ret = 0;
	TransferDescriptorDataWord_t atc = { 0 };
	TransferDescriptorDataWord_t atr = { 0 };

	/*read ATC from SA 28R*/
	memcpy(&atc.all_bits, ptr_atc, sizeof(uint32_t));

	/*Check if the ATC received was the answer to a reset procedure*/
	if (((atc.reset == 1) && (atc.blockCount == 0)) == TRUE) {
		handler_1553.ATR.all_bits = 0U | 0x2000U; /* Set Mode bit to 1 */
		handler_1553.ATC.all_bits = 0U | 0x2000U;
		memcpy(ptr_atr, &atr.all_bits, sizeof(uint32_t));
		tm_reset_active = FALSE;
	} else if (atc.all_bits == handler_1553.ATR.all_bits) {
		handler_1553.ATC.all_bits = atc.all_bits;
	} else {
		ret = 1;
	}

	return (ret);
}

void asw_read_last_OBT_received(void) {
	icu_status_code status = ICU_SUCCESSFUL;
	uint32_t coarse = 0, fine = 0, ret = 0;
	OBT_TimeData_t time_message = { 0 };
	uint8_t errors[EVENT_MAX_PARAMETER_LENGTH];
	CommandWord_t cmd_word = { 0 };

	memcpy(time_message, ptr_time_rx, 5 * sizeof(uint16_t));

	if ((handler_1553.time_message[1] == time_message[1])
			&& (handler_1553.time_message[2] == time_message[2])
			&& (handler_1553.time_message[3] == time_message[3])) {
		memcpy(&errors[0], ptr_time_rx, 5 * sizeof(uint16_t));
		status = asw_PUS_5_TM_report(EVENT_OBT_NOT_UPDATED, 10, errors);
	}

	coarse = (uint32_t) (((uint32_t) time_message[1] << 16U)
			| (uint32_t) time_message[2]);
	fine = (uint32_t) ((((uint32_t) time_message[3] << 16U)
			| (uint32_t) time_message[4]) & 0xFFFFFF00U);

	ret = icu_obt_preload_obt(coarse, fine);
	if (ret != 0) {
		errors[0] = (uint8_t) 1U << 2U;
		status = asw_PUS_5_TM_report(EVENT_OBT_ERROR, 1, errors);
	}

	/*Create 1553 message*/
	/* Create command word */
	cmd_word = asw_set_cmd_word(RT_BROADCAST_1553, RECEIVE_BIT_1553, TIME_MSG_SA, 5);

	/*Create message*/
	ret = icu_mil_std_1553_bc_drv_create_msg(flag_bc_msgs, MSG_INTERGAP_DELAY, cmd_word,
			time_message, &dpu_1553_msg[TIME_RCV_MSG_INDEX]);

	if (ret != 0) {
		status = asw_PUS_5_TM_report(EVENT_BC_1553_ERROR, 0, NULL );
	}

	memcpy(ptr_time_tx, ptr_time_rx, 5 * sizeof(uint16_t));

	return;
}

uint32_t asw_check_last_rcv_cmd_word(const RTCmdWordTableEntry_t *last_rcv_msgs,
		uint16_t number_of_msgs) {
	uint32_t ret = 0;
	uint32_t word_reset = 0;
	int32_t i = 0;

	/*i starts from 1 because index 0 is the synch message received*/
	for (i = 0; i < (int32_t) (number_of_msgs - 1); i++) {

		/*Check if BC request a reset of the telemetry protocol*/
		if ((last_rcv_msgs[i].cmd_word.subaddress == SA_TM_RESET)
				&& (last_rcv_msgs[i].cmd_word.tx_rx == 0)) {
			memcpy(&word_reset, ptr_tm_reset, sizeof(uint32_t));

			/*Check if bit 15 of the second Data Word is equal to 1*/
			if ((word_reset & 0x0001U) > 0) {
				asw_telemetry_reset();
			}
		}

		/*Check for ATC*/
		else if ((last_rcv_msgs[i].cmd_word.subaddress == SA_ATR_ATC)
				&& (last_rcv_msgs[i].cmd_word.tx_rx == 0)) {
			ret = asw_read_ATC();
		}

		/* Check data wrap-around */
		else {
			if (last_rcv_msgs[i].cmd_word.subaddress == SA_WRAP_AROUND) {
				ret = asw_rt_wrap_around();
			}
		}
	}

	return (ret);
}

uint32_t asw_rt_wrap_around(void) {
	uint32_t ret = 0;

	memcpy(ptr_wrap_around_tx, ptr_wrap_around_rx, 32 * sizeof(uint16_t));

	return (ret);
}
