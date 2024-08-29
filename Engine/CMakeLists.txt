# ------------------------------------------------------------------------------

set(ENGINE_HEADERS
    # HEADER da Engine src
    src/AABB/aabb.hpp
    src/BVH/bvh.hpp
    src/Camera/camera.hpp
    src/Color/color.hpp
    src/Constant_Medium/constant_medium.hpp
    src/HitTable/hittable_list.hpp
    src/HitTable/hittable.hpp
    src/Interval/interval.hpp
    src/Quad/quad.hpp
    src/ReadObjectFile/read_object_file.hpp
    src/Sphere/sphere.hpp
    src/Texture/texture.hpp
    src/Triangle/triangle.hpp
)

set(ENGINE_SOURCES
    # SOURCE da Engine
    src/AABB/aabb.cpp
    src/BVH/bvh.cpp
    src/Camera/camera.cpp
    src/Camera/event.cpp
    src/Camera/initialize.cpp
    src/Camera/util.cpp
    src/Color/color.cpp
    src/Constant_Medium/constant_medium.cpp
    src/HitTable/hittable_list.cpp
    src/HitTable/hittable.cpp
    src/Interval/interval.cpp
    src/Quad/quad.cpp
    src/ReadObjectFile/read_object_file.cpp
    src/Sphere/sphere.cpp
    src/Texture/texture.cpp
    src/Triangle/triangle.cpp
)

add_library(Engine_lib 
    ${ENGINE_HEADERS}
    ${ENGINE_SOURCES}
)

target_include_directories(Engine_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ------------------------------------------------------------------------------