/**
 * @file qs_cmdthread.cpp
 * @author Fernando Morani
 * @date 28 Mar 2022
 * @brief Thread class that manages command
 */

#include "qs_cmdthread.h"



QS_CmdThread::QS_CmdThread(QObject *parent)
	: QObject(parent)
{
    /// No command running at the start up
    setCmdRunning(false);
}

QS_CmdThread::~QS_CmdThread()
{
}

void QS_CmdThread::onRunCommand(int _idCmd)
{
    qDebug() << " QS_CmdThread::onRunCommand started";

    bool l_bo_Result = false;

    setCmdRunning(true);

	switch (_idCmd)
	{
    case QS_BOOTP_RES:
        break;
    case QS_BOOTP_READ_FW:
        break;
    case QS_BOOTP_READ_REV:
        break;
    case QS_BOOTP_READ_DEV:
        break;
    case QS_BOOTP_READ_BOOT:
        break;
    case QS_BOOTP_RESET:
        break;
    case QS_BOOTP_ERASE:
        break;
    case QS_BOOTP_READ_FLASH:
        break;
    case QS_BOOTP_WRITE_FLASH:
        break;
    case QS_BOOTP_START_FW_UP:
        QThread::msleep(2000); // Just to try
        break;
    default:
        qDebug() << " QS_CmdThread::onRunCommand case not managed";
		l_bo_Result = false;
		break;
    } // end switch

    qDebug() << " QS_CmdThread::onRunCommand ended";
    setCmdRunning(false);
	emit cmdResultReady(l_bo_Result);
}

