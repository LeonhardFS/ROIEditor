#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle(tr("ROIEditor"));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::dragEnterEvent(QDragEnterEvent * event) {
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

// used to add images to the processing queue
void MainWindow::dropEvent(QDropEvent *event) {
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    int droppedUrlCnt = droppedUrls.size();
    for(int i = 0; i < droppedUrlCnt; i++) {
        QString localPath = droppedUrls[i].toLocalFile();
        QFileInfo fileInfo(localPath);
        if(fileInfo.isFile()) {
            // file
            if (ImageBay::isImageFileExt(fileInfo.suffix()))
                imageBay.addFile(fileInfo.absoluteFilePath());
        }
            else {
                if(fileInfo.isDir())
                    imageBay.addDirectory(fileInfo.absoluteDir());
            }
    }
    
    event->acceptProposedAction();
}
