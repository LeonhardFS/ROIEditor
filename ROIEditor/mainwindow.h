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
#include <QMenu>
#include <QMenuBar>
#include <QFileDialog>

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
    
private slots:
    bool open();
    bool save();
    bool saveAs();

private:
    Ui::MainWindow *ui;
    QStatusBar *statusBar;
    QWidget *widget;
    ImageBay imageBay;
    
    // Actions
    QAction *closeAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *openAction;
    QMenu *fileMenu;
    
    // workaround for qt double closing bug
    bool dispatchedClosing;
    
    // was the file once saved?
    QString fileName;
    
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
    
    // ask the user if it is ok to continue and discard changes
    bool okToContinue();

    void loadFile(const QString& fileName);
    bool saveFile(const QString& fileName);
protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent*);
    
    void paintEvent(QPaintEvent *);
    
    // used for dragging the ROI around
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);
    
    // used for keyboard input
    void keyPressEvent(QKeyEvent*);
    
    // closing event called by menu or system
    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
