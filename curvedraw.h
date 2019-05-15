#ifndef CURVEDRAW2_H
#define CURVEDRAW2_H

#include <vector>

#include "curvendimension/curven.hpp"
#include "meshtypes.h"
#include "sketchcurve.h"
#include "sketchvertex.h"
#include "utils/standardCamera.h"
#include "utils/utils.h"

class CurveDraw {

  private:
    struct Config {
        float pointSize;
        float lineWidth;

        float lineColor[3];
        float drawPointColor[3];
        float edgePointColor[3];
    };

  public:
    CurveDraw();
    virtual ~CurveDraw();

    /*!
     * \brief Return current state of drawing mode
     * \return state of drawing
     */
    bool isDrawing() const;

    void draw() const;

    /*!
     * \brief Start draw mode after calls CurveDraw::reset()
     * \param firstPoint first point to add to drawing
     * \param face face that contains the point
     * \sa CurveDraw::reset()
     */
    void startDraw();

    /*!
     * \brief Add a point to drawing. If drawing mode is not active, the point
     * is ignored
     * \param point point to add
     * \param viewDir view direction of camera
     * \param face face that contains the point
     */
    void addPoint(CMesh *mesh, const vcg::Point3<CMesh::ScalarType> &point,
                  CMesh::FacePointer face,
                  const vcg::Point3<CMesh::ScalarType> &viewDir,
                  float viewProjectionMatrix[16]);

    /*!
     * \brief Deactivate drawing mode
     */
    void endDraw(CMesh *mesh, const vcg::Point3<CMesh::ScalarType> &viewDir,
                 float viewProjectionMatrix[16], bool loop = true);

    /*!
     * \brief Get the number of points added to draw
     * \return number of points
     */
    inline std::size_t getSize() const { return originalCurve.size(); }

    /*!
     * \brief Delete all added points (but not deactivate drawing mode)
     * \sa CurveDraw::endDraw()
     */
    void reset();

    /*!
     * \brief Reduce the number of points in curve
     *
     * The maximum distance between two points is the curve length/number of
     * points.
     *
     * \param tol tolerance to use on Douglas-Peucker algorithm
     */
    void simplify(float tol);

    /*!
     * \brief Resample the curve by setting the maximum distance between two
     * points to (curve length / number of points)
     *
     * The method also sets the meanDistance member.
     */
    void resample();

    /*!
     * \brief Resample the curve
     *
     * If the distance between two consecutive points if greater than
     * maxDistance, the segment is splitted in by adding
     * ceil(segment distance / maxDistance) points.
     * The method also sets the meanDistance value.
     *
     * \param maxDistance maximum distance between two points
     */
    void resample(float maxDistance);

    float getMeanDistance() const { return meanDistance; }

  private:
    void addPoint(CMesh *mesh, const vcg::Point3<CMesh::ScalarType> &point,
                  CMesh::FacePointer face,
                  const vcg::Point3<CMesh::ScalarType> &viewDir,
                  float viewProjectionMatrix[16], bool lastPoint);

    bool getCurvePointsBetween(CMesh *mesh,
                               const vcg::Point3<CMesh::ScalarType> &p1,
                               CMesh::FacePointer f1,
                               const vcg::Point3<CMesh::ScalarType> &p2,
                               CMesh::FacePointer f2,
                               const vcg::Point3<CMesh::ScalarType> &viewDir,
                               float mvpMatrix[16], SketchCurve &result);

  public:
    static Config config;

  private:
    bool loop;
    float meanDistance;
    bool drawMode;

    SketchCurve originalCurve;
    SketchCurve currentCurve;
};

#endif // CURVEDRAW2_H
