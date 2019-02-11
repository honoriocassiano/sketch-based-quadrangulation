#ifndef CURVEDRAW2_H
#define CURVEDRAW2_H

#include <vector>

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
    std::vector<vcg::Point3<PMesh::ScalarType>> getPoints() const;

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
     * \param face face that contains the point
     */
    void addPoint(const vcg::Point3<PMesh::ScalarType> &point,
                  PMesh::FacePointer face);

    /*!
     * \brief Deactivate drawing mode
     */
    void endDraw();

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

  private:
    bool drawMode;
    std::vector<PMesh::FacePointer> faces;
    std::vector<vcg::Point3<PMesh::ScalarType>> addedPoints;
};

#endif // CURVEDRAW2_H
