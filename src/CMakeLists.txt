# -------------------------------------------------------------------

set(PROJECT_HEADER                    # Directory of all header file
  # HEADER Scenes 
  Object/obj.hpp
  Esfera/esfera.hpp
  Render/render.hpp
  CornellBox/cornell_box.hpp
)

set(PROJECT_SOURCE                    # Directory of all source file
  # SOURCE Scenes
  Object/obj.cpp
  Render/render.cpp
  Render/setup.cpp
  Esfera/esfera.cpp
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

# Find SDL2 libraries
find_package(SDL2 REQUIRED)
# Find Threads libraries
find_package(Threads REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${SDL2_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Threads::Threads
  ${SDL2_LIBRARIES}
  Engine_lib
)

# -------------------------------------------------------------------