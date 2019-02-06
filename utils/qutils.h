#ifndef QUTILS_H
#define QUTILS_H

#include "QGLViewer/vec.h"
#include <QDebug>

QDebug operator<<(QDebug dbg, const qglviewer::Vec &c) {
    QDebugStateSaver saver(dbg);
    dbg.nospace() << "(" << c.x << ", " << c.y << ", " << c.z << ")";

    return dbg;
}

#endif // QUTILS_H
