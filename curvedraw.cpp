#include "curvedraw.h"

namespace qgl = qglviewer;

/*
 CURVEDRAW CLASS DEFINITIONS
*/

CurveDraw::CurveDraw() : drawMode(false) {}

CurveDraw::~CurveDraw() {}

bool CurveDraw::isDrawing() const { return drawMode; }

std::vector<vcg::Point3<PMesh::ScalarType>> CurveDraw::getPoints() const {
    return addedPoints;
}

void CurveDraw::startDraw() {
    reset();
    drawMode = true;
}

void CurveDraw::addPoint(const vcg::Point3<PMesh::ScalarType> &point,
                         PMesh::FacePointer face) {
    if (drawMode) {
        addedPoints.push_back(point);
        faces.push_back(face);
    }
}

void CurveDraw::reset() {
    addedPoints.clear();
    faces.clear();
}

void CurveDraw::endDraw() { drawMode = false; }
