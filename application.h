#ifndef APPLICATION_H
#define APPLICATION_H

#include "meshtypes.h"

#include <string>
#include <utils/utils.h>

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
    Status hideMesh();

    PMesh *getMesh();

    void drawMesh() const;

  private:
    PMesh mesh;

    CurrentState state;
};

#endif // APPLICATION_H
