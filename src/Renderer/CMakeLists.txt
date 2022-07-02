cmake_minimum_required (VERSION 3.12)

project("DSCSRenderer")
set(CMAKE_SHARED_LIBRARY_PREFIX="")

add_library(${PROJECT_NAME} STATIC
			"DSCS/AnimationBuffer.hpp"
            "DSCS/Renderer.hpp" 
            "DSCS/Renderer.cpp"
            "DSCS/DataObjects/AnimationDataBlocks.hpp"
            "DSCS/DataObjects/AnimationSampler.cpp" 
            "DSCS/DataObjects/AnimationSampler.hpp"
            "DSCS/DataObjects/OpenGLDSCSMaterial.cpp"
            "DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
            "DSCS/DataObjects/OpenGLDSCSMesh.cpp"
            "DSCS/DataObjects/OpenGLDSCSMesh.hpp"
            "DSCS/DataObjects/OpenGLDSCSModel.hpp" 
            "DSCS/DataObjects/OpenGLDSCSTexture.cpp" 
            "DSCS/DataObjects/OpenGLDSCSTexture.hpp"
            "DSCS/DataObjects/SkeletonDataBlocks.cpp"
            "DSCS/DataObjects/SkeletonDataBlocks.hpp"
            "DSCS/RenderObjects/Camera.cpp"
            "DSCS/ShaderSystem/cgGL/cgGLShaderBackend.cpp"
            "DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
            "DSCS/ShaderSystem/cgGL/cgGLShaderObject.cpp"
            "DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
            "DSCS/ShaderSystem/cgGL/ShaderVariables.hpp"
            "DSCS/ShaderSystem/cgGL/Utils.cpp" 
            "DSCS/ShaderSystem/cgGL/Utils.hpp" 
            "DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.hpp" 
            "DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.cpp" 
            "DSCS/ShaderSystem/cgGL/ShaderUniforms/BaseTexture.hpp" 
            "DSCS/ShaderSystem/cgGL/ShaderUniforms/ShaderUniforms.hpp"
            "DSCS/ShaderSystem/cgGL/ShaderUniforms/Tex2D.hpp"
            "DSCS/ShaderSystem/cgGL/ShaderUniforms/TexCube.hpp"
            "FileFormats/DSCS/DSCSModelForwardDeclaration.hpp" 
            "FileFormats/DSCS/DSCStoOpenGL.cpp" 
            "FileFormats/DSCS/DSCStoOpenGL.hpp" 
            "FileFormats/DSCS/AnimFile/AnimReadWrite.cpp" 
            "FileFormats/DSCS/AnimFile/AnimReadWrite.hpp" 
            "FileFormats/DSCS/GeomFile/CamDataStruct.hpp"
            "FileFormats/DSCS/GeomFile/GeomForwardDeclarations.hpp"
            "FileFormats/DSCS/GeomFile/GeomReadWrite.cpp"
            "FileFormats/DSCS/GeomFile/GeomReadWrite.hpp"
            "FileFormats/DSCS/GeomFile/LightSourceDataStruct.hpp"
            "FileFormats/DSCS/GeomFile/MaterialReadWrite.cpp"
            "FileFormats/DSCS/GeomFile/MaterialReadWrite.hpp"
            "FileFormats/DSCS/GeomFile/MeshReadWrite.cpp" 
            "FileFormats/DSCS/GeomFile/MeshReadWrite.hpp" 
            "FileFormats/DSCS/NameFile/NameReadWrite.cpp"
            "FileFormats/DSCS/NameFile/NameReadWrite.hpp" 
            "FileFormats/DSCS/SkelFile/SkelReadWrite.cpp" 
            "FileFormats/DSCS/SkelFile/SkelReadWrite.hpp" 
            "FileFormats/Textures/DDS.cpp" 
            "FileFormats/Textures/DDS.hpp" 
            "glad/include/glad/glad.h"
            "glad/include/KHR/khrplatform.h"
            "glad/src/glad.c"
            "serialisation/Exceptions.cpp"
            "serialisation/Exceptions.hpp" 
            "serialisation/Macros.hpp"
            "serialisation/ReadWriter.cpp" 
            "serialisation/ReadWriter.hpp"
            "serialisation/serialisation.hpp"
            "Utils/BitManip.hpp"
            "Utils/BitManip.cpp"
            "Utils/Float16.hpp"
            "Utils/Float16.cpp"
            "Utils/Hashing.hpp"
            "Utils/Hashing.cpp"
            "Utils/Matrix.hpp"
            "Utils/Matrix.cpp"
            "Utils/OpenGL.hpp"
            "Utils/OpenGL.cpp"
            "Utils/SubarrayView.hpp"
            "Utils/Vector.hpp"
            "Utils/Vector.cpp"
 )
 
# Set up Cg
# Static
if (WIN32)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/cg.lib"
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/cgGL.lib"
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/glut32.lib")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/cg.dll"  $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/cgGL.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/glut32.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
endif (WIN32)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/mac/libCg.dylib")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/mac/libCg.dylib"
    )
endif(APPLE)
if (UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} 
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/linux64/libCg.so"
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/linux64/libCgGL.so")

    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/linux64/libCg.so"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/linux64/libCgGL.so"
    )
endif(UNIX AND NOT APPLE)
