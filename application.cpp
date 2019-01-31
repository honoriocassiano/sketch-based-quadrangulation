#include "application.h"

#include "qdebug.h"

#include <wrap/io_trimesh/import.h>

Application::Application() {}

Application::~Application() {}

bool Application::loadMesh(QString filename) {
    mesh.Clear();
    int err = vcg::tri::io::Importer<PMesh>::Open(
        mesh, filename.toStdString().c_str());

    if (!err) {
        qDebug() << "NO ERROR! " << mesh.face.size();
    } else {
        qDebug() << "ERROR!";
    }
}
