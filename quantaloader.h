#ifndef QUANTALOADER_H
#define QUANTALOADER_H

#include <QMainWindow>

namespace Ui {
class QuantaLoader;
}

class QuantaLoader : public QMainWindow
{
    Q_OBJECT

public:
    explicit QuantaLoader(QWidget *parent = nullptr);
    ~QuantaLoader();

private:
    Ui::QuantaLoader *ui;
};

#endif // QUANTALOADER_H
