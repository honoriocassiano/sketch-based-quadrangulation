#include "mesh.h"

#include "wrap/io_trimesh/import.h"
#include <GL/gl.h>
#include <wrap/gl/space.h>

Mesh::Mesh() {}

Mesh::~Mesh() {}

Status Mesh::load(string filename) {
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

void Mesh::draw() const {
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
