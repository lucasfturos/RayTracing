#pragma once

#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct ObjectProgramSource {
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;
};

class ReadObjectFile {
  private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> textures;

    void reset();
    void parseLine(const std::string &line);
    void loadFile(const std::string &filePath);

  public:
    ReadObjectFile(const std::string &filePath);

    ObjectProgramSource sources();
};