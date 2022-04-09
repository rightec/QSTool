/**
 * @file qs_bootprotocol.cpp
 * @author Fernando Morani
 * @date 27 Mar 2022
 * @brief Class to manage the QS Boot protocol in a GUI environment
 *
 */
#include "qs_bootprotocol.h"


QS_BootProtocol::QS_BootProtocol(QObject *parent)
    : QThread(parent)
{

}

QS_BootProtocol::~QS_BootProtocol()
{

}

void QS_BootProtocol::run()
{
    /// Start Test code
    int i = 0;
    while (true){
        i++;
        if ((i % 10000000) == 0){
            qDebug() << "QS_BootProtocol::run() " << i;
        }
    }
    /// End Test code
}
