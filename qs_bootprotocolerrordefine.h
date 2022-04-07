/**
 * @file qs_bootprotocolerrordefine.h
 * @author Fernando Morani
 * @date 07 Apr 2022
 * @brief All macros and define used in the boot protocol to trace error
 *
 */
#ifndef QS_BOOTPROTOCOLERRORDEFINE_H
#define QS_BOOTPROTOCOLERRORDEFINE_H

#define QS_BOOTP_NO_ERROR           0x00 /* Rx message No error detected                    */
#define QS_BOOTP_ERR_RX_LEN         0x01 /* Rx message lenght less than minimum expected    */
#define QS_BOOTP_ERR_WRONG_STX      0x02 /* Rx message lenght STX not received              */
#define QS_BOOTP_ERR_WRONG_PAYLEN   0x03 /* Rx message Wrong payload length                 */
#define QS_BOOTP_ERR_POLICY         0x04 /* Rx message Wrong policy                         */
#define QS_BOOTP_ERR_CMD_ID         0x05 /* Rx message CMD ID not as expected               */
#define QS_BOOTP_ERR_CRC            0x06 /* Rx message CRC Error                            */
#define QS_BOOTP_ERR_WRONG_ETX      0x07 /* Rx message lenght ETX not received              */

#endif // QS_BOOTPROTOCOLERRORDEFINE_H
