#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>
#include <QFile>
#include <QTcpSocket>
#include <QQueue>
#include <QTimer>

#include <cstdio>
#include <stack>

#include "protocol.h"
#include "transmissionmodel.h"

class Download : public QObject
{
    Q_OBJECT
public:
    TransmissionInfo m_info;

private:
    size_t m_totalBytes;
    size_t m_downloadBytes;
    size_t m_blockCnt;
    char m_sha[64];
    QFile *m_downloadFile;
    QTcpSocket *m_downloadSocket;
    QList<QPair<QString,QString>> m_serverTable;

    QTimer timer;
    size_t pre;
    TransmissionData data;

public:
    Download(QFile * DownloadFile,QByteArray RawMsg);
    ~Download();

private:
    void ServerConnect();
    void GetSpeed();

public slots:
    void on_DisConnect();
    void on_ServerConnected();
    void on_ServerReturned();
    void on_ServerLose();

signals:
    void proccess(int pro);
    void done();
    void info(TransmissionData info);
};

#endif // DOWNLOAD_H
