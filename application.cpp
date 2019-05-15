#include "application.h"

#include "qdebug.h"

#include "vcg/complex/algorithms/update/topology.h"
#include <wrap/gl/space.h>

Application::Application() : currentMode(Mode::DRAW) {}

Application::~Application() {}

Status Application::loadMesh(std::string filename) {
    return mesh.load(filename);
}

Status Application::showMesh() {
    if (mesh.get()->VertexNumber() > 0) {
        state.meshVisible = true;

        return Status::OK;
    }

    return Status::make(false, "Cannot display an empty mesh!");
}

Status Application::switchShowingMesh() {
    if (state.meshVisible) {
        return hideMesh();
    }
    return showMesh();
}

Status Application::showPatches() {
    // TODO Add checks here
    state.patchVisible = true;

    return Status::OK;
}
Status Application::hidePatches() {
    // TODO Add checks here
    state.patchVisible = false;

    return Status::OK;
}

Status Application::switchShowingPatches() {
    if (state.patchVisible) {
        return hidePatches();
    }
    return showPatches();
}

Status Application::hideMesh() {
    state.meshVisible = false;

    return Status::OK;
}

bool Application::switchToMode(Mode m) {

    switch (currentMode) {
    case Mode::DRAW:
        if (m == Mode::EDIT_CURVE) {
            if (!drawer.isDrawing() && !drawer.isEmpty()) {
                currentMode = m;
            }
        }
        break;

    case Mode::EDIT_CURVE:
        if (m == Mode::DRAW) {
            currentMode = m;
        }
        break;
    }

    return (currentMode == m);
}

void Application::draw() {

    glPushMatrix();
    if (state.meshVisible) {
        mesh.draw();
    }

    if (state.drawVisible) {
        drawer.draw();
    }

    if (state.patchVisible) {
        mesh.drawEdges();
    }

    glPopMatrix();
}

Status Application::showDrawing() {
    if (drawer.getSize() > 0) {
        state.drawVisible = true;

        return Status::OK;
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
    return Status::OK;
}

Application::CurrentState::CurrentState() {
    drawVisible = false;
    meshVisible = false;
    patchVisible = false;
}
