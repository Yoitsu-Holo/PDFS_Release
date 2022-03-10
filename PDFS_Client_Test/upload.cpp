#include "upload.h"

Upload::Upload(QString ServerHost,int ServerPort,QFile *file,QString Path,UserInfo *User)
{
    m_uploadSocket=new QTcpSocket();
    m_uploadSocket->connectToHost(ServerHost,ServerPort);
    m_uploadFile=file;
    m_fileName=m_uploadFile->fileName().split('/').back();
    m_filePath=Path+m_fileName;
    m_userInfo=User;
    m_bufSize=64*1024;
    pre=0;

    //建立型号与槽
    connect(m_uploadSocket,&QTcpSocket::connected,this,&Upload::on_ServerConnected);
    connect(m_uploadSocket,&QTcpSocket::disconnected,this,&Upload::on_ServerConnectionLose);
    connect(this,&Upload::ServerConnected,this,&Upload::on_SendHeader);
    connect(m_uploadSocket,&QTcpSocket::readyRead,this,&Upload::on_ServerReturned);

    connect(&timer,&QTimer::timeout,this,&Upload::GetSpeed);
    timer.start(1000);
}

Upload::~Upload()
{
    m_uploadFile->close();
    m_uploadSocket->disconnectFromHost();
    m_uploadSocket->close();
    this->disconnect();
    delete m_uploadSocket;
    delete m_uploadFile;
    delete m_userInfo;
}

void Upload::GetSpeed()
{
    data.progress=m_sentSize*1000/m_fileSize;
    data.speed=m_sentSize-pre;
    data.size=m_fileSize;
    pre=m_sentSize;
    timer.start(500);
    emit info(data);
}

void Upload::on_ServerConnected()
{
    emit this->ServerConnected();
}

void Upload::on_ServerConnectionLose()
{
    emit this->ServerConnectionLose();
}

void Upload::on_SendHeader()
{
    QByteArray header;
    header.append(opCode_SendFile);
    header.append(m_userInfo->key);
    header.append(m_filePath.toLatin1());
    m_uploadSocket->write(header);
}

void Upload::on_SendSize(qint64 sentSize)
{
    m_sentSize-=sentSize;
    emit proccess((int)((m_fileSize-m_sentSize)*1000/m_fileSize));
//    qDebug()<<"sned:"<<sentSize <<" | last: "<<m_sentSize;
    if(m_sentSize)
    {
        //发送文件
        m_outBlock=m_uploadFile->read(qMin(m_sentSize,m_bufSize));
        m_uploadSocket->write(m_outBlock);
        m_outBlock.resize(0);
    }
    else //文件发送结束
    {
        emit this->done();
        this->~Upload();
    }
}

void Upload::on_ServerReturned()
{
    QByteArray serverReturn=m_uploadSocket->readAll();
    qDebug()<<"server return : "<<serverReturn.toHex();
    if(serverReturn[0]==char(255))
    {
        m_sentSize=m_fileSize=m_uploadFile->size();
        connect(m_uploadSocket,&QTcpSocket::bytesWritten,this,&Upload::on_SendSize);
        QDataStream sendOut(&m_outBlock,QIODevice::WriteOnly);
        sendOut.setVersion(QDataStream::Qt_6_2);
        on_SendSize(0);
    }
}
