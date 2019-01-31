#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "application.h"
#include "qdebug.h"
#include "qfiledialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->loadMeshButton, SIGNAL(clicked()), this, SLOT(loadMesh()));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadMesh() {
    QString fileName = QFileDialog::getOpenFileName(
        this, tr("Open Mesh"), "./", tr("Poly Model (*.ply *.obj *.off)"));

    if (!fileName.isEmpty()) {
        //        qDebug() << fileName;
        Application app;

        app.loadMesh(fileName);
    }
}
