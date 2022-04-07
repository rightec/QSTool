/**
 * @file qs_guidefine.h
 * @author Fernando Morani
 * @date 27 Mar 2022
 * @brief Defines to be used in GUI
 *
 */

#ifndef QS_GUIDEFINE_H
#define QS_GUIDEFINE_H

// #define QS_HEADER_RES   "://Resources/header.jpg"
// ":/Resources/header.jpg"
// #define QS_HEADER_RES       "qrc:/Resources/header.jpg"
// #define QS_HEADER_RES       "Resources/header.bmp"
#define QS_HEADER_RES       ":/Resources/header.bmp"
#define QS_HEADER_BOOT      "PIC18F4xQ43 BootLoader Tool"
#define QS_SECTION_FW       "Upgrade FW"
#define QS_SECTION_CMD      "Command Panel"
#define QS_APPLICATION_NAME QS_HEADER_BOOT

#define QS_REF_RECT_X       5
#define QS_REF_RECT_Y       160
#define QS_REF_RECT_W       1014
#define QS_REF_RECT_H       635

/*! String error section
  */
#define QS_ERR_GENERAL_SW           "Unexpected S/W Error"
#define QS_ERR_CMD_EXECUTED         "Command Correctly Executed"
#define QS_ERR_CMD_NOT_EXECUTED     "Command NOT Executed"
#define QS_ERR_CMD_CURR_SEND        "A Command is already sending"
#define QS_ERR_CMD_CONN_FAIL        "Connection failure"


#define QS_BOOTP_NO_ERROR_STN           " Rx message No error detected "
#define QS_BOOTP_ERR_RX_LEN_STN         " Rx message lenght less than minimum expected "
#define QS_BOOTP_ERR_WRONG_STX_STN      " Rx message lenght STX not received  "
#define QS_BOOTP_ERR_WRONG_PAYLEN_STN   " Rx message Wrong payload length  "
#define QS_BOOTP_ERR_POLICY_STN         " Rx message Wrong policy   "
#define QS_BOOTP_ERR_CMD_ID_STN         " Rx message CMD ID not as expected "
#define QS_BOOTP_ERR_CRC_STN            " Rx message CRC Error   "
#define QS_BOOTP_ERR_WRONG_ETX_STN      " Rx message lenght ETX not received"


typedef enum {
    QS_CMD_PANEL_SECTION = 0,
    QS_FW_UPGRADE_SECTION = 1,
}QS_PanelSection;

typedef enum {
    QS_NO_ERROR = 0,
    QS_ERROR_DETECTED = 1,
    QS_WRITE_TX = 2,
    QS_WRITE_RX = 3
}QS_SignalLevel;

#endif // QS_GUIDEFINE_H
