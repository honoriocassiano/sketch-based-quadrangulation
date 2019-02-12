#ifndef QUTILS_H
#define QUTILS_H

#include "QGLViewer/vec.h"
#include <QDebug>

#include "QGLViewer/camera.h"
#include "meshtypes.h"
#include "vcg/space/point3.h"

QDebug operator<<(QDebug dbg, const qglviewer::Vec &c);

QDebug operator<<(QDebug dbg, const vcg::Point3<PMesh::ScalarType> &p);

vcg::Point3<float> qtToVCG(const qglviewer::Vec &v);

qglviewer::Vec vcgToQT(const vcg::Point3<float> &v);

/*!
 * \brief Return the intersection with plane z=0 using the camera and a
 * position relative to window
 * \param cam current camera
 * \param ip position relative to window
 * \return point of collision with plane z=0
 */
vcg::Point3<PMesh::ScalarType> getMousePosition3D(const qglviewer::Camera *cam,
                                                  const QPoint &ip);

#endif // QUTILS_H
