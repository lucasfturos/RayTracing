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
  src/Engine/include/constante.hpp
  src/Engine/include/material.hpp
  src/Engine/include/camera.hpp
  src/Engine/include/perlin.hpp
  src/Engine/include/aabb.hpp
  src/Engine/include/vec3.hpp 
  src/Engine/include/ray.hpp
  # HEADER da Engine src
  src/Engine/src/Constant_Medium/constant_medium.hpp
  src/Engine/src/HitTable/hittable_list.hpp
  src/Engine/src/HitTable/hittable.hpp
  src/Engine/src/Triangle/triangle.hpp
  src/Engine/src/ReadObj/read_obj.hpp
  src/Engine/src/Texture/texture.hpp
  src/Engine/src/Sphere/sphere.hpp
  src/Engine/src/Color/color.hpp
  src/Engine/src/Torus/torus.hpp
  src/Engine/src/Rect/rect.hpp
  src/Engine/src/Box/box.hpp
  src/Engine/src/BVH/bvh.hpp
  # HEADER Scenes 
  src/Scenes/CornellBox/cornell_box.hpp
  src/Scenes/Object/teapot.hpp
  src/Scenes/Render/render.hpp
  src/Scenes/Object/obj.hpp
  src/Scenes/Esfera/esfera.hpp
)

set(PROJECT_SOURCE                    # Directory of all source file
  # SOURCE da Engine
  src/Engine/src/Constant_Medium/constant_medium.cpp
  src/Engine/src/HitTable/hittable_list.cpp
  src/Engine/src/HitTable/hittable.cpp
  src/Engine/src/Triangle/triangle.cpp
  src/Engine/src/ReadObj/read_obj.cpp
  src/Engine/src/Texture/texture.cpp
  src/Engine/src/Sphere/sphere.cpp
  src/Engine/src/Color/color.cpp
  src/Engine/src/Torus/torus.cpp
  src/Engine/src/Rect/rect.cpp
  src/Engine/src/Box/box.cpp
  src/Engine/src/BVH/bvh.cpp
  # SOURCE Scenes
  src/Scenes/Object/obj.cpp
  src/Scenes/Render/render.cpp
  src/Scenes/Esfera/esfera.cpp
  src/Scenes/Render/ray_color.cpp
  src/Scenes/CornellBox/cornell_box.cpp
  # SOURCE main
  src/main.cpp 
)

# List all the image files
set(RESOURCES_DIRECTORY 
  "${CMAKE_SOURCE_DIR}/img"
  "${CMAKE_SOURCE_DIR}/obj"
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
  PRIVATE src 
)

# ------------------------------------------------------------------------------
