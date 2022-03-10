#include "mainconnect.h"

MainConnect::MainConnect(QObject *parent)
    : QObject{parent}
{
    //指针初始化
    m_tcpClient=new QTcpSocket();
    m_serverReturnedMsg = new QByteArray();

    //信号槽连接
    connect(m_tcpClient,&QTcpSocket::connected,this,&MainConnect::on_Connected);
    connect(m_tcpClient,&QTcpSocket::disconnected,this,&MainConnect::on_Disconnected);
    connect(m_tcpClient,&QTcpSocket::readyRead,this,&MainConnect::on_SocketReadyRead);
}

MainConnect::~MainConnect()
{
    delete m_tcpClient;
    delete m_serverReturnedMsg;
}

void MainConnect::ConnectServer(QString host,unsigned short port)
{
    m_tcpClient->connectToHost(host,port);
}

void MainConnect::SendMsg(QByteArray msg)
{
    m_tcpClient->write(msg);
}

void MainConnect::SendMsg(QByteArray msg,size_t len)
{
    m_tcpClient->write(msg,len);
}

QByteArray MainConnect::GetServerMsg()
{
    return *m_serverReturnedMsg;
}

void MainConnect::Close()
{
    m_tcpClient->disconnectFromHost();
    m_tcpClient->close();
}

void MainConnect::on_Connected()
{
    emit em_ServerReady();
}

void MainConnect::on_Disconnected()
{
    emit em_ServerLose();
}

void MainConnect::on_SocketReadyRead()
{
    m_serverReturnedMsg->clear();
    *m_serverReturnedMsg = m_tcpClient->readAll();
    emit em_ServerReturned();
}
