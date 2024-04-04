# ------------------------------------------------------------------------------

set(ENGINE_HEADERS
    # HEADER da Engine includes
    include/constante.hpp
    include/material.hpp
    include/camera.hpp
    include/perlin.hpp
    include/aabb.hpp
    include/vec3.hpp 
    include/ray.hpp
    # HEADER da Engine src
    src/Constant_Medium/constant_medium.hpp
    src/ReadObjectFile/read_object_file.hpp
    src/HitTable/hittable_list.hpp
    src/HitTable/hittable.hpp
    src/Triangle/triangle.hpp
    src/Texture/texture.hpp
    src/Sphere/sphere.hpp
    src/Color/color.hpp
    src/Rect/rect.hpp
    src/Box/box.hpp
    src/BVH/bvh.hpp
)

set(ENGINE_SOURCES
    # SOURCE da Engine
    src/Constant_Medium/constant_medium.cpp
    src/ReadObjectFile/read_object_file.cpp
    src/HitTable/hittable_list.cpp
    src/HitTable/hittable.cpp
    src/Triangle/triangle.cpp
    src/Texture/texture.cpp
    src/Sphere/sphere.cpp
    src/Color/color.cpp
    src/Rect/rect.cpp
    src/Box/box.cpp
    src/BVH/bvh.cpp
)

add_library(engine_lib 
    ${ENGINE_HEADERS}
    ${ENGINE_SOURCES}
)

target_include_directories(engine_lib 
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)

# ------------------------------------------------------------------------------