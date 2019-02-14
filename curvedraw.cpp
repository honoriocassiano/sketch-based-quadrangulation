#include "curvedraw.h"
#include "utils/glutils.h"
#include "vcg/simplex/face/pos.h"
#include "vcg/space/intersection2.h"
#include "wrap/gl/space.h"

// TODO Remove these includes
#include "qdebug.h"
#include "utils/qutils.h"

namespace qgl = qglviewer;

#define CROSS(dest, v1, v2)                                                    \
    {                                                                          \
        dest[0] = v1[1] * v2[2] - v1[2] * v2[1];                               \
        dest[1] = v1[2] * v2[0] - v1[0] * v2[2];                               \
        dest[2] = v1[0] * v2[1] - v1[1] * v2[0];                               \
    }

/*
 CURVEDRAW CLASS DEFINITIONS
*/

CurveDraw::CurveDraw() : drawMode(false) {}

CurveDraw::~CurveDraw() {}

bool CurveDraw::isDrawing() const { return drawMode; }

std::vector<vcg::Point3<PMesh::ScalarType>> CurveDraw::getPoints() const {
    return addedPoints;
}

void CurveDraw::draw() const {
    glDisable(GL_LIGHTING);

    glColor3f(0, 0, 0);

    glLineWidth(2.5);
    glBegin(GL_LINE_STRIP);

    for (const auto &p : curvePoints) {
        vcg::glVertex(p);
    }

    glEnd();

    glEnable(GL_LIGHTING);
}

void CurveDraw::startDraw() {
    reset();
    drawMode = true;
}

void CurveDraw::addPoint(const vcg::Point3<PMesh::ScalarType> &point,
                         PMesh::FacePointer face,
                         const vcg::Point3<PMesh::ScalarType> &viewDir,
                         float mvpMatrix[16]) {

    if (drawMode) {

        //        qDebug() << "PROJECTED: " << vcg::Point3<PMesh::ScalarType>(x,
        //        y, z);
        auto pathExists = true;

        if (faces.empty() || face == faces.back()) {
            pointsMap.push_back(curvePoints.size());

            curvePoints.push_back(point);
        } else {

            std::vector<vcg::Point3<PMesh::ScalarType>> temp;

            auto firstFace = faces.back();
            auto currentFace = firstFace;

            auto pStartPoint = getProjectedPoint(addedPoints.back(), mvpMatrix);
            auto pEndPoint = getProjectedPoint(point, mvpMatrix);

            auto lastEdge = -1;

            // Find the first edge
            {
                /// Projected points
                auto projV0 =
                    getProjectedPoint(currentFace->V(0)->P(), mvpMatrix);
                auto projV1 =
                    getProjectedPoint(currentFace->V(1)->P(), mvpMatrix);
                auto projV2 =
                    getProjectedPoint(currentFace->V(2)->P(), mvpMatrix);

                /// Projected drawed segment
                auto projSegment =
                    vcg::Segment2<PMesh::ScalarType>(pStartPoint, pEndPoint);

                /// Projected intersection point to check on 2D orthogonal to
                /// viewDir
                vcg::Point2<PMesh::ScalarType> projIntersection;

                /// Projected intersection point to check on 3D
                vcg::Point3<PMesh::ScalarType> intersection;

                /// Edge points on 3D space
                vcg::Point3<PMesh::ScalarType> edgePoint1;
                vcg::Point3<PMesh::ScalarType> edgePoint2;

                /// Projected edges
                auto projSeg01 =
                    vcg::Segment2<PMesh::ScalarType>(projV0, projV1);
                auto projSeg12 =
                    vcg::Segment2<PMesh::ScalarType>(projV1, projV2);

                /// Check the intersection on 2D plane
                if (vcg::SegmentSegmentIntersection(projSegment, projSeg01,
                                                    projIntersection)) {
                    lastEdge = 0;

                    edgePoint1 = currentFace->V(0)->P();
                    edgePoint2 = currentFace->V(1)->P();

                } else if (vcg::SegmentSegmentIntersection(
                               projSegment, projSeg12, projIntersection)) {
                    lastEdge = 1;

                    edgePoint1 = currentFace->V(1)->P();
                    edgePoint2 = currentFace->V(2)->P();
                } else {
                    lastEdge = 2;

                    edgePoint1 = currentFace->V(2)->P();
                    edgePoint2 = currentFace->V(0)->P();
                }

                /// Get the intersection point on 3D space

                /// Plane that cut the face. That plane passes through drawed
                /// vertices with direction=viewDir
                vcg::Plane3<PMesh::ScalarType> plane;

                /// Normal to the plane
                vcg::Point3<PMesh::ScalarType> normal;

                /// Line formed by the edge
                vcg::Line3<PMesh::ScalarType> edgeLine(
                    edgePoint1, (edgePoint2 - edgePoint1));

                CROSS(normal, viewDir, (addedPoints.back() - point));

                plane.Init(point, normal);

                /// Calculate the intersection point on 3D space
                vcg::IntersectionPlaneLine(plane, edgeLine, intersection);

                /// Add point to possible curve points
                temp.push_back(intersection);
            }

            auto pos = vcg::face::Pos<PMesh::FaceType>(currentFace, lastEdge);

            /// Flip to adjacent face
            pos.FlipF();

            /// Get new face and the edge index on this face
            currentFace = pos.F();
            lastEdge = pos.E();

            auto projLine = vcg::Line2<PMesh::ScalarType>(
                pStartPoint, pEndPoint - pStartPoint);

            while (currentFace != face) {

                if (currentFace == nullptr) {
                    // TODO Add status return
                    pathExists = false;
                    break;
                }

                qDebug() << "Face: " << currentFace;

                int nextEdge;

                /// Projected intersection point to check on 2D orthogonal to
                /// viewDir
                vcg::Point2<PMesh::ScalarType> projIntersection;

                /// Projected intersection point to check on 3D
                vcg::Point3<PMesh::ScalarType> intersection;

                /// Edge points on 3D space
                vcg::Point3<PMesh::ScalarType> edgePoint1;
                vcg::Point3<PMesh::ScalarType> edgePoint2;

                /// Projected edges
                vcg::Segment2<PMesh::ScalarType> projEdge;

                projEdge.Set(
                    getProjectedPoint(currentFace->V(lastEdge)->P(), mvpMatrix),
                    getProjectedPoint(
                        currentFace->V((lastEdge + 1) % currentFace->VN())->P(),
                        mvpMatrix));

                if (vcg::LineSegmentIntersection(projLine, projEdge,
                                                 projIntersection)) {
                    nextEdge = (lastEdge + 1) % currentFace->VN();

                    edgePoint1 = currentFace->V(lastEdge)->P();
                    edgePoint2 =
                        currentFace->V((lastEdge + 1) % currentFace->VN())->P();

                } else {
                    nextEdge = (lastEdge + 2) % currentFace->VN();

                    edgePoint1 = currentFace->V(lastEdge)->P();
                    edgePoint2 =
                        currentFace->V((lastEdge + 2) % currentFace->VN())->P();
                }

                /// Get the intersection point on 3D space

                /// Plane that cut the face. That plane passes through drawed
                /// vertices with direction=viewDir
                vcg::Plane3<PMesh::ScalarType> plane;

                /// Normal to the plane
                vcg::Point3<PMesh::ScalarType> normal;

                /// Line formed by the edge
                vcg::Line3<PMesh::ScalarType> edgeLine(
                    edgePoint1, (edgePoint2 - edgePoint1));

                CROSS(normal, viewDir, (addedPoints.back() - point));

                plane.Init(point, normal);

                /// Calculate the intersection point on 3D space
                vcg::IntersectionPlaneLine(plane, edgeLine, intersection);

                /// Add point to possible curve points
                temp.push_back(intersection);

                /// New Pos
                pos = vcg::face::Pos<PMesh::FaceType>(currentFace, nextEdge);

                /// Flip to adjacent face
                pos.FlipF();

                /// Get new face and the edge index on this face
                currentFace = pos.F();
                lastEdge = pos.E();
            }

            if (pathExists) {

                temp.push_back(point);

                curvePoints.reserve(curvePoints.size() + temp.size());
                curvePoints.insert(curvePoints.end(), temp.begin(), temp.end());

                pointsMap.push_back(curvePoints.size() - 1);
            }
        }

        if (pathExists) {

            addedPoints.push_back(point);
            faces.push_back(face);
        }
    }
}

void CurveDraw::reset() {
    addedPoints.clear();
    curvePoints.clear();
    faces.clear();
}

void CurveDraw::endDraw() { drawMode = false; }

#undef CROSS
