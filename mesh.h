#ifndef MESH_H
#define MESH_H

#include "meshtypes.h"
#include "utils/utils.h"

class Mesh {
  public:
    Mesh();
    ~Mesh();

    Status load(std::string filename);

    void draw() const;

    inline PMesh *get() { return &mesh; }

    inline CMesh *getTrimesh() { return &trimesh; }

    inline const PMesh *get() const { return &mesh; }

    inline const CMesh *getTrimesh() const { return &trimesh; }

  private:
    PMesh mesh;
    CMesh trimesh;
};

#endif // MESH_H
