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
        m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        for (int l_i = 0; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
            m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
        }
        setReadTimeout(QS_SERIAL_RESET_READ_TMT);
        qDebug() << "QS_CmdThread::prepCommand - Started timeout of" <<
                    QS_SERIAL_RESET_READ_TMT <<
                    " at time: " <<
                   QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss,zzz");

        break;
    case QS_BOOTP_READ_FW:
        m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        for (int l_i = 0; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
            m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
        }
        setReadTimeout(QS_SERIAL_READ_FW_READ_TMT);
        break;
    case QS_BOOTP_READ_REV:
        m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        for (int l_i = 0; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
            m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
        }
        setReadTimeout(QS_SERIAL_READ_REV_READ_TMT);
        break;
    case QS_BOOTP_READ_DEV:
        m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        for (int l_i = 0; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
            m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
        }
        setReadTimeout(QS_SERIAL_READ_DEV_READ_TMT);
        break;
    case QS_BOOTP_READ_BOOT:
        m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        for (int l_i = 0; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
            m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
        }
        setReadTimeout(QS_SERIAL_READ_BOOT_READ_TMT);
        break;
    case QS_BOOTP_RES:
        qDebug() << " QS_CmdThread::onRunCommand RES case not managed";
        l_bo_Result = false;
        break;
    case QS_BOOTP_ERASE:
        m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        m_cmdToSend.qs_Payload[0] = m_BankInfoToSend.BANK_Info_Number;
        for (int l_i = 1; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
            m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
        }
        setReadTimeout(QS_SERIAL_ERASE_READ_TMT);
        break;
    case QS_BOOTP_READ_FLASH:
        m_cmdToSend.qs_PayLen = sizeof(READ_FROM_FLASH_T);
        m_cmdToSend.qs_Payload[0] = (m_ReadFromFlashToSend.READ_Address & 0xFF00) >> 8 ;
        m_cmdToSend.qs_Payload[1] = (m_ReadFromFlashToSend.READ_Address & 0x00FF)  ;
        m_cmdToSend.qs_Payload[2] = (m_ReadFromFlashToSend.READ_Info_Number & 0xFF00) >> 8 ;
        m_cmdToSend.qs_Payload[3] = (m_ReadFromFlashToSend.READ_Info_Number & 0x00FF) ;
        setReadTimeout(QS_SERIAL_READ_FLASH_READ_TMT);
        break;
    case QS_BOOTP_WRITE_FLASH:
        m_cmdToSend.qs_PayLen = m_WriteInfoToSend.WRITE_LEN;
        /// This is a dummy command.
        /// Payload[0] is the block to write
        /// Payload[1] is the element to write
        /// Payload[2.. 255] element +1 repeated
        // memcpy(static_cast<void*>(&m_cmdToSend.qs_Payload[0]),static_cast<void*>(&m_WriteInfoToSend.WRITE_Block[0]),m_cmdToSend.qs_PayLen);

        for (int i=0; i<m_cmdToSend.qs_PayLen;i++ ){
            m_cmdToSend.qs_Payload[i] = m_WriteInfoToSend.WRITE_Block[i];
        } // end for

        if (m_cmdToSend.qs_PayLen < QS_BOOTP_MIN_PAY_LEN){
            for (int l_i = m_cmdToSend.qs_PayLen; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
                m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
            }
            m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        }

        setReadTimeout(QS_SERIAL_WRITE_FLASH_READ_TMT);

        break;
    case QS_BOOTP_START_FW_UP:
        m_cmdToSend.qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        for (int l_i = 0; l_i < QS_BOOTP_MIN_PAY_LEN; l_i++){
            m_cmdToSend.qs_Payload[l_i] = QS_BOOTP_VOID_PAYLOAD;
        }
        if (m_CurrCmdPolInfo == QS_BOOTP_POL_DEF){
            /// TO DO - Execute the full upgrade procedure
        } else {
            /// TO DO - Send the dummy cmd and wait for answer
        }
        setReadTimeout(QS_SERIAL_START_FW_UP_READ_TMT);

        break;
    default:
        qDebug() << " QS_CmdThread::prepCommand case not managed";
        l_bo_Result = false;
        break;
    } // end switch

    uint8_t *crc_l = &m_cmdToSend.qs_CrcLow;
    uint8_t *crc_h = &m_cmdToSend.qs_CrcHigh;
    uint16_t crc_initial = 0x0000;
    uint16_t lenTxDecoder = m_cmdToSend.qs_PayLen;
    uint8_t *pBuf = &m_cmdToSend.qs_Payload[0];

    // CalcCrc16_Poly(0x0000, 0x8005, answerBuf, lenTxDecoder, &crcDecoderL, &crcDecoderH);
    CalcCrc16_Poly(crc_initial, POLY_IBM, pBuf, lenTxDecoder, crc_l, crc_h);

    qDebug() << " QS_CmdThread::prepCommand started: payload len is " << m_cmdToSend.qs_PayLen;
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

void QS_CmdThread::setWriteFlashInfoToSend(QString _hexString)
{
    QByteArray l_byteArray;
    QString l_qs = _hexString;
    int l_len = 0;

    l_byteArray += l_qs;
    l_len = l_byteArray.size();

    if (l_len > QS_BOOTP_BLOCK_SIZE)
    {
        l_len = QS_BOOTP_BLOCK_SIZE;
    } // else

    memset(static_cast<void*>(&m_WriteInfoToSend.WRITE_Block[0]),0,QS_BOOTP_BLOCK_SIZE);
    for (int l_i = 0; l_i< l_len; l_i++){
        m_WriteInfoToSend.WRITE_Block[l_i] = static_cast<uint8_t>(l_byteArray.at(l_i));
    } // end for

    m_WriteInfoToSend.WRITE_LEN = static_cast<uint8_t>(l_len);
}
/*
void QS_CmdThread::setWriteFlashInfoToSend(uint8_t _block, uint8_t _element)
{
    memset(static_cast<void*>(&m_WriteInfoToSend),0,sizeof(WRITE_TO_FLASH_T));
    m_WriteInfoToSend.WRITE_Block = _block;
    m_WriteInfoToSend.WRITE_element = _element;
}
*/

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
    // qDebug() << "QS_CmdThread::queueItemInCmdBuffer - m_CmdBuffIndex" << m_CmdBuffIndex;

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

void QS_CmdThread::parseRxData()
{
    uint8_t l_crcLow = 0;
    uint8_t l_crcHigh = 0;
    uint8_t l_cmdId = 0;
    uint8_t l_tempParse = 0;
    uint16_t l_payloadLen = 0;
    uint8_t l_payloadLenL = 0;
    uint8_t l_payloadLenH = 0;
    uint16_t l_msgLen = 0;
    uint8_t l_crcBuffer[QS_BOOTP_MAX_CMD_LEN];
    bool l_b_retVal = false;
    int l_i_parseCount = 0;
    int l_i_Size =m_responseData.size();

    qDebug() << "QS_CmdThread::parseRxData()";

    memset(static_cast<void*>(&m_cmdReceived),0,sizeof(QS_BOOT_PROT_T));
    setProtError(1000);

    if (l_i_Size < QS_BOOTP_MIN_CMD_LEN){
        setProtError(QS_BOOTP_ERR_RX_LEN);
    } else {
        /// Start Parsing
        if (m_responseData.at(l_i_parseCount) == QS_BOOTP_STX){
            qDebug() << "QS_CmdThread::parseRxData() STX DONE";
            m_cmdReceived.qs_Stx = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));

            l_i_parseCount++;
            l_payloadLenL = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
            l_i_parseCount++;
            l_payloadLenH = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
            l_payloadLen =  (0xFF00 & (static_cast<uint16_t>(l_payloadLenH) << 8)) |
                            (0x00FF & static_cast<uint16_t>(l_payloadLenL));

            if ((l_payloadLen <=QS_BOOTP_MAX_PAY_LEN ) && (l_payloadLen >= QS_BOOTP_MIN_PAY_LEN)){
                qDebug() << "QS_CmdThread::parseRxData() PAYLOAD LEN DONE";
                m_cmdReceived.qs_PayLen = l_payloadLen;
                l_i_parseCount++;
                /// Is the sender
                qDebug() << "QS_CmdThread::parseRxData() SENDER DONE";
                /// TO DO verifyTheSender();
                m_cmdReceived.qs_Sender = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
                l_i_parseCount++;
                /// Is the policy
                l_b_retVal = verifyPolicy(m_responseData.at(l_i_parseCount));
                if (l_b_retVal == true){
                    qDebug() << "QS_CmdThread::parseRxData() POLICY DONE";
                    m_cmdReceived.qs_Policy = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
                    l_i_parseCount++;
                    /// Is the cmd
                    l_tempParse = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
                    if ((l_tempParse - QS_BOOTP_RESP_OFFSET) == m_cmdToSend.qs_CmdId){
                        l_cmdId = m_cmdToSend.qs_CmdId;
                        qDebug() << "QS_CmdThread::parseRxData() CMD DONE";
                        m_cmdReceived.qs_CmdId = l_cmdId;

                        l_i_parseCount++;
                        /// Payload
                        for (int i = 0; i <l_payloadLen; i++){
                            m_cmdReceived.qs_Payload[i]=static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
                            l_i_parseCount++;
                        }
                        qDebug() << "QS_CmdThread::parseRxData() PAYLOAD DONE";
                        // l_i_parseCount = l_i_parseCount + l_payloadLen;

                        // l_i_parseCount++;
                        l_crcLow = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
                        l_i_parseCount++;
                        l_crcHigh = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));

                        memset(l_crcBuffer,0,QS_BOOTP_MAX_CMD_LEN);
                        /* From LEN to the last of the payload buffer */
                        l_msgLen = QS_BOOTP_MIN_CMD_LEN - QS_BOOTP_MIN_PAY_LEN +l_payloadLen -1 -1 -2;
                        for (int i = 1; i <= l_msgLen; i++){
                            l_crcBuffer[i-1] = static_cast<uint8_t>(m_responseData.at(i));
                        } // end for
                        uint8_t crc_l;
                        uint8_t crc_h;
                        uint16_t crc_initial = 0x0000;
                        uint8_t *pBuf = &l_crcBuffer[0];

                        CalcCrc16_Poly(crc_initial, POLY_IBM, pBuf, l_msgLen, &crc_l, &crc_h);
                        if ((crc_l == l_crcLow) && (crc_h == l_crcHigh)){
                            qDebug() << "QS_CmdThread::parseRxData() CRC DONE";
                            m_cmdReceived.qs_CrcLow = l_crcLow;
                            m_cmdReceived.qs_CrcHigh = l_crcHigh;
                            l_i_parseCount++;
                            /// Is the ETX
                            if (m_responseData.at(l_i_parseCount) == QS_BOOTP_ETX){
                                qDebug() << "QS_CmdThread::parseRxData() ETX DONE";
                                m_cmdReceived.qs_Etx = static_cast<uint8_t>(m_responseData.at(l_i_parseCount));
                                if (isExpectedCommand() == true){
                                    if (l_payloadLen >= retMinExpectedPayLen(l_cmdId)){
                                        setProtError(QS_BOOTP_NO_ERROR);
                                    } else {
                                        setProtError(QS_BOOTP_ERR_LEN_NOT_EXP);
                                    }
                                } else {
                                    setProtError(QS_BOOTP_ERR_CMD_NOT_EXP);
                                }

                            } else {
                                setProtError(QS_BOOTP_ERR_WRONG_ETX);
                            }
                        } else {
                            setProtError(QS_BOOTP_ERR_CRC);
                        }
                    } else {
                        setProtError(QS_BOOTP_ERR_CMD_ID);
                    }
                } else {
                 setProtError(QS_BOOTP_ERR_POLICY);
                }
            } else {
                setProtError(QS_BOOTP_ERR_WRONG_PAYLEN);
            }
        } else {
           setProtError(QS_BOOTP_ERR_WRONG_STX);
        }
    }
}

bool QS_CmdThread::isExpectedCommand()
{
    bool l_bRetVal = false;
    uint8_t u8_CmdPos = QS_BOOTP_CMD_POS;

    /// Cmd info is in the byte QS_BOOTP_CMD_POS o f the struct
    if (m_responseData.at(u8_CmdPos) == (m_cmdToSend.qs_CmdId - QS_BOOTP_RESP_OFFSET)){
        l_bRetVal = true;
    } //

    return l_bRetVal;
}

bool QS_CmdThread::isExpectedLength(uint8_t _idCmd)
{
    bool l_bRetVal = true;



    return l_bRetVal;
}

uint16_t QS_CmdThread::retMinExpectedPayLen(uint8_t _idCmd)
{
    uint16_t l_qs_PayLen = 0;

    switch (_idCmd)
    {
    case QS_BOOTP_RESET:
    case QS_BOOTP_READ_REV:
    case QS_BOOTP_READ_DEV:
    case QS_BOOTP_ERASE:
    case QS_BOOTP_START_FW_UP:
    case QS_BOOTP_WRITE_FLASH:
        l_qs_PayLen = QS_BOOTP_MIN_PAY_LEN;
        break;
    case QS_BOOTP_READ_FLASH:
        l_qs_PayLen = sizeof(READ_FROM_FLASH_T);
        break;
    case QS_BOOTP_READ_FW:
    case QS_BOOTP_READ_BOOT:
        l_qs_PayLen = static_cast<uint16_t>(sizeof(FW_SW_VERSION_T));
        break;
    default:
        qDebug() << " QS_CmdThread::retMinExpectedPayLen case not managed";
        break;
    } // end switch

    return l_qs_PayLen;

}


void QS_CmdThread::setProtError(int _err)
{
    if (_err != m_ProtcolError){
        m_ProtcolError = _err;
        emit protErrorFound(_err);
    } // else
}

bool QS_CmdThread::verifyPolicy(char _char)
{
    // For now we return always true
    bool l_bRetVal = true;

    uint8_t l_DevId = _char & QS_POLICY_MASK_DEV_ID;
    uint8_t l_BuildType = _char & QS_POLICY_MASK_BUILD_TYPE;
    uint8_t l_AppType = _char & QS_POLICY_MASK_APP_TYPE;

    // We have to verify what to do
    Q_UNUSED(l_DevId);
    Q_UNUSED(l_BuildType);
    Q_UNUSED(l_AppType);

    return l_bRetVal;
}

