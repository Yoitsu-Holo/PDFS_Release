#include "logonwindow.h"
#include "ui_logonwindow.h"

LogonWindow::LogonWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogonWindow)
{
    ui->setupUi(this);
}

LogonWindow::~LogonWindow()
{
    delete ui;
}
