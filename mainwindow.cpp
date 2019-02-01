#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "qdebug.h"
#include "qfiledialog.h"

#include "utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->loadMeshButton, SIGNAL(clicked()), this, SLOT(loadMesh()));

    connect(this, SIGNAL(notifyStatusBar(QString)), ui->statusBar,
            SLOT(showMessage(QString)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadMesh() {
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Mesh"), "./", tr("Poly Model (*.ply *.obj *.off)"));

    if (!fileName.isEmpty()) {
        //        qDebug() << fileName;
        Application app;

        Status status = app.loadMesh(fileName.toStdString());

        emit notifyStatusBar(QString::fromStdString(status.message));
    }
}
