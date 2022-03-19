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
    QProgressBar *l_WidgetTemp = nullptr;
    QString l_S_danger = "QProgressBar::chunk {background: red;border-bottom-right-radius: 5px;border-bottom-left-radius: 5px;border: .px solid black;}";
    QTimer *l_Tmr = nullptr;
    QString l_S_FailCommand = "";
    QString l_S_DoneCommand = "";

    switch (static_cast<int>(getGuiSection()))
    {
    case QS_FW_UPGRADE_SECTION:
        l_WidgetTemp = ui->m_prg_UpdateFW;
        l_Tmr = &m_Tmr_UpdateFWProgBar;
        /*
        if (m_FWUpdatePanelState == IDS_UPDATE_PANEL_ACTIVE){
            /// In active only he GET command is allowed
            m_msgBox.setText(IDS_MSG_GET_FW_VER_FAIL);
            l_S_FailCommand = IDS_MSG_GET_FW_VER_FAIL;
            l_S_DoneCommand = IDS_MSG_GET_FW_VER_DONE;
        }
        else {
            if (_res == false){
                m_msgBox.setText(IDS_MSG_GET_FW_VER_FAIL);
            }
            else {
                m_msgBox.setText(IDS_MSG_UPDATE_OK);
            }
            l_S_FailCommand = IDS_MSG_UPDATE_FW_FAIL;
            l_S_DoneCommand = IDS_MSG_UPDATE_FW_DONE;
            l_Tmr = &m_Tmr_UpdateFWProgBar;
        }
        */
        break;
    case QS_CMD_PANEL_SECTION:
            // m_msgBox.setText(IDS_MSG_CHANGE_BAND_DONE);
        break;
    default:
        l_bo_Error = true;
        qDebug() << "QuantaLoader::onCmdResultReady - Not Plausible branch";
        break;
    }

    if (l_bo_Error == false){
        if (l_WidgetTemp != nullptr){
            /// it is a command connected with the progress bar?
            if (l_WidgetTemp->isVisible() == true){
                /// yes it is a command connected with prog bar
                l_WidgetTemp->setValue(l_WidgetTemp->maximum());
                if (_res == true){
                    qDebug() << "QuantaLoader::onCmdResultReady: " <<  l_S_DoneCommand;
                    if (m_msgBox.exec() == QMessageBox::Yes){
                    } ///else
                }
                else {
                    qDebug() << "QuantaLoader::onCmdResultReady: " << l_S_FailCommand;
                    l_WidgetTemp->setStyleSheet(l_S_danger);
                    /// Display a QMessage Box
                    if (m_msgBox.exec() == QMessageBox::Yes){
                    } ///else
                }

                QThread::msleep(3000);
                if (l_Tmr != nullptr){
                    /// In case of command connected to prog bar
                    l_Tmr->stop();
                } /// else
                l_WidgetTemp->setVisible(false);
                l_WidgetTemp->setValue(0);

            }
            else {
                /// It is a command not related to the progress bar
                if (_res == true){
                    qDebug() << "RadarGui_000::onCmdResultReady: " << l_S_DoneCommand;
                }
                else {
                    qDebug() << "RadarGui_000::onCmdResultReady: " << l_S_FailCommand;
                    /// Display a QMessage Box
                    if (m_msgBox.exec() == QMessageBox::Yes){
                    } ///else
                }
            }
        }
        else {
            qDebug() << "QuantaLoader::onCmdResultReady: ???????";
        }
    } /// else
}

void QuantaLoader::onFwUpdateUpdateprog()
{
    int l_i_refVal = 0; // TO DO m_p_CmdThread->m_SetVarie.getCurrBlockCurrentFW();
    int l_i_currBarVal = -1;
    int l_i_tgtBarVal = -1;
    int l_i_tgtBatValTimed = -1;
    int l_i_tgtTimer = -1;
    qDebug() << "GetCurrentBlock " << l_i_refVal;
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


void QuantaLoader::initProgressBar()
{
    ui->m_prg_UpdateFW->setVisible(false);
    ui->m_prg_UpdateFW->setValue(0);
}

