#ifndef QUANTALOADER_H
#define QUANTALOADER_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include <QProgressBar>
#include <QMessageBox>
#include <QTimer>
#include <QSerialPortInfo>
#include <QTextEdit>

#include "qs_guidefine.h"
#include "qs_cmdthread.h"
#include "qs_bootprotocol.h"


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
    /*!
     * \brief onResponse: Slot catching the answer from serial
     * \param s
     */
 //   void onResponse(const QString &s);
    void onResponse(int _size);
    /*!
     * \brief onSendCmdTimeout. Timeout failure when sending command
     * \param s: Error string
     */
    void onSendCmdTimeout(const QString &s);

    void on_m_btnChoosePanel_clicked();

    void on_m_btn_clearUpgradeLog_clicked();

    void on_m_btn_ClearCmdPanelLog_clicked();

    void on_m_bnt_openDialog_clicked();

private:
    Ui::QuantaLoader    *ui;
    QRect               m_RefRect;
    QMessageBox         m_msgBox;
    QTimer              m_Tmr_UpdateFWProgBar;
    QThread             m_workerThread;
    QString             m_StringFromHex;

    QS_CmdThread        *m_p_CmdThread;
    QS_PanelSection     m_guiSection;
    QS_UpgradeState     m_upgradeState;
    QS_BootProtocol     *m_p_UpgradeThread;


    bool                m_isConnected;

    /*!
     * \brief s/getUpgradeState: Managing the upgrade state variable
     * \param _state
     */
    void setUpgradeState (QS_UpgradeState _state){m_upgradeState = _state;}
    QS_UpgradeState getUpgradeState (){return m_upgradeState;}

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

    /*!
     * \brief activateProgBar
     */
    void activateProgBar(bool _enable);

    /*!
     * \brief preSendCommand. Prepare the command to be sent to the low layer
     * \return: true if the command has correctly send to the low layer
     */
    bool preSendCommand(int _cmdId);

    /*!
     * \brief writeSendToLog: write current send command to serial log
     */
    void writeSendToLog(QTextEdit * _refTextEdit);

    /*!
     * \brief writeReadToLog: Write the command answer to the log
     * \param _size
     */
    void writeReadToLog(int _size, QTextEdit* _refTextEdit, bool _enableParse);

    /*!
     * \brief enableCmdButton: Enable/Disable all the command buttons on both panels
     * \param _enable
     */
    void enableCmdButton(bool _enable);

    /*!
     * \brief setLogColorByLevel: Method used to differentiate color in the log
     * \param _refTextEdit
     * \param _errLevel
     * \param _textToWrite
     */
    void setLogColorByLevel(QTextEdit *_refTextEdit, QS_SignalLevel _errLevel, QString _textToWrite);

    /*!
     * \brief startUpgradeProcedure
     * \return true if the upgrade procedure is started
     */
    bool startUpgradeProcedure();

    /*!
     * \brief upgradeFwRun: Managing all teh upgrade procedure
     */
    void upgradeFwRun();

    /*!
     * \brief parsePayload
     */
    void parsePayload();

    /*!
     * \brief parseFwVersion
     * \return
     */
    QString parseFwVersion();

    /*!
     * \brief parseRegId
     * \return
     */
    QString parseRegId();

private slots:
    void onWriteStringToSend(QString _stringToSend);

    void onCmdResultReady(bool _res);

    void onFwUpdateUpdateprog();

    void onUpdateProcCmdSent(int _idCmd);

    void onRxErrorCatch(int _err);

    void on_m_btn_reset_clicked();

    void on_m_btn_startUpgrade_clicked();

    void on_m_btn_readBoot_clicked();

    void on_m_btn_readDID_clicked();

    void on_m_btn_readRID_clicked();

    void on_m_btn_readFW_clicked();

    void on_m_btn_EraseFlash_clicked();

    void on_m_btn_ReadFlash_clicked();

    void on_m_btn_writeFlash_clicked();

    void on_m_btn_startFwUpgrade_clicked();

    void on_m_btn_connect_clicked();

signals:
    void operate(int _operate);
};

#endif // QUANTALOADER_H
