cmake_minimum_required(VERSION 3.9.0)
SET(PROJECT_NAME "PSP-UFU")
project(${PROJECT_NAME})
set(default_build_type "Release")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
endif()

find_package(wxWidgets REQUIRED COMPONENTS net core base std adv ribbon aui propgrid richtext stc gl)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(glfw3 3.1 REQUIRED)
include(${wxWidgets_USE_FILE})
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/Project/vendor/)
add_definitions(${OpenGL_DEFINITIONS})
add_definitions(${GLUT_DEFINITIONS})
    
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
message(STATUS "wxWidgets_INCLUDE_DIRS    : ${wxWidgets_INCLUDE_DIRS}")
message(STATUS "wxWidgets_LIBRARY_DIRS    : ${wxWidgets_LIBRARY_DIRS}")
message(STATUS "wxWidgets_LIBRARIES       : ${wxWidgets_LIBRARIES}")
message(STATUS "wxWidgets_CXX_FLAGS       : ${wxWidgets_CXX_FLAGS}")

file(GLOB PSP_SRC 
    "Project/*.h"
    "Project/artProvider/*.h"
    "Project/fparser/fparser.hh"
    "Project/fparser/fpconfig.hh"
    "Project/rapidXML/*.hpp"
    "Project/wxMathPlot/*.h"
    "Project/*.cpp"
    "Project/artProvider/*.cpp"
    "Project/fparser/fparser.cc"
    "Project/fparser/fpoptimizer.cc"
    "Project/wxMathPlot/*.cpp"
    "Project/vendor/glm/*.hpp"
    "Project/vendor/glm/detail/*.hpp"
    "Project/vendor/glm/detail/*.inl"
    "Project/vendor/glm/ext/*.hpp"
    "Project/vendor/glm/ext/*.inl"
    "Project/vendor/glm/gtc/*.hpp"
    "Project/vendor/glm/gtc/*.inl"
    "Project/vendor/glm/gtx/*.hpp"
    "Project/vendor/glm/gtx/*.inl"
    "Project/vendor/glm/simd/*.h"
)

add_executable(${PROJECT_NAME} WIN32 ${PSP_SRC})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} glfw)
