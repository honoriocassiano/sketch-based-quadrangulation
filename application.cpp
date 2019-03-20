#include "application.h"

#include "qdebug.h"

#include "vcg/complex/algorithms/update/topology.h"
#include <GL/gl.h>
#include <wrap/gl/space.h>

Application::Application() {}

Application::~Application() {}

Status Application::loadMesh(std::string filename) {
    return mesh.load(filename);
}

Status Application::showMesh() {
    if (mesh.get()->VertexNumber() > 0) {
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
        mesh.draw();
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
