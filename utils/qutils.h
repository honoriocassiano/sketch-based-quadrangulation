#ifndef QUTILS_H
#define QUTILS_H

#include "QGLViewer/vec.h"
#include <QDebug>

#include "vcg/space/point3.h"

QDebug operator<<(QDebug dbg, const qglviewer::Vec &c) {
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "(" << c.x << ", " << c.y << ", " << c.z << ")";

    return dbg;
}

vcg::Point3<float> qtToVCG(const qglviewer::Vec &v) {
    return vcg::Point3<float>(v.x, v.y, v.z);
}

qglviewer::Vec vcgToQT(const vcg::Point3<float> &v) {
    return qglviewer::Vec(v.X(), v.Y(), v.Z());
}

#endif // QUTILS_H
