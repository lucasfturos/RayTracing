#pragma once

#include "../../include/constante.hpp"
#include "../HitTable/hittable.hpp"
#include <cassert>
#include <iostream>

constexpr int table_rgb[][3] = {
    {95, 95, 135},   {128, 0, 0},     {0, 128, 0},     {128, 128, 0},
    {0, 0, 128},     {128, 0, 128},   {0, 128, 128},   {192, 192, 192},
    {128, 128, 128}, {255, 0, 0},     {0, 255, 0},     {255, 255, 0},
    {0, 0, 255},     {255, 0, 255},   {0, 255, 255},   {255, 255, 255},
    {0, 0, 0},       {0, 0, 95},      {0, 0, 135},     {0, 0, 175},
    {0, 0, 215},     {0, 0, 255},     {0, 95, 0},      {0, 95, 95},
    {0, 95, 135},    {0, 95, 175},    {0, 95, 215},    {0, 95, 255},
    {0, 135, 0},     {0, 135, 95},    {0, 135, 135},   {0, 135, 175},
    {0, 135, 215},   {0, 135, 255},   {0, 175, 0},     {0, 175, 95},
    {0, 175, 135},   {0, 175, 175},   {0, 175, 215},   {0, 175, 255},
    {0, 215, 0},     {0, 215, 95},    {0, 215, 135},   {0, 215, 175},
    {0, 215, 215},   {0, 215, 255},   {0, 255, 0},     {0, 255, 95},
    {0, 255, 135},   {0, 255, 175},   {0, 255, 215},   {0, 255, 255},
    {95, 0, 0},      {95, 0, 95},     {95, 0, 135},    {95, 0, 175},
    {95, 0, 215},    {95, 0, 255},    {95, 95, 0},     {95, 95, 95},
    {95, 95, 135},   {95, 95, 175},   {95, 95, 215},   {95, 95, 255},
    {95, 135, 0},    {95, 135, 95},   {95, 135, 135},  {95, 135, 175},
    {95, 135, 215},  {95, 135, 255},  {95, 175, 0},    {95, 175, 95},
    {95, 175, 135},  {95, 175, 175},  {95, 175, 215},  {95, 175, 255},
    {95, 215, 0},    {95, 215, 95},   {95, 215, 135},  {95, 215, 175},
    {95, 215, 215},  {95, 215, 255},  {95, 255, 0},    {95, 255, 95},
    {95, 255, 135},  {95, 255, 175},  {95, 255, 215},  {95, 255, 255},
    {135, 0, 0},     {135, 0, 95},    {135, 0, 135},   {135, 0, 175},
    {135, 0, 215},   {135, 0, 255},   {135, 95, 0},    {135, 95, 95},
    {135, 95, 135},  {135, 95, 175},  {135, 95, 215},  {135, 95, 255},
    {135, 135, 0},   {135, 135, 95},  {135, 135, 135}, {135, 135, 175},
    {135, 135, 215}, {135, 135, 255}, {135, 175, 0},   {135, 175, 95},
    {135, 175, 135}, {135, 175, 175}, {135, 175, 215}, {135, 175, 255},
    {135, 215, 0},   {135, 215, 95},  {135, 215, 135}, {135, 215, 175},
    {135, 215, 215}, {135, 215, 255}, {135, 255, 0},   {135, 255, 95},
    {135, 255, 135}, {135, 255, 175}, {135, 255, 215}, {135, 255, 255},
    {175, 0, 0},     {175, 0, 95},    {175, 0, 135},   {175, 0, 175},
    {175, 0, 215},   {175, 0, 255},   {175, 95, 0},    {175, 95, 95},
    {175, 95, 135},  {175, 95, 175},  {175, 95, 215},  {175, 95, 255},
    {175, 135, 0},   {175, 135, 95},  {175, 135, 135}, {175, 135, 175},
    {175, 135, 215}, {175, 135, 255}, {175, 175, 0},   {175, 175, 95},
    {175, 175, 135}, {175, 175, 175}, {175, 175, 215}, {175, 175, 255},
    {175, 215, 0},   {175, 215, 95},  {175, 215, 135}, {175, 215, 175},
    {175, 215, 215}, {175, 215, 255}, {175, 255, 0},   {175, 255, 95},
    {175, 255, 135}, {175, 255, 175}, {175, 255, 215}, {175, 255, 255},
    {215, 0, 0},     {215, 0, 95},    {215, 0, 135},   {215, 0, 175},
    {215, 0, 215},   {215, 0, 255},   {215, 95, 0},    {215, 95, 95},
    {215, 95, 135},  {215, 95, 175},  {215, 95, 215},  {215, 95, 255},
    {215, 135, 0},   {215, 135, 95},  {215, 135, 135}, {215, 135, 175},
    {215, 135, 215}, {215, 135, 255}, {215, 175, 0},   {215, 175, 95},
    {215, 175, 135}, {215, 175, 175}, {215, 175, 215}, {215, 175, 255},
    {215, 215, 0},   {215, 215, 95},  {215, 215, 135}, {215, 215, 175},
    {215, 215, 215}, {215, 215, 255}, {215, 255, 0},   {215, 255, 95},
    {215, 255, 135}, {215, 255, 175}, {215, 255, 215}, {215, 255, 255},
    {255, 0, 0},     {255, 0, 95},    {255, 0, 135},   {255, 0, 175},
    {255, 0, 215},   {255, 0, 255},   {255, 95, 0},    {255, 95, 95},
    {255, 95, 135},  {255, 95, 175},  {255, 95, 215},  {255, 95, 255},
    {255, 135, 0},   {255, 135, 95},  {255, 135, 135}, {255, 135, 175},
    {255, 135, 215}, {255, 135, 255}, {255, 175, 0},   {255, 175, 95},
    {255, 175, 135}, {255, 175, 175}, {255, 175, 215}, {255, 175, 255},
    {255, 215, 0},   {255, 215, 95},  {255, 215, 135}, {255, 215, 175},
    {255, 215, 215}, {255, 215, 255}, {255, 255, 0},   {255, 255, 95},
    {255, 255, 135}, {255, 255, 175}, {255, 255, 215}, {255, 255, 255},
    {8, 8, 8},       {18, 18, 18},    {28, 28, 28},    {38, 38, 38},
    {48, 48, 48},    {58, 58, 58},    {68, 68, 68},    {78, 78, 78},
    {88, 88, 88},    {98, 98, 98},    {108, 108, 108}, {118, 118, 118},
    {128, 128, 128}, {138, 138, 138}, {148, 148, 148}, {158, 158, 158},
    {168, 168, 168}, {178, 178, 178}, {188, 188, 188}, {198, 198, 198},
    {208, 208, 208}, {218, 218, 218}, {228, 228, 228}, {238, 238, 238}};

constexpr int table_hsl[][3] = {
    {0, 0, 0},      {0, 100, 25},   {120, 100, 25}, {60, 100, 25},
    {240, 100, 25}, {300, 100, 25}, {180, 100, 25}, {0, 0, 75},
    {0, 0, 50},     {0, 100, 50},   {120, 100, 50}, {60, 100, 50},
    {240, 100, 50}, {300, 100, 50}, {180, 100, 50}, {0, 0, 100},
    {0, 0, 0},      {240, 100, 18}, {240, 100, 26}, {240, 100, 34},
    {240, 100, 42}, {240, 100, 50}, {120, 100, 18}, {180, 100, 18},
    {97, 100, 26},  {7, 100, 34},   {13, 100, 42},  {17, 100, 50},
    {120, 100, 26}, {62, 100, 26},  {180, 100, 26}, {93, 100, 34},
    {2, 100, 42},   {8, 100, 50},   {120, 100, 34}, {52, 100, 34},
    {66, 100, 34},  {180, 100, 34}, {91, 100, 42},  {98, 100, 50},
    {120, 100, 42}, {46, 100, 42},  {57, 100, 42},  {68, 100, 42},
    {180, 100, 42}, {89, 100, 50},  {120, 100, 50}, {42, 100, 50},
    {51, 100, 50},  {61, 100, 50},  {70, 100, 50},  {180, 100, 50},
    {0, 100, 18},   {300, 100, 18}, {82, 100, 26},  {72, 100, 34},
    {66, 100, 42},  {62, 100, 50},  {60, 100, 18},  {0, 0, 37},
    {240, 17, 45},  {240, 33, 52},  {240, 60, 60},  {240, 100, 68},
    {7, 100, 26},   {120, 17, 45},  {180, 17, 45},  {210, 33, 52},
    {220, 60, 60},  {225, 100, 68}, {7, 100, 34},   {120, 33, 52},
    {150, 33, 52},  {180, 33, 52},  {200, 60, 60},  {210, 100, 68},
    {3, 100, 42},   {120, 60, 60},  {140, 60, 60},  {160, 60, 60},
    {180, 60, 60},  {195, 100, 68}, {7, 100, 50},   {120, 100, 68},
    {135, 100, 68}, {150, 100, 68}, {165, 100, 68}, {180, 100, 68},
    {0, 100, 26},   {17, 100, 26},  {300, 100, 26}, {86, 100, 34},
    {77, 100, 42},  {71, 100, 50},  {2, 100, 26},   {0, 17, 45},
    {300, 17, 45},  {270, 33, 52},  {260, 60, 60},  {255, 100, 68},
    {60, 100, 26},  {60, 17, 45},   {0, 0, 52},     {240, 20, 60},
    {240, 50, 68},  {240, 100, 76}, {3, 100, 34},   {90, 33, 52},
    {120, 20, 60},  {180, 20, 60},  {210, 50, 68},  {220, 100, 76},
    {2, 100, 42},   {100, 60, 60},  {120, 50, 68},  {150, 50, 68},
    {180, 50, 68},  {200, 100, 76}, {8, 100, 50},   {105, 100, 68},
    {120, 100, 76}, {140, 100, 76}, {160, 100, 76}, {180, 100, 76},
    {0, 100, 34},   {27, 100, 34},  {13, 100, 34},  {300, 100, 34},
    {88, 100, 42},  {81, 100, 50},  {2, 100, 34},   {0, 33, 52},
    {330, 33, 52},  {300, 33, 52},  {280, 60, 60},  {270, 100, 68},
    {6, 100, 34},   {30, 33, 52},   {0, 20, 60},    {300, 20, 60},
    {270, 50, 68},  {260, 100, 76}, {60, 100, 34},  {60, 33, 52},
    {60, 20, 60},   {0, 0, 68},     {240, 33, 76},  {240, 100, 84},
    {1, 100, 42},   {80, 60, 60},   {90, 50, 68},   {120, 33, 76},
    {180, 33, 76},  {210, 100, 84}, {8, 100, 50},   {90, 100, 68},
    {100, 100, 76}, {120, 100, 84}, {150, 100, 84}, {180, 100, 84},
    {0, 100, 42},   {33, 100, 42},  {22, 100, 42},  {11, 100, 42},
    {300, 100, 42}, {90, 100, 50},  {6, 100, 42},   {0, 60, 60},
    {340, 60, 60},  {320, 60, 60},  {300, 60, 60},  {285, 100, 68},
    {7, 100, 42},   {20, 60, 60},   {0, 50, 68},    {330, 50, 68},
    {300, 50, 68},  {280, 100, 76}, {8, 100, 42},   {40, 60, 60},
    {30, 50, 68},   {0, 33, 76},    {300, 33, 76},  {270, 100, 84},
    {60, 100, 42},  {60, 60, 60},   {60, 50, 68},   {60, 33, 76},
    {0, 0, 84},     {240, 100, 92}, {9, 100, 50},   {75, 100, 68},
    {80, 100, 76},  {90, 100, 84},  {120, 100, 92}, {180, 100, 92},
    {0, 100, 50},   {37, 100, 50},  {28, 100, 50},  {18, 100, 50},
    {9, 100, 50},   {300, 100, 50}, {2, 100, 50},   {0, 100, 68},
    {345, 100, 68}, {330, 100, 68}, {315, 100, 68}, {300, 100, 68},
    {1, 100, 50},   {15, 100, 68},  {0, 100, 76},   {340, 100, 76},
    {320, 100, 76}, {300, 100, 76}, {1, 100, 50},   {30, 100, 68},
    {20, 100, 76},  {0, 100, 84},   {330, 100, 84}, {300, 100, 84},
    {0, 100, 50},   {45, 100, 68},  {40, 100, 76},  {30, 100, 84},
    {0, 100, 92},   {300, 100, 92}, {60, 100, 50},  {60, 100, 68},
    {60, 100, 76},  {60, 100, 84},  {60, 100, 92},  {0, 0, 100},
    {0, 0, 3},      {0, 0, 7},      {0, 0, 10},     {0, 0, 14},
    {0, 0, 18},     {0, 0, 22},     {0, 0, 26},     {0, 0, 30},
    {0, 0, 34},     {0, 0, 37},     {0, 0, 40},     {0, 0, 46},
    {0, 0, 50},     {0, 0, 54},     {0, 0, 58},     {0, 0, 61},
    {0, 0, 65},     {0, 0, 69},     {0, 0, 73},     {0, 0, 77},
    {0, 0, 81},     {0, 0, 85},     {0, 0, 89},     {0, 0, 93},
};

class Color {
  private:
    int find_ansi_rgb(int r, int g, int b);
    int find_ansi_hsl(int h, int s, int l);
    int distance_hsl(int i, int h, int s, int l);

    void rgb_to_hsl(int r, int g, int b, int *h, int *s, int *l);

  public:
    void write_color(std::ostream &out, color pixel_color,
                     int samples_per_pixel);

    void run_color(std::ostream &out, color pixel_color, int samples_per_pixel);
};
