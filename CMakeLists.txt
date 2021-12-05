# CMake Versioning - required for C++20
cmake_minimum_required (VERSION 3.12)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define project
project ("DSCSModelEditorCMake")

# Include sub-projects
add_subdirectory ("src")

# Done
