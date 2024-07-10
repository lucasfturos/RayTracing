#include "render.hpp"

Render::Render(const Scene &root, int opc)
    : world(root.world), lights(root.lights) {
    if (opc == 1) {
        initSDL2();
    }

    // Initialize cam
    // point3 lookfrom(26, 3, 6);
    // point3 lookfrom(13, 2, 3);
    point3 lookfrom(278, 278, -800); // Cornell Box
    // point3 lookfrom(478, 278, -600); // Cornell Box
    // point3 lookfrom(0, 2.2, 7.5); // visão de frente
    // point3 lookfrom(0, 0, 9);
    // point3 lookfrom(3, 3, 2); // Visão da diagonal
    // point3 lookfrom(15, 0, 30); // Posição da câmera para visualização
    // diagonal

    //  Visão do observador
    // point3 lookat(0, 2, 0);
    point3 lookat(278, 278, 0); // Cornell Box
    // point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);

    double aspect_ratio = 1.;
    image_width = 800;
    int max_depth = 3;
    double vfov = 40;
    // double vfov = 50;
    double defocus_angle = 0;
    double focus_dist = 10;

    cam = make_shared<camera>(aspect_ratio, image_width, samples_per_pixel,
                              max_depth, vfov, defocus_angle, focus_dist,
                              lookfrom, lookat, vup);

    image_height = cam->getHeight();
}

Render::~Render() {
    if (ren) {
        SDL_DestroyRenderer(ren);
        ren = nullptr;
    }
    if (win) {
        SDL_DestroyWindow(win);
        win = nullptr;
    }
    SDL_Quit();
}

void Render::initSDL2() {
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

void Render::run() {
    // Color
    color_ptr = make_shared<Color>();
    color background(0, 0, 0);
    double illumination = 0.5;
    background = color(illumination, illumination, illumination);

    // Janela
    bool quit = false;
    SDL_Event event;
    bool ren_complete = false;
    int current_scanline = 0;

    SDL_RenderClear(ren);
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            event.type == SDL_QUIT ? (quit = true) : 0;
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                quit = true;
                break;
            default:
                break;
            }
        }

        if (!ren_complete) {
            cam->render(bvh_node(world), lights, background, current_scanline,
                        [&](int i, double scale, const color &pixel_color) {
                            int x = i;
                            int y = current_scanline;

                            color_ptr->write_color_SDL(ren, pixel_color, scale);
                            SDL_RenderDrawPoint(ren, x, y);
                        });
            current_scanline++;

            if (current_scanline == image_height) {
                ren_complete = true;
                std::cout << "Done \n";
            }
        }
        SDL_RenderPresent(ren);
    }
}

void Render::run_ppm() {
    // Color
    color_ptr = make_shared<Color>();
    color background(0, 0, 0);
    double illumination = .1;
    background = color(illumination, illumination, illumination);

    // Renderização
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (auto j{0}; j < image_height + 1; ++j) {
        std::cerr << "\rLinhas de varredura restantes: " << j << ' '
                  << std::flush;
        cam->render(world, lights, background, j,
                    [&](int, double scale, const color &pixel_color) {
                        color_ptr->write_color(std::cout, pixel_color, scale);
                    });
    }

    std::cout << "\nTerminou" << '\n';
}

void Render::run_term() {
    // Color
    color_ptr = make_shared<Color>();
    color background(.0, 0.75, 1.0);

    // Renderização
    for (auto j{0}; j < image_height + 1; ++j) {
        cam->render(world, lights, background, j,
                    [&](int, double scale, const color &pixel_color) {
                        color_ptr->run_color(std::cout, pixel_color, scale);
                    });
        std::cout << "\033[0m\n";
    }
}
