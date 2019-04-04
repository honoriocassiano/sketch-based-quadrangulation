#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "QSignalMapper"
#include "application.h"
#include "qdebug.h"
#include "qfiledialog.h"

#include "utils/utils.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    connect(ui->loadMeshButton, SIGNAL(clicked()), this, SLOT(loadMesh()));

    connect(ui->viewer, SIGNAL(notifyStatusBar(QString)), ui->statusBar,
            SLOT(showMessage(QString)));

    connect(this, SIGNAL(notifyStatusBar(QString)), ui->statusBar,
            SLOT(showMessage(QString)));

    connect(this, SIGNAL(updateViewer()), ui->viewer, SLOT(update()));

    connect(ui->toleranceDPSpinBox, SIGNAL(valueChanged(double)), ui->viewer,
            SLOT(changeCurveParams()));

    connect(ui->distanceSpinBox, SIGNAL(valueChanged(double)), ui->viewer,
            SLOT(changeCurveParams()));

    connect(ui->autoDistanceCheckBox, SIGNAL(toggled(bool)), this,
            SLOT(switchAutoDistance(bool)));

    connect(ui->viewer, SIGNAL(setCurveParams(CurveDraw *)), this,
            SLOT(changeCurveParams(CurveDraw *)));
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::loadMesh() {
    //    QString fileName =
    //        QFileDialog::getOpenFileName(this, tr("Open Mesh"), "../models",
    //                                     tr("Poly Model (*.ply *.obj
    //                                     *.off)"));

    //    qDebug() << fileName;

    //    QString fileName = "../models/3holes_quad.obj";
    QString fileName = "../models/cube.obj";
    //    QString fileName = "../models/cube_tri.obj";
    //    QString fileName = "../models/magnolia.obj";

    if (!fileName.isEmpty()) {
        //        qDebug() << fileName;
        Status status = ui->viewer->loadMesh(fileName);

        //        ui->viewer->setFocus();

        ui->viewer->setFocus();

        emit notifyStatusBar(QString::fromStdString(status.message));
    }
}

void MainWindow::switchAutoDistance(bool status) {

    if (!status) {

        /// Prevent to update drawer
        ui->distanceSpinBox->blockSignals(true);

        ui->distanceSpinBox->setValue(double(ui->viewer->getMaxDistance()));

        ui->distanceSpinBox->blockSignals(false);

        /// Enable distance parameters
        ui->distanceLabel->setEnabled(true);
        ui->distanceSpinBox->setEnabled(true);
    } else {
        ui->distanceLabel->setEnabled(false);
        ui->distanceSpinBox->setEnabled(false);
    }
}

void MainWindow::changeCurveParams(CurveDraw *curve) {

    float tol = float(ui->toleranceDPSpinBox->value());

    curve->simplify(tol);

    if (ui->autoDistanceCheckBox->isChecked()) {
        curve->resample();
    } else {
        curve->resample(float(ui->distanceSpinBox->value()));
    }

    emit updateViewer();
}
