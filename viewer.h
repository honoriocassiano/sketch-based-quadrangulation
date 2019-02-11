#ifndef VIEWER_H
#define VIEWER_H

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

    void setFocus() { QGLViewer::setFocus(); }

  protected:
    void init() override;
    void draw() override;

    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    //    virtual QString helpString() const;

    //    /// keyboard and mouse event callbacks
    void keyPressEvent(QKeyEvent *e) override;

  signals:
    void notifyStatusBar(QString);

  public slots:

  private:
    Application app;
};

#endif // VIEWER_H
