#include "quantaloader.h"
#include "ui_quantaloader.h"

#include <QUrl>

QuantaLoader::QuantaLoader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuantaLoader)
{
    ui->setupUi(this);

    /*! Build the the reference RECT for sub panels*/
    m_RefRect.setX(QS_REF_RECT_X);
    m_RefRect.setY(QS_REF_RECT_Y);
    m_RefRect.setWidth(QS_REF_RECT_W);
    m_RefRect.setHeight(QS_REF_RECT_H);

    ui->m_frm_PanelCmd->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    ui->m_frm_mainUpload->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    /*! Build the Header Frame*/
    QPixmap l_logo_pic;
    QString l_S_temp;
#ifdef LOGO_AS_FILE
    l_S_temp = qApp->applicationDirPath() + "/" + QS_HEADER_RES;
    if (QFile::exists(l_S_temp)) {
        l_logo_pic.load( l_S_temp);
        ui->m_lbl_ImageHead->setPixmap(l_logo_pic);     // Place the pixmap in the label
        ui->m_lbl_ImageHead->setVisible(true);
        const QPixmap *l_pixMap = ui->m_lbl_ImageHead->pixmap();
        m_msgBox.setText("File Log exist: PixMap is" + l_S_temp);
        if (m_msgBox.exec()){
            //
        } //
    } else {
        m_msgBox.setText("File NOT NOT exist");
        if (m_msgBox.exec()){
            //
        } //
        ui->m_lbl_ImageHead->setText("Image not found");
    }

   #else
    l_S_temp = QS_HEADER_RES;
    l_logo_pic.load( l_S_temp);
    ui->m_lbl_ImageHead->setPixmap(l_logo_pic);     // Place the pixmap in the label
#endif
    ui->m_lbl_tool->setText( QS_HEADER_BOOT);

    /*! Select the defualt section: Upgrade section*/
    setGuiSection(QS_FW_UPGRADE_SECTION);

    /*! Setting boundary values*/
    // TO DELETE ui->m_spn_SelectBankWrite->setMaximum(QS_BOOTP_MAX_BAK_NUM);
    ui->m_spn_SelectBankErase->setMaximum(QS_BOOTP_MAX_BAK_NUM);
    ui->m_spn_SelectStartRead->setMaximum(QS_BOOTP_FLASH_MEM_SIZE -1); // Flash size - 1 bytes

    /*! Initialize widgets*/
    initProgressBar();
    connect(&m_Tmr_UpdateFWProgBar, SIGNAL(timeout()), this, SLOT(onFwUpdateUpdateprog()));
    m_isConnected = false;
    enableCmdButton(m_isConnected);

    /// Init MessageBox
    m_msgBox.setWindowTitle(QS_APPLICATION_NAME);
    m_msgBox.setStandardButtons(QMessageBox::Yes);

    /// message box Dialog dimension
    QSpacerItem* l_horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* l_layout = static_cast<QGridLayout*>(m_msgBox.layout());
    l_layout->addItem(l_horizontalSpacer, l_layout->rowCount(), 0, 1, l_layout->columnCount());

    /// Connection comboBox
    const auto l_infos = QSerialPortInfo::availablePorts();
        for (const QSerialPortInfo &l_info : l_infos){
            ui->m_cmb_comPort->addItem(l_info.portName());
        } // end for cycle - Filling the combo box
    ui->m_cmb_comPort->setCurrentIndex(ui->m_cmb_comPort->count() -1); // Default is 115200

    /*! Thread section starts*/
    m_p_CmdThread = new QS_CmdThread(nullptr);
    connect(m_p_CmdThread, SIGNAL(timeout(QString)), this, SLOT(onSendCmdTimeout(QString)));
    connect(m_p_CmdThread, SIGNAL(response(QString)), this, SLOT(onResponse(QString)));
    // m_p_CmdThread->moveToThread(&m_workerThread);
    // connect(&m_workerThread, &QThread::finished, m_p_CmdThread, &QObject::deleteLater);
    // connect(this, SIGNAL(operate(int)), m_p_CmdThread, SLOT(onRunCommand(int)));
    // connect(m_p_CmdThread, SIGNAL(cmdResultReady(bool)), this, SLOT(onCmdResultReady(bool)));
    // m_workerThread.start();  /// TO DO Comment for now
    // Thread section ends

    /// BaudRate comboBox
    for ( int l_i = 0;l_i < QS_SERIAL_BAUD_END; l_i++ )
    {
       uint32_t l_baudRate = m_p_CmdThread->getBaudRate(l_i);
       ui->m_cmb_BaudRate->addItem(QString::number(l_baudRate,10));
    }
    ui->m_cmb_BaudRate->setCurrentIndex(QS_SERIAL_BAUD_END -1); // Default is 115200


}

void QuantaLoader::setGuiSection(QS_PanelSection _section)
{
    if (m_guiSection != _section){
        m_guiSection = _section;

        switch (static_cast<int>(m_guiSection)) {
        case QS_CMD_PANEL_SECTION:
            displayCmdSection();
            break;
        case QS_FW_UPGRADE_SECTION:
            displayFwUpgradeSection();
            break;
        default:
            qDebug() << "QuantaLoader::setGuiSection wrong input param";
            break;
        }
    } // else
}

void QuantaLoader::displayCmdSection()
{
    /// Resize correctly command panel section (TO DO)
    /// Hide Fw Upgrade section
    /// Show Command Section
    /// Change reference button string
    ui->m_frm_PanelCmd->setGeometry(m_RefRect);
    ui->m_frm_PanelCmd->setVisible(true);
    ui->m_frm_mainUpload->setVisible(false);
    ui->m_btnChoosePanel->setText(QS_SECTION_FW);
}

void QuantaLoader::displayFwUpgradeSection()
{
    /// Resize correctly fw upgrade section (TO DO)
    /// Show Fw Upgrade section
    /// Hide Command Section
    /// Change reference button string
    ui->m_frm_mainUpload->setGeometry(m_RefRect);
    ui->m_frm_PanelCmd->setVisible(false);
    ui->m_frm_mainUpload->setVisible(true);
    ui->m_btnChoosePanel->setText(QS_SECTION_CMD);

}

void QuantaLoader::initProgressBar()
{
    ui->m_prg_UpdateFW->setVisible(false);
    ui->m_prg_UpdateFW->setValue(0);
}

void QuantaLoader::activateProgBar()
{

    QProgressBar *l_WidgetTemp = nullptr;
    QString l_S_safe = "QProgressBar::chunk {background: green;border-bottom-right-radius: 7px;border-bottom-left-radius: 7px;border: 1px solid black;}";

    l_WidgetTemp = ui->m_prg_UpdateFW;
    m_Tmr_UpdateFWProgBar.start(500);

    l_WidgetTemp->setVisible(true);
    l_WidgetTemp->setMinimum(0);
    l_WidgetTemp->setMaximum(100);
    l_WidgetTemp->setStyleSheet(l_S_safe);
}

void QuantaLoader::enableCmdButton(bool _enable)
{
    ui->m_btn_reset->setEnabled(_enable);
    ui->m_btn_readFW->setEnabled(_enable);
    ui->m_btn_readDID->setEnabled(_enable);
    ui->m_btn_readRID->setEnabled(_enable);
    ui->m_btn_readBoot->setEnabled(_enable);
    ui->m_btn_ReadFlash->setEnabled(_enable);
    ui->m_btn_EraseFlash->setEnabled(_enable);
    ui->m_btn_writeFlash->setEnabled(_enable);
    ui->m_btn_startUpgrade->setEnabled(_enable);
    ui->m_btn_startFwUpgrade->setEnabled(_enable);
}

bool QuantaLoader::preSendCommand(int _cmdId)
{
    bool l_b_RetVal = false;

    if (m_p_CmdThread->isCmdRunning() == false){
        l_b_RetVal = true;
        /// queue command


        m_p_CmdThread->prepCommand(_cmdId);
        /// write to log
        writeSendToLog();

        emit operate(_cmdId);
    } else {
        // A command is already running. Not possible to send other command
        l_b_RetVal = false;
    }

    return l_b_RetVal;
}

void QuantaLoader::writeSendToLog()
{
    // Reset cmd buffer
    m_p_CmdThread->resetCmdBuffer();

    /// STX
    QString l_s_write = "TX: ";
    uint8_t l_u8 = m_p_CmdThread->m_cmdToSend.qs_Stx;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" + QString::number(l_u8,16));
    l_s_write.append(" ");

    /// PAYLOAD LENGTH
    l_u8 = m_p_CmdThread->m_cmdToSend.qs_PayLen;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" + QString::number(l_u8,16));
    l_s_write.append(" ");

    /// SENDER
    l_u8 = m_p_CmdThread->m_cmdToSend.qs_Sender;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    /// POLICY
    l_u8 = m_p_CmdThread->m_cmdToSend.qs_Policy;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    /// COMMAND ID
    l_u8 = m_p_CmdThread->m_cmdToSend.qs_CmdId;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    /// PAYLOAD
    qDebug() << "QuantaLoader::writeSendToLog payload len:  " << m_p_CmdThread->m_cmdToSend.qs_PayLen;
    for (int i = 0; i < m_p_CmdThread->m_cmdToSend.qs_PayLen; i++){
        l_u8 = m_p_CmdThread->m_cmdToSend.qs_Payload[i];
        m_p_CmdThread->queueItemInCmdBuffer(l_u8);
        l_s_write.append("0x" +QString::number(l_u8,16));
        l_s_write.append(" ");
    } // end payload for
    qDebug() << "QuantaLoader::writeSendToLog end sending payload ";

    /// CRC LOW
    l_u8 = m_p_CmdThread->m_cmdToSend.qs_CrcLow;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    /// CRC HIGH
    l_u8 = m_p_CmdThread->m_cmdToSend.qs_CrcHigh;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    /// ETX
    l_u8 = m_p_CmdThread->m_cmdToSend.qs_Etx;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

//    ui->m_txt_serialLog->setStyleSheet("color: blue");
//    ui->m_txt_serialLog->insertHtml(l_s_write);

    setLogColorByLevel(ui->m_txt_serialLog, QS_WRITE_TX, l_s_write);

    m_p_CmdThread->writeToSerial();
}

void QuantaLoader::writeReadToLog(QString _string)
{
    QString l_s_write = "RX: ";
    int l_i_Size = m_p_CmdThread->m_responseData.size();
    qDebug() << "QuantaLoader::writeReadToLog red bytes: " << l_i_Size;
    for (int i= 0; i<l_i_Size; i++){
         uint8_t l_u8 = static_cast<uint8_t>(m_p_CmdThread->m_responseData.at(i));
         l_s_write.append("0x" +QString::number(l_u8,16));
         l_s_write.append(" ");
    }

//    ui->m_txt_serialLog->insertHtml(l_s_write);
//    ui->m_txt_serialLog->setStyleSheet("color: green");

    setLogColorByLevel(ui->m_txt_serialLog, QS_WRITE_RX, l_s_write);

    /* TO PARSE
    uint8_t l_u8 = m_p_CmdThread->m_cmdToSend.qs_Stx;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    uint16_t l_u16 = 0;
    l_s_write = "0x" + QString::number(l_u8,16);
    l_s_write.append(" ");

    l_u16 = m_p_CmdThread->m_cmdToSend.qs_PayLen;
    l_u8 = (l_u16 & 0xFF00) >> 8;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" + QString::number(l_u8,16));
    l_s_write.append(" ");
    l_u8 = (l_u16 & 0x00FF);
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" + QString::number(l_u8,16));
    l_s_write.append(" ");


    l_u8 = m_p_CmdThread->m_cmdToSend.qs_Sender;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    l_u8 = m_p_CmdThread->m_cmdToSend.qs_Policy;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    l_u8 = m_p_CmdThread->m_cmdToSend.qs_CmdId;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    for (int i = 0; i < m_p_CmdThread->m_cmdToSend.qs_PayLen; i++){
        l_u8 = m_p_CmdThread->m_cmdToSend.qs_Payload[i];
        m_p_CmdThread->queueItemInCmdBuffer(l_u8);
        l_s_write.append("0x" +QString::number(l_u8,16));
        l_s_write.append(" ");
    } // end payload for

    l_u8 = m_p_CmdThread->m_cmdToSend.qs_CrcLow;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    l_u8 = m_p_CmdThread->m_cmdToSend.qs_CrcHigh;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    l_u8 = m_p_CmdThread->m_cmdToSend.qs_Etx;
    m_p_CmdThread->queueItemInCmdBuffer(l_u8);
    l_s_write.append("0x" +QString::number(l_u8,16));
    l_s_write.append(" ");

    ui->m_txt_serialLog->setStyleSheet("color: blue");
    ui->m_txt_serialLog->appendPlainText(l_s_write);

    m_p_CmdThread->writeToSerial();
    */
}

void QuantaLoader::setLogColorByLevel(QTextEdit *_refTextEdit, QS_SignalLevel _errLevel, QString _textToWrite)
{
    QString l_line = _textToWrite;
    QTextCursor l_cursor = _refTextEdit->textCursor();
    QString l_alertHtml = "<font color=\"Red\">";
    QString l_notifyHtml = "<font color=\"Green\">";
    QString l_infoHtml = "<font color=\"Aqua\">";
    QString l_infoHtmlTx = "<font color=\"Black\">";
    QString l_infoHtmlRx = "<font color=\"Blue\">";
    QString l_endHtml = "</font><br>";
    int level = static_cast<int>(_errLevel);

    switch(level)
    {
    case QS_WRITE_TX:
        l_line = l_infoHtmlTx + l_line;
        break;
    case QS_WRITE_RX:
        l_line = l_infoHtmlRx + l_line;
        break;
    case QS_ERROR_DETECTED:
        l_line = l_alertHtml + l_line;
        break;
    case QS_NO_ERROR:
        l_line = l_notifyHtml + l_line;
        break;
    default:
        l_line = l_infoHtml + l_line;
        break;
    }

    l_line = l_line + l_endHtml;
    _refTextEdit->insertHtml(l_line);
    l_cursor.movePosition(QTextCursor::End);
    _refTextEdit->setTextCursor(l_cursor);
}


QuantaLoader::~QuantaLoader()
{
    delete ui;
}

void QuantaLoader::on_m_btnChoosePanel_clicked()
{
    switch (static_cast<int>(m_guiSection)) {
    case QS_CMD_PANEL_SECTION:
        // We have to go to upgrade section
        setGuiSection(QS_FW_UPGRADE_SECTION);
        break;
    case QS_FW_UPGRADE_SECTION:
        setGuiSection(QS_CMD_PANEL_SECTION);
        break;
    default:
        qDebug() << "QuantaLoader::on_m_btnChoosePanel_clicked wrong section !!!";
        break;
    }

}

void QuantaLoader::on_m_btn_clearUpgradeLog_clicked()
{
    ui->m_txt_upgradeLog->clear();  // Clear the log section
}

void QuantaLoader::on_m_btn_ClearCmdPanelLog_clicked()
{
    ui->m_txt_serialLog->clear();  // Clear the log section
}

void QuantaLoader::on_m_bnt_openDialog_clicked()
{
    QString l_S_FileName = "";
    l_S_FileName = QFileDialog::getOpenFileName(this, tr("Browsing for the FW version"), "", tr("*.hex"));
    qDebug() << "on_m_bnt_openDialog_clicked: Selected file: " << l_S_FileName;
    if (l_S_FileName != ""){
        /// It is the choosen file
        ui->m_txt_selectFile->setText(l_S_FileName);
    } /// else

}

void QuantaLoader::onCmdResultReady(bool _res)
{
    bool l_bo_Error = false;
    QString l_S_danger = "QProgressBar::chunk {background: red;border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}";
    QString l_S_FailCommand = QS_ERR_CMD_NOT_EXECUTED;
    QString l_S_DoneCommand = QS_ERR_CMD_EXECUTED;

    /*! A command could came from the UPGRADE Section
     *  either the command panel section
     */
    switch (static_cast<int>(getGuiSection()))
    {
    case QS_FW_UPGRADE_SECTION:
        /// In case of command connected to prog bar
        if (_res == false){
            ui->m_prg_UpdateFW->setStyleSheet(l_S_danger);
        } else {
            /// TO DO set style sheet
        }
        ui->m_prg_UpdateFW->setValue(ui->m_prg_UpdateFW->maximum());

        QThread::msleep(3000);  // Wait time for the user to see the failure

        /// Reset the progress bar
        m_Tmr_UpdateFWProgBar.stop();
        ui->m_prg_UpdateFW->setVisible(false);
        ui->m_prg_UpdateFW->setValue(0);
        break;
    case QS_CMD_PANEL_SECTION:
        break;
    default:
        l_bo_Error = true;
        m_msgBox.setText(QS_ERR_GENERAL_SW);
        qDebug() << "QuantaLoader::onCmdResultReady - Not Plausible branch";
        break;
    } // End swicth

    if (l_bo_Error == false){
        if (_res == true){
            m_msgBox.setText(l_S_DoneCommand);
            qDebug() << "QuantaLoader::onCmdResultReady: " << l_S_DoneCommand;
        }
        else {
            qDebug() << "QuantaLoader::onCmdResultReady: " << l_S_FailCommand;
            /// Display a QMessage Box
            m_msgBox.setText(l_S_FailCommand);
            if (m_msgBox.exec() == QMessageBox::Yes){
            } ///else
        }
    } else {
        if (m_msgBox.exec() == QMessageBox::Yes){
            qDebug() << "QuantaLoader::onCmdResultReady: ERROR";
        } ///else

    }
}

void QuantaLoader::onFwUpdateUpdateprog()
{
    int l_i_refVal = 0; // TO DO m_p_CmdThread->m_SetVarie.getCurrBlockCurrentFW();
    int l_i_currBarVal = -1;
    int l_i_tgtBarVal = -1;
    int l_i_tgtBatValTimed = -1;
    int l_i_tgtTimer = -1;
    qDebug() << "QuantaLoader::onFwUpdateUpdateprog - GetCurrentBlock " << l_i_refVal;
    if (l_i_refVal == -1){
        ui->m_prg_UpdateFW->setValue(ui->m_prg_UpdateFW->value() + 1);
        // TO DOm_UpdFwProgBarFlag = false;
    }
    else {
        /* TO DO
        if (m_UpdFwProgBarFlag == false){
            l_i_currBarVal = ui.m_prg_UpdateFW->value(); /// get the progress bar current value
            l_i_tgtBarVal = 100 - l_i_currBarVal; /// remaining time to end of update fw (in percentage)
            l_i_tgtBatValTimed = (l_i_refVal *IDS_UPDATE_INTER_DELAY); /// time needed in ms to send entire binary
            if (l_i_tgtBarVal == 0){
                /// Avoid division per zero
                l_i_tgtBarVal = 1;
            } /// else
            l_i_tgtTimer = (l_i_tgtBatValTimed / l_i_tgtBarVal) + 1;
            m_Tmr_UpdateFWProgBar.stop();
            m_Tmr_UpdateFWProgBar.start(l_i_tgtTimer);
            m_UpdFwProgBarFlag = true;
        }
        else {
            ui->m_prg_UpdateFW->setValue(ui->m_prg_UpdateFW->value() + 1);
        }
        */
    }
}

void QuantaLoader::on_m_btn_reset_clicked()
{    
    /// Prepare QS_BOOTP_RESET command

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);
    if (preSendCommand(QS_BOOTP_RESET) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else
}

void QuantaLoader::on_m_btn_startUpgrade_clicked()
{
    /// Prepare QS_BOOTP_START_FW_UP command
    /// This is a dummy command
    /// Only send the Start upgrade cmd and wait the answer

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DUMMY);


    if (preSendCommand(QS_BOOTP_START_FW_UP) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else
}

void QuantaLoader::on_m_btn_readBoot_clicked()
{
    /// Prepare QS_BOOTP_READ_BOOT command

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);

    if (preSendCommand(QS_BOOTP_READ_BOOT) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else
}

void QuantaLoader::on_m_btn_readDID_clicked()
{
    /// Prepare QS_BOOTP_READ_DEV command

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);

    if (preSendCommand(QS_BOOTP_READ_DEV) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else
}

void QuantaLoader::on_m_btn_readRID_clicked()
{
    /// Prepare QS_BOOTP_READ_REV command

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);

    if (preSendCommand(QS_BOOTP_READ_REV) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else

}

void QuantaLoader::on_m_btn_readFW_clicked()
{
    /// Prepare QS_BOOTP_READ_FW command

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);

    if (preSendCommand(QS_BOOTP_READ_FW) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else

}

void QuantaLoader::on_m_btn_EraseFlash_clicked()
{
    /// Prepare QS_BOOTP_ERASE command

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);

    /// Retrieve Bank info
    m_p_CmdThread->setBankInfoToSend(QS_BOOTP_OK,static_cast<uint8_t>( ui->m_spn_SelectBankErase->value()));

    if (preSendCommand(QS_BOOTP_ERASE) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else

}

void QuantaLoader::on_m_btn_ReadFlash_clicked()
{
    /// Prepare QS_BOOTP_READ_FLASH command

    /// Retrieve Address info and number of bytes to read
    m_p_CmdThread->setReadFlashInfoToSend(static_cast<uint16_t>(ui->m_spn_SelectStartRead->value()),
                                          static_cast<uint16_t>(ui->m_spn_ValuesToRead->value()));

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);

    if (preSendCommand(QS_BOOTP_READ_FLASH) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else

}

void QuantaLoader::on_m_btn_writeFlash_clicked()
{
    /// Prepare QS_BOOTP_WRITE_FLASH command

// TO RESTORE    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DUMMY);
    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);


    /// Retrieve Bank info
    /* TO DELETE m_p_CmdThread->setWriteFlashInfoToSend(static_cast<uint8_t>( ui->m_spn_SelectBankWrite->value()),
                                           static_cast<uint8_t>(ui->m_spn_ValueToWrite->value()));
    */
    QString l_s_WriteString = ui->m_txtStringToWrite->toPlainText();
    m_p_CmdThread->setWriteFlashInfoToSend(l_s_WriteString);


    if (preSendCommand(QS_BOOTP_WRITE_FLASH) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else
}

void QuantaLoader::on_m_btn_startFwUpgrade_clicked()
{
    /// TO DO - Prepare command
    /// Start the full FW Upgrade command sequence
    activateProgBar();

    m_p_CmdThread->setPolicyInfo(QS_BOOTP_POL_DEF);

    if (preSendCommand(QS_BOOTP_START_FW_UP) == false){
        m_msgBox.setText(QS_ERR_CMD_CURR_SEND);
        if (m_msgBox.exec() == QMessageBox::Yes){
        } ///else
    } // else

}

void QuantaLoader::on_m_btn_connect_clicked()
{
    bool l_b_RetVal = false;
    QString l_comPort = ui->m_cmb_comPort->currentText();
    QString l_S_BaudRate = ui->m_cmb_BaudRate->currentText();
    uint32_t l_baudRate = 0;

    if (m_isConnected == false){
       l_baudRate = l_S_BaudRate.toULong(&l_b_RetVal, 10);
        if (l_b_RetVal == true){
            l_b_RetVal = m_p_CmdThread->startSerialConnection(l_comPort,  l_baudRate);
        } // else
    } else {
        m_p_CmdThread->stopSerialConnection();
    }
    if (l_b_RetVal == false){
        /// Connection is not possible
        /// Emit a video message
        if (m_isConnected == false){
            m_msgBox.setText(QS_ERR_CMD_CONN_FAIL);
            if (m_msgBox.exec() == true){
                // nothing to do
            } // else
        } else {
            /// Set the connection flag to false
            m_isConnected = false;
        }
        /// Disable all the command button
        enableCmdButton(m_isConnected);
        /// Change button text
        ui->m_btn_connect->setText("CONNECT");
    } else {
        /// Set the connection flag to true
        m_isConnected = true;
        /// Enable all the command button
        enableCmdButton(m_isConnected);
        /// Change button text
        ui->m_btn_connect->setText("DISCONNECT");
    }
}

void QuantaLoader::onSendCmdTimeout(const QString &s)
{
    /// TO DO Emit a message
    /// TO DO Write in the log ??
    qDebug() << "QuantaLoader::onSendCmdTimeout Error is: " << s;
}

void QuantaLoader::onResponse(const QString &s)
{
    /// TO DO paring the message
    /// Write row in the log
    qDebug() << "QuantaLoader::onResponse. Catched data are: " << s;
    writeReadToLog(s);
}
