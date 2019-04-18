#ifndef SKETCHCURVE_H
#define SKETCHCURVE_H

#include "curvendimension/curven.hpp"

#include "sketchvertex.h"

class SketchCurve : public PolygonalCurve<float, 3, SketchVertex<CMesh>> {
  public:
    SketchCurve();

    SketchCurve(const PolygonalCurve<float, 3, SketchVertex<CMesh>> &curve);

    virtual ~SketchCurve();

    std::vector<vcg::Point3<float>> getVCGPoints() const;

    SketchVertex<CMesh> &front();

    const SketchVertex<CMesh> &front() const;

    SketchVertex<CMesh> &back();

    const SketchVertex<CMesh> &back() const;
};

#endif
