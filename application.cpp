#include "application.h"

#include "qdebug.h"

#include <GL/gl.h>
#include <wrap/gl/space.h>
#include <wrap/io_trimesh/import.h>

Application::Application() {}

Application::~Application() {}

Status Application::loadMesh(std::string filename) {
    mesh.Clear();
    int err = vcg::tri::io::Importer<PMesh>::Open(mesh, filename.c_str());

    if (!err) {
        qDebug() << "NO ERROR! " << mesh.face.size();

        return STATUS_OK;
    } else {
        qDebug() << "ERROR!";

        qDebug() << vcg::tri::io::Importer<PMesh>::ErrorMsg(err);

        return Status::make(false,
                            vcg::tri::io::Importer<PMesh>::ErrorMsg(err));
    }
}

void Application::drawMesh() const {
    glPushMatrix();

    //    glTranslatef(0, 0, -10);
    //    glScale(50.0);
    //    glTranslate(-center);
    //    switch (draw) {
    //    case 0: {
    //        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //        break;
    //    }
    //    case 3: {
    //        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //        glLineWidth(1);
    //        break;
    //    }
    //    default:
    //        break;
    //    }
    //    if (draw != 4) {
    //        for (unsigned int i = 0; i < mesh.face.size(); i++) {
    //            vcg::glColor(mesh.face[i].C());

    //            if (mesh.face[i].IsD())
    //                continue;
    //            if (!flagSeeSelectedQuad && mesh.face[i].IsS())
    //                continue;
    //            if (draw == 2)
    //                glBegin(GL_POINTS);
    //            else
    //                glBegin(GL_POLYGON);
    //            vcg::glNormal(mesh.face[i].N());
    //            // assert (secondmesh.face[i].VN() == 4);
    //            for (int j = 0; j < mesh.face[i].VN(); j++)
    //                vcg::glVertex(mesh.face[i].V(j)->P());
    //            glEnd();
    //        }
    //    }

    //    glBegin(GL_POLYGON);
    //    glColor3f(1.0, 1.0, 1.0);
    glColor3f(1, 0, 0);
    //    glPointSize(5.0);
    glBegin(GL_POINTS);

    //    qDebug() << "size: " << mesh.vert.size();

    for (auto it = mesh.face.begin(); it != mesh.face.end(); ++it) {
        //    for (auto it = mesh.vert.begin(); it != mesh.vert.end(); ++it) {
        //        vcg::glVertex(it->P());
        glBegin(GL_POLYGON);

        for (int j = 0; j < it->VN(); j++) {
            vcg::glVertex(it->V(j)->P());
        }

        glEnd();
    }

    //    for (auto it = mesh.vert.begin(); it != mesh.vert.end(); ++it) {
    //        vcg::glVertex(it->P());
    //    }
    //    vcg::glVertex();
    //    for (int j = 0; j < mesh.face[i].VN(); j++)
    //        vcg::glVertex(mesh.face[i].V(j)->P());
    //    glEnd();

    glPopMatrix();
}
