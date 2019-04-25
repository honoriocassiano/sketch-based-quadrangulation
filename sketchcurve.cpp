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

std::vector<int> SketchCurve::douglasPeuckerRankIgnoreEdges(float tol) const {

    // A priority queue of intervals to subdivide
    std::priority_queue<DPitem> pq;

    // The result vector
    std::vector<int> r(size(), -1);

    // Put the first and last vertices in the result
    r[0] = 0;
    r[size() - 1] = 1;

    // Add first interval to pq
    if (size() > 1)
        pq.push(DPitem(0, size() - 1, (*this)));

    // The rank counter
    int rank = 2;

    // Recursively subdivide up to tol
    while (!pq.empty()) {

        DPitem item = pq.top();
        pq.pop();
        if (item.dist < double(tol * tol)) {
            break; // All remaining points are closer
        }
        r[item.farthest] = rank++;
        if (item.farthest > item.first + 1) {
            pq.push(DPitem(item.first, item.farthest, (*this)));
        }
        if (item.last > item.farthest + 1) {
            pq.push(DPitem(item.farthest, item.last, (*this)));
        }
    }

    // Set edges to not be ignored
    for (unsigned i = 0; i < r.size(); i++) {
        if ((*this)[i].hasEdge()) {
            r[i] = 0;
        }
    }

    return r;
}

void SketchCurve::douglasPeuckerSimplifyIgnoreEdges(SketchCurve &result,
                                                    float tol) const {
    assert(size() > 0);
    std::vector<int> rank = douglasPeuckerRankIgnoreEdges(tol);
    result.clear();
    for (unsigned i = 0; i < size(); i++) {
        if (rank[i] >= 0) {
            result.add(at(i));
        }
    }
}

SketchCurve::DPitem::DPitem(unsigned f, unsigned l, const SketchCurve &poly)
    : first(f), last(l) {

    using Vector = VectorN<float, 3>;

    //    assert(last - first > 1);
    // Compute farthest point and its distance to line first-last
    const auto &p0 = poly[first];
    const auto &p1 = poly[last];
    Vector v = p1 - p0; // Direction vector from p0 to p1
    unitize(v);         // Make it a unit vector
    farthest = first;
    dist = -1;
    for (unsigned i = first + 1; i < last; i++) {
        const auto &p = poly[i];

        if (!p.hasEdge()) { ///< Ignore edge points
            PointN<float, 3> pr =
                p0 + ((p - p0) * v) * v;    // p projected onto line p0-p1
            double d = double(pr.dist2(p)); // Squared distance
            if (d > dist) {                 // Keep index of the farthest point
                farthest = i;
                dist = d;
            }
        }
    }
}
