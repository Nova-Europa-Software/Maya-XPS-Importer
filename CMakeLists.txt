# Require at least CMake 3.19
cmake_minimum_required(VERSION 3.19)

# Path to Maya SDK include directory.
# Expected to be provided by CMakePresets.json or via command line.
option(MAYASDK_INCLUDE_DIR "Path to Autodesk Maya SDK header directory")

# Path to Maya SDK library directory.
# Expected to be provided by CMakePresets.json or via command line.
option(MAYASDK_LIBRARY_DIR "Path to Autodesk Maya SDK library directory")

# Use C++20 and require compiler support for it
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Define project name and version
project(MayaXpsImport VERSION 1.0)

# Local include directory containing plugin headers
set(MXI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/xpsImport/include")

# Add header search paths for the compiler
include_directories(
    ${MAYASDK_INCLUDE_DIR}    # Autodesk Maya SDK headers
    ${MXI_INCLUDE_DIR}        # MayaXpsImport project headers
)

# Point link directories to Maya SDK
link_directories(${MAYASDK_LIBRARY_DIR})

# Link project against Maya SDK libraries
link_libraries("OpenMaya.lib" "OpenMayaAnim.lib" "Foundation.lib")

# Collect all C++ source files from the source directory
# GLOB automatically finds all .cpp files matching the pattern
file(GLOB xpsCode
    "${CMAKE_SOURCE_DIR}/xpsImport/source/*.cpp"
)

# Build a shared library (DLL on Windows, .so on Linux, .dylib on macOS)
# using all collected source files
add_library(MayaXpsImport SHARED ${xpsCode})

set_target_properties(MayaXpsImport PROPERTIES 
    PREFIX ""
    SUFFIX ".mll"
)