#ifndef APPLICATION_H
#define APPLICATION_H

#include "curvedraw.h"
#include "meshtypes.h"
#include "utils/utils.h"

#include <string>

class Application {

    struct CurrentState {
        bool drawVisible;
        bool meshVisible;

        CurrentState();
    };

  public:
    Application();

    virtual ~Application();

    Status loadMesh(std::string filename);
    Status showMesh();
    inline bool isShowingMesh() const { return state.meshVisible; }
    Status hideMesh();

    inline PMesh *getMesh() { return &mesh; }

    inline CurveDraw *getDrawer() { return &drawer; }

    void drawMesh() const;

  private:
    PMesh mesh;
    CurveDraw drawer;

    CurrentState state;
};

#endif // APPLICATION_H
