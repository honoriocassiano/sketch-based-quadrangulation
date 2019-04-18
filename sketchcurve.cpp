#include "sketchcurve.h"

SketchCurve::SketchCurve() {}

SketchCurve::SketchCurve(
    const PolygonalCurve<float, 3, SketchVertex<CMesh>> &curve)
    : PolygonalCurve<float, 3, SketchVertex<CMesh>>(curve) {}

SketchCurve::~SketchCurve() {}

std::vector<vcg::Point3<float>> SketchCurve::getVCGPoints() const {
    std::vector<vcg::Point3<float>> points;

    points.reserve(size());

    for (auto it = pPoints.begin(); it != pPoints.end(); it++) {
        points.push_back(vcg::Point3<float>(*it));
    }

    return points;
}

SketchVertex<CMesh> &SketchCurve::front() { return pPoints.front(); }

const SketchVertex<CMesh> &SketchCurve::front() const {
    return pPoints.front();
}

SketchVertex<CMesh> &SketchCurve::back() { return pPoints.back(); }

const SketchVertex<CMesh> &SketchCurve::back() const { return pPoints.back(); }
