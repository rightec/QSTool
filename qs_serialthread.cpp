/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qs_serialthread.h"


QS_SerialThread::QS_SerialThread(QObject *parent) :
    QThread(parent)
{
    m_waitTimeout = QS_SERIAL_WRITE_TIMEOUT;
}

QS_SerialThread::~QS_SerialThread()
{
    m_mutex.lock();
    m_quit = true;
    m_cond.wakeOne();
    m_mutex.unlock();
    wait();
}

void QS_SerialThread::transaction(const QString &portName, int waitTimeout, const QString &request)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_request = request;
    if (!isRunning())
        start();
    else
        m_cond.wakeOne();
}

//! [4]
void QS_SerialThread::run()
{
    qDebug() << "QS_SerialThread::run() - ENTER";

    m_mutex.lock();

    int currentWaitTimeout = m_waitTimeout;
    QString currentRequest = "Prova"; //m_request;
    m_mutex.unlock();

    while (!m_quit) {

//        if (m_serial.isOpen() == false){
//            if (!m_serial.open(QIODevice::ReadWrite)) {
//                emit error(tr("Can't open %1, error code %2")
//                           .arg(m_portName).arg(m_serial.error()));
//                return;
//            }
//        }
        // write request
//        m_requestData = currentRequest.toUtf8();
//        m_serial.write(m_requestData);
            // read response
            if (m_serial.waitForReadyRead(currentWaitTimeout)) {
                QByteArray responseData = m_serial.readAll();
                while (m_serial.waitForReadyRead(10))
                    responseData += m_serial.readAll();

                const QString response = QString::fromUtf8(responseData);
//! [12]
                emit this->response(response);
//! [10] //! [11] //! [12]
            } else {
                emit timeout(tr("Wait read response timeout %1")
                             .arg(QTime::currentTime().toString()));
            }
        m_mutex.lock();
        m_cond.wait(&m_mutex);
        currentWaitTimeout = m_waitTimeout;
        currentRequest = m_request;
        m_mutex.unlock();
    }
}

uint32_t QS_SerialThread::getBaudRate(int _index)
{
    return m_BaudeRatesAvail[_index];
}

bool QS_SerialThread::setBaudRate(uint32_t _baudRate)
{
    bool l_b_RetVal = false;

    /// Verify if the baudrate is an availabel one

    for(int i=0; i<QS_SERIAL_BAUD_END; i++){
        if (_baudRate == m_BaudeRatesAvail[i]){
            m_serial.setBaudRate(static_cast<qint32>(_baudRate));
            l_b_RetVal = true; // Find a plausible baudRate
            break; // Break the for cycle
        }
    }

    return l_b_RetVal;
}

bool QS_SerialThread::setPort(QString _comPort)
{
    bool l_b_RetVal = false;

    m_serial.setPortName(_comPort);

    if (m_serial.portName() == _comPort){
        l_b_RetVal = true;
    } // else

    return l_b_RetVal;

}

bool QS_SerialThread::openSerial()
{
    bool l_b_RetVal = false;

    if (!m_serial.open(QIODevice::ReadWrite)) {
        emit error(tr("Can't open %1, error code %2")
                   .arg(m_portName).arg(m_serial.error()));
    } else {
        l_b_RetVal = true;
        qDebug() << "QS_SerialThread::openSerial Port open and thread started";
        this->start();
    }
    return l_b_RetVal;

}

bool QS_SerialThread::closeSerial()
{
    if (m_serial.isOpen() == true){
        m_serial.close();
        if (isRunning() == true){
            terminate(); // Stop the thread
            qDebug() << "QS_SerialThread::closeSerial Port close and thread stopped";
        } else {
            qDebug() << "QS_SerialThread::closeSerial Port close but thread was not running";
        }
    } else {
        qDebug() << "QS_SerialThread::closeSerial Port was closed";
    }
    return true;
}

bool QS_SerialThread::writeToSerial()
{
    bool l_b_RetVal = false;

    m_serial.write((const char*)&m_SerialBuffer[0],m_BytesToWrite);
    // m_serial.write(m_requestData);
    if (m_serial.waitForBytesWritten(m_waitTimeout)) {
        qDebug() << "QS_SerialThread::writeToSerial open and thread started";

    } else {
        emit timeout(tr("Wait write request timeout %1")
                     .arg(QTime::currentTime().toString()));
    }
    return l_b_RetVal;
}
