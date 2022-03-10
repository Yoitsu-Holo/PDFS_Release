#include "download.h"

Download::Download(QFile *DownloadFile,QByteArray RawMsg)
{
    qDebug("RawMSG: %s",qPrintable(RawMsg.toHex()));

    m_downloadFile=DownloadFile;  //初始化文件
    m_downloadSocket=new QTcpSocket();//初始化连接
    m_downloadBytes=0;
    m_totalBytes=0;
    m_blockCnt=0;//初始化连接数量
    pre=0;

    //获取sha256
    for(int i=0;i<64;i++)
        m_sha[i]=RawMsg.at(i);
    RawMsg.remove(0,64);

    //处理总字节数
    QByteArray lenth=RawMsg.mid(0,8);
    qDebug("%s",qPrintable(lenth.toHex()));
    std::stack<char> tempSize;
    for (auto &&ele:lenth)
        m_totalBytes=(m_totalBytes<<8)+ele;

    qDebug("长度: %llu",m_totalBytes);
    RawMsg.remove(0,8);

    //处理ip:port信息
    QByteArray info;
    int pos=0;
    while(RawMsg.size())
    {
        size_t len=RawMsg.at(pos);
        info = RawMsg.mid(pos+1,pos+len);
        qDebug("mid: %d %lld %s",pos+1,len,qPrintable(info));
        m_serverTable.push_back({info.split(':').at(0),info.split(':').at(1)});
        RawMsg.remove(0,len+1);
    }

    //建立信号与槽
    connect(m_downloadSocket,&QTcpSocket::connected,this,&Download::on_ServerConnected);
    connect(m_downloadSocket,&QTcpSocket::readyRead,this,&Download::on_ServerReturned);
    connect(m_downloadSocket,&QTcpSocket::disconnected,this,&Download::on_ServerLose);
    connect(m_downloadSocket,&QTcpSocket::disconnectFromHost,this,&Download::on_DisConnect);
    ServerConnect();

    connect(&timer,&QTimer::timeout,this,&Download::GetSpeed);
    timer.start(1000);
}

Download::~Download()
{
    m_downloadFile->close();
    this->disconnect();//取消到当前对象的全部连接
    delete m_downloadFile;
    delete m_downloadSocket;

}

void Download::ServerConnect()
{
    if(!m_serverTable.empty())
    {
        m_downloadSocket->connectToHost(m_serverTable.front().first,m_serverTable.front().second.toInt());
    }
    else
    {
        GetSpeed();
        m_downloadSocket->disconnectFromHost();
        m_downloadSocket->close();
    }
}

void Download::GetSpeed()
{
    data.progress=m_downloadBytes*1000/m_totalBytes;
    data.speed=m_downloadBytes-pre;
    data.size=m_totalBytes;
    pre=m_downloadBytes;
    timer.start(500);
    emit info(data);
}

void Download::on_DisConnect()
{
    emit done();
    this->~Download();
}

void Download::on_ServerReturned()
{
    QByteArray serverReturn = m_downloadSocket->readAll();
    m_downloadBytes+=serverReturn.size();
    m_downloadFile->write(serverReturn);
    //qDebug("download : %lld / %lld",downloadBytes,totalBytes);
    emit proccess(m_downloadBytes*1000/m_totalBytes);
}

void Download::on_ServerConnected()
{
    qDebug("Connect!");
    QByteArray header;
    header.append(char(2));
    header.append(m_sha,64).append(char('-')).append(QString("%1").arg(m_blockCnt).toLatin1());
    m_blockCnt++;
    qDebug("send:: %s",qPrintable(header));
    m_downloadSocket->write(header);
}

void Download::on_ServerLose()
{
    m_serverTable.pop_front();
    this->ServerConnect();
}

