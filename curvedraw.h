#ifndef CURVEDRAW2_H
#define CURVEDRAW2_H

#include <vector>

#include "curvendimension/curven.hpp"
#include "meshtypes.h"
#include "utils/standardCamera.h"
#include "utils/utils.h"

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
    std::vector<vcg::Point3<CMesh::ScalarType>> getPoints() const;

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
    inline std::size_t getSize() const { return addedPoints.size(); }

    /*!
     * \brief Delete all added points (but not deactivate drawing mode)
     * \sa CurveDraw::endDraw()
     */
    void reset();

    /*!
     * \brief Reduce the number of points in curve
     * \param tol tolerance to use on Douglas-Peucker algorithm
     */
    void simplify(float tol);

  private:
    void addPoint(CMesh *mesh, const vcg::Point3<CMesh::ScalarType> &point,
                  CMesh::FacePointer face,
                  const vcg::Point3<CMesh::ScalarType> &viewDir,
                  float viewProjectionMatrix[16], bool lastPoint);

    bool getCurvePointsBetween(
        CMesh *mesh, const vcg::Point3<CMesh::ScalarType> &p1,
        CMesh::FacePointer f1, const vcg::Point3<CMesh::ScalarType> &p2,
        CMesh::FacePointer f2, const vcg::Point3<CMesh::ScalarType> &viewDir,
        float mvpMatrix[16],
        std::vector<vcg::Point3<CMesh::ScalarType>> &points);

  private:
    bool loop;
    bool drawMode;
    std::vector<CMesh::FacePointer> faces;

    /*!
     * \brief Maps addedPoints to curvePoints
     *
     * At position i, the value is the corresponding position of vertex
     * addedPoints[i] in curvePoints, i.e., addedPoints[i] ==
     * curvePoints[CMesh[i]]
     */
    std::vector<std::size_t> pointsMap;

    /*!
     * \brief Points to be rendered
     *
     * If two consecutive points in addedPoints belongs to different faces, one
     * or more new points are added on every edge the intersect the line between
     * them
     */
    std::vector<vcg::Point3<CMesh::ScalarType>> curvePoints;

    /*!
     * \brief Points created by the drawing
     */
    std::vector<vcg::Point3<CMesh::ScalarType>> addedPoints;

    PolygonalCurve<float, 3> curve;
};

#endif // CURVEDRAW2_H
