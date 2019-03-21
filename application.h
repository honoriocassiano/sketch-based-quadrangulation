#ifndef APPLICATION_H
#define APPLICATION_H

#include "curvedraw.h"
#include "mesh.h"

#include "meshtypes.h"
#include "utils/utils.h"

#include <string>

class Application {

    struct CurrentState {
        bool drawVisible;
        bool meshVisible;
        bool patchVisible;

        CurrentState();
    };

  public:
    Application();

    virtual ~Application();

    /*!
     * \brief Load the mesh from a file. This method does not set the mesh state
     * as visible
     * \param filename File name
     * \return STATUS_OK if success or a Status containing an error description
     */
    Status loadMesh(std::string filename);

    /*!
     * \brief Set the mesh state as visible to the next draw loop
     * \return STATUS_OK if success or an error if the mesh if empty
     */
    Status showMesh();

    Status switchShowingMesh();

    inline bool isShowingMesh() const { return state.meshVisible; }

    /*!
     * \brief Set the mesh state as invisible to the next draw loop
     * \return STATUS_OK
     */
    Status hideMesh();

    inline Mesh *getMesh() { return &mesh; }

    Status showPatches();
    Status hidePatches();
    Status switchShowingPatches();

    inline bool isShowingPatches() const { return state.meshVisible; }

    Status showDrawing();
    Status switchShowingDrawing();
    Status hideDrawing();
    inline bool isShowingDrawing() const { return state.drawVisible; }

    inline CurveDraw *getDrawer() { return &drawer; }

    /*!
     * \brief Draw the scene based on current state
     */
    void draw();

  private:
    //    PMesh mesh;
    //    CMesh trimesh;
    Mesh mesh;

    CurveDraw drawer;

    CurrentState state;
};

#endif // APPLICATION_H
