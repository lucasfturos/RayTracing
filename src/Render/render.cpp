#include "render.hpp"

Render::Render(const Scene &root, int opc)
    : win(nullptr), ren(nullptr), texture(nullptr), surface(nullptr),
      world(root.world), lights(root.lights) {
    if (opc == 1) {
        setupSDL2();
    }

    setupCamera();
}

Render::~Render() {
    if (win) {
        SDL_DestroyWindow(win);
        win = nullptr;
    }
    if (ren) {
        SDL_DestroyRenderer(ren);
        ren = nullptr;
    }
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    if (surface) {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }

    SDL_Quit();
}

void Render::run() {
    // Color
    color_ptr = make_shared<Color>();
    color background(0, 0, 0);
    double illumination = 0.3;
    background = color(illumination, illumination, illumination);

    // Mouse
    int last_mouseX = 0;
    int last_mouseY = 0;
    bool mouse_down = false;

    // Frame rate
    const int target_fps = 30;
    const int frame_delay = 1000 / target_fps;
    Uint32 frame_start = 0;
    int frame_time;

    SDL_Event event;
    bool quit = false;
    int current_scanline = 0;

    SDL_RenderClear(ren);
    while (!quit) {
        frame_start = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouse_down = true;
                    SDL_GetMouseState(&last_mouseX, &last_mouseY);
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    mouse_down = false;
                }
                break;
            case SDL_MOUSEMOTION:
                if (mouse_down) {
                    int delta_x = event.motion.x - last_mouseX;
                    int delta_y = event.motion.y - last_mouseY;
                    cam->processMouseMovement(delta_x, delta_y);
                    last_mouseX = event.motion.x;
                    last_mouseY = event.motion.y;
                }
                break;
            case SDL_MOUSEWHEEL:
                cam->processMouseScroll(event.wheel.y);
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    quit = true;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
        }
        if (current_scanline < image_height) {
            cam->render(bvh_node(world), lights, background, current_scanline,
                        [&](int i, double scale, const color &pixel_color) {
                            int x = i;
                            int y = current_scanline;
                            color_ptr->write_color_SDL(surface, pixel_color,
                                                       scale, x, y);
                        });
            current_scanline++;
        } else {
            current_scanline = 0;
        }

        SDL_UpdateTexture(texture, nullptr, surface->pixels, surface->pitch);
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, texture, nullptr, nullptr);

        SDL_RenderPresent(ren);

        frame_time = SDL_GetTicks() - frame_start;
        if (frame_delay > frame_time) {
            SDL_Delay(frame_delay - frame_time);
        }
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
