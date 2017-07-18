#ifndef QQWIDGET_H
#define QQWIDGET_H
#include <QPainter>
#include <QImage>
#include "qevent.h"
#include <QWidget>

class ImageWidget : public QWidget
{
     Q_OBJECT

public:
    ImageWidget();
    void setQImage(const QImage& imga);
    void resetRoi();
    void setRoi(const QRect& b);
    void updateTopLeft(const QPoint& p);
    void updateBottomRight(const QPoint& p);
    void paintEvent(QPaintEvent *event);
    QRect& getCurrentRect(){return box;}
protected:
    virtual void	mouseMoveEvent(QMouseEvent *event);
    virtual void	mousePressEvent(QMouseEvent *event);
    virtual void	mouseReleaseEvent(QMouseEvent *event);

private:
     QImage qqImage;
     QRect box;
     int boxmodify;
};

#endif // QQWIDGET_H
