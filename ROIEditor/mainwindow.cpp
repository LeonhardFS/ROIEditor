#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle(tr("ROIEditor"));
    
    // add status bar
    statusBar = new QStatusBar;
    setStatusBar(statusBar);
    
    statusBar->showMessage("drag & drop files to start assigning regions of interest...");
    
    curImageIndex = -1;
    
    setMinimumSize(350, 250);
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
    // check if images are already added to the Application
    bool imagesPresent = imageBay.count() > 0;
    
    // collect all dropped files
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
    
    // if no images are present, load the first image!
    // check that urls contained some first...
    if(!imagesPresent && imageBay.count() > 0) {
        curImageIndex = 0;
        loadImage(imageBay.get(curImageIndex));
    }
}

// load image from file and display it
void MainWindow::loadImage(AnnotatedImageFile *aif) {
    
    curImage.load(aif->sFileName);
    
    update();
}

// paint event
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    QPen darkGrayPen;
    //darkGrayPen.setStyle(Qt::DashDotLine);
    darkGrayPen.setWidth(4.0);
    darkGrayPen.setBrush(Qt::darkGray);
    darkGrayPen.setCapStyle(Qt::RoundCap);
    darkGrayPen.setJoinStyle(Qt::RoundJoin);
    
    int width = size().width();
    int height = size().height() - statusBar->size().height();
    
    painter.fillRect(0, 0, width, height, QColor(250,250,250));
    
    // draw rounded rectangle with message
    double rw = min(width / 2., height / 2.);
    QRectF rrect( (width - rw ) / 2., (height - rw) / 2., rw, rw);
    double radius = 20.0;
    painter.setPen(darkGrayPen);
    painter.drawRoundedRect(rrect, radius, radius);
    
    QFont font;
    font.setPixelSize(20);
    painter.setFont(font);
    painter.drawText(rrect, Qt::AlignCenter, "Drop image\n files here");
}

