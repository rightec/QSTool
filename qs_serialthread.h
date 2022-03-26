/**
 * @file qs_serialthread.h
 * @author Fernando Morani
 * @date 30 Mar 2022
 * @brief Header file for the serial thread class
 *
 */
#ifndef QS_SerialThread_H
#define QS_SerialThread_H

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include <QSerialPort>
#include <QTime>
#include <QDebug>

#include "qs_serialdefine.h"


class QS_SerialThread : public QThread
{
    Q_OBJECT

public:
    explicit QS_SerialThread(QObject *parent = nullptr);
    ~QS_SerialThread() override;

    void transaction(const QString &portName, int waitTimeout, const QString &request);

    /*!
     * \brief getBaudRate: Get Baud rate from the array
     * \param _index
     * \return
     */
    uint32_t getBaudRate(int _index);

    /*!
     * \brief setBaudRate: Set the baudRate for the seril port
     * \param _baudRate
     * \return: false if setting is wrong
     */
    bool setBaudRate(uint32_t _baudRate);

    /*!
     * \brief setPort: Set the reference port for the serial connection
     * \param _comPort
     * \return: false if setting is wrong
     */
    bool setPort(QString _comPort);

    /*!
     * \brief openSerial: Open serial connection
     * \return: true if the port is open
     */
    bool openSerial();

    /*!
     * \brief closeSerial: Close serial connection
     * \return
     */
    bool closeSerial();

    /*!
     * \brief writeToSerial
     * \return: true if write to serial send the packet
     */
    bool writeToSerial();

    QByteArray m_requestData;
    uint8_t    m_SerialBuffer[QS_SERIAL_MAX_BUF_LEN ];
    uint16_t   m_BytesToWrite;


signals:
    void response(const QString &s);
    void error(const QString &s);
    void timeout(const QString &s);


private:

    void run() override;

    QString m_portName;
    QString m_request;
    int m_waitTimeout = 0;
    QMutex m_mutex;
    QWaitCondition m_cond;
    bool m_quit = false;
    QSerialPort m_serial;

    uint32_t m_BaudeRatesAvail[QS_SERIAL_BAUD_END] = {
        QS_SERIAL_BAUD_2400,QS_SERIAL_BAUD_4800,
        QS_SERIAL_BAUD_9600,QS_SERIAL_BAUD_19200,
        QS_SERIAL_BAUD_38400,QS_SERIAL_BAUD_57600,
        QS_SERIAL_BAUD_115200
    };
};

#endif // QS_SerialThread_H
