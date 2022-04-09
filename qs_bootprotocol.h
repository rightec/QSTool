/**
 * @file qs_bootprotocol.h
 * @author Fernando Morani
 * @date 30 Mar 2022
 * @brief Header file for the Boot protocol class
 * Used to control f/w upgrade procedure
 *
 */
#ifndef QS_BOOTPROTOCOL_H
#define QS_BOOTPROTOCOL_H

#include <QObject>
#include <QThread>
#include <QDebug>

class QS_BootProtocol : public QThread
{
    Q_OBJECT
public:
    QS_BootProtocol(QObject *parent);
    ~QS_BootProtocol();

private:
    void run() override;
};

#endif // QS_BOOTPROTOCOL_H
