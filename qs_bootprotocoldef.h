/**
 * @file qs_bootprotocolstruct.h
 * @author Fernando Morani
 * @date 27 Mar 2022
 * @brief All macros and define used in the boot protocol
 *
 */
#ifndef QS_BOOTPROTOCOLDEF_H
#define QS_BOOTPROTOCOLDEF_H

#define QS_BOOTP_OK                 0x00 /* Command acknowledge - Use in the payload*/
#define QS_BOOTP_FAIL               0x01 /* Command failed - Use in the payload*/

#define QS_BOOTP_MAX_PAY_LEN        256  /*  Max payload lenght */

#define QS_BOOTP_STX                0x02 /* (STX) */
#define QS_BOOTP_ETX                0x03 /* (ETX) */

#define QS_BOOTP_SENDER_SW          0x20 /*  Tool s/w is the sender of the packet */
#define QS_BOOTP_SENDER_APP_FW_D    0x21 /*  Application f/w in debug version is the sender of the packet */
#define QS_BOOTP_SENDER_APP_FW_R    0x22 /*  Application f/w in release version is the sender of the packet */
#define QS_BOOTP_SENDER_BOOT_FW     0x23 /*  Boot f/w is the sender of the packet */

#define QS_BOOTP_RESP_OFFSET        0x80 /*  Offset for command identifier field during response */

#define QS_BOOTP_POL_DEF            0x00 /*  Default value for the policy field */
// #define QS_BOOTP_PAY_OFF            QS_BOOTP_OK /*  Default value to fill cmd without any significant payload */

/*!
 * Section to define command ID
 *  0x00	Reserved                Reserved
 *  0x41	READ_FW_VERSION         Get all FW-items info and version
 *  0x42	READ_REV_ID             Get Revision ID info
 *  0x43	READ_DEV_ID             Get Device ID info
 *  0x44	READ_BOOT_VERSION       Get BootLoader version
 *  0x50	RESET                   Reset command
 *  0x51	ERASE                   Erase Flash
 *  0x52	READ                    Read Flash
 *  0x53	WRITE                   Write Flash
 *  0x70	START_FW_UPGRADE	    Start FW_Upgrade (jump to Bootloader…)
 */

#define QS_BOOTP_RES            0x00
#define QS_BOOTP_READ_FW        0x41
#define QS_BOOTP_READ_REV       0x42
#define QS_BOOTP_READ_DEV       0x43
#define QS_BOOTP_READ_BOOT      0x44
#define QS_BOOTP_RESET          0x50
#define QS_BOOTP_ERASE          0x51
#define QS_BOOTP_READ_FLASH     0x52
#define QS_BOOTP_WRITE_FLASH    0x53
#define QS_BOOTP_START_FW_UP    0x70

#define QS_BOOTP_MAX_BAK_NUM    10      // Max number of bank in the flash - TO FIND IN DATASHEET
#define QS_BOOTP_FLASH_MEM_SIZE 0x1000  // Max number of bank in the flash - TO FIND IN DATASHEET

#endif /* QS_BOOTPROTOCOLDEF_H*/