#ifndef VIEWER_H
#define VIEWER_H

#include "utils/standardCamera.h"
#include "utils/utils.h"
#include <QCoreApplication>
#include <QGLViewer/qglviewer.h>

#include "application.h"

namespace qgl = qglviewer;

class CurveDraw {

  public:
    CurveDraw();
    virtual ~CurveDraw();

    /*!
     * \brief Return current state of drawing mode
     * \return state of drawing
     */
    bool isDrawing() const;

    /*!
     * \brief Get all points added to drawing
     * \return vector containing the points
     */
    QVector<qgl::Vec> getPoints() const;

    /*!
     * \brief Start draw mode after calls CurveDraw::reset()
     * \param firstPoint first point to add to drawing
     * \sa CurveDraw::reset()
     */
    void startDraw(const qgl::Vec &firstPoint);

    /*!
     * \brief Add a point to drawing. If drawing mode is not active, the point
     * is ignored
     * \param point point to add
     */
    void addPoint(const qgl::Vec &point);

    /*!
     * \brief Deactivate drawing mode
     */
    void endDraw();

    /*!
     * \brief Return the intersection with plane z=0 using the camera and a
     * position relative to window
     * \param cam current camera
     * \param ip position relative to window
     * \return point of collision with plane z=0
     */
    qgl::Vec getMousePosition3D(const qglviewer::Camera *cam,
                                const QPoint &ip) const;

    /*!
     * \brief Delete all added points (but not deactivate drawing mode)
     * \sa CurveDraw::endDraw()
     */
    void reset();

  private:
    bool drawMode;
    QVector<qgl::Vec> points;
};

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

    void mouseMoveEvent(QMouseEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
    //    virtual QString helpString() const;

    //    /// keyboard and mouse event callbacks
    //    virtual void keyPressEvent(QKeyEvent *e);

  signals:

  public slots:

  private:
    Application app;
    CurveDraw drawer;
};

#endif // VIEWER_H
