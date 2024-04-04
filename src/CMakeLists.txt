# -------------------------------------------------------------------

set(PROJECT_HEADER                    # Directory of all header file
  # HEADER Scenes 
  CornellBox/cornell_box.hpp
  Render/render.hpp
  Object/obj.hpp
  Esfera/esfera.hpp
)

set(PROJECT_SOURCE                    # Directory of all source file
  # SOURCE Scenes
  Object/obj.cpp
  Render/render.cpp
  Esfera/esfera.cpp
  Render/ray_color.cpp
  CornellBox/cornell_box.cpp
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
  PRIVATE engine_lib
)

# Specifies include directories to use when compiling a given target
target_include_directories(${PROJECT_NAME} 
  PRIVATE src 
)

# -------------------------------------------------------------------