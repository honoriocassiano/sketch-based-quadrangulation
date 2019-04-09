#include "curvedraw.h"
#include "utils/debug.h"
#include "utils/glutils.h"
#include "vcg/complex/algorithms/update/flag.h"
#include "vcg/simplex/face/pos.h"
#include "vcg/space/intersection2.h"
#include "wrap/gl/space.h"

// TODO Remove these includes

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

std::vector<vcg::Point3<CMesh::ScalarType>> CurveDraw::getPoints() const {
    return addedPoints;
}

void CurveDraw::draw() const {
    glDisable(GL_LIGHTING);

    glColor3f(0, 1, 0);

    glLineWidth(2.5);

    if (loop) {
        glBegin(GL_LINE_LOOP);
    } else {
        glBegin(GL_LINE_STRIP);
    }

    for (const auto &p : curvePoints) {
        vcg::glVertex(p);
    }

    glEnd();

    glDepthFunc(GL_ALWAYS);

    // Draw simplified curve

    glColor3f(1, 0, 0);
    glPointSize(4);

    glBegin(GL_POINTS);

    for (const auto &p : currentCurve.getPoints()) {
        glVertex3f(p.x(), p.y(), p.z());
    }

    glEnd();

    // End

    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
}

void CurveDraw::startDraw() {
    reset();
    loop = false;
    drawMode = true;
}

void CurveDraw::addPoint(CMesh *mesh,
                         const vcg::Point3<CMesh::ScalarType> &point,
                         CMesh::FacePointer face,
                         const vcg::Point3<CMesh::ScalarType> &viewDir,
                         float mvpMatrix[16], bool lastPoint) {
    if (drawMode) {

        if (faces.empty() || face == faces.back()) {
            faces.push_back(face);
            addedPoints.push_back(point);
            curvePoints.push_back(point);

            auto p = PointN<float, 3>(point.V());

            originalCurve.add(p);
            currentCurve.add(p);

            pointsMap.push_back(0);

        } else {

            std::vector<vcg::Point3<CMesh::ScalarType>> points;

            bool pathExists =
                getCurvePointsBetween(mesh, addedPoints.back(), faces.back(),
                                      point, face, viewDir, mvpMatrix, points);

            if (pathExists) {
                if (!lastPoint) {

                    points.push_back(point);

                    auto p = PointN<float, 3>(point.V());

                    originalCurve.add(p);
                    currentCurve.add(p);

                    addedPoints.push_back(point);
                    faces.push_back(face);
                }

                curvePoints.reserve(curvePoints.size() + points.size());
                curvePoints.insert(curvePoints.end(), points.begin(),
                                   points.end());

                pointsMap.push_back(curvePoints.size() - 1);
            }
        }
    }
}

bool CurveDraw::getCurvePointsBetween(
    CMesh *mesh, const vcg::Point3<CMesh::ScalarType> &p1,
    CMesh::FacePointer f1, const vcg::Point3<CMesh::ScalarType> &p2,
    CMesh::FacePointer f2, const vcg::Point3<CMesh::ScalarType> &viewDir,
    float mvpMatrix[], std::vector<vcg::Point3<CMesh::ScalarType>> &points) {

    bool hasPath = true;

    auto firstFace = f1;
    auto currentFace = firstFace;

    auto pStartPoint = getProjectedPoint(p1, mvpMatrix);
    auto pEndPoint = getProjectedPoint(p2, mvpMatrix);

    auto lastEdge = -1;

    // Find the first edge
    {
        /// Projected points
        auto projV0 = getProjectedPoint(currentFace->V(0)->P(), mvpMatrix);
        auto projV1 = getProjectedPoint(currentFace->V(1)->P(), mvpMatrix);
        auto projV2 = getProjectedPoint(currentFace->V(2)->P(), mvpMatrix);

        /// Projected drawed segment
        auto projSegment =
            vcg::Segment2<CMesh::ScalarType>(pStartPoint, pEndPoint);

        /// Projected intersection point to check on 2D orthogonal to
        /// viewDir
        vcg::Point2<CMesh::ScalarType> projIntersection;

        /// Projected intersection point to check on 3D
        vcg::Point3<CMesh::ScalarType> intersection;

        /// Edge points on 3D space
        vcg::Point3<CMesh::ScalarType> edgePoint1;
        vcg::Point3<CMesh::ScalarType> edgePoint2;

        /// Projected edges
        auto projSeg01 = vcg::Segment2<CMesh::ScalarType>(projV0, projV1);
        auto projSeg12 = vcg::Segment2<CMesh::ScalarType>(projV1, projV2);

        /// Check the intersection on 2D plane
        if (vcg::SegmentSegmentIntersection(projSegment, projSeg01,
                                            projIntersection)) {
            lastEdge = 0;

            edgePoint1 = currentFace->V(0)->P();
            edgePoint2 = currentFace->V(1)->P();

        } else if (vcg::SegmentSegmentIntersection(projSegment, projSeg12,
                                                   projIntersection)) {
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
        vcg::Plane3<CMesh::ScalarType> plane;

        /// Normal to the plane
        vcg::Point3<CMesh::ScalarType> normal;

        /// Line formed by the edge
        vcg::Line3<CMesh::ScalarType> edgeLine(edgePoint1,
                                               (edgePoint2 - edgePoint1));

        CROSS(normal, viewDir, (addedPoints.back() - p2));

        plane.Init(p2, normal);

        /// Calculate the intersection point on 3D space
        vcg::IntersectionPlaneLine(plane, edgeLine, intersection);

        /// Add point to possible curve points
        points.push_back(intersection);

        currentFace->SetV();
    }

    auto pos = vcg::face::Pos<CMesh::FaceType>(currentFace, lastEdge);

    /// Flip to adjacent face
    pos.FlipF();

    /// Get new face and the edge index on this face
    currentFace = pos.F();
    lastEdge = pos.E();

    auto projLine =
        vcg::Line2<CMesh::ScalarType>(pStartPoint, pEndPoint - pStartPoint);

    while (currentFace != f2) {

        if (currentFace == nullptr || currentFace->IsV()) {
            Debug() << "No path found between " << p1 << " and " << p2;

            hasPath = false;
            points.clear();
            break;
        }

        /// Set current face as visited
        currentFace->SetV();

        int nextEdge;

        /// Projected intersection point to check on 2D orthogonal to
        /// viewDir
        vcg::Point2<CMesh::ScalarType> projIntersection;

        /// Projected intersection point to check on 3D
        vcg::Point3<CMesh::ScalarType> intersection;

        /// Edge points on 3D space
        vcg::Point3<CMesh::ScalarType> edgePoint1;
        vcg::Point3<CMesh::ScalarType> edgePoint2;

        /// Projected edges
        vcg::Segment2<CMesh::ScalarType> projEdge;

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
        vcg::Plane3<CMesh::ScalarType> plane;

        /// Normal to the plane
        vcg::Point3<CMesh::ScalarType> normal;

        /// Line formed by the edge
        vcg::Line3<CMesh::ScalarType> edgeLine(edgePoint1,
                                               (edgePoint2 - edgePoint1));

        CROSS(normal, viewDir, (addedPoints.back() - p2));

        plane.Init(p2, normal);

        /// Calculate the intersection point on 3D space
        vcg::IntersectionPlaneLine(plane, edgeLine, intersection);

        /// Add point to possible curve points
        points.push_back(intersection);

        /// New Pos
        pos = vcg::face::Pos<CMesh::FaceType>(currentFace, nextEdge);

        /// Flip to adjacent face
        pos.FlipF();

        /// Get new face and the edge index on this face
        currentFace = pos.F();
        lastEdge = pos.E();
    }

    vcg::tri::UpdateFlags<CMesh>::FaceClearV(*mesh);

    return hasPath;
}

void CurveDraw::addPoint(CMesh *mesh,
                         const vcg::Point3<CMesh::ScalarType> &point,
                         CMesh::FacePointer face,
                         const vcg::Point3<CMesh::ScalarType> &viewDir,
                         float mvpMatrix[16]) {

    addPoint(mesh, point, face, viewDir, mvpMatrix, false);
}

void CurveDraw::reset() {
    addedPoints.clear();
    pointsMap.clear();
    curvePoints.clear();
    faces.clear();

    originalCurve.clear();
    currentCurve.clear();
}

void CurveDraw::simplify(float tol) {

    PolygonalCurve<float, 3> result;

    Debug() << "before DP: " << originalCurve.size();

    originalCurve.douglasPeuckerSimplify(result, tol);

    Debug() << "after DP: " << result.size();

    currentCurve = result;

    this->meanDistance = currentCurve.length() / currentCurve.size();
}

void CurveDraw::resample() { resample(this->meanDistance); }

void CurveDraw::resample(float maxDistance) {

    PolygonalCurve<float, 3> result;

    Debug() << "before RES: " << originalCurve.size();

    for (unsigned i = 0; i < originalCurve.size(); ++i) {

        auto p1 = originalCurve[i];
        auto p2 = originalCurve[(i + 1) % originalCurve.size()];

        float dist = p1.dist2(p2);

        result.add(p1);

        if (dist > maxDistance) {
            int nPoints = int(ceil(dist / maxDistance));

            for (int j = 0; j < nPoints; ++j) {

                float a = float(j) / nPoints;

                PointN<float, 3> p = a * p1 + (1 - a) * p2;

                result.add(p);
            }
        }
    }

    Debug() << "after RES: " << result.size();

    currentCurve = result;
}

void CurveDraw::endDraw(CMesh *mesh,
                        const vcg::Point3<CMesh::ScalarType> &viewDir,
                        float mvpMatrix[16], bool _loop) {
    addPoint(mesh, curvePoints.front(), faces.front(), viewDir, mvpMatrix,
             true);

    originalCurve.close(_loop);

    this->meanDistance = originalCurve.length() / originalCurve.size();

    drawMode = false;
    loop = _loop;
}

#undef CROSS
