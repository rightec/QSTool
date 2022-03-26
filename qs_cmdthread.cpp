/**
 * @file qs_cmdthread.cpp
 * @author Fernando Morani
 * @date 28 Mar 2022
 * @brief Thread class that manages command
 */

#include "qs_cmdthread.h"



QS_CmdThread::QS_CmdThread(QObject *parent)
    : QS_SerialThread(parent)
{
    resetCmdBuffer(); // Init buffer
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

    /// TO DO: Send the command via serial or activate upgrade procedure

    qDebug() << " QS_CmdThread::onRunCommand ended";
    setCmdRunning(false);
	emit cmdResultReady(l_bo_Result);
}

bool QS_CmdThread::prepCommand(int _idCmd)
{
    qDebug() << " QS_CmdThread::prepCommand started";

    bool l_bo_Result = false;

    memset(static_cast<void*>(&m_cmdToSend),0,sizeof(QS_BOOT_PROT_T));
    m_cmdToSend.qs_Stx = QS_BOOTP_STX;
    m_cmdToSend.qs_CmdId = static_cast<uint8_t>(_idCmd); // Set command id
    m_cmdToSend.qs_Sender = QS_BOOTP_SENDER_SW;
    m_cmdToSend.qs_Etx = QS_BOOTP_ETX;
    m_cmdToSend.qs_Policy = m_CurrCmdPolInfo;

    switch (_idCmd)
    {
    case QS_BOOTP_RESET:
        m_cmdToSend.qs_PayLen = 1;
        m_cmdToSend.qs_Payload[0] = QS_BOOTP_VOID_PAYLOAD;
        break;
    case QS_BOOTP_READ_FW:
        m_cmdToSend.qs_PayLen = 1;
        m_cmdToSend.qs_Payload[0] = QS_BOOTP_VOID_PAYLOAD;
        break;
    case QS_BOOTP_READ_REV:
        m_cmdToSend.qs_PayLen = 1;
        m_cmdToSend.qs_Payload[0] = QS_BOOTP_VOID_PAYLOAD;
        break;
    case QS_BOOTP_READ_DEV:
        m_cmdToSend.qs_PayLen = 1;
        m_cmdToSend.qs_Payload[0] = QS_BOOTP_VOID_PAYLOAD;
        break;
    case QS_BOOTP_READ_BOOT:
        m_cmdToSend.qs_PayLen = 1;
        m_cmdToSend.qs_Payload[0] = QS_BOOTP_VOID_PAYLOAD;
        break;
    case QS_BOOTP_RES:
        qDebug() << " QS_CmdThread::onRunCommand RES case not managed";
        l_bo_Result = false;
        break;
    case QS_BOOTP_ERASE:
        m_cmdToSend.qs_PayLen = sizeof(BANK_INFO_T);
        memcpy(static_cast<void*>(&m_cmdToSend.qs_Payload[0]),static_cast<void*>(&m_BankInfoToSend),sizeof(BANK_INFO_T));
        break;
    case QS_BOOTP_READ_FLASH:
        m_cmdToSend.qs_PayLen = sizeof(READ_FROM_FLASH_T);
        m_cmdToSend.qs_Payload[0] = (m_ReadFromFlashToSend.READ_Address & 0xFF00) >> 8 ;
        m_cmdToSend.qs_Payload[1] = (m_ReadFromFlashToSend.READ_Address & 0x00FF)  ;
        m_cmdToSend.qs_Payload[2] = (m_ReadFromFlashToSend.READ_Info_Number & 0xFF00) >> 8 ;
        m_cmdToSend.qs_Payload[3] = (m_ReadFromFlashToSend.READ_Info_Number & 0x00FF) ;
        break;
    case QS_BOOTP_WRITE_FLASH:
        m_cmdToSend.qs_PayLen = QS_BOOTP_BLOCK_SIZE;
        /// This is a dummy command.
        /// Payload[0] is the block to write
        /// Payload[1] is the element to write
        /// Payload[2.. 255] element +1 repeated
        memcpy(static_cast<void*>(&m_cmdToSend.qs_Payload[0]),static_cast<void*>(&m_WriteInfoToSend),sizeof(WRITE_TO_FLASH_T));
        for (int i=sizeof(WRITE_TO_FLASH_T); i<QS_BOOTP_BLOCK_SIZE;i++ ){
            m_cmdToSend.qs_Payload[i] = m_cmdToSend.qs_Payload[1] + 1;
        }
        break;
    case QS_BOOTP_START_FW_UP:
        m_cmdToSend.qs_PayLen = 1;
        m_cmdToSend.qs_Payload[0] = QS_BOOTP_VOID_PAYLOAD;
        if (m_CurrCmdPolInfo == QS_BOOTP_POL_DEF){
            /// TO DO - Execute the full upgrade procedure
        } else {
            /// TO DO - Send the dummy cmd and wait for answer
        }
        break;
    default:
        qDebug() << " QS_CmdThread::prepCommand case not managed";
        l_bo_Result = false;
        break;
    } // end switch

    return l_bo_Result;
}

void QS_CmdThread::setBankInfoToSend(uint8_t _ack, uint8_t _bankNumber)
{
    memset(static_cast<void*>(&m_BankInfoToSend),0,sizeof(BANK_INFO_T));
    m_BankInfoToSend.BANK_Info_Ack = _ack;
    m_BankInfoToSend.BANK_Info_Number = _bankNumber;
}

void QS_CmdThread::setReadFlashInfoToSend(uint16_t _address, uint16_t _bytes_to_read)
{
    memset(static_cast<void*>(&m_ReadFromFlashToSend),0,sizeof(READ_FROM_FLASH_T));
    m_ReadFromFlashToSend.READ_Address = _address;
    m_ReadFromFlashToSend.READ_Info_Number = _bytes_to_read;
}

void QS_CmdThread::setWriteFlashInfoToSend(uint8_t _block, uint8_t _element)
{
    memset(static_cast<void*>(&m_WriteInfoToSend),0,sizeof(WRITE_TO_FLASH_T));
    m_WriteInfoToSend.WRITE_Block = _block;
    m_WriteInfoToSend.WRITE_element = _element;
}

void QS_CmdThread::setPolicyInfo(POLICY_INFO _policy)
{
    m_CurrCmdPolInfo = _policy;
}

bool QS_CmdThread::startSerialConnection(QString _comPort, uint32_t _baudRate)
{
    bool l_b_RetVal = false;

    l_b_RetVal = setPort(_comPort);
    if (l_b_RetVal == true){
        l_b_RetVal = setBaudRate(_baudRate);
        if (l_b_RetVal == true){
            l_b_RetVal = openSerial();
        } // else
    } // else

    return l_b_RetVal;

}

void QS_CmdThread::stopSerialConnection()
{
    closeSerial();
}

void QS_CmdThread::resetCmdBuffer()
{
    m_CmdBuffIndex = 0;
    m_BytesToWrite = 0;
    memset(&m_FullCmdBuffer[0],0,QS_BOOTP_MAX_CMD_LEN);
    // m_requestData.clear();
    memset(&m_SerialBuffer[0],0,QS_SERIAL_MAX_BUF_LEN);
}

void QS_CmdThread::queueItemInCmdBuffer(uint8_t _item)
{
    if (m_CmdBuffIndex < QS_BOOTP_MAX_CMD_LEN){
        m_FullCmdBuffer[m_CmdBuffIndex] = _item;
        // const char *c = reinterpret_cast<const char *>(&m_FullCmdBuffer[m_CmdBuffIndex]);
        // m_requestData.append(c);
        m_SerialBuffer[m_CmdBuffIndex] = _item;
        m_CmdBuffIndex++;
        m_BytesToWrite++;
    } else {
        // Restart the queue - This is an error
        m_CmdBuffIndex = 0;
        qDebug() << "QS_CmdThread::queueItemInCmdBuffer - Buffer overflow ????";
    }
}
