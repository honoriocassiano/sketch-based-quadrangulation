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

bool intersectRayMesh(
    /* Input Mesh */ CMesh *m,
    /* Ray */ const vcg::Line3<typename CMesh::ScalarType> &ray,
    /* Intersect Point */ vcg::Point3<typename CMesh::ScalarType> &hitPoint,
    /* Baricentric coord 1*/ typename CMesh::ScalarType &bar1,
    /* Baricentric coord 2*/ typename CMesh::ScalarType &bar2,
    /* Baricentric coord 3*/ typename CMesh::ScalarType &bar3,
    /* FacePointer */ typename CMesh::FacePointer &fp);

#endif // INTERSECTION_H
