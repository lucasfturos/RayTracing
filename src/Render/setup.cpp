#include "render.hpp"

void Render::setupSDL2() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar o SDL: " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }

    win = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED,
                           SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height,
                           SDL_WINDOW_VULKAN);
    if (!win) {
        std::cerr << "Erro ao criar a janela: " << SDL_GetError() << '\n';
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    ren = SDL_CreateRenderer(win, -1,
                             SDL_RENDERER_SOFTWARE | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        std::cerr << "Erro ao criar o renderizador: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(win);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void Render::setupCamera() {
    // Initialize cam
    // point3 lookfrom(26, 3, 6);
    // point3 lookfrom(13, 2, 3);
    // point3 lookfrom(278, 278, -800); // Cornell Box
    // point3 lookfrom(478, 278, -600); // Cornell Box
    point3 lookfrom(0, 2.2, 7.5); // visão de frente
    // point3 lookfrom(0, 0, 9);
    // point3 lookfrom(3, 3, 2); // Visão da diagonal
    // point3 lookfrom(15, 0, 30); // Posição da câmera para visualização
    // diagonal

    //  Visão do observador
    point3 lookat(0, 2, 0);
    // point3 lookat(278, 278, 0); // Cornell Box
    // point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);

    double aspect_ratio = 1.5;
    image_width = screen_width;
    int max_depth = 3;
    // double vfov = 40;
    double vfov = 50;
    double defocus_angle = 0;
    double focus_dist = 10;

    cam = make_shared<camera>(aspect_ratio, image_width, samples_per_pixel,
                              max_depth, vfov, defocus_angle, focus_dist,
                              lookfrom, lookat, vup);

    image_height = cam->getHeight();
}
