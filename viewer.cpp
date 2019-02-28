#include "viewer.h"
#include "qdebug.h"

#include "QGLViewer/manipulatedFrame.h"

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

        vcg::Point3<PMesh::ScalarType> hitPoint;
        vcg::Line3<PMesh::ScalarType> ray(qtToVCG(orig), qtToVCG(dir));

        PMesh::ScalarType b1, b2, b3;
        PMesh::FacePointer face = nullptr;

        if (intersectRayMesh(app.getMesh(), ray, hitPoint, b1, b2, b3, face)) {

            if (e->button() == Qt::MouseButton::LeftButton) {
                lastState.windowPosition = e->pos();
                lastState.hitFace = face;
                lastState.hitPoint = hitPoint;
                lastState.valid = true;
            } else {

                float matrix[16];
                camera()->getModelViewProjectionMatrix(matrix);

                app.getDrawer()->endDraw(
                    app.getMesh(), qtToVCG(camera()->viewDirection()), matrix);

                lastState.valid = false;
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

            vcg::Point3<PMesh::ScalarType> hitPoint;
            vcg::Line3<PMesh::ScalarType> ray(qtToVCG(orig), qtToVCG(dir));

            PMesh::ScalarType b1, b2, b3;
            PMesh::FacePointer face = nullptr;

            if (intersectRayMesh(app.getMesh(), ray, hitPoint, b1, b2, b3,
                                 face)) {

                if (max(abs(e->pos().x() - lastState.windowPosition.x()),
                        abs(e->pos().y() - lastState.windowPosition.y())) > 5) {

                    auto drawer = app.getDrawer();

                    if (!drawer->isDrawing()) {
                        drawer->startDraw();
                    }

                    float matrix[16];

                    camera()->getModelViewProjectionMatrix(matrix);

                    if (!drawer->isDrawing()) {

                        drawer->addPoint(app.getMesh(), lastState.hitPoint,
                                         lastState.hitFace,
                                         qtToVCG(camera()->viewDirection()),
                                         matrix);
                    }

                    drawer->addPoint(app.getMesh(), hitPoint, face,
                                     qtToVCG(camera()->viewDirection()),
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

    } else if (e->key() == Qt::Key::Key_D) {
        st = app.switchShowingDrawing();
    }

    if (!st.result) {
        emit notifyStatusBar(QString::fromStdString(st.message));
    }

    update();
}

void Viewer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app.draw();
}
