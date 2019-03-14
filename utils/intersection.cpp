#include "intersection.h"

#include "qdebug.h"

#include <vcg/complex/algorithms/intersection.h>

// using ScalarType = PMesh::ScalarType;

bool intersectRayMesh(
    /* Input Mesh */ CMesh *m,
    /* Ray */ const vcg::Line3<typename CMesh::ScalarType> &ray,
    /* Intersect Point */ vcg::Point3<typename CMesh::ScalarType> &hitPoint,
    /* Baricentric coord 1*/ typename CMesh::ScalarType &bar1,
    /* Baricentric coord 2*/ typename CMesh::ScalarType &bar2,
    /* Baricentric coord 3*/ typename CMesh::ScalarType &bar3,
    /* FacePointer */ typename CMesh::FacePointer &fp) {
    // typedef typename TriCMesh::FaceContainer FaceContainer;

    using ScalarType = typename CMesh::ScalarType;

    typename CMesh::FaceIterator fi;
    bool hit = false;

    if (m == nullptr) {
        return false;
    }

    ScalarType lastDist = INFINITY;
    ScalarType dist;
    vcg::Point3<ScalarType> p1;
    vcg::Point3<ScalarType> p2;
    vcg::Point3<ScalarType> p3;

    for (fi = m->face.begin(); fi != m->face.end(); ++fi) {
        p1 = vcg::Point3<ScalarType>((*fi).P(0).X(), (*fi).P(0).Y(),
                                     (*fi).P(0).Z());
        p2 = vcg::Point3<ScalarType>((*fi).P(1).X(), (*fi).P(1).Y(),
                                     (*fi).P(1).Z());
        p3 = vcg::Point3<ScalarType>((*fi).P(2).X(), (*fi).P(2).Y(),
                                     (*fi).P(2).Z());
        if (vcg::IntersectionLineTriangle<ScalarType>(ray, p1, p2, p3, dist,
                                                      bar1, bar2)) {
            bar3 = (1 - bar1 - bar2);

            if (dist < lastDist) {
                hitPoint = p1 * bar3 + p2 * bar1 + p3 * bar2;
                fp = &(*fi);
                hit = true;
                lastDist = dist;
            }
        }
    }

    return hit;
}
