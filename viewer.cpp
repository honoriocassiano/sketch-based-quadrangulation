#include "viewer.h"
#include "qdebug.h"

#include "QGLViewer/manipulatedFrame.h"
#include "mainwindow.h"

#include "utils/intersection.h"
#include "utils/qutils.h"
#include "vcg/complex/algorithms/intersection.h"

namespace qgl = qglviewer;

Viewer::Viewer(QWidget *parent) : QGLViewer(parent) {

    auto *sc = new StandardCamera();
    auto *c = this->camera();

    sc->fitSphere(qgl::Vec(0, 0, 0), 10);

    this->setCamera(sc);

    delete c;

    // Set multisampling
    auto currentFormat = format();
    currentFormat.setSamples(4);
    setFormat(currentFormat);
}

Viewer::~Viewer() {}

Status Viewer::loadMesh(QString filename) {

    auto st = app.loadMesh(filename.toStdString());
    app.showMesh();

    update();

    return st;
}

Application *Viewer::getApp() { return &app; }

void Viewer::init() {
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glPolygonOffset(1, 1);
}

void Viewer::mousePressEvent(QMouseEvent *e) {

    if ((e->button() == Qt::MouseButton::LeftButton) ||
        (e->button() == Qt::MouseButton::RightButton)) {

        qgl::Vec orig, dir;

        this->camera()->convertClickToLine(e->pos(), orig, dir);

        vcg::Point3<CMesh::ScalarType> hitPoint;
        vcg::Line3<CMesh::ScalarType> ray(qtToVCG(orig), qtToVCG(dir));

        CMesh::ScalarType b1, b2, b3;
        CMesh::FacePointer face = nullptr;

        if (intersectRayMesh(app.getMesh()->getTrimesh(), ray, hitPoint, b1, b2,
                             b3, face)) {

            if (e->button() == Qt::MouseButton::LeftButton) {
                lastState.windowPosition = e->pos();
                lastState.hitFace = face;
                lastState.hitPoint = hitPoint;
                lastState.valid = true;
            } else if (app.getDrawer()->getSize() > 3) {

                float matrix[16];
                camera()->getModelViewProjectionMatrix(matrix);

                auto drawer = app.getDrawer();

                drawer->endDraw(app.getMesh()->getTrimesh(),
                                qtToVCG(camera()->viewDirection()), matrix);

                lastState.valid = false;

                emit setCurveParams(drawer);
            }

        } else {
            lastState.valid = false;
            QGLViewer::mousePressEvent(e);
        }
    } else {
        lastState.valid = false;
        QGLViewer::mousePressEvent(e);
    }
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {

    if (lastState.valid) {
        if (e->buttons() == Qt::MouseButton::LeftButton) {

            qgl::Vec orig, dir;

            this->camera()->convertClickToLine(e->pos(), orig, dir);

            vcg::Point3<CMesh::ScalarType> hitPoint;
            vcg::Line3<CMesh::ScalarType> ray(qtToVCG(orig), qtToVCG(dir));

            CMesh::ScalarType b1, b2, b3;
            CMesh::FacePointer face = nullptr;

            if (intersectRayMesh(app.getMesh()->getTrimesh(), ray, hitPoint, b1,
                                 b2, b3, face)) {

                if (max(abs(e->pos().x() - lastState.windowPosition.x()),
                        abs(e->pos().y() - lastState.windowPosition.y())) > 5) {

                    auto drawer = app.getDrawer();

                    if (!drawer->isDrawing()) {
                        drawer->startDraw();
                    }

                    float matrix[16];

                    camera()->getModelViewProjectionMatrix(matrix);

                    if (!drawer->isDrawing()) {

                        drawer->addPoint(app.getMesh()->getTrimesh(),
                                         lastState.hitPoint, lastState.hitFace,
                                         qtToVCG(camera()->viewDirection()),
                                         matrix);
                    }

                    drawer->addPoint(app.getMesh()->getTrimesh(), hitPoint,
                                     face, qtToVCG(camera()->viewDirection()),
                                     matrix);

                    if (!app.isShowingDrawing()) {
                        app.showDrawing();
                    }

                    lastState.windowPosition = e->pos();
                    lastState.hitFace = face;
                    lastState.hitPoint = hitPoint;

                    update();
                }
            }
        } else {
            lastState.valid = false;
            QGLViewer::mouseMoveEvent(e);
        }
    } else {
        QGLViewer::mouseMoveEvent(e);
    }
}

void Viewer::keyPressEvent(QKeyEvent *e) {

    Status st;

    if (e->key() == Qt::Key::Key_M) {
        st = app.switchShowingMesh();

    } else if (e->key() == Qt::Key::Key_R) {
        app.getDrawer()->reset();

    } else if (e->key() == Qt::Key::Key_D) {
        st = app.switchShowingDrawing();
    } else if (e->key() == Qt::Key::Key_P) {
        st = app.switchShowingPatches();
    } else if (e->key() == Qt::Key::Key_H) {
        emit help();
    }

    if (!st.result) {
        emit notifyStatusBar(QString::fromStdString(st.message));
    }

    update();
}

void Viewer::changeCurveParams() { emit setCurveParams(app.getDrawer()); }

void Viewer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app.draw();
}

QString Viewer::helpString() const {
    QString text("");

    return text;
}
