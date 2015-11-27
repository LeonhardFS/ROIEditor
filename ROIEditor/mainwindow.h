#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>
#include <QDir>

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
    ImageBay imageBay;

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent*);
};

#endif // MAINWINDOW_H
