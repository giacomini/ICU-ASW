/**
 * \file	tc_header.h
 *
 * \brief	This file contains declaration of function for TC formatting and execution
 *
 * \author	Vito Capobianco, <vito.capobianco_at_oato.inaf.it>
 *
 * \internal
 * Created: 	5-05-2015
 *
 */
#ifndef TC_HEADER_H_
#define TC_HEADER_H_

#include "db_type_def.h"
#include "tc_header.h"

/**
 *  \ingroup TC_HANDLER_CONSTANTS
 *  \{
 */
#define TC_VERSION_MASK 0xe000U //!< Mask for TC_VERSION field
#define TC_TYPE_MASK 0x1000U //!< Mask for TC_TYPE field
#define TC_DATA_FIELD_MASK 0x800U //!< Mask for TC_DATA_FIELD field
#define TC_PROCESSID_MASK 0x7ffU //!< Mask for TC_PROCESSID field
#define TC_SEQUENCE_NUMBER_MASK 0xc000U //!< Mask for TC_SEQUENCE_NUMBER field
#define TC_SEQUENCE_COUNTER_MASK 0x3fffU //!< Mask for TC_SEQUENCE_COUNTER field
#define TC_DATA_FIELD_FLAG 0X80U //!< Mask for TC_DATA_FIELD_FLAG field
#define TC_DATA_FIELD_PUS 0x38U //!< Mask for TC_DATA_FIELD_PUS field
#define TC_DATA_FIELD_ACK 0xfU //!< Mask for TC_DATA_FIELD_ACK field
#define ILLEGAL_APID_ERROR 0U //!< Error code returned in case of illegal APID during TC validation
#define ERROR_TC_LENGTH 1U //!< Error code returned in case of bad packet length during TC validation
#define INCORRECT_CHECKSUM 2U //!< Error code returned in case of bad checksum during TC validation
#define ILLEGAL_TYPE 3U //!< Error code returned in case of PUS Service not supported during TC validation
#define ILLEGAL_SUBTYPE 4U //!< Error code returned in case of PUS SubService not supported during TC validation
#define ILLEGAL_PACKET_HEADER 5U //!< Error code returned in case of bad packet header during TC validation
#define CMD_NOT_EXECUTABLE 6U //!< Error code returned in case of system bad state during TC validation
#define CMD_ID_NOT_FOUND 7U //!< Error code returned in case of bad Function ID during TC validation. Only for PUS Service(8,1) commands
#define CMD_OK 8U //!< TC accepted during TC validation
/* bytes, req. NI-ICU-ASW-I-322 */
#define TC_MAX_DATA_SIZE 1004U //!< Maximum size in bytes of Data area for a PUS TC
/* Number of telecommands PUS 8,1 implemented */
#define NUMBER_OF_PUS_8_1_TCS 50U //!< Number of PUS Service 8,1 telecommands
/* Number of all telecommands implemented (not PUS 8,1)*/
#define NUMBER_OF_TC 24U //!< Number of other PUS Services telecommands
/* Ack Flags of telecommands */
#define PUS_1_1_GEN_FLAG 0x1U //!< Value used in PUS TC ACK_FLAG to generate PUS 1,1 packet
#define PUS_1_7_GEN_FLAG 0x8U //!< Value used in PUS TC ACK_FLAG to generate PUS 1,7 packet
/**
 * \}
 */

/**
 *  \ingroup TC_HANDLER_TYPES
 *  \{
 */

/**
 * \struct	TC_pkt_header_id
 *
 * \brief	Definition of PUS TC packet header IDfield
 */
/* PUS Packet Header identification */
typedef union {
	uint16_t all_bits;

	struct {
		uint8_t version :3;
		uint8_t type :1;
		uint8_t data_field :1;
		uint16_t processId :11;
	};
} TC_pkt_header_id;

/* PUS Packet Sequence identification */
/**
 * \struct	TC_pkt_header_seq
 *
 * \brief	Definition of PUS TC packet header sequence field
 */
typedef union {
	uint16_t all_bits;

	struct {
		uint8_t seq_number :2;
		uint16_t seq_count :14;
	};
} TC_pkt_header_seq;

/* PUS Packet Data Field identification */

/**
 * \struct	TC_pkt_data_field
 *
 * \brief	Definition of PUS TC packet data field field
 */
typedef union {
	uint32_t all_bits;

	struct {
		uint8_t flag :1;
		uint8_t pus_version :3;
		uint8_t ack :4;
		uint8_t service_type;
		uint8_t service_subtype;
		uint8_t sourceId;
	};
} TC_pkt_data_field;

/* Telecommand structure definition */
/**
 * \struct	telecommand_t
 *
 * \brief	Definition of PUS TC packet structure
 */
typedef struct {
	TC_pkt_header_id pkt_id;
	TC_pkt_header_seq pkt_seq;
	uint16_t TC_pkt_length;
	TC_pkt_data_field data_field;
	uint8_t TC_pkt_data[TC_MAX_DATA_SIZE];
	uint16_t TC_err_control;
} telecommand_t;

/* Struct used to define TC table structure */
/**
 * \struct	mask_TC_t
 *
 * \brief	Definition of an ICU TC table entry
 */

typedef struct {
	uint8_t type;
	uint8_t subtype;
	uint32_t cmd_id;
	uint8_t modes;
	uint16_t timeOut;
	icu_status_code (*ptrf)(telecommand_t);
} mask_TC_t;

/**
 * \}
 */

/**
 *  \ingroup TC_HANDLER_VARIABLES
 *  \{
 */
/* Global variable used to handle tc after acceptance check */
extern mask_TC_t ptrTC;

/* Global variable telecommand. It is used to rebuild data stream as TC PUS
 * format*/
extern telecommand_t telecommand;

/*
 * Global variable containing all telecommands information except PUS Service 8,1
 */
extern mask_TC_t tc_table[NUMBER_OF_TC];

/*
 * Global variable containing all PUS Service 8,1 telecommand informations
 */
extern mask_TC_t tc_8_1[NUMBER_OF_PUS_8_1_TCS];

/**
 * \}
 */

/**
 *  \ingroup TC_HANDLER_FUNCTIONS
 *  \{
 */
/**
 * \brief Function used to format data received from S/C into Telecommand
 * \param[in] tc_data data received
 * \return Telecommand Telecommand in a PUS data structure
 */
telecommand_t asw_create_tc(uint8_t tc_data[]);

/**
 * \brief Function used to format Packet Id from data received from S/C
 * \param[in] tc_data data received
 * \return void
 */
void asw_tc_set_pkt_id(const uint8_t tc_data[]);

/**
 * \brief Function used to format Packet Sequence from data received from S/C
 * \param[in] tc_data data received
 * \return void
 */
void asw_tc_set_pkt_seq(const uint8_t tc_data[]);

/**
 * \brief Function used to format Packet Length from data received from S/C
 * \param[in] tc_data data received
 * \return void
 */
void asw_tc_set_pkt_length(const uint8_t tc_data[]);

/**
 * \brief Function used to format Data field from data received from S/C
 * \param[in] tc_data data received
 * \return void
 */
void asw_tc_set_pkt_data_field(const uint8_t tc_data[]);

/**
 * \brief Function used to format Data from data received from S/C
 * \param[in] tc_data data received
 * \return void
 */
void asw_tc_set_pkt_data(const uint8_t tc_data[]);

/**
 * \brief Function used to format Checksum from data received from S/C
 * \param[in] tc_data data received
 * \return void
 */
void asw_tc_set_pkt_checksum(const uint8_t tc_data[]);

/**
 * \brief Function used to validate telecommand received from S/C
 * \param[in] tc TC to be validated
 * \param[in] tc_data Byte stream of the received TC
 * \param[out] ptr_mask_tc Pointer to the tc table entry
 * \return int8_t CMD_OK if TC is accepted, an error code otherwise.
 */
uint16_t asw_validate_command(telecommand_t tc, uint8_t tc_data[],
		mask_TC_t *ptr_mask_tc);

/**
 * \brief Function used to create the ISO checksum
 * \param[in] values Data stream
 * \param[in] n Data size in bytes
 * \return checksum Generated checksum
 */
uint16_t asw_create_ISO_checksum(uint8_t values[], uint16_t n);

/**
 * \brief Function used to decript the ISO checksum
 * \param[in] values Data stream
 * \param[in] n Data size in bytes
 * \return checksum  0 if OK, wrong checksum otherwise
 */
uint8_t asw_decript_ISO_checksum(uint8_t values[], uint16_t n);

/**
 * \brief	Function used top initialize the CRC16 table
 */
void asw_init_CRC16_table(void);

/**
 * \brief Function to calculate the CRC
 * \param[in] pData Address from where the CRC shall be calculated.
 * \param[in] numBytes  Number of bytes to calculate the CRC.
 * \return	CRC value Generated checksum
 */
uint16_t asw_create_checksum(uint8_t *pData, uint16_t numBytes);

/**
 * \brief Function used to check validity of received PUS service and subservice parameters.
 * \param[in] pus_type PUS Service type
 * \param[in] pus_subtype PUS Service subtype
 * \return 0 if service and subservice exists, 1 otherwise.
 */
uint8_t asw_check_subtype(uint8_t pus_type, uint8_t pus_subtype);

/**
 * \}
 */

/**
 * \}
 */

#endif /* TC_HEADER_H_ */
