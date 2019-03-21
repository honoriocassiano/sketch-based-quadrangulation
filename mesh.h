#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

#include "meshtypes.h"
#include "quad_tracer.h"
#include "utils/utils.h"

#include "wrap/gl/trimesh.h"

class Mesh {
  public:
    Mesh();
    ~Mesh();

    Status load(std::string filename);

    void draw() const;
    void drawEdges();

    inline PMesh *get() { return &mesh; }

    inline CMesh *getTrimesh() { return &trimesh; }

    inline const PMesh *get() const { return &mesh; }

    inline const CMesh *getTrimesh() const { return &trimesh; }

  private:
    void computeBorderPatch();

  private:
    PMesh mesh;
    CMesh trimesh;

    CMesh borderPatch;

    vcg::GlTrimesh<CMesh> glEdgeMesh;

    QuadMeshTracer<PMesh> tracer;
};

#endif // MESH_H
