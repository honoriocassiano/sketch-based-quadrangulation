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

Status Application::showMesh() {
    if (mesh.VertexNumber() > 0) {
        state.meshVisible = true;

        return STATUS_OK;
    }

    return Status::make(false, "Cannot display an empty mesh!");
}

Status Application::switchShowingMesh() {
    if (state.meshVisible) {
        return hideMesh();
    }
    return showMesh();
}

Status Application::hideMesh() {
    state.meshVisible = false;

    return STATUS_OK;
}

void Application::draw() const {

    glPushMatrix();
    if (state.meshVisible) {

        for (auto it = mesh.face.begin(); it != mesh.face.end(); ++it) {
            //    for (auto it = mesh.vert.begin(); it != mesh.vert.end(); ++it)
            //    {
            //        vcg::glVertex(it->P());
            glBegin(GL_POLYGON);

            for (int j = 0; j < it->VN(); j++) {
                vcg::glVertex(it->V(j)->P());
            }

            glEnd();
        }
    }
    glPopMatrix();
}

Application::CurrentState::CurrentState() {
    drawVisible = false;
    meshVisible = false;
}
