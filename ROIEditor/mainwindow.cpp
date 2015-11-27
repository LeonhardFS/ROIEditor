#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
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

void MainWindow::dropEvent(QDropEvent *event) {
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    int droppedUrlCnt = droppedUrls.size();
    for(int i = 0; i < droppedUrlCnt; i++) {
        QString localPath = droppedUrls[i].toLocalFile();
        QFileInfo fileInfo(localPath);
        if(fileInfo.isFile()) {
        // file
                    QMessageBox::information(this, tr("Dropped file"), fileInfo.absoluteFilePath());
                }
                else if(fileInfo.isDir()) {
        // directory
                    QMessageBox::information(this, tr("Dropped directory"), fileInfo.absoluteFilePath());
                }
                else {
        // none
                    QMessageBox::information(this, tr("Dropped, but unknown"), tr("Unknown: %1").arg(fileInfo.absoluteFilePath()));
                }
    }

    event->acceptProposedAction();
}
