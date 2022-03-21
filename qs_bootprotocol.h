/**
 * @file qs_bootprotocol.h
 * @author Fernando Morani
 * @date 30 Mar 2022
 * @brief Header file for the Boot protocol class
 *
 */
#ifndef QS_BOOTPROTOCOL_H
#define QS_BOOTPROTOCOL_H


class QS_BootProtocol
{
public:
    QS_BootProtocol();

private:
    bool QS_SendMsg();
};

#endif // QS_BOOTPROTOCOL_H
