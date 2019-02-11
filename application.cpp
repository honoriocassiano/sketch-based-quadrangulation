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

        glEnable(GL_POLYGON_OFFSET_FILL);

        glColor3f(1, 1, 1);

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

        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if (state.drawVisible) {

        glColor3f(1, 0, 0);

        glBegin(GL_LINE_STRIP);

        for (const auto &p : drawer.getPoints()) {
            vcg::glVertex(p);
        }

        glEnd();
    }
    glPopMatrix();
}

Status Application::showDrawing() {
    if (drawer.getSize() > 0) {
        state.drawVisible = true;

        return STATUS_OK;
    }

    return Status::make(false, "No drawing to show!");
}

Status Application::switchShowingDrawing() {
    if (state.drawVisible) {
        return hideDrawing();
    }

    return showDrawing();
}

Status Application::hideDrawing() {
    state.drawVisible = false;
    return STATUS_OK;
}

Application::CurrentState::CurrentState() {
    drawVisible = false;
    meshVisible = false;
}
