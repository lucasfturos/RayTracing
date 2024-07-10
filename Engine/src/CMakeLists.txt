# ------------------------------------------------------------------------------

set(ENGINE_HEADERS
    # HEADER da Engine src
    AABB/aabb.hpp
    Box/box.hpp
    BVH/bvh.hpp
    Color/color.hpp
    Constant_Medium/constant_medium.hpp
    HitTable/hittable_list.hpp
    HitTable/hittable.hpp
    Interval/interval.hpp
    ReadObjectFile/read_object_file.hpp
    Rect/rect.hpp
    Sphere/sphere.hpp
    Texture/texture.hpp
    Triangle/triangle.hpp
)

set(ENGINE_SOURCES
    # SOURCE da Engine
    AABB/aabb.cpp
    Box/box.cpp
    BVH/bvh.cpp
    Color/color.cpp
    Constant_Medium/constant_medium.cpp
    HitTable/hittable_list.cpp
    HitTable/hittable.cpp
    Interval/interval.cpp
    ReadObjectFile/read_object_file.cpp
    Rect/rect.cpp
    Sphere/sphere.cpp
    Texture/texture.cpp
    Triangle/triangle.cpp
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