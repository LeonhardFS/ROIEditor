#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAcceptDrops(true);
    setWindowTitle(tr("ROIEditor"));
    
    // add status bar & widget
    statusBar = new QStatusBar;
    setStatusBar(statusBar);
    
    curImageIndex = -1;
    
    setMinimumSize(350, 250);
    
    statusImageInfo();
    roiDragEvent = false;
    dispatchedClosing = false;
    
    fileName = QString("");
    
    
    // add close slot
    openAction = new QAction(this);
    openAction->setText("Open");
    openAction->setShortcut(QKeySequence::Open);
    closeAction = new QAction(this);
    closeAction->setText("Close editor");
    closeAction->setShortcut(QKeySequence::Close);
    saveAction = new QAction(this);
    saveAction->setText("Save");
    saveAction->setShortcut(QKeySequence::Save);
    saveAsAction = new QAction(this);
    saveAsAction->setText("Save As");
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    
    
    connect(closeAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(closeAction);
    
    
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
                imageBay.addDirectory(fileInfo.absolutePath());
        }
    }
    event->acceptProposedAction();
    
    // if no images are present, load the first image!
    // check that urls contained some first...
    if(!imagesPresent && imageBay.count() > 0) {
        curImageIndex = 0;
        loadImage(imageBay.get(curImageIndex));
    }
    
    // update image info on status bar
    statusImageInfo();
    
}

// load image from file and display it
void MainWindow::loadImage(AnnotatedImageFile *aif) {
    statusBar->showMessage("loading " + aif->sFileName + "...");
    curImage.load(aif->sFileName);
    update();
    
    // small hack, set window size to image size
    QSize size = curImage.size();
    size.setHeight(size.height() + statusBar->size().height());
    setMinimumSize(size);
    //setMaximumSize(size);
}

// for the u=current set image, display info
void MainWindow::statusImageInfo() {
    AnnotatedImageFile *aif = imageBay.get(curImageIndex);
    if(aif) {
        // update status Bar
        QFileInfo fileInfo(aif->sFileName);
        statusBar->showMessage(fileInfo.fileName() + QString("   image %1 / %2").arg(QString::number(curImageIndex + 1), QString::number(imageBay.count())));
    }
    else
        statusBar->showMessage("drag & drop files to start assigning regions of interest...");
}
// paint event
void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    
    // if images are already loaded, display them...
    if(imageBay.count() > 0) {
        // draw image
        painter.drawPixmap(QPoint(0., 0.), curImage);
        
        // draw the region of interst on top
        RegionOfInterest<double> roi = imageBay.get(curImageIndex)->ROI;
        
        QPen redPen;
        redPen.setWidth(2.0);
        redPen.setBrush(QColor(254, 178, 76));
        redPen.setCapStyle(Qt::RoundCap);
        redPen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(redPen);
        painter.setOpacity(0.8);
        painter.drawRect(QRectF(roi.x, roi.y, roi.w, roi.h));
        // draw diagonals
        painter.setOpacity(0.2);
        painter.drawLine(roi.x, roi.y, roi.x + roi.w, roi.y + roi.h);
        painter.drawLine(roi.x, roi.y + roi.h, roi.x + roi.w, roi.y);
        // draw halflines
        painter.setOpacity(0.5);
        painter.drawLine(roi.x + roi.w / 2., roi.y, roi.x + roi.w / 2., roi.y + roi.h);
        painter.drawLine(roi.x, roi.y + roi.h / 2., roi.x + roi.w, roi.y + roi.h / 2.);
    }
    // no images yet loaded?
    else {
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
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    
    // only when application is in ImageState
    if(imageBay.count() > 0) {
        if(event->buttons() & Qt::LeftButton) {
            
            QPointF pos = event->localPos();
            mouse_x = pos.x();
            mouse_y = pos.y();
            // check if the press occured inside the ROI
            if (imageBay.get(curImageIndex)->ROI.containsPoint(mouse_x, mouse_y)) {
                roiDragEvent = true;
            }
            
        }
    }
    
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    
    // update roi pos based on current offset
    if(roiDragEvent) {
        QPointF pos = event->localPos();
        
        imageBay.get(curImageIndex)->clampedUpdate(pos.x() - mouse_x,
                                                   pos.y() - mouse_y);
        mouse_x = pos.x();
        mouse_y = pos.y();
        
        // temporarily set status bar to pixel coordinates
        statusBar->showMessage(QString("(%1, %2, %3, %4)").arg(QString::number(imageBay.get(curImageIndex)->ROI.x), QString::number(imageBay.get(curImageIndex)->ROI.y),QString::number(imageBay.get(curImageIndex)->ROI.x + imageBay.get(curImageIndex)->ROI.w), QString::number(imageBay.get(curImageIndex)->ROI.y + imageBay.get(curImageIndex)->ROI.h)));
        
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    roiDragEvent = false;
    statusImageInfo();
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    
    // are images already loaded?
    if(imageBay.count() > 0) {
        // goto next image via arrow keys
        if(event->key() == Qt::Key_Right || event->key() == Qt::Key_Up) {
            curImageIndex = (curImageIndex + 1) % imageBay.count();
            loadImage(imageBay.get(curImageIndex));
            update();
            statusImageInfo();
        }
        
        if(event->key() == Qt::Key_Left || event->key() == Qt::Key_Down) {
            curImageIndex = (curImageIndex - 1 + imageBay.count()) % imageBay.count();
            loadImage(imageBay.get(curImageIndex));
            update();
            statusImageInfo();
        }
    }
    
}

// check if changes occured
bool MainWindow::okToContinue() {
    if (imageBay.changed()) {
        int r = QMessageBox::warning(this, tr("ROIEditor"),
                                     tr("Data has been modified.\n"
                                        "Do you want to save your changes?"),
                                     QMessageBox::Yes | QMessageBox::No
                                     | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return save();
        } else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

// take into account that there might be changes that should be saved
void MainWindow::closeEvent(QCloseEvent *event) {
    if(!dispatchedClosing) {
        if(okToContinue()) {
            event->accept();
            dispatchedClosing = true;
        }
        else
            event->ignore();
    }
    else
        event->accept();
}

// slots for saving/loading
bool MainWindow::open() {
    
    if(okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this,
                                                        tr("Open CSV file"), ".",
                                                        tr("CSV files (*.csv)"));
        if (!fileName.isEmpty())
            loadFile(fileName);
        
        // update internal filename
        this->fileName = fileName;
    }
    return true;
}
bool MainWindow::save() {
    
    // does filename exist already?
    if(fileName.length() > 0)
        saveFile(fileName);
    else
        saveAs();
    
    return true;
}
bool MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save CSV file"), ".",
                                                    tr("CSV files (*.csv)"));

    if (fileName.isEmpty())
        return false;
    
    // update internal filename
    this->fileName = fileName;
    return saveFile(fileName);
}

void MainWindow::loadFile(const QString &fileName) {
    // load data for ImageBay
    imageBay.loadFromCSV(fileName);
    
    // set to first picture
    if(imageBay.count() > 0) {
        curImageIndex = 0;
        loadImage(imageBay.get(curImageIndex));
    }
    
    // update image info on status bar
    statusImageInfo();
}

bool MainWindow::saveFile(const QString &fileName) {
    // call ImageBay saving function
    imageBay.saveToCSV(fileName);
}
