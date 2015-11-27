#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>
#include <QPainter>
#include <QStatusBar>

#include "ImageBay.h"

class QDragEnterEvent;
class QDropEvent;
class QDir;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStatusBar *statusBar;
    ImageBay imageBay;

    // index for the current image
    int curImageIndex;
    QPixmap curImage;
    
    bool roiDragEvent;
    int mouse_x;
    int mouse_y;
    
    // load an image
    void loadImage(AnnotatedImageFile* aif);
    
    // update Status Bar with current Image
    void statusImageInfo();

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent*);
    
    void paintEvent(QPaintEvent *);
    
    // used for dragging the ROI around
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
};

#endif // MAINWINDOW_H
