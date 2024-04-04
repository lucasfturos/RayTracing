# ------------------------------------------------------------------------------

set(ENGINE_HEADERS
    # HEADER da Engine includes
    include/ray.hpp
    include/vec3.hpp 
    include/aabb.hpp
    include/perlin.hpp
    include/camera.hpp
    include/material.hpp
    include/constante.hpp
    # HEADER da Engine src
    src/Box/box.hpp
    src/BVH/bvh.hpp
    src/Rect/rect.hpp
    src/Color/color.hpp
    src/Sphere/sphere.hpp
    src/Texture/texture.hpp
    src/HitTable/hittable.hpp
    src/Triangle/triangle.hpp
    src/HitTable/hittable_list.hpp
    src/Constant_Medium/constant_medium.hpp
    src/ReadObjectFile/read_object_file.hpp
)

set(ENGINE_SOURCES
    # SOURCE da Engine
    src/Box/box.cpp
    src/BVH/bvh.cpp
    src/Rect/rect.cpp
    src/Color/color.cpp
    src/Sphere/sphere.cpp
    src/Texture/texture.cpp
    src/HitTable/hittable.cpp
    src/Triangle/triangle.cpp
    src/HitTable/hittable_list.cpp
    src/ReadObjectFile/read_object_file.cpp
    src/Constant_Medium/constant_medium.cpp
)

add_library(engine_lib 
    ${ENGINE_HEADERS}
    ${ENGINE_SOURCES}
)

target_include_directories(engine_lib 
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------------------------------------------------------------------------