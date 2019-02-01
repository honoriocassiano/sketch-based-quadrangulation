#ifndef APPLICATION_H
#define APPLICATION_H

#include "meshtypes.h"

#include <string>
#include <utils.h>

class Application {
  public:
    Application();

    virtual ~Application();

    Status loadMesh(std::string filename);

  private:
    PMesh mesh;
};

#endif // APPLICATION_H
