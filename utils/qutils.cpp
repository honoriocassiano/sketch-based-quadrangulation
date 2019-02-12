#include "qutils.h"

QDebug operator<<(QDebug dbg, const qglviewer::Vec &c) {
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "(" << c.x << ", " << c.y << ", " << c.z << ")";

    return dbg;
}

QDebug operator<<(QDebug dbg, const vcg::Point3<PMesh::ScalarType> &p) {
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "(" << p.X() << ", " << p.Y() << ", " << p.Z() << ")";

    return dbg;
}

vcg::Point3<float> qtToVCG(const qglviewer::Vec &v) {
    return vcg::Point3<float>(v.x, v.y, v.z);
}

qglviewer::Vec vcgToQT(const vcg::Point3<float> &v) {
    return qglviewer::Vec(v.X(), v.Y(), v.Z());
}

/*!
 * \brief Return the intersection with plane z=0 using the camera and a
 * position relative to window
 * \param cam current camera
 * \param ip position relative to window
 * \return point of collision with plane z=0
 */
vcg::Point3<PMesh::ScalarType> getMousePosition3D(const qglviewer::Camera *cam,
                                                  const QPoint &ip) {
    qglviewer::Vec point(ip.x(), ip.y(), 0.5);
    point = cam->unprojectedCoordinatesOf(point);
    qglviewer::Vec dir = point - cam->position();
    dir.normalize();
    // parameter of the plane where we are goint to project
    // see
    // http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
    qglviewer::Vec normalPlane = cam->viewDirection();
    qreal nl = normalPlane * dir;
    assert(nl > 1e-8);
    qglviewer::Vec l0 = cam->position();
    //        qglviewer::Vec p0 = l0 + 0.5 * normalPlane;
    qglviewer::Vec p0 = l0 + normalPlane;
    qglviewer::Vec p0l0 = p0 - l0;
    qreal distance = p0l0 * normalPlane / nl;
    dir = distance * dir;
    qglviewer::Vec pos = l0 + dir;

    return qtToVCG(pos);
}
