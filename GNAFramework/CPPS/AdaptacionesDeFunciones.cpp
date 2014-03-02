#include "AdaptacionesDeFunciones.h"

#define NO_SDL_GLEXT
#include <SDL/SDL_opengl.h>
#include "GL/glext.h"

bool ShadersSuport = false;
bool BuffersSuport = false;


#if defined(__APPLE__)

void setupExtensions() {
    ShadersSuport = true;
    BuffersSuport = true;
};

#elif defined(__X11__) 
#define WIN32_OR_X11

#elif defined(__WIN32__)

PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;
PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;

#define WIN32_OR_X11

#else
void setupExtensions() {
    ShadersSuport = false;
    BuffersSuport = false;
};
#endif


#if defined(WIN32_OR_X11)


/////////////////////////////////// BUFFERS ////////////////////////////////////

//Simple Buffers
PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;

//FrameBuffers
#ifdef _WIN32
// Framebuffer object
PFNGLGENFRAMEBUFFERSEXTPROC                     glGenFramebuffersEXT         = NULL;             // FBO name generation procedure
PFNGLDELETEFRAMEBUFFERSEXTPROC                  glDeleteFramebuffersEXT      = NULL;             // FBO deletion procedure
PFNGLBINDFRAMEBUFFEREXTPROC                     glBindFramebufferEXT         = NULL;             // FBO bind procedure
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC              glCheckFramebufferStatusEXT  = NULL;             // FBO completeness test procedure
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL; // return various FBO parameters
PFNGLGENERATEMIPMAPEXTPROC                      glGenerateMipmapEXT          = NULL;             // FBO automatic mipmap generation procedure
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                glFramebufferTexture2DEXT    = NULL;             // FBO texdture attachement procedure
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC             glFramebufferRenderbufferEXT = NULL;             // FBO renderbuffer attachement procedure
PFNGLDRAWBUFFERSPROC                            glDrawBuffers                = NULL;
// Renderbuffer object
PFNGLGENRENDERBUFFERSEXTPROC                    glGenRenderbuffersEXT        = NULL;             // renderbuffer generation procedure
PFNGLDELETERENDERBUFFERSEXTPROC                 glDeleteRenderbuffersEXT = NULL;                 // renderbuffer deletion procedure
PFNGLBINDRENDERBUFFEREXTPROC                    glBindRenderbufferEXT = NULL;                    // renderbuffer bind procedure
PFNGLRENDERBUFFERSTORAGEEXTPROC                 glRenderbufferStorageEXT = NULL;                 // renderbuffer memory allocation procedure
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC          glGetRenderbufferParameterivEXT = NULL;          // return various renderbuffer parameters
PFNGLISRENDERBUFFEREXTPROC                      glIsRenderbufferEXT = NULL;                      // determine renderbuffer object type
#endif



//////////////////////////////////// SHADERS ///////////////////////////////////


PFNGLACTIVETEXTUREARBPROC        glActiveTextureARB        = NULL;

PFNGLCREATEPROGRAMOBJECTARBPROC  glCreateProgramObjectARB  = NULL;
PFNGLDELETEOBJECTARBPROC         glDeleteObjectARB         = NULL;
PFNGLCREATESHADEROBJECTARBPROC   glCreateShaderObjectARB   = NULL;
PFNGLSHADERSOURCEARBPROC         glShaderSourceARB         = NULL;
PFNGLCOMPILESHADERARBPROC        glCompileShaderARB        = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC         glAttachObjectARB         = NULL;
PFNGLGETINFOLOGARBPROC           glGetInfoLogARB           = NULL;
PFNGLLINKPROGRAMARBPROC          glLinkProgramARB          = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC     glUseProgramObjectARB     = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC   glGetUniformLocationARB   = NULL;

PFNGLUNIFORM4FARBPROC glUniform4fARB = NULL;
PFNGLUNIFORM3FARBPROC glUniform3fARB = NULL;
PFNGLUNIFORM2FARBPROC glUniform2fARB = NULL;
PFNGLUNIFORM1FARBPROC glUniform1fARB = NULL;

PFNGLUNIFORM1IARBPROC glUniform1iARB = NULL;

PFNGLUNIFORMMATRIX4FVARBPROC glUniformMatrix4fvARB = NULL;

#include <cstring>

static bool findString(const char* in, char* list) {
    int thisLength = strlen(in);
    while (*list != 0) {
        int length = strcspn(list, " ");

        if (thisLength == length)
            if (!strncmp(in, list, length))
                return true;

        list += length + 1;
    }
    return false;
}


// <editor-fold defaultstate="collapsed" desc="Shader Extensions">
static bool setupShaderEXT(char *error, char * extensionList) {

    //Verificando compatibilidad de la tarjeta grafica:
    bool notSuported = false;
    if (!findString("GL_ARB_shader_objects", extensionList)) {
        error += sprintf(error, "Shaders Objects no Soportados.\n");
        notSuported = true;
    }
    if (!findString("GL_ARB_shading_language_100", extensionList)) {
        error += sprintf(error, "Lenguaje de Shader GLSL_1.0 no Soportado.\n");
        notSuported = true;
    }
    if (!findString("GL_ARB_vertex_shader", extensionList)) {
        error += sprintf(error, "Vertex Shader no Soportado.\n");
        notSuported = true;
    }
    if (!findString("GL_ARB_fragment_shader", extensionList)) {
        error += sprintf(error, "Pixel/Fragment Shader no Soportado.\n");
        notSuported = true;
    }
    if (notSuported) {
        return false;
    }


    //Texturas:
    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)
            SDL_GL_GetProcAddress("glActiveTextureARB");

    //Objetos de Shaders
    glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)
            SDL_GL_GetProcAddress("glCreateProgramObjectARB");
    glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)
            SDL_GL_GetProcAddress("glDeleteObjectARB");
    glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)
            SDL_GL_GetProcAddress("glCreateShaderObjectARB");
    glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)
            SDL_GL_GetProcAddress("glShaderSourceARB");
    glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)
            SDL_GL_GetProcAddress("glCompileShaderARB");
    glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)
            SDL_GL_GetProcAddress("glGetObjectParameterivARB");
    glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)
            SDL_GL_GetProcAddress("glAttachObjectARB");
    glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)
            SDL_GL_GetProcAddress("glGetInfoLogARB");
    glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)
            SDL_GL_GetProcAddress("glLinkProgramARB");
    glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)
            SDL_GL_GetProcAddress("glUseProgramObjectARB");
    glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)
            SDL_GL_GetProcAddress("glGetUniformLocationARB");

    //Parametros de Shaders:
    glUniform1fARB = (PFNGLUNIFORM1FARBPROC)
            SDL_GL_GetProcAddress("glUniform1fARB");
    glUniform2fARB = (PFNGLUNIFORM2FARBPROC)
            SDL_GL_GetProcAddress("glUniform2fARB");
    glUniform3fARB = (PFNGLUNIFORM3FARBPROC)
            SDL_GL_GetProcAddress("glUniform3fARB");
    glUniform4fARB = (PFNGLUNIFORM4FARBPROC)
            SDL_GL_GetProcAddress("glUniform4fARB");

    glUniform1iARB = (PFNGLUNIFORM1IARBPROC)
            SDL_GL_GetProcAddress("glUniform1iARB");

    glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)
            SDL_GL_GetProcAddress("glUniformMatrix4fvARB");

    if (false
            || glActiveTextureARB == NULL
            //|| glMultiTexCoord2fARB == NULL
            || glCreateProgramObjectARB == NULL
            || glDeleteObjectARB == NULL
            || glCreateShaderObjectARB == NULL
            || glShaderSourceARB == NULL
            || glCompileShaderARB == NULL
            || glGetObjectParameterivARB == NULL
            || glAttachObjectARB == NULL
            || glGetInfoLogARB == NULL
            || glLinkProgramARB == NULL
            || glUseProgramObjectARB == NULL
            || glGetUniformLocationARB == NULL
            || glUniform1fARB == NULL
            || glUniform3fARB == NULL
            || glUniform1iARB == NULL

            || glUniformMatrix4fvARB == NULL
            ) {

        error += sprintf(error, "Direcciones de funciones no encontradas (Problema de compatibilidad).\n");
        return false;
    }

    return true;

}

// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Buffer Extensions">
static bool setupBufferEXT(char *error, char * extensionList) {

    //Verificando compatibilidad de la tarjeta grafica:
    bool notSuported = false;
    if (!findString("GL_EXT_framebuffer_object", extensionList)) {
        error += sprintf(error, "Frame Buffer Objects no Soportados.\n");
        notSuported = true;
    }
    
    //FALTAN VALIDAR LOS BUFFERS NORMALES.
    
    
    if (notSuported) {
        return false;
    }

    
    //Simple Buffers:
    glBindBufferARB     = (PFNGLBINDBUFFERARBPROC)    SDL_GL_GetProcAddress("glBindBufferARB");
    glGenBuffersARB     = (PFNGLGENBUFFERSARBPROC)    SDL_GL_GetProcAddress("glGenBuffersARB");    
    glBufferDataARB     = (PFNGLBUFFERDATAARBPROC)    SDL_GL_GetProcAddress("glBufferDataARB");
    glDeleteBuffersARB  = (PFNGLDELETEBUFFERSARBPROC) SDL_GL_GetProcAddress("glDeleteBuffersARB");
    
    
#if defined(__WIN32__)
    //Frame Buffers:
    glGenFramebuffersEXT                     = (PFNGLGENFRAMEBUFFERSEXTPROC)         SDL_GL_GetProcAddress("glGenFramebuffersEXT");
    glDeleteFramebuffersEXT                  = (PFNGLDELETEFRAMEBUFFERSEXTPROC)      SDL_GL_GetProcAddress("glDeleteFramebuffersEXT");
    glBindFramebufferEXT                     = (PFNGLBINDFRAMEBUFFEREXTPROC)         SDL_GL_GetProcAddress("glBindFramebufferEXT");
    glCheckFramebufferStatusEXT              = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)  SDL_GL_GetProcAddress("glCheckFramebufferStatusEXT");
    glGenerateMipmapEXT                      = (PFNGLGENERATEMIPMAPEXTPROC)          SDL_GL_GetProcAddress("glGenerateMipmapEXT");
    glFramebufferTexture2DEXT                = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)    SDL_GL_GetProcAddress("glFramebufferTexture2DEXT");
    glFramebufferRenderbufferEXT             = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC) SDL_GL_GetProcAddress("glFramebufferRenderbufferEXT");
    glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC) SDL_GL_GetProcAddress("glGetFramebufferAttachmentParameterivEXT");
    
    //Render Buffers:
    glGenRenderbuffersEXT                    = (PFNGLGENRENDERBUFFERSEXTPROC)        SDL_GL_GetProcAddress("glGenRenderbuffersEXT");
    glDeleteRenderbuffersEXT                 = (PFNGLDELETERENDERBUFFERSEXTPROC)     SDL_GL_GetProcAddress("glDeleteRenderbuffersEXT");
    glBindRenderbufferEXT                    = (PFNGLBINDRENDERBUFFEREXTPROC)        SDL_GL_GetProcAddress("glBindRenderbufferEXT");
    glRenderbufferStorageEXT                 = (PFNGLRENDERBUFFERSTORAGEEXTPROC)     SDL_GL_GetProcAddress("glRenderbufferStorageEXT");
    glIsRenderbufferEXT                      = (PFNGLISRENDERBUFFEREXTPROC)          SDL_GL_GetProcAddress("glIsRenderbufferEXT");
    glGetRenderbufferParameterivEXT          = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC) SDL_GL_GetProcAddress("glGetRenderbufferParameterivEXT");
#endif
    glDrawBuffers = (PFNGLDRAWBUFFERSPROC) SDL_GL_GetProcAddress("glDrawBuffers");
    
    if (true
            //Simple Buffers:
            && glBindBufferARB
            && glGenBuffersARB
            && glBufferDataARB
            && glDeleteBuffersARB
            
            //Frame Buffers:
            && glGenFramebuffersEXT
            && glDeleteFramebuffersEXT
            && glBindFramebufferEXT
            && glCheckFramebufferStatusEXT
            && glGetFramebufferAttachmentParameterivEXT
            && glGenerateMipmapEXT
            && glFramebufferTexture2DEXT
            && glFramebufferRenderbufferEXT
            && glDrawBuffers
            
            //Render Buffers:
            && glGenRenderbuffersEXT
            && glDeleteRenderbuffersEXT
            && glBindRenderbufferEXT
            && glRenderbufferStorageEXT
            && glGetRenderbufferParameterivEXT
            && glIsRenderbufferEXT
            ) {
        return true;
    } else {
        error += sprintf(error, "Direcciones de funciones no encontradas (Problema de compatibilidad).\n");
        return false;
    }
}
// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Windows 32 Extensions">
static bool setupWindows32EXT(char *error, char * extensionList) {
    
    glBlendEquation     = (PFNGLBLENDEQUATIONPROC)     SDL_GL_GetProcAddress("glBlendEquation");
    glDrawRangeElements = (PFNGLDRAWRANGEELEMENTSPROC) SDL_GL_GetProcAddress("glDrawRangeElements");
    
    if (false
            || glBlendEquation        == NULL
            || glDrawRangeElements    == NULL
            ) {
        error += sprintf(error, "Direcciones de funciones no encontradas (Problema de compatibilidad).\n");
        return false;
    }

    return true;

}
// </editor-fold>


bool setupExtensions(char *error) {
    char* extensionList = (char*) glGetString(GL_EXTENSIONS);
    
    if(setupShaderEXT(error, extensionList))
        if(setupBufferEXT(error, extensionList))
            #if defined(__WIN32__)
            if(setupWindows32EXT(error, extensionList))
            #endif
                return true;
    
    return false;
}
#endif