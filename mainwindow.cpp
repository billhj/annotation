#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    img = new ImageWidget();
    setCentralWidget(img);

    createActions();
    model = new QStringListModel(this);
    ui->listView->setEditTriggers(QAbstractItemView::AnyKeyPressed |
                                QAbstractItemView::DoubleClicked);
    connect(model,SIGNAL(clicked(const QModelIndex)),this,SLOT(itemClicked(QModelIndex)));
    connect(ui->openImage,SIGNAL(clicked()),this,SLOT(openSelectedImage()));
    connect(ui->saveboundingbox,SIGNAL(clicked()),this,SLOT(updateCSList()));
    connect(ui->writefile,SIGNAL(clicked()),this,SLOT(saveToTxt()));
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::loadFile(const QString &fileName)
{
    QFileInfo filep(fileName);
    filename = filep.fileName();
    dir = filep.dir().canonicalPath();
    ui->directory->setText(dir);
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
        return false;
    }

    setImage(newImage);
    updateDirImages();

    setWindowFilePath(fileName);

    const QString message = tr("Opened \"%1\", %2x%3, Depth: %4")
        .arg(QDir::toNativeSeparators(fileName)).arg(image.width()).arg(image.height()).arg(image.depth());
    statusBar()->showMessage(message);
    return true;
}


void MainWindow::updateDirImages()
{
    QStringList stringList;
    QDirIterator it(dir, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile f(it.next());
        QFileInfo filep(f.fileName());
        stringList.append(filep.fileName());
        //qDebug() << filep.fileName();
        //f.open(QIODevice::ReadOnly);
        //qDebug() << f.fileName() << f.readAll().trimmed().toDouble() / 1000 << "MHz";
    }

    model->setStringList(stringList);
    ui->listView->setModel(model);

    // if you want to add additional feature to listview.
    ui->listView->setEditTriggers(QAbstractItemView::AnyKeyPressed |
                            QAbstractItemView::DoubleClicked);

}

void MainWindow::openSelectedImage()
{
    // Get the position
    int row = ui->listView->currentIndex().row();
    if(row == -1) return;
    // Get row for Edit mode
    QModelIndex index = model->index(row);
    filename = index.data().toString();

    QString fileName = dir+"/"+filename;
    QImageReader reader(fileName);
    reader.setAutoTransform(true);
    const QImage newImage = reader.read();
    if (newImage.isNull()) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot load %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName), reader.errorString()));
    }

    setImage(newImage);
}

void MainWindow::itemClicked (QModelIndex index )
{
   //textEdit->setText(index.data().toString());
    qDebug()<<"a";
}

void MainWindow::setImage(const QImage &newImage)
{
    img->setQImage(newImage);
}


bool MainWindow::saveFile(const QString &fileName)
{
    QImageWriter writer(fileName);

    if (!writer.write(image)) {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(),
                                 tr("Cannot write %1: %2")
                                 .arg(QDir::toNativeSeparators(fileName)), writer.errorString());
        return false;
    }
    const QString message = tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName));
    statusBar()->showMessage(message);
    return true;
}


static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

void MainWindow::open()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

void MainWindow::saveAs()
{
    QFileDialog dialog(this, tr("Save File As"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptSave);

    while (dialog.exec() == QDialog::Accepted && !saveFile(dialog.selectedFiles().first())) {}
}

void MainWindow::saveToTxt()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "",
                               tr("txt (*.txt)"));
    if(!fileName.isEmpty())
        onelist.save(fileName.toStdString());
}

void MainWindow::loadFromTxt()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                               "",
                               tr("txt (*.txt)"));
    if(!fileName.isEmpty())
    {
        onelist.read(fileName.toStdString());
    }
}

void MainWindow::copy()
{
#ifndef QT_NO_CLIPBOARD
    QGuiApplication::clipboard()->setImage(image);
#endif // !QT_NO_CLIPBOARD
}

#ifndef QT_NO_CLIPBOARD
static QImage clipboardImage()
{
    if (const QMimeData *mimeData = QGuiApplication::clipboard()->mimeData()) {
        if (mimeData->hasImage()) {
            const QImage image = qvariant_cast<QImage>(mimeData->imageData());
            if (!image.isNull())
                return image;
        }
    }
    return QImage();
}
#endif // !QT_NO_CLIPBOARD

void MainWindow::paste()
{
#ifndef QT_NO_CLIPBOARD
    const QImage newImage = clipboardImage();
    if (newImage.isNull()) {
        statusBar()->showMessage(tr("No image in clipboard"));
    } else {
        setImage(newImage);
        setWindowFilePath(QString());
        const QString message = tr("Obtained image from clipboard, %1x%2, Depth: %3")
            .arg(newImage.width()).arg(newImage.height()).arg(newImage.depth());
        statusBar()->showMessage(message);
    }
#endif // !QT_NO_CLIPBOARD
}




void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    QAction *openAct = fileMenu->addAction(tr("&Open..."), this, &MainWindow::open);
    openAct->setShortcut(QKeySequence::Open);
    QAction *saveTextAct = fileMenu->addAction(tr("&SaveToTxt..."), this, &MainWindow::saveToTxt);

    saveAsAct = fileMenu->addAction(tr("&Save As..."), this, &MainWindow::saveAs);
    saveAsAct->setEnabled(false);

    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
    exitAct->setShortcut(tr("Ctrl+Q"));

    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    copyAct = editMenu->addAction(tr("&Copy"), this, &MainWindow::copy);
    copyAct->setShortcut(QKeySequence::Copy);
    copyAct->setEnabled(false);

    QAction *pasteAct = editMenu->addAction(tr("&Paste"), this, &MainWindow::paste);
    pasteAct->setShortcut(QKeySequence::Paste);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));


    viewMenu->addSeparator();

}

void MainWindow::updateActions()
{
    saveAsAct->setEnabled(!image.isNull());
    copyAct->setEnabled(!image.isNull());

}

void MainWindow::updateCSList()
{
    int row = ui->listView->currentIndex().row();
    if(row == -1 && filename.isEmpty()) return;

    ClassificationStruct cs;
    cs.filename = filename.toStdString();
    cs.label = "break";
    QRect& rec = img->getCurrentRect();
    cs.top_x = rec.topLeft().x();
    cs.top_y = rec.topLeft().y();
    cs.bottom_x = rec.bottomRight().x();
    cs.bottom_y = rec.bottomRight().y();
    onelist.addData(cs);

}

