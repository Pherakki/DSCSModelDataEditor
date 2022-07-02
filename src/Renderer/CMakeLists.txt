cmake_minimum_required (VERSION 3.12)

project("DSCSRenderer")
set(CMAKE_SHARED_LIBRARY_PREFIX="")

add_library(${PROJECT_NAME} STATIC
            "libs/glad/include/glad/glad.h"
            "libs/glad/include/KHR/khrplatform.h"
            "libs/glad/src/glad.c"
			"src/DSCS/AnimationBuffer.hpp"
            "src/DSCS/Renderer.hpp" 
            "src/DSCS/Renderer.cpp"
            "src/DSCS/DataObjects/AnimationDataBlocks.hpp"
            "src/DSCS/DataObjects/AnimationSampler.cpp" 
            "src/DSCS/DataObjects/AnimationSampler.hpp"
            "src/DSCS/DataObjects/OpenGLDSCSMaterial.cpp"
            "src/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
            "src/DSCS/DataObjects/OpenGLDSCSMesh.cpp"
            "src/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
            "src/DSCS/DataObjects/OpenGLDSCSModel.hpp" 
            "src/DSCS/DataObjects/OpenGLDSCSTexture.cpp" 
            "src/DSCS/DataObjects/OpenGLDSCSTexture.hpp"
            "src/DSCS/DataObjects/SkeletonDataBlocks.cpp"
            "src/DSCS/DataObjects/SkeletonDataBlocks.hpp"
            "src/DSCS/RenderObjects/Camera.cpp"
            "src/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.cpp"
            "src/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
            "src/DSCS/ShaderSystem/cgGL/cgGLShaderObject.cpp"
            "src/DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
            "src/DSCS/ShaderSystem/cgGL/ShaderVariables.hpp"
            "src/DSCS/ShaderSystem/cgGL/Utils.cpp" 
            "src/DSCS/ShaderSystem/cgGL/Utils.hpp" 
            "src/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.hpp" 
            "src/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.cpp" 
            "src/DSCS/ShaderSystem/cgGL/ShaderUniforms/BaseTexture.hpp" 
            "src/DSCS/ShaderSystem/cgGL/ShaderUniforms/ShaderUniforms.hpp"
            "src/DSCS/ShaderSystem/cgGL/ShaderUniforms/Tex2D.hpp"
            "src/DSCS/ShaderSystem/cgGL/ShaderUniforms/TexCube.hpp"
            "src/FileFormats/DSCS/DSCSModelForwardDeclaration.hpp" 
            "src/FileFormats/DSCS/DSCStoOpenGL.cpp" 
            "src/FileFormats/DSCS/DSCStoOpenGL.hpp" 
            "src/FileFormats/DSCS/AnimFile/AnimReadWrite.cpp" 
            "src/FileFormats/DSCS/AnimFile/AnimReadWrite.hpp" 
            "src/FileFormats/DSCS/GeomFile/CamDataStruct.hpp"
            "src/FileFormats/DSCS/GeomFile/GeomForwardDeclarations.hpp"
            "src/FileFormats/DSCS/GeomFile/GeomReadWrite.cpp"
            "src/FileFormats/DSCS/GeomFile/GeomReadWrite.hpp"
            "src/FileFormats/DSCS/GeomFile/LightSourceDataStruct.hpp"
            "src/FileFormats/DSCS/GeomFile/MaterialReadWrite.cpp"
            "src/FileFormats/DSCS/GeomFile/MaterialReadWrite.hpp"
            "src/FileFormats/DSCS/GeomFile/MeshReadWrite.cpp" 
            "src/FileFormats/DSCS/GeomFile/MeshReadWrite.hpp" 
            "src/FileFormats/DSCS/NameFile/NameReadWrite.cpp"
            "src/FileFormats/DSCS/NameFile/NameReadWrite.hpp" 
            "src/FileFormats/DSCS/SkelFile/SkelReadWrite.cpp" 
            "src/FileFormats/DSCS/SkelFile/SkelReadWrite.hpp" 
            "src/FileFormats/Textures/DDS.cpp" 
            "src/FileFormats/Textures/DDS.hpp" 
            "src/serialisation/Exceptions.cpp"
            "src/serialisation/Exceptions.hpp" 
            "src/serialisation/Macros.hpp"
            "src/serialisation/ReadWriter.cpp" 
            "src/serialisation/ReadWriter.hpp"
            "src/serialisation/serialisation.hpp"
            "src/Utils/BitManip.hpp"
            "src/Utils/BitManip.cpp"
            "src/Utils/Float16.hpp"
            "src/Utils/Float16.cpp"
            "src/Utils/Hashing.hpp"
            "src/Utils/Hashing.cpp"
            "src/Utils/Matrix.hpp"
            "src/Utils/Matrix.cpp"
            "src/Utils/OpenGL.hpp"
            "src/Utils/OpenGL.cpp"
            "src/Utils/SubarrayView.hpp"
            "src/Utils/Vector.hpp"
            "src/Utils/Vector.cpp"
 )
 
# Set up Cg
# Static
if (WIN32)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/windows/cg.lib"
        "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/windows/cgGL.lib"
        "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/windows/glut32.lib")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/windows/cg.dll"  $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/windows/cgGL.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/windows/glut32.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif (WIN32)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/mac/libCg.dylib")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/mac/libCg.dylib"
    )
endif(APPLE)
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/linux64/libCg.so"
        "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/linux64/libCgGL.so")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/linux64/libCg.so"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/src/Renderer/libs/Cg/linux64/libCgGL.so"
    )
endif(UNIX AND NOT APPLE)
