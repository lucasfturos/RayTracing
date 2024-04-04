#pragma once

#include "../../include/vec3.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ObjectProgramSource {
    std::vector<vec3> vertices;
    std::vector<ivec3> faces;
    std::vector<vec3> normals;
};

class ReadObjectFile {
  private:
    std::vector<vec3> vertices;
    std::vector<ivec3> faces;
    std::vector<vec3> normals;

    void reset();
    void parseLine(const std::string &line);
    void loadFile(const std::string &filePath);

  public:
    ReadObjectFile(const std::string &filePath);

    ObjectProgramSource sources();
};