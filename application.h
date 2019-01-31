#ifndef APPLICATION_H
#define APPLICATION_H

#include "meshtypes.h"

#include <qstring.h>

class Application {
  public:
    Application();

    virtual ~Application();

    bool loadMesh(QString filename);

  private:
    PMesh mesh;
};

#endif // APPLICATION_H
