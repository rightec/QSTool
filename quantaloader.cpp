#include "quantaloader.h"
#include "ui_quantaloader.h"

QuantaLoader::QuantaLoader(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QuantaLoader)
{
    ui->setupUi(this);
}

QuantaLoader::~QuantaLoader()
{
    delete ui;
}
