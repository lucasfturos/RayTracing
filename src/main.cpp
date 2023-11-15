#include "Scenes/CornellBox/cornell_box.hpp"
#include "Scenes/Esfera/esfera.hpp"
#include "Scenes/Object/obj.hpp"
#include "Scenes/Render/render.hpp"

using std::vector;

int main(int argc, char *argv[]) {
    try {
        if (argc < 2) {
            throw std::invalid_argument("Informe um modelo para a cena!!!");
        }

        shared_ptr<Render> render;
        auto obj{make_shared<Object>()};
        auto esfera{make_shared<Esfera>()};
        auto room{make_shared<CornellBox>()};
        auto obj_read{make_shared<ReadObj>("obj/teapot.obj")};
        unordered_map<std::string, int> options_scenes{
            {"Esfera", 1},
            {"Room", 2},
            {"Obj", 3},
        };
        unordered_map<std::string, int> options_render{
            {"SDL", 1},
            {"PPM", 2},
            {"Term", 3},
        };
        vector<std::string> input = {
            argv[1],
            argv[2],
        };
        vector<int> idx = {
            options_scenes.count(input[0]) > 0 ? options_scenes[input[0]] : -1,
            options_render.count(input[1]) > 0 ? options_render[input[1]] : -1,
        };

        if (idx[0] == -1 || idx[1] == -1) {
            throw std::invalid_argument("Opção inválida!!!");
        }

        vector<int> opc = {idx[0], idx[1]};
        switch (opc[0]) {
        case 1:
            switch (opc[1]) {
            case 1:
                render = make_shared<Render>(esfera->single_scene(), 1);
                render->run();
                break;
            case 2:
                render = make_shared<Render>(esfera->single_scene(), 0);
                render->run_ppm();
                break;
            case 3:
                render = make_shared<Render>(esfera->single_scene(), 0);
                render->run_term();
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (opc[1]) {
            case 1:
                render = make_shared<Render>(room->cornell_box(), 1);
                render->run();
                break;
            case 2:
                render = make_shared<Render>(room->cornell_box(), 0);
                render->run_ppm();
                break;
            case 3:
                render = make_shared<Render>(room->cornell_box(), 0);
                render->run_term();
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (opc[1]) {
            case 1:
                render = make_shared<Render>(
                    obj->single_scene(obj_read->getVertices(),
                                      obj_read->getFaces()),
                    1);
                render->run();
                break;
            case 2:
                render = make_shared<Render>(
                    obj->single_scene(obj_read->getVertices(),
                                      obj_read->getFaces()),
                    0);
                render->run_ppm();
                break;
            case 3:
                render = make_shared<Render>(
                    obj->single_scene(obj_read->getVertices(),
                                      obj_read->getFaces()),
                    0);
                render->run_term();
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }

    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        std::exit(EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}
