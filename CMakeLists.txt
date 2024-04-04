# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.5)  # CMake version check

# --- Project settings ---------------------------------------------------------

set(PROJECT_NAME                      # Define all project info
  Ray 
  LANGUAGES CXX
  VERSION 1.0.0
)

project(${PROJECT_NAME})              # Create project "Ray"

set(CMAKE_CXX_STANDARD 20)            # Enable c++14 standard

# List all the image files
set(RESOURCES_DIRECTORY 
  # "${CMAKE_SOURCE_DIR}/img"
  "${CMAKE_SOURCE_DIR}/obj"
)

# Copy the entire img directory to the binary directory during build
file(COPY ${RESOURCES_DIRECTORY} 
  DESTINATION ${CMAKE_BINARY_DIR}
)

# Flags of compiling
add_compile_options(
  -Wall -Wextra -Wpedantic -Werror -pedantic -g -Ofast 
)

# Add subdirectories for source file
add_subdirectory(src)
add_subdirectory(Engine)

# ------------------------------------------------------------------------------
