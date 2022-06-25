#pragma once

#include "../Renderer/DSCS/Renderer.hpp"

using namespace Rendering::DSCS;

#ifdef __cplusplus
extern "C" {
#endif

#define SHARED_EXPORT

#ifdef _WIN32
    #ifdef SHARED_EXPORT
        #define DLL_EXPORT __declspec(dllexport)
    #else
        #define DLL_EXPORT __declspec(dllimport)
    #endif
#else
    #define DLL_EXPORT
#endif

    DLL_EXPORT Renderer* __stdcall new_Renderer();
    DLL_EXPORT void __stdcall del_Renderer(Renderer* renderer);
    DLL_EXPORT int __stdcall initRenderer(Renderer* renderer);
    DLL_EXPORT void __stdcall refreshRenderSettings(Renderer* renderer);
    DLL_EXPORT void __stdcall recalculateGlobalUniforms(Renderer* renderer);
    DLL_EXPORT char* __stdcall render(Renderer* renderer, int fbo_id, int x, int y, int width, int height, int stride, long pixel_address);
    DLL_EXPORT void __stdcall loadModel(Renderer* renderer, char* filepath);
    DLL_EXPORT void __stdcall setCameraPosition(Renderer* renderer, float x, float y, float z);

#ifdef __cplusplus
}
#endif
