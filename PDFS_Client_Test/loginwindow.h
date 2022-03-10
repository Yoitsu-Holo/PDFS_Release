#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QFile>

//控制无边框的头文件
#include <windows.h>
#include <windowsx.h>
#include <QMouseEvent>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT
    friend class MainWindow;

public:


private:
    Ui::LoginWindow *ui;
    int m_boundaryWidth;
    QPoint m_clickPos;

public:
    LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
    void initUI();//初始化UI界面

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);

private slots:
    void on_logonButton_clicked();
    void on_closeButton_clicked();
    void on_login_success();
    void on_loginButton_clicked();

signals:
    void em_buttonLoginClicked(QString username,QString password);
    void em_buttonLogonClicked();
    void em_buttonCloseClicked();
};

#endif // LOGINWINDOW_H
