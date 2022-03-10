#include "transmissionmodel.h"

TransmissionDelegate::TransmissionDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

void TransmissionDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if(index.isValid())
    {
        QVariant variant = index.data(Qt::UserRole+1);
        TransmissionInfo data = variant.value<TransmissionInfo>();
        //data.visSelect=false;
        //    data.isSelect=true;
        //    data.fileName="Test";
        //    data.extName="chess";
        //    data.progress=1000;
        //    data.speed=rand();
        //    data.fileSize=123123;
        //    data.visProgress=data.visSize=data.visSpeed=true;

        QRectF rawRect;
        painter->save();
        rawRect.setX(option.rect.x());
        rawRect.setY(option.rect.y());
        rawRect.setWidth(option.rect.width());
        rawRect.setHeight(option.rect.height());

        if(true)//绘制外框
        {
            painter->setPen(QPen(Qt::black));
            painter->setBrush(Qt::NoBrush);
            QPainterPath path;
            path.moveTo(rawRect.topLeft());
            path.lineTo(rawRect.topRight());
            path.lineTo(rawRect.bottomRight());
            path.lineTo(rawRect.bottomLeft());
            path.lineTo(rawRect.topLeft());
            painter->drawPath(path);
        }

        if(data.visSelect)//文件选择状态
        {
            //绘制选择框外层
            QRectF selectRect;
            selectRect=QRectF(rawRect.x(),rawRect.y(),60,rawRect.height());

            painter->setPen(QPen(Qt::black));
            painter->setBrush(Qt::NoBrush);

            QPainterPath selectBox;

            selectBox.addRect(selectRect.topLeft().x() + 10,selectRect.topLeft().y() + (selectRect.height()-10)/2,10,10);
            painter->drawPath(selectBox);

            //绘制选择框内层
            QRectF selectBoxInside=QRectF(selectRect.topLeft().x() + 12,selectRect.topLeft().y() + (selectRect.height()-6)/2,6,6);

            if(option.state.testFlag(QStyle::State_Selected))
            {
                painter->setPen(QPen(Qt::black));
                painter->setBrush(Qt::black);
                painter->drawRect(selectBoxInside);
            }
            else if(option.state.testFlag(QStyle::State_MouseOver))
            {
                painter->setPen(QPen(Qt::gray));
                painter->setBrush(Qt::gray);
                painter->drawRect(selectBoxInside);
            }
        }

        if(data.visIcon)//图标状态
        {
            QString iconPath;
            if(data.extName=="<DIR>" || data.extName=="<PRE>")
                iconPath=":/resources/svg/folder.svg";
            else
                iconPath=":/resources/svg/"+data.extName+".svg";

            if(!QFile(iconPath).exists())
                iconPath=":/resources/svg/file.svg";

            QRectF iconRect = QRectF(rawRect.left()+30,rawRect.top()+1,24,24);
            QSvgRenderer* svgRender = new QSvgRenderer();
            svgRender->load(iconPath);

            QFileInfo file_info("."+data.extName);
            QFileIconProvider icon_provider;
            QIcon icon = icon_provider.icon(file_info);

            QPixmap* pixmap = new QPixmap(24,24);
            //QPixmap pixmap = icon.pixmap(24,24);
            pixmap->fill(Qt::transparent);//设置背景透明
            QPainter p(pixmap);
            svgRender->render(&p);
            QImage iconImage = pixmap->toImage();

            painter->drawImage(iconRect,iconImage);
        }

        if(data.visName)//文件名状态
        {
            //文件名称
            QRectF rect2;
            rect2=QRectF(rawRect.x()+60,rawRect.y(),150,rawRect.height());

            //绘制名字
            painter->setPen(QPen(Qt::black));
            painter->setBrush(Qt::NoBrush);
            painter->setFont(QFont("Microsoft YaHei", 10));

            QRect fileNameRect = QRect(rect2.left() +10, rect2.top()+3, rect2.width()-30, 20);
            painter->drawText(fileNameRect,Qt::AlignLeft,data.fileName);
        }

        if(data.visProgress)//传输状态
        {
            //绘制传输进度条本体
            QRectF progressRect;
            progressRect=QRectF(rawRect.x()+210,rawRect.y(),rawRect.width()-370,rawRect.height());
            painter->setPen(QPen(Qt::blue,3));
            painter->setBrush(Qt::NoBrush);

            QPointF progressStart=QPointF(progressRect.topLeft() + QPointF(10, progressRect.height()/2));
            double progressLen=progressRect.width()-100;

            QPainterPath progressBar;
            progressBar.moveTo(progressStart);
            progressBar.lineTo(progressStart + QPointF(progressLen*data.progress/1000+1, 0));

            painter->drawPath(progressBar);

            //绘制传输进度
            painter->setPen(QPen(Qt::black,1));
            painter->setFont(QFont("Microsoft YaHei", 10));
            painter->setBrush(Qt::NoBrush);

            QRect pregressLogRect = QRect(progressRect.right() - 80, progressRect.top() + 3, 40, 20);
            painter->drawText(pregressLogRect,Qt::AlignLeft,QString("%1").arg(data.progress*1.0/10).append("%"));
        }

        if(data.visSpeed)//传输速度
        {
            QRectF speedRect;
            speedRect=QRectF(rawRect.topRight().x()-160,rawRect.y(),80,rawRect.height());

            //绘制传输速度
            QString str;
            if(data.speed>=(1ll<<30))
                str=QString("%1").arg(data.speed*1.0/(1ll<<30)).mid(0,5).append("GB/s");
            else if(data.speed>=(1ll<<20))
                str=QString("%1").arg(data.speed*1.0/(1ll<<20)).mid(0,5).append("MB/s");
            else if(data.speed>=(1ll<<10))
                str=QString("%1").arg(data.speed*1.0/(1ll<<10)).mid(0,5).append("KB/s");
            else
                str=QString("%1").arg(data.speed).mid(0,5).append("B/s");

            painter->setPen(QPen(Qt::black,1));
            painter->setFont(QFont("Microsoft YaHei", 10));
            painter->setBrush(Qt::NoBrush);

            QRectF speedLogRect = QRectF(speedRect.left() + 10, speedRect.top()+3, speedRect.width()-10, 20);
            painter->drawText(speedLogRect,Qt::AlignLeft,str);
        }

        if(data.visSize) //文件大小
        {
            QRectF fileSizeRect;
            fileSizeRect=QRectF(rawRect.topRight().x()-80,rawRect.y(),80,rawRect.height());

            //绘制文件大小
            QString str;
            if(data.fileSize>=(1ll<<30))
            {
                str=QString("%1").arg(data.fileSize*1.0/(1ll<<30)).mid(0,5);
                str+="GB";
            }
            else if(data.fileSize>=(1ll<<20))
            {
                str=QString("%1").arg(data.fileSize*1.0/(1ll<<20)).mid(0,5);
                str+="MB";
            }
            else if(data.fileSize>=(1ll<<10))
            {
                str=QString("%1").arg(data.fileSize*1.0/(1ll<<10)).mid(0,5);
                str+="KB";
            }
            else
            {
                str=QString("%1").arg(data.fileSize).mid(0,5);
                str+="B";
            }

            painter->setPen(QPen(Qt::black,1));
            painter->setFont(QFont("Microsoft YaHei", 10));
            painter->setBrush(Qt::NoBrush);

            QRectF fileSizeLogRect = QRect(fileSizeRect.left() + 10, fileSizeRect.top()+3, fileSizeRect.width()-10, 20);
            painter->drawText(fileSizeLogRect,Qt::AlignLeft,str);
        }

        painter->restore();
    }
}

