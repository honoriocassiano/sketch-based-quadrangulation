#include "glutils.h"

vcg::Point2<PMesh::ScalarType>
getProjectedPoint(vcg::Point3<PMesh::ScalarType> point, float mvpMatrix[16]) {
    float wp = mvpMatrix[3] * point.X() + mvpMatrix[7] * point.Y() +
               mvpMatrix[11] * point.Z() + mvpMatrix[15];

    auto z = (mvpMatrix[2] * point.X() + mvpMatrix[6] * point.Y() +
              mvpMatrix[10] * point.Z() + mvpMatrix[14]) /
             wp;

    /// Get the transformed points
    auto x = (mvpMatrix[0] * point.X() + mvpMatrix[4] * point.Y() +
              mvpMatrix[8] * point.Z() + mvpMatrix[12]) /
             (wp * z);
    auto y = (mvpMatrix[1] * point.X() + mvpMatrix[5] * point.Y() +
              mvpMatrix[9] * point.Z() + mvpMatrix[13]) /
             (wp * z);

    return vcg::Point2<PMesh::ScalarType>(x, y);
}
