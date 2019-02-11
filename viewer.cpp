#include "viewer.h"
#include "qdebug.h"

#include "QGLViewer/manipulatedFrame.h"

#include "utils/intersection.h"
#include "utils/qutils.h"
#include "vcg/complex/algorithms/intersection.h"

namespace qgl = qglviewer;

/*
 VIEWER CLASS DEFINITIONS
*/

Viewer::Viewer(QWidget *parent) : QGLViewer(parent) {

    auto *sc = new StandardCamera();
    auto *c = this->camera();

    sc->fitSphere(qgl::Vec(0, 0, 0), 10);
    //    sc->setPosition(qglviewer::Vec(0, 0, 20));

    this->setCamera(sc);

    delete c;
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

    //    this->setManipulatedFrame(new qglviewer::ManipulatedFrame());

    //    auto constraint = new ManipulatedFrameSetConstraint();
    //    constraint->addObjectToSet(new Object());

    //    this->manipulatedFrame()->setConstraint(constraint);

    //    setBackgroundColor(QColor(0, 0, 0));
    //    setBackgroundColor(QColor(255, 255, 255));
    //    restoreStateFromFile();

    //    glClearColor(0, 0, 0, 1);
    //    glClearColor(0, 0, 0, 0);
    //    glEnable(GL_LIGHTING);
    //    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    //    glEnable(GL_COLOR_MATERIAL);
    //    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glPolygonOffset(1, 1);
}

void Viewer::mousePressEvent(QMouseEvent *e) {

    if (e->button() == Qt::MouseButton::LeftButton) {

        qgl::Vec orig, dir;

        this->camera()->convertClickToLine(e->pos(), orig, dir);

        vcg::Point3<PMesh::ScalarType> hitPoint;
        vcg::Line3<PMesh::ScalarType> ray(qtToVCG(orig), qtToVCG(dir));

        PMesh::ScalarType b1, b2, b3;
        PMesh::FacePointer face = nullptr;

        if (intersectRayMesh(app.getMesh(), ray, hitPoint, b1, b2, b3, face)) {

            auto drawer = app.getDrawer();

            if (!drawer->isDrawing()) {
                drawer->startDraw();
            }

            drawer->addPoint(hitPoint, face);

            qDebug() << "Points: " << drawer->getPoints();
        } else {
            //        qDebug() << ":(";
        }
    } else if (e->button() == Qt::MouseButton::RightButton) {
        app.getDrawer()->endDraw();
    }

    QGLViewer::mousePressEvent(e);
}

void Viewer::mouseMoveEvent(QMouseEvent *e) {
    /*if (modeTemporal == InputCurve) {
         if (!painter.isDrawing)
             return;
         painter.CurrentPoint =
             painter.mousePosition3D(this->camera(), e->pos());
         painter.points.push_back(painter.CurrentPoint);
         painter.addSegmentStroke(painter.LastPoint, painter.CurrentPoint);
         painter.LastPoint = painter.CurrentPoint;
         // cout<<"added "<<painter.CurrentPoint.x<<"
         // "<<painter.CurrentPoint.y<<" "<<painter.CurrentPoint.z<<endl;
         // tri::Clean<CMesh>::RemoveDuplicateVertex(painter.LineStroke);
         // tri::Clean<CMesh>::RemoveUnreferencedVertex(painter.LineStroke);
         // tri::Allocator<CMesh>::CompactEveryVector(painter.LineStroke);
         update();
     } else {
         QGLViewer::mouseMoveEvent(e);
     }*/
    QGLViewer::mouseMoveEvent(e);
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

    //    qDebug() << "aaa";

    //    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //    glDisable(GL_CULL_FACE);

    //    qDebug() << "draw";

    app.draw();

    // glClearColor(1.0f, 1.0f, 1.0f, 0.0f);	// White Background
    // glClearColor(0.2f, 0.8f, 0.2f, 0.0f);	// Green Background
    //    if(takingsnapshot){
    //        this->setBackgroundColor(Qt::white);
    //        this->setForegroundColor(Qt::white);
    //    }
    //    else
    //        glClearColor(0.09f, 0.5f, 0.52f, 0.71f);	//  Background
    //    //glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    //    glPolygonMode( GL_FRrONT_AND_BACK, GL_FILL );
    //    glDisable(GL_CULL_FACE);
    //    Point3<CMesh::ScalarType> center(0,0,0);
    //    if(modeTemporal==InputCurve){
    //        painter.renderStroke();
    //    }
    //    else if(flagCurveSample){
    //        painter.renderSample();
    //    }
    //    // the trimesh drawing calls
    //    twomeshes.renderTrianglesMeshes(center,drawmodet1,drawmodet2);
    //    twomeshes.renderTriangleBoolean(center,drawmodet1);

    //    // the edgemeshes drawing calls
    //    twomeshes.renderEdgeMeshes();

    //    // drawing quad meshes
    //    twomeshes.renderMesh1(center,drawmode1);
    //    twomeshes.renderMesh2(center,drawmode2);
    //    twomeshes.renderQuadBoolean(center,drawmodequadresult);
    //    twomeshes.renderIntersectionCurve();
    //    twomeshes.renderSmoothIntersectionCurve();

    //    // draw list of arrows
    //    if(!arrows.empty()){
    //      //drawPolyline(arrows,2.5);
    //      drawDirectedPolyline(arrows,2.5);
    //    }
    //    if(!texto.empty()){
    //        for(int i=0;i<texto.size();i++){
    //            Point3f aux=texto[i].first;
    //            this->renderText(aux[0],aux[1],aux[2],texto[i].second);
    //        }
    //    }
    //    // Draws manipulatedFrame (the set's rotation center)
    //     if (manipulatedFrame()->isManipulated()) {
    //       glPushMatrix();
    //       glMultMatrixd(manipulatedFrame()->matrix());
    //       if(selection_.contains(0))
    //           twomeshes.renderMesh1(center,drawmode1);
    //       if(selection_.contains(1))
    //           twomeshes.renderMesh2(center,drawmode2);
    //       drawAxis(1);
    //       glPopMatrix();
    //     }
    //     for(int i = 0; i < int(objects_.size()); i++)
    //         objects_.at(i)->draw();
}
