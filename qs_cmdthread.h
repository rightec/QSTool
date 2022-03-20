/**
 * @file qs_cmdthread.h
 * @author Fernando Morani
 * @date 28 Mar 2022
 * @brief Header file for the thread class that manages command
 */

#include <QObject>
#include <QThread>
#include <QDebug>

#include "qs_bootprotocoldef.h"
#include "qs_bootprotocolstruct.h"


class QS_CmdThread : public QObject
{
	Q_OBJECT

public:
    QS_CmdThread(QObject *parent);
    ~QS_CmdThread();

    /*!
     * \brief setCmdRunning: Set the command is running flag
     * \param _isRunning
     */
    void setCmdRunning(bool _isRunning){m_cmdRunning = _isRunning;}

    /*!
     * \brief isCmdRunning: Get if a command is currently running
     * \return
     */
    bool isCmdRunning(){return m_cmdRunning;}


    QS_BOOT_PROT_T      m_cmdToSend;
    BANK_INFO_T         m_BankInfoToSend;
    WRITE_TO_FLASH_T    m_WriteInfoToSend;
    READ_FROM_FLASH_T   m_ReadFromFlashToSend;


public slots:
	void onRunCommand(int _idCmd);

private:
    bool m_cmdRunning;  // When true A command is currently running

signals:
	void cmdResultReady(bool _result);

};
