#pragma once

#include "vec3.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ObjectProgramSource {
    std::vector<vec3> vertices;
    std::vector<ivec3> faces;
    std::vector<vec3> normals;
    std::vector<vec2> textures;
};

class ReadObjectFile {
  private:
    std::vector<vec3> vertices;
    std::vector<ivec3> faces;
    std::vector<vec3> normals;
    std::vector<vec2> textures;

    void reset();
    void parseLine(const std::string &line);
    void parseFace(const std::string &line, std::istringstream &iss);
    void loadFile(const std::string &filePath);

  public:
    ReadObjectFile(const std::string &filePath);

    ObjectProgramSource sources() const;
};