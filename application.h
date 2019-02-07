#ifndef APPLICATION_H
#define APPLICATION_H

#include "meshtypes.h"

#include <string>
#include <utils/utils.h>

class Application {
  public:
    Application();

    virtual ~Application();

    Status loadMesh(std::string filename);

    PMesh *getMesh();

    void drawMesh() const;

  private:
    PMesh mesh;
};

#endif // APPLICATION_H
