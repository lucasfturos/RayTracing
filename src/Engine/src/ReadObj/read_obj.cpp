#include "read_obj.hpp"

void ReadObj::loadOBJ(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Falha ao abrir o arquivo: " + filename);
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "v") {
            vec3 vertex;
            iss >> vertex.e[0] >> vertex.e[1] >> vertex.e[2];
            vertices.push_back(vertex);
        } else if (token == "f") {
            point3 face;
            iss >> face.e[0] >> face.e[1] >> face.e[2];
            faces.push_back(face);
        }
    }

    file.close();
}
