#pragma once
#include "../../include/vec3.hpp"
#include <fstream>
#include <sstream>
#include <string>

class ReadObj {
  public:
    ReadObj(const std::string &filename) { loadOBJ(filename); }

    const std::vector<vec3> &getVertices() const { return vertices; }

    const std::vector<point3> &getFaces() const { return faces; }

  private:
    std::vector<vec3> vertices;
    std::vector<point3> faces;

    void loadOBJ(const std::string &filename);
};
