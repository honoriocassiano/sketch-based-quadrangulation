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
 CURVEDRAW CLASS STATIC MEMBERS
*/

CurveDraw::Config CurveDraw::config = {
    /*pointSize*/ 4,
    /*lineWidth*/ 2.5,
    /*lineColor*/ {0, 1, 0},
    /*drawPointColor*/ {1, 0, 0},
    /*edgePointColor*/ {0, 0, 1}};

/*
 CURVEDRAW CLASS DEFINITIONS
*/

CurveDraw::CurveDraw() : drawMode(false) { glInit(); }

CurveDraw::~CurveDraw() {}

bool CurveDraw::isDrawing() const { return drawMode; }

void CurveDraw::draw() const {
    glDisable(GL_LIGHTING);

    glColor3fv(config.lineColor);

    glLineWidth(config.lineWidth);

    if (loop) {
        glBegin(GL_LINE_LOOP);
    } else {
        glBegin(GL_LINE_STRIP);
    }

    for (const auto &p : originalCurve.getPoints()) {
        vcg::glVertex(p);
    }

    glEnd();

    glDepthFunc(GL_ALWAYS);

    // Draw simplified curve

    glPointSize(config.pointSize);

    glBegin(GL_POINTS);

    for (const auto &p : currentCurve.getPoints()) {
        if (p.hasEdge()) {
            glColor3fv(config.edgePointColor);
        } else {
            glColor3fv(config.drawPointColor);
        }

        glVertex3f(p.x(), p.y(), p.z());
    }

    glEnd();

    // End

    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHTING);
}

void CurveDraw::drawPicking() const {

    glPointSize(config.pointSize * 5);

    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);

    glBegin(GL_POINTS);

    for (unsigned i = 0; i < currentCurve.size(); i++) {

        auto p = currentCurve[i];

        if (!p.hasEdge()) {

            auto index = i + 1;

            //            int r = (index & 0x000000FF) >> 0;
            //            int g = (index & 0x0000FF00) >> 8;
            //            int b = (index & 0x00FF0000) >> 16;

            int r, g, b;

            //            r = g = b = 255;

            r = GLubyte(index / (1 << 16));
            g = GLubyte((index % (1 << 16)) / (1 << 8));
            b = GLubyte(index % (1 << 8));

            glColor3i(r, g, b);
            vcg::glVertex(p);
        }
    }

    //    glColor3ub(255, 255, 255);
    //    //    glVertex3f(0, 0, 0);
    //    glVertex2f(0, 0);

    glEnd();

    glDisable(GL_DEPTH_TEST);
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

        if (originalCurve.isEmpty() || face == originalCurve.back().getFace()) {
            auto p = SketchVertex<CMesh>(point, face);

            originalCurve.add(p);
            currentCurve.add(p);

        } else {

            SketchCurve temp;

            bool pathExists = getCurvePointsBetween(
                mesh, originalCurve.back(), originalCurve.back().getFace(),
                point, face, viewDir, mvpMatrix, temp);

            if (pathExists) {
                if (!lastPoint) {

                    auto p = SketchVertex<CMesh>(point, face);

                    temp.add(p);
                }

                originalCurve.join(temp);
                currentCurve.join(temp);
            }
        }
    }
}

bool CurveDraw::getCurvePointsBetween(
    CMesh *mesh, const vcg::Point3<CMesh::ScalarType> &p1,
    CMesh::FacePointer f1, const vcg::Point3<CMesh::ScalarType> &p2,
    CMesh::FacePointer f2, const vcg::Point3<CMesh::ScalarType> &viewDir,
    float mvpMatrix[], SketchCurve &result) {

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

        CROSS(normal, viewDir, (vcg::Point3<float>(originalCurve.back()) - p2));

        plane.Init(p2, normal);

        /// Calculate the intersection point on 3D space
        vcg::IntersectionPlaneLine(plane, edgeLine, intersection);

        result.add(SketchVertex<CMesh>(intersection, currentFace, lastEdge));

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

            result.clear();
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

        CROSS(normal, viewDir, (vcg::Point3<float>(originalCurve.back()) - p2));

        plane.Init(p2, normal);

        /// Calculate the intersection point on 3D space
        vcg::IntersectionPlaneLine(plane, edgeLine, intersection);

        /// Add point to possible curve points
        result.add(SketchVertex<CMesh>(intersection, currentFace, lastEdge));

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
    originalCurve.clear();
    currentCurve.clear();
}

void CurveDraw::simplify(float tol) {

    SketchCurve result;

    Debug() << "before DP: " << originalCurve.size();

    //    originalCurve.douglasPeuckerSimplify(result, tol);
    originalCurve.douglasPeuckerSimplifyIgnoreEdges(result, tol);

    Debug() << "after DP: " << result.size();

    currentCurve = result;

    this->meanDistance = currentCurve.length() / currentCurve.size();
}

void CurveDraw::resample() { resample(this->meanDistance); }

void CurveDraw::resample(float maxDistance) {

    SketchCurve result;

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

void CurveDraw::glInit() {
    //    glGenFramebuffers(1, &fboSelection);
    //    glGenTextures(1, &texSelection);
    //    glGenRenderbuffers(1, &depthSelection);
}

void CurveDraw::endDraw(CMesh *mesh,
                        const vcg::Point3<CMesh::ScalarType> &viewDir,
                        float mvpMatrix[16], bool _loop) {

    addPoint(mesh, originalCurve.front(), originalCurve.front().getFace(),
             viewDir, mvpMatrix, true);

    originalCurve.close(_loop);

    this->meanDistance = originalCurve.length() / originalCurve.size();

    drawMode = false;
    loop = _loop;
}

#undef CROSS
