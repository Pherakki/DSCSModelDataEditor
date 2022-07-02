cmake_minimum_required(VERSION 3.12.0)
set (CMAKE_CXX_STANDARD 20)

project("pyDSCSRenderer")

FIND_PACKAGE(Python3)
FIND_PACKAGE(PythonLibs REQUIRED)

# boost
set(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE(Boost COMPONENTS python${Python3_VERSION_MAJOR}${Python3_VERSION_MINOR} REQUIRED)

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS} )

set(CMAKE_SHARED_LIBRARY_PREFIX="")

add_library(${PROJECT_NAME} SHARED
			"Python.cpp"
)

target_include_directories(${PROJECT_NAME}
	PUBLIC
	${Python_INCLUDE_DIRS}
	${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
	${Boost_LIBRARIES}
	${PYTHON_LIBRARIES}
)

message(Incl: ${Python_INCLUDE_DIRS})
message(Libs: ${PYTHON_LIBRARIES})

include_directories("../Renderer")
target_link_libraries(${PROJECT_NAME} DSCSRenderer)
