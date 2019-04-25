#ifndef SKETCHCURVE_H
#define SKETCHCURVE_H

#include "curvendimension/curven.hpp"

#include "sketchvertex.h"

class SketchCurve : public PolygonalCurve<float, 3, SketchVertex<CMesh>> {

    struct DPitem {
        unsigned first;    ///< index of the first vertex of the range
        unsigned last;     ///< index of the last vertex of the range
        unsigned farthest; ///< index between first and last which
        /// is farthest from the line segment defined by the vertices
        /// first and last
        double dist; ///< squared perpendicular distance from the line segment

        /// \brief Constructor.
        ///
        /// Builds an item corresponding to the a vertex range of a polygonal
        /// curve.
        ///
        /// \param f  index of first point in poly
        /// \param l  index of last point in poly
        /// \param poly polygonal curve which is being generalized
        DPitem(unsigned f, unsigned l, const SketchCurve &poly);

        /// Operator <
        inline bool operator<(const DPitem &other) const {
            return dist < other.dist;
        }
    };

  public:
    SketchCurve();

    SketchCurve(const PolygonalCurve<float, 3, SketchVertex<CMesh>> &curve);

    virtual ~SketchCurve();

    std::vector<vcg::Point3<float>> getVCGPoints() const;

    SketchVertex<CMesh> &front();

    const SketchVertex<CMesh> &front() const;

    SketchVertex<CMesh> &back();

    const SketchVertex<CMesh> &back() const;

    //!
    //! \brief Performs a Douglas-Peucker analysis of the curve ignoring edge
    //! points
    //! \param tol
    //! \return
    //!
    std::vector<int> douglasPeuckerRankIgnoreEdges(float tol) const;

    void douglasPeuckerSimplifyIgnoreEdges(SketchCurve &result,
                                           float tol) const;
};

#endif
