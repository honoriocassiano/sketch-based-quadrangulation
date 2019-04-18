#ifndef SKETCHVERTEX_H
#define SKETCHVERTEX_H

#include "curvendimension/vectorn.hpp"
#include "meshtypes.h"

template <typename MeshType>
class SketchVertex : public PointN<typename MeshType::ScalarType, 3> {

    typedef typename MeshType::FacePointer FacePointer;
    typedef typename MeshType::ScalarType Scalar;

  public:
    SketchVertex(Scalar x = 0, Scalar y = 0, Scalar z = 0,
                 FacePointer face = nullptr, int edge = -1)
        : PointN<Scalar, 3>(x, y, z), face(face), edge(edge) {}

    SketchVertex(const vcg::Point3<Scalar> &point, FacePointer face = nullptr,
                 int edge = -1)
        : PointN<Scalar, 3>(point.X(), point.Y(), point.Z()), face(face),
          edge(edge) {}

    SketchVertex(const VectorN<Scalar> &point, FacePointer face = nullptr,
                 int edge = -1)
        : PointN<Scalar, 3>(point), face(face), edge(edge) {}

    inline bool hasEdge() const { return edge >= 0; }
    inline int getEdge() const { return edge; }

    inline const FacePointer getFace() const { return face; }
    inline FacePointer getFace() { return face; }

    void setData(FacePointer face, int edge = -1) {
        this->face = face;
        this->edge = edge;
    }

    inline operator vcg::Point3<Scalar>() const {
        return vcg::Point3<Scalar>(this->coord[0], this->coord[1],
                                   this->coord[2]);
    }

  private:
    FacePointer face;
    int edge;
};

#endif
