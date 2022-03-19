/**
 * @file qs_cmdthread.h
 * @author Fernando Morani
 * @date 28 Mar 2022
 * @brief Header file for the thread class that manages command
 */

#include <QObject>
#include <QThread>
#include <QDebug>

#include <qs_bootprotocoldef.h>


class QS_CmdThread : public QObject
{
	Q_OBJECT

public:
    QS_CmdThread(QObject *parent);
    ~QS_CmdThread();


public slots:
	void onRunCommand(int _idCmd);

private:


signals:
	void cmdResultReady(bool _result);

};
