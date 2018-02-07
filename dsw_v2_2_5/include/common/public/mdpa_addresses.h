/**
 * \file	mdpa_addresses.h
 *
 * \brief	This file defines structures with the standard registers for the
 * 			MDPA with LEON2 processor and macros to access to the content of the
 * 			registers of the different MDPA modules handled by the DSW
 *
 * \author	Santiago Carmona Meco, <scarmona@srg.aut.uah.es>
 *
 * \internal
 * Created: 	14-January-2016
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2016, Santiago Carmona Meco
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 */
#ifndef ICU_MDPA_ADDRESSES_H
#define ICU_MDPA_ADDRESSES_H

/**
 *  @addtogroup DSW_COMMON Common
 *  @{
 */
/**
 *  @defgroup DSW_MDPA_ADDRESSES MDPA Addresses
 *  @{
 */

/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>
#include <public/hw_map.h>




/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/

/* APB_BUS1 */

#define MEM_CONFIG_REG_1				(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS ))) // + 0x0000
#define MEM_CONFIG_REG_2				(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0004U)))
#define MEM_CONFIG_REG_3				(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0008U)))
#define AHB_FAIL_ADDR_REG				(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x000CU)))
#define AHB_STATUS_REG					(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0010U)))
#define CACHE_CONTROL_REG				(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0014U)))

#define SRAM_READ_WAITSTATE_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0030U)))
#define SRAM_WRITE_WAITSTATE_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0034U)))

#define MDPA_TIMER_1_COUNTER_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0040U)))
#define MDPA_TIMER_1_RELOAD_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0044U)))
#define MDPA_TIMER_1_CONTROL_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0048U)))

#define MDPA_TIMER_2_COUNTER_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0050U)))
#define MDPA_TIMER_2_RELOAD_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0054U)))
#define MDPA_TIMER_2_CONTROL_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0058U)))

#define MDPA_PRESCALER_COUNTER_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0060U)))
#define MDPA_PRESCALER_RELOAD_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0064U)))

#define MDPA_INTERRUPT_MASK_PRIO_REG	(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0090U)))
#define MDPA_INTERRUPT_PENDING_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0094U)))
#define MDPA_INTERRUPT_FORCE_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0098U)))
#define MDPA_INTERRUPT_CLEAR_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x009CU)))

#define MDPA_IOPORT_INPUT_OUTPUT_REG	(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x00A0U)))
#define MDPA_IOPORT_DIRECTION_REG		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x00A4U)))
#define MDPA_IOPORT_INTERRUPT_CONF_REG	(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x00A8U)))

#define MDPA_HP_CIC_MASK_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0100U)))
#define MDPA_HP_CIC_PENDING_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0104U)))
#define MDPA_HP_CIC_CLEAR_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0108U)))
#define MDPA_LP_CIC_MASK_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x010CU)))
#define MDPA_LP_CIC_PENDING_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0110U)))
#define MDPA_LP_CIC_CLEAR_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0114U)))


#define REG_5MHZ_CLK_CTRL_REG			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0140U)))
#define GP_CLK_CTRL_REG					(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0144U)))
#define SPW_CLK_CTRL_REG				(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0148U)))
#define PLL_STATUS_REG					(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x014CU)))
#define BYPASS_CTRL_REG					(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0150U)))


/* SPW REGISTERS Module 1 Link 1 */
#define SPW_M1L1_STATUS_CONTROL_REG 	(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0240U)))

#define SPW_M1L1_TX_START_ADDRESS_REG 	(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0248U)))
#define SPW_M1L1_TX_END_ADDRESS_REG 	(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x024CU)))
#define SPW_M1L1_TX_CURRENT_ADDRESS_REG (*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0250U)))
#define SPW_M1L1_TX_RX_CONFIG_REG 		(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0254U)))

#define SPW_M1_CONF_REG 				(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0300U)))
#define SPW_M1_TIME_CTRL_REG 			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0304U)))
#define SPW_M1_TIME_CODE_REG 			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0308U)))
#define SPW_M1_IRQ_MASK_REG 			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x030CU)))
#define SPW_M1_IRQ_STATUS_REG 			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0310U)))
#define SPW_M1_IRQ_CLEAR_REG 			(*((volatile uint32_t *)(APB_BUS1_BASE_ADDRESS + 0x0314U)))


/* APB_BUS2 */

#define GPIO2_OUTPUT_REG				(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x0104U)))
#define GPIO2_DIRECTION_REG				(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x0108U)))
#define MDPA_VERSIONING_REG				(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x010CU)))

/* SPW REGISTERS Module 2 Link 3 */
#define SPW_M2L3_STATUS_CONTROL_REG 	(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x02C0U)))

#define SPW_M2L3_TX_START_ADDRESS_REG 	(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x02C8U)))
#define SPW_M2L3_TX_END_ADDRESS_REG 	(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x02CCU)))
#define SPW_M2L3_TX_CURRENT_ADDRESS_REG (*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x02D0U)))
#define SPW_M2L3_TX_RX_CONFIG_REG 		(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x02D4U)))

#define SPW_M2_CONF_REG 				(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x0300U)))
#define SPW_M2_TIME_CTRL_REG 			(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x0304U)))
#define SPW_M2_TIME_CODE_REG 			(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x0308U)))
#define SPW_M2_IRQ_MASK_REG 			(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x030CU)))
#define SPW_M2_IRQ_STATUS_REG 			(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x0310U)))
#define SPW_M2_IRQ_CLEAR_REG 			(*((volatile uint32_t *)(APB_BUS2_BASE_ADDRESS + 0x0314U)))


/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 * \struct regmap_apb_bus_1
 *
 * \brief Structure used to manage the registers of the APB Bus 1
 * 			0x80000000
 */
struct regmap_apb_bus_1
{
	/** \brief Memory Configuration Register 1 */
	uint32_t memory_configuration_1;            /* 00 */
	/** \brief Memory Configuration Register 2 */
	uint32_t memory_configuration_2;            /* 04 */
	/** \brief Memory Configuration Register 3 */
	uint32_t memory_configuration_3;            /* 08 */
	/** \brief AHB Failing Address Register */
	uint32_t ahb_failing_address;               /* 0C */
	/** \brief AHB Status Register */
	uint32_t ahb_status;                        /* 10 */
	/** \brief Cache Control Register */
	uint32_t cache_control;                     /* 14 */
	/** \brief Power Down Register */
	uint32_t power_down;                        /* 18 */
	/** \brief Write Protection Register 1 */
	uint32_t write_protection_1;                /* 1C */
	/** \brief Write Protection Register 2 */
	uint32_t write_protection_2;                /* 20 */
	/** \brief LEON Configuration Register */
	uint32_t leon_configuration;                /* 24 */
	/** \brief Padding */
	uint32_t foo1[2];                           /* 28 */
	/**	\brief SRAM read wait state Register*/
	uint32_t ram_ws_rd;							/* 30 */
	/**	\brief SRAM write wait state Register*/
	uint32_t ram_ws_wr;							/* 34 */
	/** \brief Padding */
	uint32_t foo2[2];							/* 38 */
	/** \brief Timer 1 Counter Register */
	uint32_t timer_1_counter;                   /* 40 */
	/** \brief Timer 1 Reload Register */
	uint32_t timer_1_reload;                    /* 44 */
	/** \brief Timer 1 Control Register */
	uint32_t timer_1_control;                   /* 48 */
	/** \brief Padding */
	uint32_t foo3;				                /* 4C */
	/** \brief Timer 2 Counter Register */
	uint32_t timer_2_counter;                   /* 50 */
	/** \brief Timer 2 Reload Register */
	uint32_t timer_2_reload;                    /* 54 */
	/** \brief Timer 2 Control Register */
	uint32_t timer_2_control;                   /* 58 */
	/** \brief Padding */
	uint32_t foo4;                              /* 5C */
	/** \brief Scaler Counter Register */
	uint32_t prescaler_counter;	                /* 60 */
	/** \brief Scaler Reload Register */
	uint32_t prescaler_reload;                	/* 64 */
	/** \brief Padding */
	uint32_t foo5[2];                           /* 68 */
	/** \brief UART 1 Data Register */
	uint32_t uart_1_data;                       /* 70 */
	/** \brief UART 1 Status Register */
	uint32_t uart_1_status;                     /* 74 */
	/** \brief UART 1 Control Register */
	uint32_t uart_1_control;                    /* 78 */
	/** \brief UART 1 Scaler Register */
	uint32_t uart_1_scaler;                     /* 7C */
	/** \brief UART 2 Data Register */
	uint32_t uart_2_data;                       /* 80 */
	/** \brief UART 2 Status Register */
	uint32_t uart_2_status;                     /* 84 */
	/** \brief UART 2 Control Register */
	uint32_t uart_2_control;                    /* 88 */
	/** \brief UART 2 Scaler Register */
	uint32_t uart_2_scaler;                     /* 8C */
	/** \brief Interrupt Mask and Priority Register */
	uint32_t irq_mask_and_priority;             /* 90 */
	/** \brief Interrupt Pending Register */
	uint32_t irq_pending;                       /* 94 */
	/** \brief Interrupt Force Register */
	uint32_t irq_force;                         /* 98 */
	/** \brief Interrupt Clear Register */
	uint32_t irq_clear;                         /* 9C */
	/** \brief I/O Port input/output register */
	uint32_t ioport_io;                         /* A0 */
	/** \brief I/O Port Direction Register */
	uint32_t ioport_direction;                  /* A4 */
	/** \brief I/O Port Interrupt Config Register */
	uint32_t ioport_irq_config;                 /* A8 */
	/** \brief Padding */
	uint32_t foo6[6];                           /* AC */
	/** \brief DSU UART Status Register */
	uint32_t dsu_uart_status;                   /* C4 */
	/** \brief DSU UART Control Register */
	uint32_t dsu_uart_control;                  /* C8 */
	/** \brief DSU UART Scaler Register */
	uint32_t dsu_uart_scaler;                   /* CC */
	/**	\brief Start 1 address write protection*/
	uint32_t wr_prot_sta1;						/* D0 */
	/**	\brief End 1 address write protection*/
	uint32_t wr_prot_end1;						/* D4 */
	/**	\brief Start 2 address write protection*/
	uint32_t wr_prot_sta2;						/* D8 */
	/**	\brief End 2 address write protection*/
	uint32_t wr_prot_end2;						/* DC */
	/**	\brief Padding*/
	uint32_t foo7[8];							/* E0 */
	/**	\brief HP CIC mask Register*/
	uint32_t hp_cic_mask;						/* 100 */
	/**	\brief HP CIC pending Register*/
	uint32_t hp_cic_pending;					/* 104 */
	/**	\brief HP CIC clear Register*/
	uint32_t hp_cic_clear;						/* 108 */
	/**	\brief LP CIC mask Register*/
	uint32_t lp_cic_mask;						/* 10C */
	/**	\brief LP CIC pending Register*/
	uint32_t lp_cic_pending;					/* 110 */
	/**	\brief LP CIC clear Register*/
	uint32_t lp_cic_clear;						/* 114 */
	/**	\brief Padding*/
	uint32_t foo8[10];							/* 118 */
	/**	\brief 5M CLK Control Register*/
	uint32_t _5m_clk_control;					/* 140 */
	/**	\brief MIL CLK Control Register*/
	uint32_t mil_clk_control;					/* 144 */
	/**	\brief SPW CLK Control Register*/
	uint32_t spw_clk_control;					/* 148 */
	/**	\brief PLL Status Register*/
	uint32_t pll_status;						/* 14C */
	/**	\brief BYPASS Control Register*/
	uint32_t bypass_control;					/* 150 */
};


/**
 * \struct	regmap_apb_bus_2
 *
 * \brief Structure used to manage the registers of the APB Bus 2
 */
struct regmap_apb_bus_2
{
	/** \brief MILBUS 1 Configuration Register */
	uint32_t milbus_1_conf;            			/* 00 */
	/** \brief MILBUS 1 Extended Memory BAR*/
	uint32_t milbus_1_extended_memory_bar;      /* 04 */
	/** \brief MILBUS 1 Command Status Register */
	uint32_t milbus_1_command_status;            /* 08 */
	/** \brief MILBUS 1 Nominal Interrupt Register */
	uint32_t milbus_1_nominal_irq;               /* 0C */
	/** \brief MILBUS 1 Error Interrupt Register */
	uint32_t milbus_1_error_irq;                 /* 10 */
	/** \brief MILBUS 1 Reset Interrupt Register */
	uint32_t milbus_1_reset_irq;                 /* 14 */
	/** \brief MILBUS 1 Program Counter Register */
	uint32_t milbus_1_program_counter_regiser;   /* 18 */
	/** \brief MILBUS 1 Reception Table Index Register */
	uint32_t milbus_1_rx_table_index;            /* 1C */
	/** \brief MILBUS 1 Transmission Table Index Register */
	uint32_t milbus_1_tx_table_index;            /* 20 */
	/** \brief Padding */
	uint32_t foo1[7];  				              /* 24 */
	/**	\brief CAN Setup*/
	uint32_t can_setup;							/* 40 */
	/**	\brief CAN Status*/
	uint32_t can_status;						/* 44 */
	/** \brief CAN Filter */
	uint32_t can_filter;	                   /* 48 */
	/** \brief CAN TX Message 0 */
	uint32_t can_tx_msg_0;		                /* 4C */
	/** \brief CAN TX Message 1 */
	uint32_t can_tx_msg_1;              	     /* 50 */
	/** \brief CAN TX Message 2 */
	uint32_t can_tx_msg_2;          	          /* 54 */
	/** \brief CAN RX Message 0 */
	uint32_t can_rx_msg_0;		                /* 58 */
	/** \brief CAN RX Message 1 */
	uint32_t can_rx_msg_1;        	        	 /* 5C */
	/** \brief CAN RX Message 2 */
	uint32_t can_rx_msg_2;                   	 /* 60 */
	/** \brief CAN Error Counters */
	uint32_t can_error_counters;                   /* 64 */
	/** \brief CAN Trigger Match */
	uint32_t can_trigger_match;                    /* 68 */
	/** \brief CAN Bus Timeout Preload Register */
	uint32_t can_timeout_preload;	                /* 6C */
	/** \brief Padding */
	uint32_t foo2[4];                			 /* 70 */
	/** \brief MILBUS 2 Configuration Register */
	uint32_t milbus_2_conf;            			/* 80 */
	/** \brief MILBUS 2 Extended Memory BAR*/
	uint32_t milbus_2_extended_memory_bar;      /* 84 */
	/** \brief MILBUS 2 Command Status Register */
	uint32_t milbus_2_command_status;            /* 88 */
	/** \brief MILBUS 2 Nominal Interrupt Register */
	uint32_t milbus_2_nominal_irq;               /* 8C */
	/** \brief MILBUS 2 Error Interrupt Register */
	uint32_t milbus_2_error_irq;                 /* 90 */
	/** \brief MILBUS 2 Reset Interrupt Register */
	uint32_t milbus_2_reset_irq;                 /* 94 */
	/** \brief MILBUS 2 Program Counter Register */
	uint32_t milbus_2_program_counter_regiser;   /* 98 */
	/** \brief MILBUS 2 Reception Table Index Register */
	uint32_t milbus_2_rx_table_index;            /* 9C */
	/** \brief MILBUS 2 Transmission Table Index Register */
	uint32_t milbus_2_tx_table_index;            /* A0 */
	/** \brief Padding */
	uint32_t foo3[3];                			 /* A4 */
	/** \brief Watchdog prescaler Register  */
	uint32_t watchdog_prescaler;            	/* B0 */
	/** \brief Watchdog preload Register  */
	uint32_t watchdog_register;        			 /* B4 */
	/** \brief Padding */
	uint32_t foo4[2];                			 /* B8 */
	/** \brief Embedded Test IP Status */
	uint32_t embedded_test_ip_status;         	 /* C0 */
	/** \brief Embedded Test IP Control  */
	uint32_t embedded_test_ip_control;         	/* C4 */
	/** \brief Embedded Test IP TDI Send  */
	uint32_t embedded_test_ip_tdi_send;       	 /* C8 */
	/** \brief Embedded Test IP TMI Send */
	uint32_t embedded_test_ip_tmi_send;          /* CC */
	/** \brief Embedded Test IP TDO Received */
	uint32_t embedded_test_ip_tdo_received;      /* D0 */
	/** \brief Padding */
	uint32_t foo5[11];                			 /* D4 */
	/**\brief GPIO 2 input*/
	uint32_t gpio_2_input;						 /* 100 */
	/**\brief GPIO 2 output register*/
	uint32_t gpio_2_output;						 /* 104 */
	/**\brief GPIO 2 direction register*/
	uint32_t gpio_2_direction;						 /* 108 */
	/**\brief MDPA Versioning Register*/
	uint32_t mdpa_versioning;						 /* 10C */
	/**\brief WARM Reset Detection Register*/
	uint32_t warm_reset_detection;						 /* 110 */
	/**\brief SW Reset enable Register*/
	uint32_t sw_reset_enable;						 /* 114 */
	/**\brief SW Reset Register*/
	uint32_t sw_reset;						 /* 118 */
	/**\brief MILBUS 1 API control/status Register*/
	uint32_t milbus_1_api_control_status;						 /* 11C */
	/**\brief MILBUS 2 API control/status Register*/
	uint32_t milbus_2_api_control_status;						 /* 120 */
	/**\brief MILBUS RT Address Register*/
	uint32_t milbus_rt_address;						 /* 124 */
	/** \brief Padding */
	uint32_t foo6[6];								/* 128 */
	/**\brief SIF Register*/
	uint32_t sif_register;						 /* 140 */
	/** \brief SIF Status Register */
	uint32_t sif_status;								/* 144 */
	/** \brief Padding */
	uint32_t foo7[6];								/* 148 */
	/** \brief OBTF CSYNC 1 Register */
	uint32_t obtf_sync_1;								/* 160 */
	/**\brief OBTF CSYNC 2 Register*/
	uint32_t obtf_sync_2;						 /* 164 */
	/** \brief OBTF CSYNC 3 Register */
	uint32_t obtf_sync_3;								/* 168 */
	/** \brief OBTF Filter Register */
	uint32_t obtf_filter;								/* 16C */
	/**\brief SCET Enable Register*/
	uint32_t scet_enable;						 /* 180 */
	/**\brief SCET Capture Register*/
	uint32_t scet_capture;						 /* 184 */
	/**\brief SCET SW Trigger upper Register*/
	uint32_t scet_sw_trigger_upper;						 /* 188 */
	/**\brief SCET SW Trigger lower Register*/
	uint32_t scet_sw_trigger_lower;						 /* 18C */
	/** \brief SCET irq(3) trigger upper Register */
	uint32_t scet_irq_3_upper;								/* 190 */
	/**\brief SCET irq(3) trigger lower Register*/
	uint32_t scet_irq_3_lower;						 /* 194 */
	/** \brief SCET CAN Trigger upper Register */
	uint32_t scet_can_upper;								/* 198 */
	/** \brief SCET CAN Trigger lower Register */
	uint32_t scet_can_lower;								/* 19C */
	/** \brief SCET Milbus 1 trigger upper Register */
	uint32_t scet_milbus_1_upper;								/* 1A0 */
	/**\brief SCET Milbus 1 Trigger lower Register*/
	uint32_t scet_milbus_1_lower;						 /* 1A4 */
	/** \brief SCET Milbus 2 trigger upper Register */
	uint32_t scet_milbus_2_upper;								/* 1A8 */
	/** \brief SCET Milbus 1 trigger lower Register */
	uint32_t scet_milbus_2_lower;								/* 1AC */
};


/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/


/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 * @}
 */
/**
 * @}
 */


#endif //ICU_MDPA_ADDRESSES_H
