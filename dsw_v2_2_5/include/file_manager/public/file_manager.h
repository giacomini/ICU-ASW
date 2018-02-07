/**
 * \file	file_manager.h
 *
 * \brief	This file defines the public interface of the File Manager driver
 *
 * \author	Imanol Viana Sanchez, <iviana@srg.aut.uah.es>
 *
 * \internal
 * Created: 	08/11/2015
 * Compiler: 	gcc/g++
 * Company:  Space Research Group, Universidad de Alcala.
 * Copyright: Copyright (c) 2015, Imanol Viana Sanchez
 *
 * For further information, please visit http://srg.aut.uah.es
 *
 * This software is provided under the terms of the GNU General Public v2
 * Licence. A full copy of the GNU GPL is provided in the file COPYING
 * found in the development root of the project.
 *
 */

#ifndef PUBLIC__EUCLID_DSW_FILE_MNG_IFACE_V1_H
#define PUBLIC__EUCLID_DSW_FILE_MNG_IFACE_V1_H

/**
 *  @defgroup DSW_FILE_MANAGER File Manager
 *  @{
 */


/*****************************************************************************
   APPLICATION INCLUDES
*****************************************************************************/

#include <public/basic_types.h>

/*****************************************************************************
   PROVIDED CONSTANTS
*****************************************************************************/


/*PROTECTED REGION ID(public_dsw_file_mng_iface_h) ENABLED START*/
/**
 *  @defgroup DSW_FILE_MANAGER_ERROR_CODES Error Codes
 *  @{
 *  Error codes returned by the File Manager Driver
 */


/* Possible stage error values */
#define ICU_FM_MASTER_DESC_STAGE_ERROR  		(0x01U)
#define ICU_FM_SECTOR_DESC_STAGE_ERROR  		(0x02U)
#define ICU_FM_SECTOR_DEPLOY_STAGE_ERROR 		(0x04U)

/* Segment id error value when in master descriptor stage */
#define ICU_FM_NO_SECTOR_APPLICABLE			 	0xFFFFU

/* Possible detail error values */
#define ICU_FM_MDESC_EEPROM_ADDR_ALIGN_ERROR	(0x01U)
#define ICU_FM_MDESC_EEPROM_READ_ERROR  		(0x02U)
#define ICU_FM_MDESC_MAGIC_NUMBER_ERROR  		(0x04U)
#define ICU_FM_MDESC_CRC_MD_ERROR  				(0x08U)
#define ICU_FM_MDESC_SEC_NUMBER_ERROR  			(0x10U)
#define ICU_FM_MDESC_RAM_LIMITS_ERROR		  	(0x20U)

#define ICU_FM_SDESC_EEPROM_READ_ERROR  		(0x01U)
#define ICU_FM_SDESC_CRC_DSD_ERROR				(0x02U)
#define ICU_FM_SDESC_SIZE_ERROR			  		(0x04U)
#define ICU_FM_SDESC_DEPLOY_ADDR_ERROR			(0x08U)
#define ICU_FM_SDESC_EEPROM_ADDR_ERROR			(0x10U)
#define ICU_FM_SDESC_EEPROM_ADDR_ALIGN_ERROR	(0x20U)

#define ICU_FM_SDEPLOY_CRC_DS_EEPROM_ERROR		(0x01U)
#define ICU_FM_SDEPLOY_EEPROM_READ_ERROR		(0x02U)
#define ICU_FM_SDEPLOY_CRC_DS_RAM_ERROR			(0x04U)
#define ICU_FM_SDEPLOY_RAM_ADDR_ERROR			(0x08U)



/// @cond ALTERNATIVE_ERROR_CODES
/*Alternative error code values */
#define FM_MASTER_DESC_STAGE_ERROR  		ICU_FM_MASTER_DESC_STAGE_ERROR
#define FM_SEGMENT_DESC_STAGE_ERROR			ICU_FM_SECTOR_DESC_STAGE_ERROR
#define FM_SEGMENT_DEPLOY_STAGE_ERROR 		ICU_FM_SECTOR_DEPLOY_STAGE_ERROR

#define FM_NO_SEGMENT_APPLICABLE 			ICU_FM_NO_SECTOR_APPLICABLE

#define FM_MDESC_EEPROM_ADDR_ALIGN_ERROR	ICU_FM_MDESC_EEPROM_ADDR_ALIGN_ERROR
#define FM_MDESC_EEPROM_READ_ERROR  		ICU_FM_MDESC_EEPROM_READ_ERROR
#define FM_MDESC_MAGIC_NUMBER_ERROR  		ICU_FM_MDESC_MAGIC_NUMBER_ERROR
#define FM_MDESC_CRC_MD_ERROR  				ICU_FM_MDESC_CRC_MD_ERROR
#define FM_MDESC_SEG_NUMBER_ERROR  			ICU_FM_MDESC_SEC_NUMBER_ERROR
#define FM_MDESC_RAM_LIMITS_ERROR		  	ICU_FM_MDESC_RAM_LIMITS_ERROR

#define FM_SDESC_EEPROM_READ_ERROR  		ICU_FM_SDESC_EEPROM_READ_ERROR
#define FM_SDESC_CRC_DSD_ERROR				ICU_FM_SDESC_CRC_DSD_ERROR
#define FM_SDESC_SIZE_ERROR			  		ICU_FM_SDESC_SIZE_ERROR
#define FM_SDESC_DEPLOY_ADDR_ERROR			ICU_FM_SDESC_DEPLOY_ADDR_ERROR
#define FM_SDESC_EEPROM_ADDR_ERROR			ICU_FM_SDESC_EEPROM_ADDR_ERROR
#define FM_SDESC_EEPROM_ADDR_ALIGN_ERROR	ICU_FM_SDESC_EEPROM_ADDR_ALIGN_ERROR

#define FM_SDEPLOY_CRC_DS_EEPROM_ERROR		ICU_FM_SDEPLOY_CRC_DS_EEPROM_ERROR
#define FM_SDEPLOY_EEPROM_READ_ERROR		ICU_FM_SDEPLOY_EEPROM_READ_ERROR
#define FM_SDEPLOY_CRC_DS_RAM_ERROR			ICU_FM_SDEPLOY_CRC_DS_RAM_ERROR
#define FM_SDEPLOY_RAM_ADDR_ERROR			ICU_FM_SDEPLOY_RAM_ADDR_ERROR
/// @endcond

/**
 * @}
 */

/*****************************************************************************
   PROVIDED TYPES
*****************************************************************************/

/**
 *  @defgroup DSW_FILE_MANAGER_TYPES Structures
 *  @{
 */



/**
 * \brief Struct for handling a Master descriptor
 * \struct icu_fm_master_descriptor_t
 */
typedef struct
{
	/** \brief 	Magic number to locate a Master Descriptor	*/
	uint16_t 	magic_number;
	/** \brief 	Number of segments the app software image is composed of */
	uint8_t		seg_number;
	/** \brief 	Byte Reserved for future use	*/
	uint8_t 	byte_reserved;
	/** \brief 	Start address for the BSS section of the image	*/
	uint32_t 	bss_start_address;
	/** \brief 	Size of the BSS section of the image	*/
	uint32_t 	bss_size;
	/** \brief 	Start address for the STACK of the application	*/
	uint32_t 	stack_start_address;
	/** \brief 	Size of the STACK of the application	*/
	uint32_t 	stack_size;
	/** \brief 	Start address for the TEXT section of the image	*/
	uint32_t 	text_start_address;
	/** \brief 	Size of the TEXT section of the image	*/
	uint32_t	text_size;
	/** \brief 	32 Bytes Reserved for future use	*/
	uint8_t 	array_reserved[32];
	/** \brief 	Version of the image	*/
	uint8_t 	version;
	/** \brief 	Revision of the image	*/
	uint8_t 	revision;
	/** \brief 	CRC of the Master Descriptor	*/
	uint16_t 	crc;

}
icu_fm_master_descriptor_t;


/**
 * \brief Struct for handling a code segment descriptor
 * \struct	icu_fm_segment_descriptor_t
 */
typedef struct
{
	/** \brief 	Segment descriptor identificator	*/
	uint16_t	seg_id;
	/** \brief 	CRC calculated over the Data Segment valid bytes	*/
	uint16_t 	crc_ds;
	/** \brief  EEPROM address where the segment can be found	*/
	uint32_t 	eeprom_address;
	/** \brief 	Start deployment address for the segment	*/
	uint32_t 	deployment_ram_address;
	/** \brief 	Size of the segment in bytes	*/
	uint16_t 	size;
	/** \brief 	CRC of the descriptor(first 14Bytes)	*/
	uint16_t 	crc_dsd;

}
icu_fm_segment_descriptor_t;

/**
 * 	\brief	Defines the structure of the FM File Segment
 *
 * 	\struct	icu_fm_file_definition_segment_t
 */
typedef struct
{

	/**  \brief	descriptors area viewed as a set of Master Descriptors	*/
	icu_fm_master_descriptor_t master_desc;
	/**  \brief	descriptors area viewed as a set of Segment Descriptors	*/
	icu_fm_segment_descriptor_t segment_desc[252];

}
icu_fm_file_definition_segment_t;

/**
 * @}
 */


/*****************************************************************************
  PROVIDED VARIABLES
*****************************************************************************/


/*****************************************************************************
   PROVIDED FUNCTIONS
*****************************************************************************/

/**
 *  @defgroup DSW_FILE_MANAGER_API Public Functions
 *  @{
 *  \brief	Public functions provided by the File Manager Driver
 */


/**
 * \brief	This function deploys a complete OBF in SRAM. In case an error is
 * 			found during the deployment process, it stops and returns the
 * 			appropriate error code
 *
 * \param[in] eeprom_address The eeprom address where the file is stored. It
 * 				should correspond to the beginning of a 4KiByte sector that
 * 				contains the File Definition Sector (which starts with the
 * 				Master Descriptor).
 *
 * \return	It returns 0 if the OBF has been successfully deployed, and an
 * 			error code different to zero if not. The error code shall provide
 * 			information about which sector deployment has failed, and why, of
 * 			if the master descriptor is not valid, or it has not been found,
 * 			of if a sector descriptor is missed
 */
uint32_t icu_fm_deploy_OBF(const uint32_t eeprom_address);


/**
 * \brief	This function deploys a complete OBF in a specific SRAM address.
 * 			It is especially suited to files that are not an ASW File, and
 * 			therefore the files are not linked to an explicit RAM address.
 * 			Therefore the "deployment_address" parameter in the data sector
 * 			descriptor will be ignored, and the deployment will be performed
 * 			from the user-provided RAM address forward, in a linear,
 * 			unsegmented buffer.
 *
 * \param[in] eeprom_address EEPROM address where the file is stored.
 *
 * \param[in] ram_address RAM address where the OBF will be deployed
 *
 * \return	It returns 0 if the OBF has been successfully deployed and an error
 * 			code different to zero if not. The error code shall provide
 * 			information about which sector deployment has failed, and why, of
 * 			if the master descriptor is not valid, or it has not been found,
 * 			of if a sector descriptor is missed
 */
uint32_t icu_fm_deploy_OBF_in_SRAM(const uint32_t eeprom_address,
									const uint32_t ram_address);


/**
 * @}
 */
/**
 * @}
 */



/*PROTECTED REGION END*/


#endif // PUBLIC__EUCLID_DSW_FILE_MNG_IFACE_V1_H
