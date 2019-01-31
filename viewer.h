#ifndef VIEWER_H
#define VIEWER_H

#include <QCoreApplication>
#include <QGLViewer/qglviewer.h>

//#include <vcg/complex/complex.h>
#include <vcg/complex/algorithms/clean.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/create/platonic.h>
#include <vcg/complex/algorithms/curve_on_manifold.h>
#include <vcg/complex/algorithms/geodesic.h>
#include <vcg/complex/algorithms/parametrization/poisson_solver.h>
#include <vcg/complex/algorithms/refine.h>
#include <vcg/complex/algorithms/refine_loop.h>
#include <vcg/complex/algorithms/update/bounding.h>
#include <vcg/complex/algorithms/update/color.h>
#include <vcg/complex/algorithms/update/normal.h>
#include <vcg/complex/algorithms/update/selection.h>
#include <vcg/complex/algorithms/update/texture.h>
#include <vcg/complex/algorithms/update/topology.h>
/* include the support for half edges */
//#include <vcg/complex/algorithms/update/halfedge_indexed.h>

class Viewer : public QGLViewer {
    Q_OBJECT
  public:
    explicit Viewer(QWidget *parent = nullptr);

    virtual ~Viewer();

  signals:

  public slots:
};

#endif // VIEWER_H
