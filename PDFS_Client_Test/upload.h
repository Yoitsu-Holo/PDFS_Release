#ifndef UPLOAD_H
#define UPLOAD_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QFile>
#include <QTextcodec>
#include <QtAlgorithms>

#include "protocol.h"
#include "userinfo.h"
#include "transmissionmodel.h"

class Upload : public QObject
{
    Q_OBJECT
public:
    TransmissionInfo m_info;

private:
    QTcpSocket *m_uploadSocket;//上传的socket
    QFile *m_uploadFile;      //上传的文件
    qint64 m_bufSize;         //文件缓冲区 4K
    qint64 m_sentSize;        //文件大小
    qint64 m_fileSize;
    QByteArray m_outBlock;
    QString m_filePath;       //目标文件路径
    QString m_fileName;
    UserInfo *m_userInfo;

    QTimer timer;
    size_t pre;
    TransmissionData data;

public:
    Upload(QString ServerHost,int ServerPort,QFile *file,QString Path,UserInfo *User);
    ~Upload();

private:
    void GetSpeed();

public slots:
    void on_ServerConnected();
    void on_SendHeader();
    void on_ServerConnectionLose();
    void on_ServerReturned();
    void on_SendSize(qint64 sendSize);

signals:
    void ServerConnectionLose();
    void ServerConnected();
    void Error();
    void proccess(int pro);
    void done();
    void info(TransmissionData info);
};

#endif // UPLOAD_H
