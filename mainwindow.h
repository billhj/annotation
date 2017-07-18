#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include "imagewidget.h"
#include "classificationstruct.h"
#include <QStringListModel>

class QAction;
class QMenu;
class QScrollArea;
class QScrollBar;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool loadFile(const QString &);


   private slots:
       void open();
       void saveAs();
       void copy();
       void paste();
       void saveToTxt();
       void loadFromTxt();
       void openSelectedImage();
       void updateCSList();

public slots:
       void itemClicked (QModelIndex index );
   private:
       void createActions();
       void createMenus();
       void updateActions();
       bool saveFile(const QString &fileName);
       void setImage(const QImage &newImage);
       void updateDirImages();

       QImage image;
       QAction *saveAsAct;
       QAction *copyAct;
private:
    Ui::MainWindow *ui;
    ImageWidget* img;
    CSList onelist;
    QString dir;
    QString filename;
    QStringListModel *model;
};

#endif // MAINWINDOW_H
