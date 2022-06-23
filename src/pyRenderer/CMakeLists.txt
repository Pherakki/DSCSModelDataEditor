cmake_minimum_required(VERSION 3.12.0)
set (CMAKE_CXX_STANDARD 20)

project("pyDSCSRenderer")

find_package(Python 3 COMPONENTS Development)
find_package(Boost COMPONENTS python38 REQUIRED)

set(CMAKE_SHARED_LIBRARY_PREFIX="")

add_library(${PROJECT_NAME} MODULE
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
