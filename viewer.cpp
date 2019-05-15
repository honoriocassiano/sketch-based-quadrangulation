#include "viewer.h"

#include "GL/glew.h"
#include "QGLViewer/manipulatedFrame.h"
#include "mainwindow.h"

#include "utils/debug.h"
#include "utils/intersection.h"
#include "utils/qutils.h"
#include "vcg/complex/algorithms/intersection.h"

namespace qgl = qglviewer;

void GLAPIENTRY MessageCallback(GLenum source, GLenum type, GLuint id,
                                GLenum severity, GLsizei length,
                                const GLchar *message, const void *userParam) {
    fprintf(stderr,
            "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
}

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

Viewer::~Viewer() {
    glDeleteFramebuffers(1, &fboSelection);
    glDeleteTextures(1, &texSelection);
    glDeleteRenderbuffers(1, &depthSelection);
}

Status Viewer::loadMesh(QString filename) {

    auto st = app.loadMesh(filename.toStdString());
    app.showMesh();

    update();

    return st;
}

void Viewer::beginSelectMode() {
    glEnable(GL_TEXTURE_2D);

    // Attach buffers and texture;
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fboSelection);
    glBindTexture(GL_TEXTURE_2D, texSelection);
    glBindRenderbuffer(GL_RENDERBUFFER, depthSelection);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width(),
                          height());
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, depthSelection);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB,
                 GL_UNSIGNED_BYTE, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texSelection, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewer::endSelectMode() {
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

bool Viewer::editCurve(QMouseEvent *e) {
    if ((e->button() == Qt::MouseButton::LeftButton)) {

        std::size_t pos;

        if (getSelectedVertex(e->x(), height() - e->y(), pos)) {
            Debug() << "position: " << pos;
        }
        //        qgl::Vec orig, dir;

        //        this->camera()->convertClickToLine(e->pos(), orig, dir);

        //        e->x();

        //        vcg::Point3<CMesh::ScalarType> hitPoint;
        //        vcg::Line3<CMesh::ScalarType> ray(qtToVCG(orig),
        //        qtToVCG(dir));
    } else {
        return false;
    }

    return true;
}

bool Viewer::drawCurve(QMouseEvent *e) {
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

            return false;
        }
    } else {
        lastState.valid = false;

        return false;
    }

    return true;
}

bool Viewer::getSelectedVertex(int x, int y, size_t &pos) {

    Debug() << "x: " << x << " y: " << y;

    GLubyte pixels[3];
    auto drawer = app.getDrawer();

    beginSelectMode();

    drawer->drawPicking();

    endSelectMode();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fboSelection);
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    //    pixels = glReadPixels(point[0], self.__dimension[1] - point[1], 1, 1,
    //                          GL_RGB, GL_UNSIGNED_BYTE);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixels);

    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

    Debug() << "r: " << pixels[0] << " g: " << pixels[1] << " b: " << pixels[2];

    // R + G + B;
    if (!pixels[0] && !pixels[1] && !pixels[2]) {
        return false;
    }

    pos = (pixels[0] * (1 << 16)) + (pixels[1] * (1 << 8)) + pixels[2] - 1;
    return true;

    //    index = (pixels[0] * 65025) + (pixels[1] * 255) + (pixels[2] - 1);
}

Application *Viewer::getApp() { return &app; }

void Viewer::init() {

    GLenum err = glewInit();

    if (GLEW_OK != err) {
        /* Problem: glewInit failed, something is seriously wrong. */
        Debug() << "Error: " << glewGetErrorString(err);
    }

    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);

    glPolygonOffset(1, 1);

    glGenFramebuffers(1, &fboSelection);
    glGenTextures(1, &texSelection);
    glGenRenderbuffers(1, &depthSelection);

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
}

void Viewer::mousePressEvent(QMouseEvent *e) {

    bool eventHandled = false;

    if (app.getMode() == Application::Mode::EDIT_CURVE) {
        eventHandled = editCurve(e);
    } else if (app.getMode() == Application::Mode::DRAW) {
        eventHandled = drawCurve(e);
    }

    if (!eventHandled) {
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
    } else if (e->key() == Qt::Key::Key_E) {
        if (app.getMode() == Application::Mode::DRAW) {

            app.switchToMode(Application::Mode::EDIT_CURVE);

        } else if (app.getMode() == Application::Mode::EDIT_CURVE) {

            app.switchToMode(Application::Mode::DRAW);
        }
    }

    if (!st.result) {
        emit notifyStatusBar(QString::fromStdString(st.message));
    }

    update();
}

void Viewer::changeCurveParams() { emit setCurveParams(app.getDrawer()); }

GLuint Viewer::getDepthSelection() const { return depthSelection; }

GLuint Viewer::getTexSelection() const { return texSelection; }

GLuint Viewer::getFboSelection() const { return fboSelection; }

void Viewer::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    app.draw();
}

QString Viewer::helpString() const {
    QString text("");

    return text;
}
