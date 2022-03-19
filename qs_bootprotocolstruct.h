/**
 * @file qs_bootprotocolstruct.h
 * @author Fernando Morani
 * @date 27 Mar 2022
 * @brief Defines the boot protocol structure
 *
 */
#ifndef QS_BOOTPROTOCOLSTRUCT_H
#define QS_BOOTPROTOCOLSTRUCT_H

#include <stdint.h>
#include "qs_bootprotocoldef.h"


/*!
 *  \typedef Stuctrured type to be used as payload in the following commands:
 *  READ_FW_VERSION
 *  READ_BOOT_VERSION.
 */
typedef struct
{
    uint16_t        FW_Erp_Identifier;   /* codice identificativo del FW */
    uint8_t         FW_Erp_Version;      /* Minor Version del FW */
    uint8_t         FW_Erp_BuildNumber;  /* incremental Build Number del FW */
    uint32_t        FW_Erp_Crc32;        /* CRC32 del FW */
    /* 8 bytes */
} FW_SW_VERSION_T;


/*!
 *  \typedef Stuctrured type to be used as payload in the following commands:
 *  READ_REV_ID
 *  READ_DEV_ID.
 */
typedef struct
{
    uint8_t         ID_Info_VersionL;      /* Info Version - Low Byte */
    uint8_t         ID_Info_VersionH;      /* Info Version - High Byte */
    /* 8 bytes */
} ID_INFO_VERSION_T;

/*!
 *  \typedef Stuctrured type to be used as payload in the following commands:
 *  ERASE
 *  WRITE  when coming from FW or BootLoader and it fails
 */
typedef struct
{
    uint8_t         BANK_Info_Ack;         /* QS_BOOTP_OK if the command has been executed or requested
                                            QS_BOOTP_FAIL if not*/
    uint8_t         BANK_Info_Number;      /* Bank number to erase or WRITE */
} BANK_INFO_T;

/*!
 *  \typedef Stuctrured type to be used as payload in the following commands:
 *  READ when coming from FW or BootLoader and it fails
 *  If READ flash is OK, it fill the payloads with all the requested bytes *
 */
typedef struct
{
    uint8_t         READ_Info_Ack;         /* QS_BOOTP_FAIL */
    uint8_t         READ_Info_Number;      /* Number of bytes not red */
} READ_INFO_ANSWER_T;


/*!
 *  \typedef Stuctrured type to be used as payload in the following commands:
 *  READ when coming from SW
 */
typedef struct
{
    uint16_t         READ_Address;         /* Flash Address to read */
    uint16_t         READ_Info_Number;     /* Number of bytes to read */
} READ_FROM_FLASH_T;



typedef struct QS_bootProt{
    uint8_t qs_Stx;         /*STX: Start of Message*/
    uint8_t qs_PayLen;      /*Payload lenght from 1 to 256 ---> Expressed as 0 to 255*/
                            /*Minimum payload lenght = 1*/
    uint8_t qs_Sender;      /*Sender of the packet
                            sender Address (0x20 for SW, 0x23 for Bootloader-FW; not specified for Application-FW)*/
    uint8_t qs_Policy;      /* User define field*/
    uint8_t qs_CmdId;       /*Command identifier field*/
    uint8_t *qs_Payload;    /*Payload field*/
    uint8_t qs_CrcLow;      /*CRC-16 low byte*/
    uint8_t qs_CrcHigh;     /*CRC-16 high byte*/
    uint8_t qs_Etx;         /*ETX: End of Message*/
} QS_BOOT_PROT_T;


#endif /* QS_BOOTPROTOCOLSTRUCT_H*/