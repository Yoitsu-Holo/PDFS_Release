#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //    QFile windowsIcon(":/resources/icons/database.svg");
    //    QIcon icon(windowIcon);
    //    this->setWindowIcon();

    QFileInfo file_info("asd.cpp");
    QFileIconProvider icon_provider;
    QIcon icon = icon_provider.icon(file_info);
    this->setWindowIcon(icon);

    //变量初始化
    m_userInfo = new UserInfo();
    m_server = new MainConnect();
    m_fileSystemModel = new PDFS_FileSystem();

    m_mainItemModel = nullptr;
    m_downloadItemModel = nullptr;
    m_uploadItemModel = nullptr;
    m_downloadCnt = 0;
    m_uploadCnt = 0;
    m_downloading=false;
    m_uploading=false;

    //读取文件，连接服务器
    QFile serverInfo(":/resources/server/serverInfo.ini");
    serverInfo.open(QIODevice::ReadOnly);
    QByteArray fileInfo = serverInfo.readAll();
    QByteArrayList hostInfo = fileInfo.split(':');
    qDebug("%s:%s", qPrintable(hostInfo.at(0)), qPrintable(hostInfo.at(1)));
    m_server = new MainConnect();
    m_server->ConnectServer(hostInfo.at(0), hostInfo.at(1).toInt());

    connect(m_server, &MainConnect::em_ServerReturned, this, &MainWindow::on_ServerReturned);

    //登录界面
    m_login = new LoginWindow();

    connect(m_server, &MainConnect::em_ServerReady, m_login, &LoginWindow::show);
    connect(m_login, &LoginWindow::em_buttonLoginClicked, this, &MainWindow::__LoginWindowLogin);
    connect(m_login, &LoginWindow::em_buttonLogonClicked, this, &MainWindow::__LoginWindowLogon);
    connect(m_login, &LoginWindow::em_buttonCloseClicked, this, &MainWindow::__LoginWindowClose);

    //设置模型和代理
    m_mainDelegate = new TransmissionDelegate(this);
    ui->fileList->setItemDelegate(m_mainDelegate); //为视图设置委托
    ui->fileList->setDragEnabled(false);       //控件不允许拖动
    ui->fileList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->fileList->setSelectionMode(QAbstractItemView::MultiSelection);

    m_downloadDelegate = new TransmissionDelegate(this);
    ui->downloadList->setItemDelegate(m_downloadDelegate); //为视图设置委托
    ui->downloadList->setDragEnabled(false);       //控件不允许拖动
    ui->downloadList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->downloadList->setSelectionMode(QAbstractItemView::MultiSelection);

    m_uploadDelegate = new TransmissionDelegate(this);
    ui->uploadList->setItemDelegate(m_uploadDelegate); //为视图设置委托
    ui->uploadList->setDragEnabled(false);       //控件不允许拖动
    ui->uploadList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->uploadList->setSelectionMode(QAbstractItemView::MultiSelection);

    //设置主界面右键菜单
    ui->fileList->setContextMenuPolicy(Qt::ActionsContextMenu);
    ui->fileList->addAction(ui->action_download);
    ui->fileList->addAction(ui->action_upload);
    ui->fileList->addAction(ui->action_createFolder);
    ui->fileList->addAction(ui->action_deleteFile);
    ui->fileList->addAction(ui->action_refrash);
    connect(ui->action_download,&QAction::triggered,this,&MainWindow::on_menu1_downloadRequest);
    connect(ui->action_upload,&QAction::triggered,this,&MainWindow::on_menu1_uploadRequest);
    connect(ui->action_createFolder,&QAction::triggered,this,&MainWindow::on_menu1_createFolderRequest);
    connect(ui->action_deleteFile,&QAction::triggered,this,&MainWindow::on_menu1_deleteFileRequest);
    connect(ui->action_refrash,&QAction::triggered,this,&MainWindow::on_menu1_refrashRequest);
}

MainWindow::~MainWindow()
{
    //    delete m_tempFile;
    //    delete m_login;
    //    delete m_logon;
    //    delete ui;
    //    delete m_itemModel;
    //    delete userInfo;
}

void MainWindow::__LoginWindowLogin(QString username, QString password)
{
    m_userInfo->username = username;
    m_userInfo->password = password;
    m_userInfo->username.resize(20, QChar(0));
    m_userInfo->password.resize(20, QChar(0));
    QByteArray msg;
    msg.append(opCode_Login)
            .append(m_userInfo->username.toLatin1())
            .append(m_userInfo->password.toLatin1());
    m_server->SendMsg(msg);
}

void MainWindow::__LoginWindowLogon()
{
}

void MainWindow::__LoginWindowClose()
{
    delete m_login;
    // this->~MainWindow();
    this->show();
}

void MainWindow::__NextDownload()
{
    if(m_downloadList.size()>m_downloadCnt && !m_downloading)
    {
        qDebug("download cnt : %d",m_downloadCnt);
        QByteArray msg;
        msg.append(opCode_DownloadFile).append(m_userInfo->key).append(m_downloadList.at(m_downloadCnt).toLatin1());
        m_server->SendMsg(msg);
        m_downloading=true;
    }
}

//服务器相应
void MainWindow::on_ServerReturned()
{
    //简单处理服务器返回的数据
    QByteArray serverReturn = m_server->GetServerMsg();
    qDebug("return : %s", qPrintable(serverReturn.toHex()));
    int serverCode = (unsigned int)serverReturn.front();
    serverReturn.remove(0, 1);

    //对于返回码判断
    if(serverCode==stCode_Undefined) //未定义操作
    {
        QMessageBox::information(this, "Title", QString("未知错误"));
    }
    else if(serverCode==stCode_LoginSucceed) //登录成功,删除登录ui,启动主ui
    {
        m_userInfo->key = serverReturn;
        RequestFileTree("/");
        delete m_login;
        this->show();
    }
    else if(serverCode==stCode_LoginPWDError) //登录密码错误
    {
        QMessageBox::warning(this, "Title", QString("密码错误"));
    }
    else if(serverCode==stCode_CreateUserSucceed) //创建用户成功
    {}
    else if(serverCode==stCode_UserExist) //用户已存在
    {
        QMessageBox::warning(this, "Title", QString("用户已存在"));
    }
    else if(serverCode==stCode_DeleteUserSucceed) //删除用户成功
    {}
    else if(serverCode==stCode_DeleteUserPWDFail) //删除用户时验证密码失败
    {
        QMessageBox::warning(this, "Title", QString("密码错误"));
    }
    else if(serverCode==stCode_GetFileTreeSucceed) //获取文件树成功
    {
        qDebug("return msg : %s", qPrintable(serverReturn.toHex()));
        BuildFileTree(serverReturn);
        RefreshMainList();
    }
    else if(serverCode==stCode_DownloadReturn) //下载请求返回成功
    {
        qDebug("return msg : %s", qPrintable(serverReturn.toHex()));
        m_tempFile = new QFile("D:/code/"+m_downloadList.at(m_downloadCnt));
        if(!m_tempFile->exists())
            m_tempFile->open(QIODevice::ReadWrite);
        m_tempFile->open(QIODevice::ReadWrite);
        Download *down= new Download(m_tempFile,serverReturn);
        connect(down,&Download::info,this,&MainWindow::RefreshDownloadList);
        connect(down,&Download::done,[this]{ m_downloading=false; m_downloadCnt++; __NextDownload(); });
    }
    else if(serverCode==stCode_FileUnexist) //文件不存在
    {

    }
    else if(serverCode==stCode_UploadSucceed) //上传成功
    {

    }
    else if(serverCode==stCode_DeleteFileSucceed)
    {
        QMessageBox::warning(this, "Title", QString("删除文件成功"));
    }
    else if(serverCode==stCode_PathCreateSucceed)
    {
        QMessageBox::warning(this, "Title", QString("创建文件夹成功"));
    }
    else
    {
        QMessageBox::information(this, "Title", QString("未定义操作：") + QString::number(serverCode));
    }
    RefreshMainList();
}

//获取文件树
void MainWindow::RequestFileTree(QString path)
{
    QByteArray msg;
    msg.append(opCode_RequestDir).append(m_userInfo->key).append(path.toLatin1());
    m_server->SendMsg(msg);
}

//解析文件树
void MainWindow::BuildFileTree(QByteArray dirInfo)
{
    //qDebug("build Tree : %s", qPrintable(dirInfo.toHex()));
    int num = dirInfo.front();
    int pos = 1;
    QTextCodec *coding = QTextCodec::codecForName("UTF8");
    for (int i = 1; i <= num; i++)
    {
        int FileType = dirInfo[pos++];
        int FileNameLen = dirInfo[pos++];
        QString FileName;
        FileName = coding->toUnicode(dirInfo.mid(pos, FileNameLen));
        pos += FileNameLen;
        switch (FileType)
        {
        case 1:
            m_fileSystemModel->addFile(FileName, 0);
            break;
        case 2:
            m_fileSystemModel->addDir(FileName);
            break;
        }
    }
}

//刷新文件树
void MainWindow::RefreshMainList()
{
    std::vector fileTree = m_fileSystemModel->getFileTree();
    if(m_mainItemModel!=nullptr)
        delete m_mainItemModel;
    m_mainItemModel = new QStandardItemModel();
    for (auto &&Ele : fileTree)
    {
        QStandardItem *Item = new QStandardItem;
        Item->setData(QVariant::fromValue(Ele), Qt::UserRole + 1); //整体存取
        m_mainItemModel->appendRow(Item); //追加Item
    }
    ui->fileList->setModel(m_mainItemModel);
}

//刷新下载列表
void MainWindow::RefreshDownloadList(TransmissionData pro=TransmissionData())
{
    if(m_downloadItemModel!=nullptr)
        delete m_downloadItemModel;
    m_downloadItemModel = new QStandardItemModel();
    int cnt=0;
    for (auto &&Ele : m_downloadList)
    {
        QStandardItem *Item = new QStandardItem;
        QFileInfo downloadFileInfo(Ele);
        TransmissionInfo downloadInfo={downloadFileInfo.fileName(),downloadFileInfo.suffix(),0,0,0,0,1,1,1,1,1};
        if(cnt<m_downloadCnt)
            downloadInfo.progress=1000;
        else if(cnt==m_downloadCnt)
        {
            downloadInfo.progress=pro.progress;
            downloadInfo.speed=pro.speed;
            downloadInfo.fileSize=pro.size;
        }
        Item->setData(QVariant::fromValue(downloadInfo), Qt::UserRole + 1); //整体存取
        m_downloadItemModel->appendRow(Item); //追加Item
        cnt++;
    }
    ui->downloadList->setModel(m_downloadItemModel);
    ui->fileList->clearSelection();
    __NextDownload();
}

//刷新上传列表
void MainWindow::RefreshUploadList(TransmissionData pro=TransmissionData())
{
    if(m_uploadItemModel!=nullptr)
        delete m_uploadItemModel;
    m_uploadItemModel = new QStandardItemModel();
    int cnt=0;
    for (auto &&Ele : m_uploadList)
    {
        QStandardItem *Item = new QStandardItem;
        QFileInfo uploadFileInfo(Ele);
        TransmissionInfo uploadInfo={uploadFileInfo.fileName(),uploadFileInfo.suffix(),0,0,size_t(QFile(Ele).size()),false,true,true,true,true,true};
        if(m_uploadCnt>cnt)
            uploadInfo.progress=1000;
        else if(m_uploadCnt==cnt)
        {
            uploadInfo.progress=pro.progress;
            uploadInfo.speed=pro.speed;
            uploadInfo.fileSize=pro.size;
        }
        Item->setData(QVariant::fromValue(uploadInfo), Qt::UserRole + 1); //整体存取
        m_uploadItemModel->appendRow(Item); //追加Item
        cnt++;
    }
    ui->uploadList->setModel(m_uploadItemModel);
}

void MainWindow::on_fileList_doubleClicked(const QModelIndex &index)
{
    qDebug("%d",index.row());
    int indexRow = index.row();
    if(indexRow==0)
    {
        m_fileSystemModel->outPath();
        RefreshMainList();
    }
    else if(m_mainItemModel->data(index).value<TransmissionInfo>().extName == "<DIR>")
    {
        QString dirName = m_mainItemModel->data(index).value<TransmissionInfo>().fileName;
        m_fileSystemModel->inPath(dirName);
        if(!m_fileSystemModel->getFileTree().size())
        {
            RequestFileTree(m_fileSystemModel->getPath());
        }
    }
}

void MainWindow::on_menu1_downloadRequest()
{
    qDebug("download");
    QModelIndexList selectModelList = ui->fileList->selectionModel()->selectedIndexes();
    for(auto &&Ele:selectModelList)
    {
        TransmissionInfo data = Ele.data(Qt::UserRole+1).value<TransmissionInfo>();
        m_downloadList.append(m_fileSystemModel->getPath()+data.fileName);
    }
    RefreshDownloadList();
}

void MainWindow::on_menu1_uploadRequest()
{
    qDebug("upload");
    QString filePath=QFileDialog::getOpenFileName(this);
    m_tempFile = new QFile(filePath);
    QFileInfo fileInfo(filePath);
    m_uploadList.append(filePath);
    RefreshUploadList();
}

void MainWindow::on_menu1_createFolderRequest()
{
    qDebug("createFolder");
}

void MainWindow::on_menu1_deleteFileRequest()
{
    qDebug("deleteFile");
}

void MainWindow::on_menu1_refrashRequest()
{
    qDebug("refrash");
    RequestFileTree(m_fileSystemModel->getPath());
}
