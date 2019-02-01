#include "application.h"

#include "qdebug.h"

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

        return Status{false, vcg::tri::io::Importer<PMesh>::ErrorMsg(err)};
    }
}
