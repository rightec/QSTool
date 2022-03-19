#ifndef QUANTALOADER_H
#define QUANTALOADER_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QProgressBar>
#include <QMessageBox>
#include <QTimer>

#include "qs_guidefine.h"
#include "qs_cmdthread.h"


namespace Ui {
class QuantaLoader;
}

class QuantaLoader : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuantaLoader(QWidget *parent = nullptr);
    ~QuantaLoader();

private slots:
    void on_m_btnChoosePanel_clicked();

    void on_m_btn_clearUpgradeLog_clicked();

    void on_m_btn_ClearCmdPanelLog_clicked();

    void on_m_bnt_openDialog_clicked();

private:
    Ui::QuantaLoader    *ui;
    QS_PanelSection     m_guiSection;
    QRect               m_RefRect;
    QMessageBox         m_msgBox;
    QTimer              m_Tmr_UpdateFWProgBar;

    QS_CmdThread        *m_p_CmdThread;
    QThread             m_workerThread;

    /*!
     * \brief setGuiSection: Change the current section of the GUI
     * \param _section: Target section
     */
    void setGuiSection(QS_PanelSection _section);

    /*!
     * \brief getGuiSection: Get the current value of the section of the GUI
     * \return
     */
    QS_PanelSection getGuiSection() {return  m_guiSection;}

    /*!
     * \brief displayCmdSection: Display command section of the GUI
     */
    void displayCmdSection();

    /*!
     * \brief displayFwUpgradeSection: Display FW Upgrade Section of the GUI
     */
    void displayFwUpgradeSection();

    /*!
     * \brief initProgressBar
     */
    void initProgressBar();

private slots:
    void onCmdResultReady(bool _res);
    void onFwUpdateUpdateprog();

signals:
    void operate(int _operate);
};

#endif // QUANTALOADER_H
