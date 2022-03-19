#include "quantaloader.h"
#include "ui_quantaloader.h"

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
    l_logo_pic.load( QS_HEADER_RES);
    ui->m_lbl_ImageHead->setPixmap(l_logo_pic);     // Place the pixmap in the label
    ui->m_lbl_tool->setText( QS_HEADER_BOOT);

    /*! Select the defualt section: Upgrade section*/
    setGuiSection(QS_FW_UPGRADE_SECTION);

    /*! Setting boundary values*/
    ui->m_spn_SelectBankWrite->setMaximum(QS_BOOTP_MAX_BAK_NUM);
    ui->m_spn_SelectBankErase->setMaximum(QS_BOOTP_MAX_BAK_NUM);
    ui->m_spn_SelectStartRead->setMaximum(QS_BOOTP_FLASH_MEM_SIZE -1); // Flash size - 1 bytes

    /*! Initialize widgets*/
    initProgressBar();
    connect(&m_Tmr_UpdateFWProgBar, SIGNAL(timeout()), this, SLOT(onFwUpdateUpdateprog()));

    /// Init MessageBox
    m_msgBox.setWindowTitle(QS_APPLICATION_NAME);
    m_msgBox.setStandardButtons(QMessageBox::Yes);

    /// message box Dialog dimension
    QSpacerItem* l_horizontalSpacer = new QSpacerItem(500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* l_layout = static_cast<QGridLayout*>(m_msgBox.layout());
    l_layout->addItem(l_horizontalSpacer, l_layout->rowCount(), 0, 1, l_layout->columnCount());



    /*! Thread section starts*/
    m_p_CmdThread = new QS_CmdThread(nullptr);
    m_p_CmdThread->moveToThread(&m_workerThread);
    connect(&m_workerThread, &QThread::finished, m_p_CmdThread, &QObject::deleteLater);
    connect(this, SIGNAL(operate(int)), m_p_CmdThread, SLOT(onRunCommand(int)));
    connect(m_p_CmdThread, SIGNAL(cmdResultReady(bool)), this, SLOT(onCmdResultReady(bool)));
    m_workerThread.start();
    // Thread section ends

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
    /// TO DO - Prepare command
    emit operate(QS_BOOTP_RESET);
}

void QuantaLoader::on_m_btn_startUpgrade_clicked()
{
    /// TO DO - Prepare command
    /// Only send the Start upgrade cmd and wait the answer
    emit operate(QS_BOOTP_DUMMY_START_UP);
}

void QuantaLoader::on_m_btn_readBoot_clicked()
{
    /// TO DO - Prepare command
    emit operate(QS_BOOTP_READ_BOOT);
}

void QuantaLoader::on_m_btn_readDID_clicked()
{
    /// TO DO - Prepare command
    emit operate(QS_BOOTP_READ_DEV);
}

void QuantaLoader::on_m_btn_readRID_clicked()
{
    /// TO DO - Prepare command
    emit operate(QS_BOOTP_READ_REV);
}

void QuantaLoader::on_m_btn_readFW_clicked()
{
    /// TO DO - Prepare command
    emit operate(QS_BOOTP_READ_FW);
}

void QuantaLoader::on_m_btn_EraseFlash_clicked()
{
    /// TO DO - Prepare command
    /// Retrieve Bank info
    emit operate(QS_BOOTP_ERASE);
}

void QuantaLoader::on_m_btn_ReadFlash_clicked()
{
    /// TO DO - Prepare command
    /// Retrieve Address info and number of bytes to read
    emit operate(QS_BOOTP_READ_FLASH);
}

void QuantaLoader::on_m_btn_writeFlash_clicked()
{
    /// TO DO - Prepare command
    /// Retrieve Bank info and item to write
    emit operate(QS_BOOTP_WRITE_FLASH);

}

void QuantaLoader::on_m_btn_startFwUpgrade_clicked()
{
    /// TO DO - Prepare command
    /// Start the full FW Upgrade command sequence
    activateProgBar();
    emit operate(QS_BOOTP_START_FW_UP);
}
