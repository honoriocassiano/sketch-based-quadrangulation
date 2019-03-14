#include "application.h"

#include "qdebug.h"

#include "vcg/complex/algorithms/update/topology.h"
#include <GL/gl.h>
#include <wrap/gl/space.h>
#include <wrap/io_trimesh/import.h>

Application::Application() {}

Application::~Application() {}

Status Application::loadMesh(std::string filename) {
    mesh.Clear();
    int err = vcg::tri::io::Importer<PMesh>::Open(mesh, filename.c_str());

    /// Create a triangular mesh from input
    trimesh.Clear();
    vcg::tri::PolygonSupport<CMesh, PMesh>::ImportFromPolyMesh(trimesh, mesh);
    // update bounding box
    vcg::tri::UpdateBounding<CMesh>::Box(trimesh);
    // update Normals
    vcg::tri::UpdateNormal<CMesh>::PerVertexNormalizedPerFace(trimesh);
    vcg::tri::UpdateNormal<CMesh>::PerFaceNormalized(trimesh);

    /// Update face-face topology
    vcg::tri::UpdateTopology<PMesh>::FaceFace(mesh);
    vcg::tri::UpdateTopology<CMesh>::FaceFace(trimesh);

    if (!err) {
        return STATUS_OK;
    } else {
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

        glBegin(GL_TRIANGLES);

        for (auto it = trimesh.face.begin(); it != trimesh.face.end(); ++it) {

            vcg::glNormal(it->cN());

            vcg::glVertex((*it).cV(0)->P());
            vcg::glVertex((*it).cV(1)->P());
            vcg::glVertex((*it).cV(2)->P());
        }
        glEnd();

        glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if (state.drawVisible) {

        drawer.draw();
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
