cmake_minimum_required(VERSION 3.19)

option(MAYASDK_INCLUDE_DIR "Path to Autodesk Maya SDK header directory")
option(MAYASDK_LIBRARY_DIR "Path to Autodesk Maya SDK library directory")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(MayaXpsImport VERSION 1.0)

set(MXI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/xpsImport/include")

include_directories(
	${MAYASDK_INCLUDE_DIR}
	${MXI_INCLUDE_DIR}
)

file(GLOB xpsCode "${CMAKE_SOURCE_DIR}/xpsImport/source/*.cpp")
add_library(MayaXpsImport SHARED ${xpsCode})