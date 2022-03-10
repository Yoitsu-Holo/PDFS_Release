#ifndef TRANSMISSIONMODEL_H
#define TRANSMISSIONMODEL_H

#include <QFile>
#include <QObject>
#include <QStandardItemModel>
#include <QStyledItemDelegate>
#include <QSvgRenderer>
#include <QPainter>
#include <QPainterPath>
#include <QFileIconProvider>

struct TransmissionData
{
    TransmissionData():progress(0),speed(0),size(0){}
    int progress;
    int speed;
    int size;
};

struct TransmissionInfo
{
    QString fileName;
    QString extName;
    int progress;//range:0~1000
    float speed;
    size_t fileSize;
    bool visSelect;
    bool visIcon;
    bool visName;
    bool visProgress;
    bool visSpeed;
    bool visSize;
};
//注册模型
Q_DECLARE_METATYPE(TransmissionInfo)

class TransmissionDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    TransmissionDelegate(QObject *parent = nullptr);
//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
//    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
//    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
//    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
//    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // TRANSMISSIONMODEL_H
