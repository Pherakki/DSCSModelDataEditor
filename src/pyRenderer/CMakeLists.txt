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
            "../glad/include/glad/glad.h"
            "../glad/include/KHR/khrplatform.h"
            "../glad/src/glad.c"
			"../Renderer/DSCS/AnimationBuffer.hpp"
            "../Renderer/DSCS/Renderer.hpp" 
            "../Renderer/DSCS/Renderer.cpp"
            "../Renderer/DSCS/DataObjects/AnimationDataBlocks.hpp"
            "../Renderer/DSCS/DataObjects/AnimationSampler.cpp" 
            "../Renderer/DSCS/DataObjects/AnimationSampler.hpp"
            "../Renderer/DSCS/DataObjects/OpenGLDSCSMaterial.cpp"
            "../Renderer/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
            "../Renderer/DSCS/DataObjects/OpenGLDSCSMesh.cpp"
            "../Renderer/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
            "../Renderer/DSCS/DataObjects/OpenGLDSCSModel.hpp" 
            "../Renderer/DSCS/DataObjects/OpenGLDSCSTexture.cpp" 
            "../Renderer/DSCS/DataObjects/OpenGLDSCSTexture.hpp"
            "../Renderer/DSCS/DataObjects/SkeletonDataBlocks.cpp"
            "../Renderer/DSCS/DataObjects/SkeletonDataBlocks.hpp"
            "../Renderer/DSCS/RenderObjects/Camera.cpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.cpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderObject.cpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/ShaderVariables.hpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/Utils.cpp" 
            "../Renderer/DSCS/ShaderSystem/cgGL/Utils.hpp" 
            "../Renderer/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.hpp" 
            "../Renderer/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.cpp" 
            "../Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/BaseTexture.hpp" 
            "../Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/ShaderUniforms.hpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/Tex2D.hpp"
            "../Renderer/DSCS/ShaderSystem/cgGL/ShaderUniforms/TexCube.hpp"
            "../Renderer/FileFormats/DSCS/DSCSModelForwardDeclaration.hpp" 
            "../Renderer/FileFormats/DSCS/DSCStoOpenGL.cpp" 
            "../Renderer/FileFormats/DSCS/DSCStoOpenGL.hpp" 
            "../Renderer/FileFormats/DSCS/AnimFile/AnimReadWrite.cpp" 
            "../Renderer/FileFormats/DSCS/AnimFile/AnimReadWrite.hpp" 
            "../Renderer/FileFormats/DSCS/GeomFile/CamDataStruct.hpp"
            "../Renderer/FileFormats/DSCS/GeomFile/GeomForwardDeclarations.hpp"
            "../Renderer/FileFormats/DSCS/GeomFile/GeomReadWrite.cpp"
            "../Renderer/FileFormats/DSCS/GeomFile/GeomReadWrite.hpp"
            "../Renderer/FileFormats/DSCS/GeomFile/LightSourceDataStruct.hpp"
            "../Renderer/FileFormats/DSCS/GeomFile/MaterialReadWrite.cpp"
            "../Renderer/FileFormats/DSCS/GeomFile/MaterialReadWrite.hpp"
            "../Renderer/FileFormats/DSCS/GeomFile/MeshReadWrite.cpp" 
            "../Renderer/FileFormats/DSCS/GeomFile/MeshReadWrite.hpp" 
            "../Renderer/FileFormats/DSCS/NameFile/NameReadWrite.cpp"
            "../Renderer/FileFormats/DSCS/NameFile/NameReadWrite.hpp" 
            "../Renderer/FileFormats/DSCS/SkelFile/SkelReadWrite.cpp" 
            "../Renderer/FileFormats/DSCS/SkelFile/SkelReadWrite.hpp" 
            "../Renderer/FileFormats/Textures/DDS.cpp" 
            "../Renderer/FileFormats/Textures/DDS.hpp" 
            "../Renderer/serialisation/Exceptions.cpp"
            "../Renderer/serialisation/Exceptions.hpp" 
            "../Renderer/serialisation/Macros.hpp"
            "../Renderer/serialisation/ReadWriter.cpp" 
            "../Renderer/serialisation/ReadWriter.hpp"
            "../Renderer/serialisation/serialisation.hpp"
            "../Renderer/Utils/BitManip.hpp"
            "../Renderer/Utils/BitManip.cpp"
            "../Renderer/Utils/Float16.hpp"
            "../Renderer/Utils/Float16.cpp"
            "../Renderer/Utils/Hashing.hpp"
            "../Renderer/Utils/Hashing.cpp"
            "../Renderer/Utils/Matrix.hpp"
            "../Renderer/Utils/Matrix.cpp"
            "../Renderer/Utils/OpenGL.hpp"
            "../Renderer/Utils/OpenGL.cpp"
            "../Renderer/Utils/SubarrayView.hpp"
            "../Renderer/Utils/Vector.hpp"
            "../Renderer/Utils/Vector.cpp"
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