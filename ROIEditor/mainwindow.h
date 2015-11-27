#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QMessageBox>

class QDragEnterEvent;
class QDropEvent;

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

protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dropEvent(QDropEvent*);
};

#endif // MAINWINDOW_H
