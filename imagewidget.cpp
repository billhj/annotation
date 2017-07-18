#include "ImageWidget.h"

ImageWidget::ImageWidget(): QWidget()
{
    resetRoi();
    boxmodify = 0;
}

void ImageWidget::resetRoi()
{
    box.setRect(-1,-1,-1,-1);
}

void ImageWidget::setQImage(const QImage& imga){      //接受视频解析出的图片
    qqImage=imga;
    update();
}

void ImageWidget::setRoi(const QRect& b)
{
    box = b;
    update();
}

void ImageWidget::updateTopLeft(const QPoint& p)
{
    box.setTopLeft(p);
    update();
}

void ImageWidget::updateBottomRight(const QPoint& p)
{
    box.setBottomRight(p);
    update();
}

void ImageWidget::paintEvent(QPaintEvent *event){    //绘制函数

    QPainter painter(this);
    painter.setBrush(Qt::black);
    //painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色
    //if (qqImage.size().width() <= 0) return;
    //将图像按比例缩放成和窗口一样大小
    //QImage img = qqImage.scaled(this->size(),Qt::KeepAspectRatio);
    painter.drawImage(QPoint(0,0),qqImage);

    if(abs(box.topLeft().x() - box.bottomRight().x()) > 10)
    {
        QPen penHLines(Qt::red, 2, Qt::SolidLine, Qt::FlatCap, Qt::RoundJoin);
        painter.setPen(penHLines);
        painter.drawLine(box.topLeft(),box.topRight());
        painter.drawLine(box.bottomLeft(),box.bottomRight());
        painter.drawLine(box.topLeft(),box.bottomLeft());
        painter.drawLine(box.topRight(),box.bottomRight());
        painter.drawText(box.topLeft() + QPoint(0,10), QString().append(QString::number(box.topLeft().x())).append(",").append(QString::number(box.topLeft().y())));
        painter.drawText(box.bottomRight()+ QPoint(-10,-10), QString().append(QString::number(box.bottomRight().x())).append(",").append(QString::number(box.bottomRight().y())));
    }
}



void ImageWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(boxmodify==1)
    {
        updateBottomRight(event->pos());
    }
}
void ImageWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        boxmodify = 1;
        updateTopLeft(event->pos());
        updateBottomRight(event->pos());
    }
}

void ImageWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(boxmodify==1)
    {
        updateBottomRight(event->pos());
        boxmodify = 0;
    }
}


