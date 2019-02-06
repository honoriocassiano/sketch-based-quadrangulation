#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "application.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  public slots:
    void loadMesh();

    //    void updateStatusBar(QString message);

  signals:
    void notifyStatusBar(QString);

  private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
