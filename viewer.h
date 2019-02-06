#ifndef VIEWER_H
#define VIEWER_H

#include "utils/standardCamera.h"
#include "utils/utils.h"
#include <QCoreApplication>
#include <QGLViewer/qglviewer.h>

#include "application.h"

class Viewer : public QGLViewer {
    Q_OBJECT
  public:
    explicit Viewer(QWidget *parent = nullptr);
    ~Viewer() override;

    Application *getApp();

    Status loadMesh(QString filename);

  protected:
    void init() override;
    void draw() override;
    //    virtual void update();
    //    virtual void init();
    //    virtual QString helpString() const;

    //    /// keyboard and mouse event callbacks
    //    virtual void keyPressEvent(QKeyEvent *e);

    //    virtual void mousePressEvent(QMouseEvent *e);
    //    virtual void mouseMoveEvent(QMouseEvent *e);
    //    virtual void mouseReleaseEvent(QMouseEvent *e);

  signals:

  public slots:

  private:
    Application app;
};

#endif // VIEWER_H
