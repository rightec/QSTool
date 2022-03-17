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


typedef enum {
    QS_CMD_PANEL_SECTION = 0,
    QS_FW_UPGRADE_SECTION = 1,
}QS_PanelSection;

#endif // QS_GUIDEFINE_H
