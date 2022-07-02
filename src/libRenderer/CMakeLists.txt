cmake_minimum_required (VERSION 3.12)

project("SharedDSCSRenderer")
set(CMAKE_SHARED_LIBRARY_PREFIX="")

add_library(${PROJECT_NAME} SHARED
			"export.h"
			"export.cpp"
 )
 
include_directories("../Renderer")
target_link_libraries(${PROJECT_NAME} DSCSRenderer)
