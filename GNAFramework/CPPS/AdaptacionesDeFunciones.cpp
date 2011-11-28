#include "AdaptacionesDeFunciones.h"

///////////////////////////// BLENDING /////////////////////////////////////////

PFNGLBLENDEQUATIONPROC glBlendEquation = NULL;




/////////////////////////////////// BUFFERS ////////////////////////////////////

PFNGLGENBUFFERSARBPROC GNAglGenBuffersARB = NULL;
PFNGLBINDBUFFERARBPROC GNAglBindBufferARB = NULL;
PFNGLBUFFERDATAARBPROC GNAglBufferDataARB = NULL;
PFNGLDELETEBUFFERSARBPROC GNAglDeleteBuffersARB = NULL;



/////////////////////////////////// DRAWING ////////////////////////////////////
#if defined(__WIN32__)
PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements = NULL;
#endif



//////////////////////////////////// SHADERS ///////////////////////////////////
#define NO_SDL_GLEXT
#include <SDL/SDL_opengl.h>
#include "GL/glext.h"


#if defined(__WIN32__)
#define uglGetProcAddress(x) wglGetProcAddress(x)
#define WIN32_OR_X11
#elif defined(__APPLE__)
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

void setupExtensions() {
    shading_enabled = true;
}; // OS X already has these extensions
#elif defined(__X11__)
#include <GL/glx.h>
#include <GL/glxext.h>
#define uglGetProcAddress(x) (*glXGetProcAddressARB)((const GLubyte*)(x))
#define WIN32_OR_X11
#else

void setupExtensions() {
    shading_enabled = false;
}; // just fail otherwise?
#endif

#define WIN32_OR_X11
#if defined(WIN32_OR_X11)
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;

PFNGLCREATEPROGRAMOBJECTARBPROC glCreateProgramObjectARB = NULL;
PFNGLDELETEOBJECTARBPROC glDeleteObjectARB = NULL;
PFNGLCREATESHADEROBJECTARBPROC glCreateShaderObjectARB = NULL;
PFNGLSHADERSOURCEARBPROC glShaderSourceARB = NULL;
PFNGLCOMPILESHADERARBPROC glCompileShaderARB = NULL;
PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB = NULL;
PFNGLATTACHOBJECTARBPROC glAttachObjectARB = NULL;
PFNGLGETINFOLOGARBPROC glGetInfoLogARB = NULL;
PFNGLLINKPROGRAMARBPROC glLinkProgramARB = NULL;
PFNGLUSEPROGRAMOBJECTARBPROC glUseProgramObjectARB = NULL;
PFNGLGETUNIFORMLOCATIONARBPROC glGetUniformLocationARB = NULL;

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

bool setupExtensions(char *error) {
    char* extensionList = (char*) glGetString(GL_EXTENSIONS);

    
    //Verificando compatibilidad de la tarjeta grafica:
    bool notSuported = false;
    if (!findString("GL_ARB_shader_objects", extensionList))
    {
        error += sprintf(error, "Shaders Objects no Soportados.\n");
        notSuported = true;
    }
    if (!findString("GL_ARB_shading_language_100", extensionList))
    {
        error += sprintf(error, "Lenguaje de Shader GLSL_1.0 no Soportado.\n");
        notSuported = true;
    }
    if (!findString("GL_ARB_vertex_shader", extensionList))
    {
        error += sprintf(error, "Vertex Shader no Soportado.\n");
        notSuported = true;
    }
    if (!findString("GL_ARB_fragment_shader", extensionList))
    {
        error += sprintf(error, "Pixel/Fragment Shader no Soportado.\n");
        notSuported = true;
    }
    if(notSuported) {
        return false;
    }
    
    
    //Texturas:
    glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)
            uglGetProcAddress("glActiveTextureARB");

    //Objetos de Shaders
    glCreateProgramObjectARB = (PFNGLCREATEPROGRAMOBJECTARBPROC)
            uglGetProcAddress("glCreateProgramObjectARB");
    glDeleteObjectARB = (PFNGLDELETEOBJECTARBPROC)
            uglGetProcAddress("glDeleteObjectARB");
    glCreateShaderObjectARB = (PFNGLCREATESHADEROBJECTARBPROC)
            uglGetProcAddress("glCreateShaderObjectARB");
    glShaderSourceARB = (PFNGLSHADERSOURCEARBPROC)
            uglGetProcAddress("glShaderSourceARB");
    glCompileShaderARB = (PFNGLCOMPILESHADERARBPROC)
            uglGetProcAddress("glCompileShaderARB");
    glGetObjectParameterivARB = (PFNGLGETOBJECTPARAMETERIVARBPROC)
            uglGetProcAddress("glGetObjectParameterivARB");
    glAttachObjectARB = (PFNGLATTACHOBJECTARBPROC)
            uglGetProcAddress("glAttachObjectARB");
    glGetInfoLogARB = (PFNGLGETINFOLOGARBPROC)
            uglGetProcAddress("glGetInfoLogARB");
    glLinkProgramARB = (PFNGLLINKPROGRAMARBPROC)
            uglGetProcAddress("glLinkProgramARB");
    glUseProgramObjectARB = (PFNGLUSEPROGRAMOBJECTARBPROC)
            uglGetProcAddress("glUseProgramObjectARB");
    glGetUniformLocationARB = (PFNGLGETUNIFORMLOCATIONARBPROC)
            uglGetProcAddress("glGetUniformLocationARB");
    
    //Parametros de Shaders:
    glUniform1fARB = (PFNGLUNIFORM1FARBPROC)
            uglGetProcAddress("glUniform1fARB");
    glUniform2fARB = (PFNGLUNIFORM2FARBPROC)
            uglGetProcAddress("glUniform2fARB");
    glUniform3fARB = (PFNGLUNIFORM3FARBPROC)
            uglGetProcAddress("glUniform3fARB");
    glUniform4fARB = (PFNGLUNIFORM4FARBPROC)
            uglGetProcAddress("glUniform4fARB");

    glUniform1iARB = (PFNGLUNIFORM1IARBPROC)
            uglGetProcAddress("glUniform1iARB");

    glUniformMatrix4fvARB = (PFNGLUNIFORMMATRIX4FVARBPROC)
            uglGetProcAddress("glUniformMatrix4fvARB");

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
#endif