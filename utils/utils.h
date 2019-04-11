#ifndef UTILS_H
#define UTILS_H

#include <string>

struct Status {
    bool result;
    std::string message;

    static Status make(bool r, std::string m) { return Status{r, m}; }

    operator bool() const { return result; }

    static const Status OK;
};

template <typename MeshType> class VertexData {

    typedef typename MeshType::FacePointer FacePointer;

  public:
    VertexData(FacePointer f) {
        face = f;
        edge = -1;
    }
    VertexData(FacePointer f, int e) {
        face = f;
        edge = e;
    }

    inline bool hasEdge() const { return edge >= 0; }

    inline FacePointer getFace() { return face; }
    inline FacePointer getFace() const { return face; }

    inline int getEdge() const { return edge; }

  private:
    FacePointer face;
    int edge;
};

#endif // UTILS_H
