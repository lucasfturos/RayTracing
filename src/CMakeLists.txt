# -------------------------------------------------------------------

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
  Engine/src/ReadObjectFile/read_object_file.hpp
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
  Scenes/Render/render.hpp
  Scenes/Object/obj.hpp
  Scenes/Esfera/esfera.hpp
)

set(PROJECT_SOURCE                    # Directory of all source file
  # SOURCE da Engine
  Engine/src/Constant_Medium/constant_medium.cpp
  Engine/src/ReadObjectFile/read_object_file.cpp
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
  Scenes/Object/obj.cpp
  Scenes/Render/render.cpp
  Scenes/Esfera/esfera.cpp
  Scenes/Render/ray_color.cpp
  Scenes/CornellBox/cornell_box.cpp
  # SOURCE main
  main.cpp 
)

# Add executable target with source and header files listed in 
# PROJECT_HEADER and PROJECT_SOURCE variable
add_executable(${PROJECT_NAME} 
  ${PROJECT_HEADER} 
  ${PROJECT_SOURCE}
)

# Find SDL2 and GLM libraries
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
  PRIVATE ${SDL2_LIBRARIES}
  PRIVATE glm::glm
)

# Specifies include directories to use when compiling a given target
target_include_directories(${PROJECT_NAME} 
  PRIVATE src 
)

# -------------------------------------------------------------------