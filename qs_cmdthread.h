/**
 * @file qs_cmdthread.h
 * @author Fernando Morani
 * @date 28 Mar 2022
 * @brief Header file for the thread class that manages command
 */

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QBuffer>
#include <QDataStream>

#include "qs_bootprotocoldef.h"
#include "qs_bootprotocolstruct.h"
#include "qs_serialthread.h"
#include "qs_bootprotocolerrordefine.h"


class QS_CmdThread : public QS_SerialThread
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
     * \brief setBankInfoToSend: set payload info for QS_BOOTP_ERASE
     * \param _ack
     * \param _bankNumber
     */
    void setBankInfoToSend(uint8_t _ack, uint8_t _bankNumber);

    /*!
     * \brief setReadFlashInfoToSend set payload info for QS_BOOTP_READ_FLASH
     * \param _address
     * \param _bytes_to_read
     */
    void setReadFlashInfoToSend(uint16_t _address, uint16_t _bytes_to_read);

    /*!
     * \brief setWriteFlashInfoToSend  set payload info for QS_BOOTP_WRITE_FLASH
     * \param _block
     * \param _element     
     * void setWriteFlashInfoToSend(uint8_t _block, uint8_t _element);
    */

    /*!
     * \brief setWriteFlashInfoToSend
     * \param _hexString
     */
    void setWriteFlashInfoToSend(QString _hexString);

    /*!
     * \brief setPolicyInfo: Set policy info for the current command
     * \param _policy
     */
    void setPolicyInfo(POLICY_INFO _policy);

    /*!
     * \brief isCmdRunning: Get if a command is currently running
     * \return
     */
    bool isCmdRunning(){return m_cmdRunning;}

    /*!
     * \brief prepCommand: Prepare command to send
     * \param _idCmd
     * \return
     */
    bool prepCommand(int _idCmd);

    /*!
     * \brief startSerialConnection: Start serial connection
     * \param _comPort
     * \param _baudRate
     * \return
     */
    bool startSerialConnection(QString _comPort, uint32_t _baudRate);

    /*!
     * \brief stopSerialConnection
     */
    void stopSerialConnection();

    /*!
     * \brief resetCmdBuffer
     */
    void resetCmdBuffer();

    /*!
     * \brief queueItemInCmdBuffer
     * \param _item
     */
    void queueItemInCmdBuffer(uint8_t _item);

    /*!
     * \brief parseRxData
     */
    void parseRxData();


    QS_BOOT_PROT_T      m_cmdToSend;


public slots:
	void onRunCommand(int _idCmd);

private:
    bool                        m_cmdRunning;  // When true A command is currently running
    BANK_INFO_T                 m_BankInfoToSend;
    READ_FROM_FLASH_T           m_ReadFromFlashToSend;
    WRITE_TO_BUFFER_FLASH_T     m_WriteInfoToSend;
    POLICY_INFO                 m_CurrCmdPolInfo; // Policy info of the current command
    uint8_t                     m_FullCmdBuffer[QS_BOOTP_MAX_CMD_LEN ];
    int                         m_CmdBuffIndex;
    int                         m_ProtcolError;

    /*!
     * \brief setProtError: Set protcol error var
     * \param _err
     */
    void setProtError(int _err);

    /*!
     * \brief verifyPolicy: Verify if the policy is correct
     * \param _char
     * \return
     */
    bool verifyPolicy(char _char);

signals:
	void cmdResultReady(bool _result);
    void protErrorFound(int _err);
};
