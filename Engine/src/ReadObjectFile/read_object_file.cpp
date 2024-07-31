#include "read_object_file.hpp"
#include <algorithm>

ReadObjectFile::ReadObjectFile(const std::string &filePath) {
    loadFile(filePath);
}

void ReadObjectFile::reset() {
    vertices.clear();
    faces.clear();
    normals.clear();
    textures.clear();
}

void ReadObjectFile::parseLine(const std::string &line) {
    if (line.empty() || line[0] == '#') {
        return;
    }

    std::istringstream iss(line);
    std::string type;
    iss >> type;

    if (type == "v") {
        vec3 vertex;
        iss >> vertex.x >> vertex.y >> vertex.z;
        vertices.push_back(vertex);
    } else if (type == "f") {
        parseFace(line, iss);
    } else if (type == "vn") {
        vec3 normal;
        iss >> normal.x >> normal.y >> normal.z;
        normals.push_back(normal);
    } else if (type == "vt") {
        vec2 texture;
        iss >> texture.x >> texture.y;
        textures.push_back(texture);
    }
}

void ReadObjectFile::parseFace(const std::string &line,
                               std::istringstream &iss) {
    ivec3 face;
    char slash;
    ivec3 v, t, n;

    int slashCount = std::count(line.begin(), line.end(), '/');
    try {
        if (slashCount == 6) {
            iss >> v.x >> slash >> t.x >> slash >> n.x >> v.y >> slash >> t.y >>
                slash >> n.y >> v.z >> slash >> t.z >> slash >> n.z;
        } else if (slashCount == 3) {
            iss >> v.x >> slash >> t.x >> v.y >> slash >> t.y >> v.z >> slash >>
                t.z;
        } else if (slashCount == 2) {
            iss >> v.x >> slash >> n.x >> v.y >> slash >> n.y >> v.z >> slash >>
                n.z;
        } else {
            iss >> v.x >> v.y >> v.z;
        }
        face.x = v.x - 1;
        face.y = v.y - 1;
        face.z = v.z - 1;
    } catch (const std::exception &e) {
        std::cerr << "Error parsing face line: " << line
                  << " Exception: " << e.what() << '\n';
    }

    faces.push_back(face);
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

    if (file.bad()) {
        throw std::runtime_error("Error reading file: " + filePath);
    }

    file.close();
}

ObjectProgramSource ReadObjectFile::sources() const {
    return {vertices, faces, normals, textures};
}
