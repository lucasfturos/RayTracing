# ------------------------------------------------------------------------------

set(ENGINE_HEADERS
    # HEADER da Engine src
    Box/box.hpp
    BVH/bvh.hpp
    Rect/rect.hpp
    Color/color.hpp
    Sphere/sphere.hpp
    Texture/texture.hpp
    HitTable/hittable.hpp
    Triangle/triangle.hpp
    HitTable/hittable_list.hpp
    Constant_Medium/constant_medium.hpp
    ReadObjectFile/read_object_file.hpp
)

set(ENGINE_SOURCES
    # SOURCE da Engine
    Box/box.cpp
    BVH/bvh.cpp
    Rect/rect.cpp
    Color/color.cpp
    Sphere/sphere.cpp
    Texture/texture.cpp
    HitTable/hittable.cpp
    Triangle/triangle.cpp
    HitTable/hittable_list.cpp
    ReadObjectFile/read_object_file.cpp
    Constant_Medium/constant_medium.cpp
)

add_library(engine_lib 
    ${ENGINE_HEADERS}
    ${ENGINE_SOURCES}
)

target_include_directories(engine_lib 
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
  PRIVATE include
)

# ------------------------------------------------------------------------------