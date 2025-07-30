cmake_minimum_required(VERSION 3.5...4.0)
project(PSP-UFU VERSION 1.0 LANGUAGES CXX)

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Output folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)

# wxWidgets with extra modules
FetchContent_Declare(
  wxwidgets
  GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
  GIT_TAG v3.3.1
)

# Force build of required modules
set(wxBUILD_SHARED OFF CACHE BOOL "" FORCE)
set(wxBUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(wxBUILD_TESTS OFF CACHE BOOL "" FORCE)
set(wxUSE_WEBVIEW OFF CACHE BOOL "" FORCE)

set(wxBUILD_AUI ON CACHE BOOL "" FORCE)
set(wxBUILD_PROPGRID ON CACHE BOOL "" FORCE)
set(wxBUILD_XRC ON CACHE BOOL "" FORCE)
set(wxBUILD_RICHTEXT ON CACHE BOOL "" FORCE)
set(wxBUILD_XML ON CACHE BOOL "" FORCE)
set(wxBUILD_STC ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(wxwidgets)

# Include directories
include_directories(
  ${CMAKE_SOURCE_DIR}/vendors/include
)

# Project source files
file(GLOB_RECURSE PROJECT_SOURCES
  "${CMAKE_SOURCE_DIR}/Project/*.cpp"
  "${CMAKE_SOURCE_DIR}/Project/*.h"
)

file(GLOB_RECURSE VENDOR_SOURCES
  "${CMAKE_SOURCE_DIR}/vendors/*.cpp"
  "${CMAKE_SOURCE_DIR}/vendors/*.h"
)

set(FPARSER_SOURCES
  ${CMAKE_SOURCE_DIR}/Project/extLibs/fparser/fparser.cc
  ${CMAKE_SOURCE_DIR}/Project/extLibs/fparser/fpoptimizer.cc
  ${CMAKE_SOURCE_DIR}/Project/extLibs/fparser/fpconfig.hh
  ${CMAKE_SOURCE_DIR}/Project/extLibs/fparser/fparser.hh
)

if(WIN32)
  set(WIN_RESOURCES
    ${CMAKE_SOURCE_DIR}/Project/win_resources.rc
  )
endif()

add_executable(PSP-UFU
  ${PROJECT_SOURCES}
  ${VENDOR_SOURCES}
  ${FPARSER_SOURCES}
  ${WIN_RESOURCES}
)

# Link directories and libraries depending on OS and architecture
if(WIN32)
  target_link_directories(PSP-UFU PRIVATE
    "${CMAKE_SOURCE_DIR}/vendors/fftw/fftw-3.3.5-dll32"
    "${CMAKE_SOURCE_DIR}/vendors/chatdir/win32"
  )
  target_link_libraries(PSP-UFU PRIVATE
    wx::core
    wx::base
    wx::aui
    wx::propgrid
    wx::xrc
    wx::richtext
    wx::ribbon
    wx::xml
    wx::stc
    chartdir70.lib
    libfftw3-3.lib
  )
elseif(UNIX)
  # Linux 64-bit
  target_link_directories(PSP-UFU PRIVATE
    "${CMAKE_SOURCE_DIR}/vendors/chatdir/linux64"
  )
  target_link_libraries(PSP-UFU PRIVATE
    wx::core
    wx::base
    wx::aui
    wx::propgrid
    wx::xrc
    wx::richtext
    wx::ribbon
    wx::xml
    wx::stc
    chartdir70
    fftw3
  )
else()
  message(FATAL_ERROR "Unsupported platform or architecture")
endif()

# MSVC specific settings
if(MSVC)
  target_link_options(PSP-UFU PRIVATE "/MANIFEST:NO" "/SUBSYSTEM:WINDOWS")
  target_compile_definitions(PSP-UFU PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Additional includes
target_include_directories(PSP-UFU PRIVATE
  ${CMAKE_SOURCE_DIR}/Project
  ${CMAKE_SOURCE_DIR}/vendors/include
)

# Post build commands: copy DLLs and data folder on Windows
if(WIN32)
  add_custom_command(TARGET PSP-UFU POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/vendors/fftw/fftw-3.3.5-dll32/libfftw3-3.dll"
      $<TARGET_FILE_DIR:PSP-UFU>
  
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/vendors/chatdir/win32/chartdir70.dll"
      $<TARGET_FILE_DIR:PSP-UFU>
  
    COMMAND ${CMAKE_COMMAND} -E copy_directory )
endif()