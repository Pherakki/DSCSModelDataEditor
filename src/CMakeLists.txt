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
                "UI/MainWindow.cpp"
                "UI/MainWindow.hpp"
                "UI/RenderWidget.cpp"
                "UI/EditorTabs/SkeletonEditor.hpp" 
                "UI/EditorTabs/AnimationEditor.cpp"
                "UI/CgSyntaxHighlighter.hpp" 
                "UI/EditorTabs/MeshEditor.hpp" 
                "UI/Spoiler/Spoiler.hpp" 
                "UI/Spoiler/Spoiler.cpp"  
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
)

# Allow CMake to build libraries
cmake_policy(SET CMP0079 NEW)

# Set up Qt5
message(PROJECT_SOURCE_DIR="${Qt5_DIR}")
# The Qt5_DIR needs to be set to Qt5/lib/cmake/Qt5 of your Qt5 install
set(Qt5_DIR "../libs/Qt5/lib/cmake/Qt5") # Need to get rid of this eventually?
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

# Link the Renderer
add_subdirectory("DSCSRenderer")
target_link_libraries(${PROJECT_NAME} DSCSRenderer)


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
