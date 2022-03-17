#include "quantaloader.h"
#include "ui_quantaloader.h"

QuantaLoader::QuantaLoader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuantaLoader)
{
    ui->setupUi(this);

    /*! Build the Header Frame*/
    QPixmap l_logo_pic;
    l_logo_pic.load( QS_HEADER_RES);
    ui->m_lbl_ImageHead->setPixmap(l_logo_pic);     // Place the pixmap in the label
    ui->m_lbl_tool->setText( QS_HEADER_BOOT);

    /*! Select the defualt section: Upgrade section*/
    setGuiSection(QS_CMD_PANEL_SECTION);
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
