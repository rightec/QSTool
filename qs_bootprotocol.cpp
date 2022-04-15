/**
 * @file qs_bootprotocol.cpp
 * @author Fernando Morani
 * @date 27 Mar 2022
 * @brief Class to manage the QS Boot protocol in a GUI environment
 *
 */
#include "qs_bootprotocol.h"

const uint16_t g_WriteBlockSize = 64;

QS_BootProtocol::QS_BootProtocol(QObject *parent)
    : QThread(parent)
{
    resetUpgradeProc();
    connect(&m_logTimer,SIGNAL(timeout()),this, SLOT(onLogTimerElapsed()));
    m_logTimer.setInterval(1000);
}


QS_BootProtocol::~QS_BootProtocol()
{

}

void QS_BootProtocol::resetUpgradeProc()
{
    setUpgradeState(QS_UPGRADE_IDLE);
    setUpgradeTriggerFlag(false);
    setUpgradeCmdAnswer(false);
    setDeviceId(0);
}

void QS_BootProtocol::upgradeStartCase()
{
    if (getUpgradeCmdAnswer() == true){
        // Answer received
        // reset flag
        setUpgradeCmdAnswer(false);
        // Verify DEV ID to set max size of the
        if (m_DeviceId == QS_PIC18F46Q43_DEV_ID){
                m_maxBankSize = 65536;
        } else {
            if (m_DeviceId == QS_PIC18F47Q43_DEV_ID){
                m_maxBankSize = 131072;
            } else{
                // ERROR STOP THE PROCEDURE
                setUpgradeState(QS_UPGRADE_END);
            }
        }
        setUpgradeState(QS_UPGRADE_SEND);
        emit upgradeCmd(QS_BOOTP_START_FW_UP);
    } else {
        //
    }

}

void QS_BootProtocol::upgradeSendCase()
{
    if (getUpgradeCmdAnswer() == true){
        // Answer received
        // reset flag
        setUpgradeCmdAnswer(false);
        qDebug() << "QS_BootProtocol::upgradeSendCas() - Start Wait Cycle" <<
                    QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");
        // Verify payload is ok - It is done by the upper level
        if (m_SenderId == QS_BOOTP_SENDER_BOOT_FW){
            // Set a short wait cycle
            msleep(500);
        } else {
            // Set a long wait cycle
            msleep(5000);
        }
        qDebug() << "QS_BootProtocol::upgradeSendCas() - End Wait Cycle" <<
                    QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");

        // Go to next state
        setUpgradeState(QS_UPGRADE_ERASE);
        emit upgradeCmd(QS_BOOTP_ERASE);
    } else {
        //
    }

}

void QS_BootProtocol::upgradeEraseCase()
{
    if (getUpgradeCmdAnswer() == true){
        // Answer received
        // reset flag
        setUpgradeCmdAnswer(false);
        qDebug() << "QS_BootProtocol::upgradeEraseCase()";
        // Verify payload is ok - It is done by the upper level

        // Go to next state
        if (m_HexFile.size() > 0){
            m_FileSizeToSend = m_HexFile.size();
            if (m_HexFile.open(QIODevice::ReadOnly | QIODevice::Text) == true){
                // Associate to stream
                m_in = new QTextStream(&m_HexFile);
                if (m_in != nullptr){
                    setUpgradeState(QS_UPGRADE_WRITE_CYCLE);
                } else {
                    setUpgradeState(QS_UPGRADE_END);
                }
            } else {
                setUpgradeState(QS_UPGRADE_END);
            }
        } else {
            setUpgradeState(QS_UPGRADE_END);
        }
    } else {
        //
    }

}

void QS_BootProtocol::upgradeWriteCycleCase()
{
    QString l_s_Line;
    int i = 0;

    if (m_FileSizeToSend > 0){
        // There is still something to send
        if (m_isAWriteCmdRunning == true){
            // A Write command is running
            // Wait for an answer
            if (getUpgradeCmdAnswer() == true){
                // Answer has been received - Reset flag
                setUpgradeCmdAnswer(false);
                m_isAWriteCmdRunning = false;
            } else {
                // Nothing to do
                asm("nop");
            }
        } else {
            /// Read from file and send a write command
            if (m_bStringToNextWrite == true){
                // There is someting to add send
                i = m_sTemp.size();
                l_s_Line = m_sTemp;
            } else {
                // Nothing to add
                i = 0;
            }

            while ((i < g_WriteBlockSize ) && (!(m_in->atEnd()))){
                m_sTemp = m_in->readLine();
                i = i+m_sTemp.size();
                if (i < g_WriteBlockSize){
                    // I can append this string to the line
                    l_s_Line.append(m_in->readLine());
                } else {
                    // I can't append temp should go in the next write cmd
                    m_bStringToNextWrite = true;
                    i = g_WriteBlockSize; // Forcing the exit
                }
            } // end while

            /// Update size to send
            m_FileSizeToSend = m_FileSizeToSend - l_s_Line.size();
            /// Emit the string to send
            m_isAWriteCmdRunning = true;
            emit sendWriteString(l_s_Line);
            emit upgradeCmd(QS_BOOTP_WRITE_FLASH);
        }


    } else {
        // No more string to send
        emit sendWriteString(l_s_Line);
        emit upgradeCmd(QS_BOOTP_WRITE_FLASH);
        setUpgradeState(QS_UPGRADE_WRITE_END);
    }
}

void QS_BootProtocol::upgradeWriteEndCase()
{
    if (getUpgradeCmdAnswer() == true){
        // Answer has been received - Reset flag
        setUpgradeCmdAnswer(false);
        setUpgradeState(QS_UPGRADE_END);
        qDebug() << "QS_BootProtocol::upgradeWriteEndCase()";
    } else {
        // Nothing to do
        asm("nop");
    }

}

void QS_BootProtocol::idleStartCase()
{
    // Verify the start trigger
    if (getUpgradeTriggerFlag() == true){
        setUpgradeState(QS_UPGRADE_START);
        emit upgradeCmd(QS_BOOTP_READ_DEV);
    } else {
        //
    }

}

void QS_BootProtocol::run()
{
    /// Start Test code
   /*!
     int i = 0;
     while (true){
        i++;
        if ((i % 100000000) == 0){
            qDebug() << "QS_BootProtocol::run() " << i;
        }        
        qDebug() << "QS_BootProtocol::run() state is: " << m_upgradeState;
    }
    */

    /// End Test code


    while (m_upgradeState != QS_UPGRADE_END){
        switch (m_upgradeState) {
        case QS_UPGRADE_IDLE:
            idleStartCase();
            break;
        case QS_UPGRADE_START:
            upgradeStartCase();
            break;
        case QS_UPGRADE_SEND:
            upgradeSendCase();
            break;
        case QS_UPGRADE_ERASE:
             upgradeEraseCase();
            break;
        case QS_UPGRADE_WRITE_CYCLE:
            upgradeWriteCycleCase();
            break;
        case QS_UPGRADE_WRITE_END:
            upgradeWriteEndCase();
            break;
        case QS_UPGRADE_END:
            break;
        } // end switch
    } // end while

    qDebug() << "QS_BootProtocol::run() thread ended";

}

void QS_BootProtocol::onLogTimerElapsed()
{
    qDebug() << "QS_BootProtocol::run() state is: " << m_upgradeState;
}
