# CMakeList.txt : CMake project for DSCSModelEditorCMake, include source and define
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
                "glad/include/glad/glad.h"
                "glad/include/KHR/khrplatform.h"
                "glad/src/glad.c"
                "Renderer/Utils/Hashing.cpp" 
                "Renderer/Utils/Matrix.cpp" 
                "Renderer/Utils/Vector.cpp" 
                "UI/MainWindow.cpp"
                "UI/MainWindow.hpp"
                "UI/RenderWidget.cpp"
                "UI/EditorTabs/SkeletonEditor.hpp" 
                "UI/EditorTabs/AnimationEditor.cpp"
                "UI/CgSyntaxHighlighter.hpp" 
                "UI/EditorTabs/MeshEditor.hpp" 
                "UI/Spoiler/Spoiler.hpp" 
                "UI/Spoiler/Spoiler.cpp"  
                "Renderer/Utils/Float16.hpp"  
                "Renderer/Utils/Float16.cpp"
                "UI/EditorTabs/MaterialEditor/MaterialEditor.hpp" 
                "UI/EditorTabs/MaterialEditor/OpenGLSettingsEditor.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/CodeEditor.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderEditorTabs.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/PrebuiltShaders.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderFactory.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderFactory.cpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexShader.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/Utils.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexShader.cpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/FragmentShader.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/FragmentShader.cpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/Utils.cpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/CodeGenerator.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/CodeGenerator/Node.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/CodeGenerator/DependencyGraph.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexGenerator/NewVertexShader.cpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexGenerator/NewVertexShader.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexGenerator/Blocks/ZBias.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexGenerator/Blocks/GenerateUVTransform.hpp"  
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexGenerator/Blocks/GenerateUVTransform/UI.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexGenerator/Blocks/GenerateUVTransform/Logic.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/VertexGenerator/Blocks/GenerateHeader.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BaseWidgets/CheckableWidget.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BaseWidgets/EditorTextbox.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BaseWidgets/FileCombobox.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BaseWidgets/ToggleableCombobox.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BaseWidgets/ToggleableTextboxes.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BaseWidgets/TextboxArrayWidget.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BaseWidgets/TitleWidget.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/BumpmapBox.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/DiffuseColor.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/GlassMap.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/Illumination.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/ParallaxBox.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/PositionSettings.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/Reflection.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/SpecularColor.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/TextureLayer1.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/TextureLayer2.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/TextureMap.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/TextureSlot.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/Widgets/UVSlot.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/TabMaterials.hpp"
                "UI/SelectedObjectReferences.hpp" 
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/CodeGenerator/VertexShader/VertexShader.hpp"
                "UI/EditorTabs/MaterialEditor/ShaderEditors/ShaderFactory/ShaderGenerator/CodeGenerator/VertexShader/VertexShader.cpp"
                "Renderer/Utils/OpenGL.cpp"
                "Renderer/Utils/BitManip.hpp" 
                "Renderer/Utils/BitManip.cpp" 
                "Renderer/DSCS/AnimationBuffer.hpp"
                "Renderer/DSCS/Renderer.hpp" 
                "Renderer/DSCS/Renderer.cpp"
                "Renderer/DSCS/DataObjects/AnimationDataBlocks.hpp"
                "Renderer/DSCS/DataObjects/AnimationSampler.cpp" 
                "Renderer/DSCS/DataObjects/AnimationSampler.hpp"
                "Renderer/DSCS/DataObjects/OpenGLDSCSMaterial.cpp"
                "Renderer/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
                "Renderer/DSCS/DataObjects/OpenGLDSCSMesh.cpp"
                "Renderer/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
                "Renderer/DSCS/DataObjects/OpenGLDSCSModel.hpp" 
                "Renderer/DSCS/DataObjects/OpenGLDSCSTexture.cpp" 
                "Renderer/DSCS/DataObjects/OpenGLDSCSTexture.hpp"
                "Renderer/DSCS/DataObjects/SkeletonDataBlocks.cpp"
                "Renderer/DSCS/DataObjects/SkeletonDataBlocks.hpp"
                "Renderer/DSCS/RenderObjects/Camera.cpp"
                "Renderer/DSCS/RenderObjects/TextureLibrary.hpp" 
                "Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.cpp"
                "Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
                "Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderObject.cpp"
                "Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
                "Renderer/DSCS/ShaderSystem/cgGL/ShaderVariables.hpp"
                "Renderer/DSCS/ShaderSystem/cgGL/Utils.cpp" 
                "Renderer/DSCS/ShaderSystem/cgGL/Utils.hpp" 
                "Renderer/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.hpp" 
                "Renderer/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.cpp" 
                "Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/BaseTexture.hpp" 
                "Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/ShaderUniforms.hpp"
                "Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/Tex2D.hpp"
                "Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/TexCube.hpp"
                "Renderer/FileFormats/DSCS/DSCSModelForwardDeclaration.hpp" 
                "Renderer/FileFormats/DSCS/DSCStoOpenGL.cpp" 
                "Renderer/FileFormats/DSCS/DSCStoOpenGL.hpp" 
                "Renderer/FileFormats/DSCS/AnimFile/AnimReadWrite.cpp" 
                "Renderer/FileFormats/DSCS/AnimFile/AnimReadWrite.hpp" 
                "Renderer/FileFormats/DSCS/GeomFile/CamDataStruct.hpp"
                "Renderer/FileFormats/DSCS/GeomFile/GeomForwardDeclarations.hpp"
                "Renderer/FileFormats/DSCS/GeomFile/GeomReadWrite.cpp"
                "Renderer/FileFormats/DSCS/GeomFile/GeomReadWrite.hpp"
                "Renderer/FileFormats/DSCS/GeomFile/LightSourceDataStruct.hpp"
                "Renderer/FileFormats/DSCS/GeomFile/MaterialReadWrite.cpp"
                "Renderer/FileFormats/DSCS/GeomFile/MaterialReadWrite.hpp"
                "Renderer/FileFormats/DSCS/GeomFile/MeshReadWrite.cpp" 
                "Renderer/FileFormats/DSCS/GeomFile/MeshReadWrite.hpp" 
                "Renderer/FileFormats/DSCS/NameFile/NameReadWrite.cpp"
                "Renderer/FileFormats/DSCS/NameFile/NameReadWrite.hpp" 
                "Renderer/FileFormats/DSCS/SkelFile/SkelReadWrite.cpp" 
                "Renderer/FileFormats/DSCS/SkelFile/SkelReadWrite.hpp" 
                "Renderer/FileFormats/Textures/DDS.cpp" 
                "Renderer/FileFormats/Textures/DDS.hpp" 
                "Renderer/serialisation/Exceptions.cpp"
                "Renderer/serialisation/Exceptions.hpp" 
                "Renderer/serialisation/Macros.hpp"
                "Renderer/serialisation/ReadWriter.cpp" 
                "Renderer/serialisation/ReadWriter.hpp"
                "Renderer/serialisation/serialisation.hpp" )

# Allow CMake to build libraries
cmake_policy(SET CMP0079 NEW)

# Set up Qt5
message(PROJECT_SOURCE_DIR="${Qt5_DIR}")
# The Qt5_DIR needs to be set to Qt5/lib/cmake/Qt5 of your Qt5 install
set(Qt5_DIR "../lib/Qt5/lib/cmake/Qt5") # Need to get rid of this eventually?
set(Qt5_PLUGINS_DIR ${Qt5_DIR}/../../../plugins)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTORCC TRUE)
set_target_properties(${PROJECT_NAME} PROPERTIES AUTOUIC TRUE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Core 5.10 REQUIRED)
find_package(Qt5Gui 5.10 REQUIRED)
find_package(Qt5Widgets 5.10 REQUIRED)

target_link_libraries(${PROJECT_NAME}
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
)

# Required for GLAD on Ubuntu
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} "${CMAKE_DL_LIBS}")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt5/
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt5/platforms/
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt5/styles/
)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Core> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Gui> $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::Widgets> $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt5/platforms/
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsVistaStylePlugin> $<TARGET_FILE_DIR:${PROJECT_NAME}>/Qt5/styles/
    )
endif (WIN32)

# Set up Cg
# Static
if (WIN32)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/Cg/windows/cg.lib"
        "${CMAKE_SOURCE_DIR}/Cg/windows/cgGL.lib"
        "${CMAKE_SOURCE_DIR}/Cg/windows/glut32.lib")

    # Try to link dlls... get errors...
    #target_link_libraries(${PROJECT_NAME} 
    #    "${CMAKE_SOURCE_DIR}/Cg/windows/cg.dll"
    #    "${CMAKE_SOURCE_DIR}/Cg/windows/cgGL.dll"
    #    "${CMAKE_SOURCE_DIR}/Cg/windows/glut32.dll")

    #add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    #    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Cg/windows/cg.dll"
    #    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Cg/windows/cgGL.dll"
    #    COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Cg/windows/glut32.dll"
    #)
endif (WIN32)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/Cg/mac/libCg.dylib")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Cg/mac/libCg.dylib"
    )
endif(APPLE)
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/Cg/linux64/libCg.so"
        "${CMAKE_SOURCE_DIR}/Cg/linux64/libCgGL.so")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Cg/linux64/libCg.so"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/Cg/linux64/libCgGL.so"
    )
endif(UNIX AND NOT APPLE)
