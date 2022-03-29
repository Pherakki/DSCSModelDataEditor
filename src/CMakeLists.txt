﻿# CMakeList.txt : CMake project for DSCSModelEditorCMake, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.12)

# Add source to this project's executable.
add_executable (${PROJECT_NAME} 
                ${MOC_Files}
                "DSCSModelEditorCMake.cpp"
                "Commands/CommandManager.cpp" 
                "Commands/Graveyard.cpp" 
                "Commands/ICommand.cpp"
                "FileFormats/DSCS/DSCStoOpenGL.cpp"
                "FileFormats/DSCS/AnimFile/AnimReadWrite.cpp" 
                "FileFormats/DSCS/GeomFile/MaterialReadWrite.cpp" 
                "FileFormats/DSCS/GeomFile/MeshReadWrite.cpp" 
                "FileFormats/DSCS/GeomFile/GeomReadWrite.cpp"
                "FileFormats/DSCS/NameFile/NameReadWrite.cpp"
                "FileFormats/DSCS/SkelFile/SkelReadWrite.cpp"
                "FileFormats/Textures/DDS.cpp"
                "Rendering/DSCS/DataObjects/AnimationSampler.cpp" 
                "Rendering/DSCS/DataObjects/OpenGLDSCSTexture.cpp" 
                "Rendering/DSCS/DataObjects/OpenGLDSCSMesh.cpp" 
                "Rendering/DSCS/DataObjects/OpenGLDSCSMaterial.cpp" 
                "Rendering/DSCS/DataObjects/SkeletonDataBlocks.cpp"
                "Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderObject.cpp" 
                "Rendering/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.cpp"
                "Rendering/DSCS/ShaderSystem/cgGL/Utils.cpp" 
                "serialisation/Exceptions.cpp"
                "serialisation/ReadWriter.cpp" 
                "Utils/Hashing.cpp" 
                "Utils/Matrix.cpp" 
                "Utils/Vector.cpp" 
                "UI/MainWindow.cpp"
                "UI/MainWindow.hpp"
                "UI/RenderWidget.cpp"
                "UI/EditorTabs/SkeletonEditor.hpp" 
                "UI/EditorTabs/AnimationEditor.cpp"
                "UI/CgSyntaxHighlighter.hpp" "UI/EditorTabs/ShaderEditors/ShaderFactory.cpp")

# Allow CMake to build libraries
cmake_policy(SET CMP0079 NEW)

# Set up Qt5
set(Qt5_DIR "../lib/Qt5/lib/cmake/Qt5") # Need to get rid of this eventually?

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Gui REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)


target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets  
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/lib/Qt5/plugins" $<TARGET_FILE_DIR:${PROJECT_NAME}> # Probably don't need the entire folder...
    )

# Set up Cg
# Static
if (WIN32)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/Cg/windows/cg.lib"
        "${CMAKE_SOURCE_DIR}/Cg/windows/cgGL.lib"
        "${CMAKE_SOURCE_DIR}/Cg/windows/glut32.lib")
endif (WIN32)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/Cg/mac/libCg.dylib")
endif(UNIX)
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/Cg/linux64/libCg.so"
        "${CMAKE_SOURCE_DIR}/Cg/linux64/libCgGL.so")
endif(UNIX)

# Shared - Doesn't work, it just copies files called cgLIB.lib etc instead of cg.dll...
# add_library(cgLIB "../lib/Cg/lib/cg.dll")
# add_library(cgGLLIB "../lib/Cg/lib/cgGL.dll")
# add_library(glut32LIB "../lib/Cg/lib/glut32.dll")

# add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:cgLIB> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:cgGLLIB> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:glut32LIB> $<TARGET_FILE_DIR:${PROJECT_NAME}>
#     )