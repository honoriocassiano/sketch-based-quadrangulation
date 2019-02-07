#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "qdebug.h"
#include "qfiledialog.h"

#include "utils/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->loadMeshButton, SIGNAL(clicked()), this, SLOT(loadMesh()));

    connect(this, SIGNAL(notifyStatusBar(QString)), ui->statusBar,
            SLOT(showMessage(QString)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadMesh() {
    //    QString fileName =
    //        QFileDialog::getOpenFileName(this, tr("Open Mesh"), "../models",
    //                                     tr("Poly Model (*.ply *.obj
    //                                     *.off)"));

    //    qDebug() << fileName;

    //    QString fileName = "../models/3holes_quad.obj";
    QString fileName = "../models/cube_tri.obj";

    if (!fileName.isEmpty()) {
        //        qDebug() << fileName;
        Status status = ui->viewer->loadMesh(fileName);

        emit notifyStatusBar(QString::fromStdString(status.message));
    }
}
