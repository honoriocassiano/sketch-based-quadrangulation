#ifndef GLUTILS_H
#define GLUTILS_H

#include "meshtypes.h"

vcg::Point2<PMesh::ScalarType>
getProjectedPoint(vcg::Point3<PMesh::ScalarType> point,
                  float modelViewProjectionMatrix[16]);

#endif // GLUTILS_H
