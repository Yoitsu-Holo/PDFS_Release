#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//于服务器的主连接

// QT Lib
#include <QFileDialog>
#include <QWidget>
#include <QTcpSocket>
#include <QTreeWidgetItem>
#include <QDir>
#include <QList>
#include <QMessageBox>
#include <QTextcodec>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QAction>
#include <QFileIconProvider>

// local Lib
#include "userinfo.h"          //登录用户信息
#include "mainconnect.h"       //主连接
#include "loginwindow.h"       //登录
#include "logonwindow.h"       //注册
#include "protocol.h"          //协议
#include "transmissionmodel.h" //文件信息
#include "PDFSFileSystem.cpp"   //存储的文件树
#include "download.h"
#include "upload.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
public:
private:
    Ui::MainWindow *ui;   //主UI
    QFile *m_tempFile;    //临时文件指针存放
    LoginWindow *m_login; //登录界面
    LogonWindow *m_logon; //注册界面

    //主文件树代理
    TransmissionDelegate *m_mainDelegate;
    QStandardItemModel *m_mainItemModel;
    //下载代理
    TransmissionDelegate *m_downloadDelegate;
    QStandardItemModel *m_downloadItemModel;
    //上传代理
    TransmissionDelegate *m_uploadDelegate;
    QStandardItemModel *m_uploadItemModel;

    UserInfo *m_userInfo;               //用户信息，包含用户名和密码
    MainConnect *m_server;              //服务器主连接
    PDFS_FileSystem *m_fileSystemModel; //文件系统
    QList<QString> m_downloadList;
    QList<QString> m_uploadList;
    int m_downloadCnt;
    int m_uploadCnt;
    bool m_downloading;
    bool m_uploading;
    //菜单选项
    QAction *m_downloadAction;
    QAction *m_uploadAction;
    QAction *m_refreshAction;
    QAction *m_createFolderAction;
    QAction *m_deleteFolderAction;
    QAction *m_deleteFileAction;

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void RequestFileTree(QString path);
    void BuildFileTree(QByteArray dirInfo);
    void RefreshMainList();
    void RefreshDownloadList(TransmissionData pro);
    void RefreshUploadList(TransmissionData pro);

private:
    void __LoginWindowLogin(QString username, QString password);
    void __LoginWindowLogon();
    void __LoginWindowClose();
    void __LogonWindowLogon(QString username, QString password);
    void __LogonWindowClose();
    void __NextDownload();
    void __NextUpload();

public slots:
    void on_ServerReturned();

signals:
    void em_LoginTextChange(QString str);
    void em_LogonTextChange(QString str);

private slots:
    void on_fileList_doubleClicked(const QModelIndex &index);
    void on_menu1_downloadRequest();
    void on_menu1_uploadRequest();
    void on_menu1_createFolderRequest();
    void on_menu1_deleteFileRequest();
    void on_menu1_refrashRequest();
};

#endif // MAINWINDOW_H
