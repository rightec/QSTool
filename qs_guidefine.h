/**
 * @file qs_guidefine.h
 * @author Fernando Morani
 * @date 27 Mar 2022
 * @brief Defines to be used in GUI
 *
 */

#ifndef QS_GUIDEFINE_H
#define QS_GUIDEFINE_H

#define QS_HEADER_RES       ":/Resources/header.jpg"
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

typedef enum {
    QS_CMD_PANEL_SECTION = 0,
    QS_FW_UPGRADE_SECTION = 1,
}QS_PanelSection;

#endif // QS_GUIDEFINE_H
