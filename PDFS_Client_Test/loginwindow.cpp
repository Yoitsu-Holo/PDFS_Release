#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    //ui->loginState->setVisible(false);
    initUI();
    ui->passEdit->setText("YoitsuHolo1");
    ui->userEdit->setText("YoitsuHolo1");
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_logonButton_clicked()
{
    emit em_buttonLogonClicked();
}

void LoginWindow::on_closeButton_clicked()
{
    emit em_buttonCloseClicked();
}

void LoginWindow::on_loginButton_clicked()
{
    emit em_buttonLoginClicked(ui->userEdit->text(),ui->passEdit->text());
}

void LoginWindow::on_login_success()
{
    this->~LoginWindow();
}

void LoginWindow::initUI()
{
    //设置窗口大小
    this->setMinimumSize(250,130);
    this->setMaximumSize(250,130);

    this->ui->userEdit->setMaxLength(20);
    this->ui->passEdit->setMaxLength(20);
    this->ui->passEdit->setEchoMode(QLineEdit::Password);

    this->setWindowFlag(Qt::FramelessWindowHint);//设置无边框

    QString qssPath;
    QFile *styleSheet;

    qssPath=":/resources/qss/MyLoginPushButton.qss";
    styleSheet=new QFile(qssPath);
    if(styleSheet->open(QIODevice::ReadOnly))
    {
        QString str=QString::fromLocal8Bit(styleSheet->readAll());
        this->ui->loginButton->setStyleSheet(str);
        this->ui->logonButton->setStyleSheet(str);
        styleSheet->close();
    }

    qssPath=":/resources/qss/MyLoginCloseButton.qss";
    styleSheet=new QFile(qssPath);
    if(styleSheet->open(QIODevice::ReadOnly))
    {
        QString str=QString::fromLocal8Bit(styleSheet->readAll());
        this->ui->closeButton->setStyleSheet(str);
        styleSheet->close();
    }
}

//控制无边框
bool LoginWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = (MSG*)message;
    switch(msg->message)
    {
    case WM_NCHITTEST:
        int xPos = GET_X_LPARAM(msg->lParam) - this->frameGeometry().x();
        int yPos = GET_Y_LPARAM(msg->lParam) - this->frameGeometry().y();
        if(xPos < m_boundaryWidth && yPos<m_boundaryWidth)                    //左上角
            *result = HTTOPLEFT;
        else if(xPos>=width()-m_boundaryWidth&&yPos<m_boundaryWidth)          //右上角
            *result = HTTOPRIGHT;
        else if(xPos<m_boundaryWidth&&yPos>=height()-m_boundaryWidth)         //左下角
            *result = HTBOTTOMLEFT;
        else if(xPos>=width()-m_boundaryWidth&&yPos>=height()-m_boundaryWidth)//右下角
            *result = HTBOTTOMRIGHT;
        else if(xPos < m_boundaryWidth)                                     //左边
            *result =  HTLEFT;
        else if(xPos>=width()-m_boundaryWidth)                              //右边
            *result = HTRIGHT;
        else if(yPos<m_boundaryWidth)                                       //上边
            *result = HTTOP;
        else if(yPos>=height()-m_boundaryWidth)                             //下边
            *result = HTBOTTOM;
        else              //其他部分不做处理，返回false，留给其他事件处理器处理
            return false;
        return true;
    }
    return false;         //此处返回false，留给其他事件处理器处理
}

void LoginWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button()==Qt::LeftButton)
        m_clickPos=e->pos();
}

void LoginWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons()&Qt::LeftButton)
        move(e->pos()+pos()-m_clickPos);
}
