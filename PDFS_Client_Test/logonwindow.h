#ifndef LOGONWINDOW_H
#define LOGONWINDOW_H

#include <QWidget>

namespace Ui {
class LogonWindow;
}

class LogonWindow : public QWidget
{
    Q_OBJECT
    friend class MainWindow;

public:
    explicit LogonWindow(QWidget *parent = nullptr);
    ~LogonWindow();

private:
    Ui::LogonWindow *ui;
};

#endif // LOGONWINDOW_H
