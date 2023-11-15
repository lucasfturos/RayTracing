#include "render.hpp"

Render::Render(const bvh_node &root, int opc) : world(root) {
    if (opc == 1) {
        initSDL2();
    }
}

Render::~Render() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}

void Render::initSDL2() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Erro ao inicializar o SDL: " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }

    win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED,
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

    // Camera
    // point3 lookfrom(26, 3, 6);
    // point3 lookfrom(13, 2, 3);
    point3 lookfrom(0, 0, 3); // visão de frente
    //  point3 lookfrom(3, 3, 2); // Visão da diagonal
    //  Visão do observador
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    color background(0, 0, 0);
    background = color(0.7, 0.7, 0.7);

    cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio);

    // Janela
    bool quit = false;
    SDL_Event event;
    bool ren_complete = false;
    int current_scanline = 0;

    int start_x = (screen_width - image_width) / 2;
    int start_y = (screen_height - image_height) / 2;

    SDL_RenderClear(ren);
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            event.type == SDL_QUIT ? (quit = 1) : 0;
            switch (event.key.keysym.sym) {
            case 'q':
                quit = 1;
                break;
            default:
                break;
            }
        }

        if (!ren_complete) {
            // Renderização em progresso
            std::cerr << "\rLinhas de varredura restantes: " << current_scanline
                      << ' ' << std::flush;
            for (auto i{0}; i < image_width; ++i) {
                color pixel_color(0, 0, 0);
                for (auto s{0}; s < samples_per_pixel; ++s) {
                    double u = (i + random_double()) / (image_width - 1);
                    double v = (current_scanline + random_double()) /
                               (image_height - 1);

                    ray r{cam->get_ray(u, v)};
                    pixel_color += ray_color(r, background, world, max_depth);

                    // Renderiza no tamanho da imagem
                    /*
                    int x = start_x + i;
                    int y = start_y + image_height - current_scanline - 1;
                    */

                    // Renderiza no tamanho da janela
                    int x = static_cast<int>(u * (screen_width - 1));
                    int y = screen_height - 1 -
                            static_cast<int>(v * (screen_height - 1));

                    color_ptr->write_color_SDL(ren, pixel_color,
                                               samples_per_pixel);
                    SDL_RenderDrawPoint(ren, x, y);
                }
            }
            current_scanline++;

            if (current_scanline == image_height) {
                ren_complete = true;
            }
        }
        SDL_RenderPresent(ren);
    }
}

void Render::run_ppm() {
    // Color
    color_ptr = make_shared<Color>();

    // Camera
    // point3 lookfrom(26, 3, 6);
    // point3 lookfrom(13, 2, 3);
    point3 lookfrom(0, 0, 2); // visão de frente
    //  point3 lookfrom(3, 3, 2); // Visão da diagonal
    //  Visão do observador
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    color background(0, 0, 0);
    background = color(0.7, 0.7, 0.7);

    cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio);

    // Renderização
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (auto j{image_height - 1}; j >= 0; --j) {
        std::cerr << "\rLinhas de varredura restantes: " << j << ' '
                  << std::flush;
        for (auto i{0}; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (auto s{0}; s < samples_per_pixel; ++s) {
                auto u{(i + random_double()) / (image_width - 1)};
                auto v{(j + random_double()) / (image_height - 1)};

                ray r{cam->get_ray(u, v)};
                pixel_color += ray_color(r, background, world, max_depth);
            }
            color_ptr->write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cout << "\nTerminou" << '\n';
}

void Render::run_term() {
    // Color
    color_ptr = make_shared<Color>();

    // Camera
    point3 lookfrom(0, 0, 1); // visão de frente
    point3 lookat(0, 0, 0);   //  Visão do observador
    vec3 vup(0, 1, 0);
    color background(0, 0, 0); // Background do céu
    background = color(.0, 0.749, 1.0);

    cam = make_shared<camera>(lookfrom, lookat, vup, vfov, aspect_ratio);

    // Renderização
    for (auto j{image_height - 1}; j >= 0; --j) {
        for (auto i{0}; i < image_width; ++i) {
            color pixel_color(0, 0, 0);
            for (auto s{0}; s < samples_per_pixel; ++s) {
                auto u{(i + random_double()) / (image_width - 1)};
                auto v{(j + random_double()) / (image_height - 1)};

                ray r{cam->get_ray(u, v)};
                pixel_color += ray_color(r, background, world, max_depth);
            }
            color_ptr->run_color(std::cout, pixel_color, samples_per_pixel);
        }
        std::cout << "\e[0m" << '\n';
    }
}
