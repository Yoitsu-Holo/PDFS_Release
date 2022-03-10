#include <vector>
#include <cstring>
#include <QMap>
#include <QString>
#include <QStringList>
#include <QList>

#include "protocol.h"
#include "transmissionmodel.h"

namespace file_inner_struct
{
using namespace std;
class PDFS_File
{
private:
    QString filename;
    QString filetype;
    size_t filesize;

public:
    PDFS_File() : filename(""), filetype(""), filesize(0){}

    PDFS_File(QString RawFileName, size_t FileSize)
    {
        int pos = RawFileName.length();
        for (pos--; pos >= 0; pos--)
            if (RawFileName[pos] == '.')
                break;

        filename = "";
        filetype = "";

        if (pos == -1)
            filename = RawFileName,
                    filetype = "unknow";
        else
            for (int i = 0; i < (int)RawFileName.length(); i++)
                if (i < pos)
                    filename += RawFileName[i];
                else if (i > pos)
                    filetype += RawFileName[i];

        filesize = FileSize;
    }

    void setFileName(QString FileName) { filename = FileName; }
    void setFileType(QString FileType) { filetype = FileType; }
    void setFileSize(size_t FileSize) { filesize = FileSize; }
    void setFileInfo(QString RawFileName, size_t FileSize)
    {
        PDFS_File tempFile(RawFileName, FileSize);
        *this = tempFile;
    }

    QString getFileName() { return filename; }
    QString getFileType() { return filetype; }
    size_t getFileSize() { return filesize; }
};

class PDFS_Dir
{
private:
    QMap<QString, PDFS_Dir *> ListDir;
    QMap<QString, PDFS_File *> ListFile;
    QString dirname;
    PDFS_Dir *prePath;

public:
    PDFS_Dir() : ListDir(), ListFile(), dirname(""), prePath(NULL) {}
    PDFS_Dir(QString DirName, PDFS_Dir *PrePath)
    {
        dirname = DirName;
        prePath = PrePath;
    }

    void setDirInfo(QString RawFileName,PDFS_Dir *PrePath)
    {
        PDFS_Dir tempDir(RawFileName, PrePath);
        *this = tempDir;
    }
    void setDirName(QString DirName) { dirname = DirName; }
    void setPrePath(PDFS_Dir *PrePath) { prePath = PrePath; }
    PDFS_Dir *getPrePath() { return prePath; }
    QString getDirName() { return dirname; }
    QMap<QString, PDFS_Dir *> getDirInfo() { return ListDir; }
    QMap<QString, PDFS_File *> getFileInfo() { return ListFile; }

    RSC addFile(PDFS_File *File)
    {
        if (ListFile.find(File->getFileName()) != ListFile.end())
            return run_FileExist;
        ListFile[File->getFileName()] = File;
        return run_NoError;
    }

    RSC addDir(PDFS_Dir *Dir)
    {
        if (ListDir.find(Dir->getDirName()) != ListDir.end())
            return run_DirExist;
        ListDir[Dir->getDirName()] = Dir;
        return run_NoError;
    }

    RSC delFile(PDFS_File *File)
    {
        if (ListFile.find(File->getFileName()) == ListFile.end())
            return run_FileUnexist;
        ListFile.erase(ListFile.find(File->getFileName()));
        return run_NoError;
    }

    RSC delDir(PDFS_Dir *Dir)
    {
        if (!ListDir.contains(Dir->getDirName()))
            return run_DirUnexist;
        ListDir.erase(ListDir.find(Dir->getDirName()));
        return run_NoError;
    }

    RSC delFile(QString FileName)
    {
        if (!ListFile.contains(FileName))
            return run_FileUnexist;
        ListFile.erase(ListFile.find(FileName));
        return run_NoError;
    }

    RSC delDir(QString DirName)
    {
        if (!ListDir.contains(DirName))
            return run_DirUnexist;
        ListDir.erase(ListDir.find(DirName));
        return run_NoError;
    }

    RSC delAll()
    {
        try {
            ListDir.clear();
            ListFile.clear();
        }  catch (std::exception) {
            return run_UnknowError;
        }

        return run_NoError;
    }
};
};

class PDFS_FileSystem
{
#define FIS file_inner_struct

private://data
    FIS::PDFS_Dir *root;
    FIS::PDFS_Dir *now;

    std::vector<QString> path;

public://method
    PDFS_FileSystem() : root(new FIS::PDFS_Dir()) { now = root; }
    QString getPath()
    {
        QString fullPath="/";
        for (auto &&dir : path)
            fullPath.append(dir).append("/");
        return fullPath;
    }
    std::vector<TransmissionInfo> getFileTree()
    {
        std::vector<TransmissionInfo> FileTree;
        TransmissionInfo FileInfo;
        FileInfo={"上级目录","<PRE>",0,0,0,0,1,1,0,0,0};
        FileTree.push_back(FileInfo);
        for (auto &&Ele : now->getDirInfo())
        {
            FileInfo={Ele->getDirName(),"<DIR>",0,0,0,0,1,1,0,0,0};
            FileTree.push_back(FileInfo);
        }
        for (auto &&Ele : now->getFileInfo())
        {
            FileInfo={Ele->getFileName()+"."+Ele->getFileType(),Ele->getFileType(),0,0,Ele->getFileSize(),1,1,1,0,0,0};
            FileTree.push_back(FileInfo);
        }
        return FileTree;
    }
    int addFile(QString RawFileName, size_t FileSize)
    {
        FIS::PDFS_File *File = new FIS::PDFS_File(RawFileName, FileSize);
        return now->addFile(File);
    }

    int addDir(QString DirName)
    {
        FIS::PDFS_Dir *Dir = new FIS::PDFS_Dir(DirName, now);

        return now->addDir(Dir);
    }

    int delAll(){ return now->delAll(); }
    int delFile(FIS::PDFS_File *File) { return now->delFile(File); }
    int delDir(FIS::PDFS_Dir *Dir) { return now->delDir(Dir); }
    int delFile(QString FileName) { return now->delFile(FileName); }
    int delDir(QString DirName) { return now->delDir(DirName); }

    int inPath(QString DirName)
    {
        auto dirInfo = now->getDirInfo();
        if (!dirInfo.contains(DirName))
            return run_DirUnexist;
        path.push_back(DirName);
        now = dirInfo[DirName];
        return run_NoError;
    }

    void outPath()
    {
        if(!path.empty())
            path.pop_back();
        if(now!=root)
            now = now->getPrePath();
    }

public://converter
    QList<QStringList> CtoQ_FileTree(std::vector<std::vector<QString>> tree)
    {
        QList<QStringList> ret;
        QStringList ele;
        for(auto &&dir:tree)
        {
            ele.clear();
            for(auto &&info:dir)
                ele.append(info);
            ret.append(ele);
        }
        return ret;
    }

    std::vector<std::vector<QString>> QtoC_FileTree(QList<QStringList> tree)
    {
        std::vector<std::vector<QString>> ret;
        std::vector<QString> ele;
        for(auto &&dir:tree)
        {
            ele.clear();
            for(auto &&info:dir)
                ele.push_back(info);
            ret.push_back(ele);
        }
        return ret;
    }
public://Debug

    void TEST()
    {
        srand(time(NULL));
        for (int i = 0; i < 10; i++)
        {
            QString s;
            s = "test=";
            s += char('a' + i);
            this->addDir(s);
            this->addFile(s + ".pdf", rand());
        }
        this->inPath("test=a");
        for (int i = 0; i < 10; i++)
        {
            QString s;
            s = "中文=";
            s += char('a' + i);
            this->addDir(s);
            this->addFile(s + ".txt", rand());
        }
    }
};
