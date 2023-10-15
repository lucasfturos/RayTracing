cmake_minimum_required(VERSION 3.5)  # CMake version check

# --- Project settings ---------------------------------------------------------

set(PROJECT_NAME                      # Define all project info
  Ray 
  LANGUAGES CXX
  VERSION 1.0.0
)

project(${PROJECT_NAME})              # Create project "Ray"

set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

set(PROJECT_HEADER                    # Directory of all header file
  # HEADER da Engine includes
  Engine/include/constante.hpp
  Engine/include/material.hpp
  Engine/include/camera.hpp
  Engine/include/perlin.hpp
  Engine/include/aabb.hpp
  Engine/include/vec3.hpp 
  Engine/include/ray.hpp
  # HEADER da Engine src
  Engine/src/Constant_Medium/constant_medium.hpp
  Engine/src/HitTable/hittable_list.hpp
  Engine/src/HitTable/hittable.hpp
  Engine/src/Triangle/triangle.hpp
  Engine/src/Texture/texture.hpp
  Engine/src/Sphere/sphere.hpp
  Engine/src/Color/color.hpp
  Engine/src/Rect/rect.hpp
  Engine/src/Box/box.hpp
  Engine/src/BVH/bvh.hpp
  # HEADER Scenes 
  Scenes/CornellBox/cornell_box.hpp
  Scenes/BuleUtah/teapot.hpp
  Scenes/Render/render.hpp
  Scenes/BuleUtah/bule.hpp
  Scenes/Esfera/esfera.hpp
)

set(PROJECT_SOURCE                    # Directory of all source file
  # SOURCE da Engine
  Engine/src/Constant_Medium/constant_medium.cpp
  Engine/src/HitTable/hittable_list.cpp
  Engine/src/HitTable/hittable.cpp
  Engine/src/Triangle/triangle.cpp
  Engine/src/Texture/texture.cpp
  Engine/src/Sphere/sphere.cpp
  Engine/src/Color/color.cpp
  Engine/src/Rect/rect.cpp
  Engine/src/Box/box.cpp
  Engine/src/BVH/bvh.cpp
  # SOURCE Scenes
  Scenes/Render/render.cpp
  Scenes/Esfera/esfera.cpp
  Scenes/BuleUtah/bule.cpp
  Scenes/Render/ray_color.cpp
  Scenes/CornellBox/cornell_box.cpp
  # SOURCE main
  main.cpp 
)

# List all the image files
set(RESOURCES_DIRECTORY 
  "${CMAKE_SOURCE_DIR}/Scenes/Esfera/img"
)

# Copy the entire img directory to the binary directory during build
file(COPY ${RESOURCES_DIRECTORY} 
  DESTINATION ${CMAKE_BINARY_DIR}
)

# Add executable target with source and header files listed in 
# PROJECT_HEADER and PROJECT_SOURCE variable
add_executable(${PROJECT_NAME} 
  ${PROJECT_HEADER} 
  ${PROJECT_SOURCE}
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
  PRIVATE ${SDL2_LIBRARIES}
)
# Specifies include directories to use when compiling a given target
target_include_directories(${PROJECT_NAME} 
  PRIVATE Scenes
  PRIVATE Engine/include
)

# ------------------------------------------------------------------------------
