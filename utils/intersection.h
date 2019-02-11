#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "meshtypes.h"
#include "vcg/space/line3.h"
#include "vcg/space/point3.h"

/*!
     Computes the intersection between a Ray and a Mesh. Returns a 3D Pointset,
   baricentric's coordinates
     and a pointer of intersected face.
*/
// template <typename TriMeshType, class ScalarType>
bool intersectRayMesh(
    /* Input Mesh */ PMesh *m,
    /* Ray */ const vcg::Line3<PMesh::ScalarType> &ray,
    /* Intersect Point */ vcg::Point3<PMesh::ScalarType> &hitPoint,
    /* Baricentric coord 1*/ PMesh::ScalarType &bar1,
    /* Baricentric coord 2*/ PMesh::ScalarType &bar2,
    /* Baricentric coord 3*/ PMesh::ScalarType &bar3,
    /* FacePointer */ PMesh::FacePointer &fp);

#endif // INTERSECTION_H
