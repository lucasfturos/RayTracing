#include "read_object_file.hpp"

ReadObjectFile::ReadObjectFile(const std::string &filePath) {
    loadFile(filePath);
}

void ReadObjectFile::reset() {
    vertices.clear();
    faces.clear();
    textures.clear();
    normals.clear();
}

void ReadObjectFile::parseLine(const std::string &line) {
    if (line.empty()) {
        return;
    }

    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "v") {
        glm::vec3 vertex;
        iss >> vertex.x >> vertex.y >> vertex.z;
        vertices.push_back(vertex);
    } else if (type == "f") {
        glm::ivec3 face;
        char slash;
        if (line.find('/') != std::string::npos) {
            iss >> face.x >> slash >> face.y >> slash >> face.z;
        } else {
            iss >> face.x >> face.y >> face.z;
            face.x--;
            face.y--;
            face.z--;
        }
        faces.push_back(face);
    } else if (type == "vt") {
        glm::vec2 texture;
        iss >> texture.x >> texture.y;
        textures.push_back(texture);
    } else if (type == "vn") {
        glm::vec3 normal;
        iss >> normal.x >> normal.y >> normal.z;
        normals.push_back(normal);
    }
}

void ReadObjectFile::loadFile(const std::string &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::invalid_argument(
            std::string("Failed to open file: " + filePath));
        return;
    }

    reset();

    std::string line;
    while (std::getline(file, line)) {
        parseLine(line);
    }

    file.close();
}

ObjectProgramSource ReadObjectFile::sources() {
    return {vertices, faces, normals, textures};
}
