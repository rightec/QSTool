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
#include <QTimer>
#include <QDateTime>
#include <QFile>

#include "qs_bootprotocoldef.h"

/*! UPGRADE PROCEDURE:
 *  0) QS_UPGRADE_IDLE: QS_UPGRADE_IDLE
 *  1) QS_UPGRADE_START:
 *      Send the command QS_BOOTP_READ_DEV
 *      wait for answer
 *  2) QS_UPGRADE_SEND:
 *      Send command QS_BOOTP_START_FW_UP
 *      wait for answer
 *  3) QS_UPGRADE_ERASE:
 *      Send command QS_BOOTP_ERASE
 *      wait for answer
 *  4) QS_UPGRADE_WRITE_CYCLE
 *      Send command QS_BOOTP_WRITE_FLASH until the end of file or error
 *      wait for answer
 *  5) QS_UPGRADE_WRITE_END
 *     Send command QS_BOOTP_WRITE_FLASH (dummy) to close the write cycle
 *     wait for answer
 *  6) QS_UPGRADE_END
 *     The procedure ends
 */

typedef enum QS_upgrade_status{
    QS_UPGRADE_IDLE = 0,
    QS_UPGRADE_START,
    QS_UPGRADE_SEND,
    QS_UPGRADE_ERASE,
    QS_UPGRADE_WRITE_CYCLE,
    QS_UPGRADE_WRITE_END,
    QS_UPGRADE_END
} QS_upgrade_status_t;

class QS_BootProtocol : public QThread
{
    Q_OBJECT
public:
    QS_BootProtocol(QObject *parent);
    ~QS_BootProtocol() override;

    QS_upgrade_status_t getUpgradeState(){return m_upgradeState;}
    void setUpgradeState(QS_upgrade_status_t _state){m_upgradeState = _state;}
    bool getUpgradeTriggerFlag(){return m_startTrigger;}
    void setUpgradeTriggerFlag(bool _state){m_startTrigger = _state; if (_state == true) m_logTimer.start();}
    bool getUpgradeCmdAnswer(){return m_waitForAnswer;}
    void setUpgradeCmdAnswer(bool _state){m_waitForAnswer = _state;}
    void setDeviceId(uint16_t _infoId){m_DeviceId = _infoId;}
    void setSenderId(uint8_t _infoSender){m_SenderId = _infoSender;}
    void setHexFileName(QString _fileName){m_HexFile.setFileName(_fileName);}

private:
    void run() override;

    void resetUpgradeProc();
    void upgradeStartCase();
    void upgradeSendCase();
    void upgradeEraseCase();
    void upgradeWriteCycleCase();
    void upgradeWriteEndCase();
    void idleStartCase();


    QS_upgrade_status_t m_upgradeState;
    bool                m_startTrigger;
    bool                m_waitForAnswer;
    bool                m_bStringToNextWrite = false;
    bool                m_isAWriteCmdRunning = false;
    uint16_t            m_DeviceId;
    uint8_t             m_SenderId;
    int                 m_maxBankSize;
    qint64              m_FileSizeToSend;
    QTimer              m_logTimer;
    QFile               m_HexFile;
    QTextStream         *m_in = nullptr;
    QString             m_sTemp;


private slots:
    void onLogTimerElapsed();

signals:
    void upgradeCmd(int _idCmd);
    void sendWriteString(QString _s_Line);

};

#endif // QS_BOOTPROTOCOL_H
