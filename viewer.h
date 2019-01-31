#ifndef VIEWER_H
#define VIEWER_H

#include <QCoreApplication>
#include <QGLViewer/qglviewer.h>

class Viewer : public QGLViewer {
    Q_OBJECT
  public:
    explicit Viewer(QWidget *parent = nullptr);

    virtual ~Viewer();

  signals:

  public slots:
};

#endif // VIEWER_H
