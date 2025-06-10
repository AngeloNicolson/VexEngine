cmake_minimum_required(VERSION 3.10)
project(GhostEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use standard, not compiler-specific extensions

find_package(Vulkan REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

include_directories(
  ${Vulkan_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
  include
)

# Loops through all the files in source recursiveley
file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(VexEngine ${SOURCES})

target_link_libraries(VexEngine
  ${Vulkan_LIBRARIES}
  ${GLFW_LIBRARIES}
  dl
  pthread
  X11
  Xxf86vm
  Xrandr
  Xi
)

