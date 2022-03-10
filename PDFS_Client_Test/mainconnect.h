#ifndef MAINCONNECT_H
#define MAINCONNECT_H

#include <QObject>
#include <QString>
#include <QHostInfo>
#include <QTcpSocket>

class MainConnect : public QObject
{
    Q_OBJECT
private:
    QTcpSocket *m_tcpClient;
    QByteArray *m_serverReturnedMsg;

public:
    MainConnect(QObject *parent=nullptr);
    ~MainConnect();
    void ConnectServer(QString host,unsigned short port);
    void SendMsg(QByteArray msg);
    void SendMsg(QByteArray msg,size_t len);
    void Close();
    QByteArray GetServerMsg();

public slots:
    void on_Connected();
    void on_Disconnected();
    void on_SocketReadyRead();

signals:
    void em_ServerReady();
    void em_ServerLose();
    void em_ServerReturned();
};

#endif // MAINCONNECT_H
