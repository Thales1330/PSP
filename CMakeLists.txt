cmake_minimum_required(VERSION 3.5)
project(PSP-UFU VERSION 1.0 LANGUAGES CXX)

# Standard install directories (bin, share, lib, etc)
include(GNUInstallDirs)

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
  GIT_TAG v3.3.3
)

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
list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/Project/data/atp/.*")

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

# --------------------------------------------------
# Linking
# --------------------------------------------------

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
  set(CHARTDIR_PATH "${CMAKE_SOURCE_DIR}/vendors/chatdir/linux64")

  target_link_directories(PSP-UFU PRIVATE "${CHARTDIR_PATH}")

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
    fftw3
    "${CHARTDIR_PATH}/libchartdir.so.7.0.0"
  )

  # runtime path configuration
  set_target_properties(PSP-UFU PROPERTIES
    BUILD_RPATH "${CHARTDIR_PATH}"
    INSTALL_RPATH "$ORIGIN/../${CMAKE_INSTALL_LIBDIR}/psp-ufu"
  )

else()
  message(FATAL_ERROR "Unsupported platform or architecture")
endif()

# MSVC specific settings
if(MSVC)
  target_link_options(PSP-UFU PRIVATE "/MANIFEST:NO" "/SUBSYSTEM:WINDOWS")
  target_compile_definitions(PSP-UFU PRIVATE _CRT_SECURE_NO_WARNINGS)
  target_compile_options(PSP-UFU PRIVATE "/utf-8")
endif()

# Additional includes
target_include_directories(PSP-UFU PRIVATE
  ${CMAKE_SOURCE_DIR}/Project
  ${CMAKE_SOURCE_DIR}/vendors/include
)

# --------------------------------------------------
# Windows post build
# --------------------------------------------------

if(WIN32)

  add_custom_command(TARGET PSP-UFU POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/vendors/fftw/fftw-3.3.5-dll32/libfftw3-3.dll"
      $<TARGET_FILE_DIR:PSP-UFU>

    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/vendors/chatdir/win32/chartdir70.dll"
      $<TARGET_FILE_DIR:PSP-UFU>

    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_SOURCE_DIR}/Project/data"
      "$<TARGET_FILE_DIR:PSP-UFU>/../data"
  )

endif()

# --------------------------------------------------
# Linux build layout
# --------------------------------------------------

if(UNIX AND NOT APPLE)

  add_custom_command(TARGET PSP-UFU POST_BUILD

    COMMAND ${CMAKE_COMMAND} -E rm -f ${CHARTDIR_PATH}/libchartdir.so.7.0

    COMMAND ${CMAKE_COMMAND} -E create_symlink
      ${CHARTDIR_PATH}/libchartdir.so.7.0.0
      ${CHARTDIR_PATH}/libchartdir.so.7.0

    COMMAND ${CMAKE_COMMAND} -E copy_directory
      "${CMAKE_SOURCE_DIR}/Project/data"
      "$<TARGET_FILE_DIR:PSP-UFU>/../data"
  )

endif()

# --------------------------------------------------
# Installation rules
# --------------------------------------------------

install(TARGETS PSP-UFU
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/Project/data
        DESTINATION ${CMAKE_INSTALL_DATADIR}/psp-ufu)

# Install ChartDirector library
if(UNIX AND NOT APPLE)

  install(FILES
    ${CHARTDIR_PATH}/libchartdir.so.7.0.0
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/psp-ufu
  )

  install(CODE "
    execute_process(
      COMMAND \${CMAKE_COMMAND} -E create_symlink
      libchartdir.so.7.0.0
      \${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/psp-ufu/libchartdir.so.7.0
    )
  ")

endif()