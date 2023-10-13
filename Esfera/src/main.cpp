#include "./Render/render.hpp"

int main() {
    auto render{make_shared<Render>()};
    render->run();
    // render->run_ppm();
    // render->run_term();

    return EXIT_SUCCESS;
}
