#ifndef QUANTALOADER_H
#define QUANTALOADER_H

#include <QMainWindow>
#include "qs_guidefine.h"

#include "QDebug"


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

private:
    Ui::QuantaLoader *ui;
    QS_PanelSection m_guiSection;

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
};

#endif // QUANTALOADER_H
