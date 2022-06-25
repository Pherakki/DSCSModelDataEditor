#include "../glad/include/glad/glad.h"
#include "export.h"
#include <iostream>

using namespace Rendering::DSCS;

Renderer* __stdcall new_Renderer        ()                                                { return new Renderer();                      }
void __stdcall del_Renderer             (Renderer* renderer)                              { renderer->~Renderer();                      }
int __stdcall initRenderer             (Renderer* renderer)                               
{
    gladLoadGL();
    //renderer->initRenderer(); 
    GLuint fbo_id;
    glGenFramebuffers(1, &fbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id); 

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo_id;
}
int initFBO(Renderer* renderer) 
{
    GLuint fbo_id;
    glGenFramebuffers(1, &fbo_id);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return fbo_id;
}

int initTEX(Renderer* renderer)
{
    return 0;
}

void __stdcall refreshRenderSettings    (Renderer* renderer)                              { renderer->refreshRenderSettings();          }
void __stdcall recalculateGlobalUniforms(Renderer* renderer)                              { renderer->recalculateGlobalUniforms();      }
//void __stdcall render                   (Renderer* renderer)                              { renderer->render();                         }
void __stdcall loadModel(Renderer* renderer, char* filepath)                              
{
    GLint initial_array_buffer = 0, initial_element_buffer = 0;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &initial_array_buffer);
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &initial_element_buffer);
    renderer->loadModel(std::string(filepath)); 
    glBindBuffer(GL_ARRAY_BUFFER, initial_array_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, initial_element_buffer);
}
void __stdcall setCameraPosition        (Renderer* renderer, float x, float y, float z)   { renderer->camera.setPosition({ x,y,z });    }

#define ERRORCODE_REGION_INIT try {
#define ERRORCODE_REGION_REPORT(code) \
} catch (const std::exception& exc) { return const_cast<char*>(exc.what()); }

char* __stdcall render(Renderer* renderer, int fbo_id, int x, int y, int width, int height, int stride, long pixel_address)
{
    GLint initial_fbo = 0;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &initial_fbo);
    GLint initial_tex = 0;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &initial_tex);

    ERRORCODE_REGION_INIT // 1
    glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
    glUseProgram(0);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.1, 0.1, 1.0);
    ERRORCODE_REGION_INIT // 2
    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);

    ERRORCODE_REGION_INIT // 3
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

    // Attach FBO texture.
    ERRORCODE_REGION_INIT // 4
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_id, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    // Render
    ERRORCODE_REGION_INIT // 5
    renderer->render();

    // Blit pixels onto surface
    ERRORCODE_REGION_INIT // 6
    glClearColor(0.8, 0.8, 0.8, 0.8);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(1.0);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();

    glFrontFace(GL_CCW);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPixelStorei(GL_PACK_ROW_LENGTH, stride);
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)pixel_address);
    glPixelStorei(GL_PACK_ROW_LENGTH, 0);

    FILE* out = fopen("out.tga", "w");
    short  TGAhead[] = { 0, 2, 0, 0, 0, 0, width, height, 24 };
    fwrite(&TGAhead, sizeof(TGAhead), 1, out);
    fwrite((void*)pixel_address, 3 * width * height, 1, out);
    fclose(out);

    //// Cleanup.
    ERRORCODE_REGION_INIT // 7
    glDeleteTextures(1, &tex_id);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, initial_fbo);
    glBindFramebuffer(GL_TEXTURE_2D, initial_tex);
    //glDeleteFramebuffers(1, &fbo_id);

    ERRORCODE_REGION_REPORT(7)
    ERRORCODE_REGION_REPORT(6)
    ERRORCODE_REGION_REPORT(5)
    ERRORCODE_REGION_REPORT(4)
    ERRORCODE_REGION_REPORT(3)
    ERRORCODE_REGION_REPORT(2)
    ERRORCODE_REGION_REPORT(1)
    return const_cast<char*>("");
}