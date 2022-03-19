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
}

QS_CmdThread::~QS_CmdThread()
{
}

void QS_CmdThread::onRunCommand(int _idCmd)
{
    qDebug() << " QS_CmdThread::onRunCommand started";

	bool l_bo_Result = false;

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
        break;
    default:
        qDebug() << " QS_CmdThread::onRunCommand case not managed";
		l_bo_Result = false;
		break;
	}
    qDebug() << " QS_CmdThread::onRunCommand ended";
	emit cmdResultReady(l_bo_Result);
}

