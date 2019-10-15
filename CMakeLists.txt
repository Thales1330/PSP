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

find_package(wxWidgets REQUIRED COMPONENTS net core base)
include(${wxWidgets_USE_FILE})
    
message(STATUS "CMAKE_BUILD_TYPE:${CMAKE_BUILD_TYPE}")
file(GLOB PSP_SRC
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME} WIN32 ${PSP_SRC})
target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})
