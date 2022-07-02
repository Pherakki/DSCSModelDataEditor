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
            "../Renderer/libs/glad/include/glad/glad.h"
            "../Renderer/libs/glad/include/KHR/khrplatform.h"
            "../Renderer/libs/glad/src/glad.c"
			"../Renderer/src/DSCS/AnimationBuffer.hpp"
            "../Renderer/src/DSCS/Renderer.hpp" 
            "../Renderer/src/DSCS/Renderer.cpp"
            "../Renderer/src/DSCS/DataObjects/AnimationDataBlocks.hpp"
            "../Renderer/src/DSCS/DataObjects/AnimationSampler.cpp" 
            "../Renderer/src/DSCS/DataObjects/AnimationSampler.hpp"
            "../Renderer/src/DSCS/DataObjects/OpenGLDSCSMaterial.cpp"
            "../Renderer/src/DSCS/DataObjects/OpenGLDSCSMaterial.hpp"
            "../Renderer/src/DSCS/DataObjects/OpenGLDSCSMesh.cpp"
            "../Renderer/src/DSCS/DataObjects/OpenGLDSCSMesh.hpp"
            "../Renderer/src/DSCS/DataObjects/OpenGLDSCSModel.hpp" 
            "../Renderer/src/DSCS/DataObjects/OpenGLDSCSTexture.cpp" 
            "../Renderer/src/DSCS/DataObjects/OpenGLDSCSTexture.hpp"
            "../Renderer/src/DSCS/DataObjects/SkeletonDataBlocks.cpp"
            "../Renderer/src/DSCS/DataObjects/SkeletonDataBlocks.hpp"
            "../Renderer/src/DSCS/RenderObjects/Camera.cpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.cpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/cgGLShaderBackend.hpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/cgGLShaderObject.cpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/cgGLShaderObject.hpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/ShaderVariables.hpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/Utils.cpp" 
            "../Renderer/src/DSCS/ShaderSystem/cgGL/Utils.hpp" 
            "../Renderer/src/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.hpp" 
            "../Renderer/src/DSCS/ShaderSystem/cgGL/OpenGLSettings/OpenGLSettings.cpp" 
            "../Renderer/src/DSCS/ShaderSystem/cgGL/ShaderUniforms/BaseTexture.hpp" 
            "../Renderer/src/DSCS/ShaderSystem/cgGL/ShaderUniforms/ShaderUniforms.hpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/ShaderUniforms/Tex2D.hpp"
            "../Renderer/src/DSCS/ShaderSystem/cgGL/ShaderUniforms/TexCube.hpp"
            "../Renderer/src/FileFormats/DSCS/DSCSModelForwardDeclaration.hpp" 
            "../Renderer/src/FileFormats/DSCS/DSCStoOpenGL.cpp" 
            "../Renderer/src/FileFormats/DSCS/DSCStoOpenGL.hpp" 
            "../Renderer/src/FileFormats/DSCS/AnimFile/AnimReadWrite.cpp" 
            "../Renderer/src/FileFormats/DSCS/AnimFile/AnimReadWrite.hpp" 
            "../Renderer/src/FileFormats/DSCS/GeomFile/CamDataStruct.hpp"
            "../Renderer/src/FileFormats/DSCS/GeomFile/GeomForwardDeclarations.hpp"
            "../Renderer/src/FileFormats/DSCS/GeomFile/GeomReadWrite.cpp"
            "../Renderer/src/FileFormats/DSCS/GeomFile/GeomReadWrite.hpp"
            "../Renderer/src/FileFormats/DSCS/GeomFile/LightSourceDataStruct.hpp"
            "../Renderer/src/FileFormats/DSCS/GeomFile/MaterialReadWrite.cpp"
            "../Renderer/src/FileFormats/DSCS/GeomFile/MaterialReadWrite.hpp"
            "../Renderer/src/FileFormats/DSCS/GeomFile/MeshReadWrite.cpp" 
            "../Renderer/src/FileFormats/DSCS/GeomFile/MeshReadWrite.hpp" 
            "../Renderer/src/FileFormats/DSCS/NameFile/NameReadWrite.cpp"
            "../Renderer/src/FileFormats/DSCS/NameFile/NameReadWrite.hpp" 
            "../Renderer/src/FileFormats/DSCS/SkelFile/SkelReadWrite.cpp" 
            "../Renderer/src/FileFormats/DSCS/SkelFile/SkelReadWrite.hpp" 
            "../Renderer/src/FileFormats/Textures/DDS.cpp" 
            "../Renderer/src/FileFormats/Textures/DDS.hpp" 
            "../Renderer/src/serialisation/Exceptions.cpp"
            "../Renderer/src/serialisation/Exceptions.hpp" 
            "../Renderer/src/serialisation/Macros.hpp"
            "../Renderer/src/serialisation/ReadWriter.cpp" 
            "../Renderer/src/serialisation/ReadWriter.hpp"
            "../Renderer/src/serialisation/serialisation.hpp"
            "../Renderer/src/Utils/BitManip.hpp"
            "../Renderer/src/Utils/BitManip.cpp"
            "../Renderer/src/Utils/Float16.hpp"
            "../Renderer/src/Utils/Float16.cpp"
            "../Renderer/src/Utils/Hashing.hpp"
            "../Renderer/src/Utils/Hashing.cpp"
            "../Renderer/src/Utils/Matrix.hpp"
            "../Renderer/src/Utils/Matrix.cpp"
            "../Renderer/src/Utils/OpenGL.hpp"
            "../Renderer/src/Utils/OpenGL.cpp"
            "../Renderer/src/Utils/SubarrayView.hpp"
            "../Renderer/src/Utils/Vector.hpp"
            "../Renderer/src/Utils/Vector.cpp"
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
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/cg.lib"
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/cgGL.lib"
        "${CMAKE_SOURCE_DIR}/src/Renderer/Cg/windows/glut32.lib")

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
