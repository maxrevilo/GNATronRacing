#ifndef ADAPDEFUN_H
#define	ADAPDEFUN_H

#include <GL/gl.h>
#include <SDL/SDL.h>



//////////////////////////////////// SHADERS ///////////////////////////////////

extern bool setupExtensions(char *error);

#ifdef __APPLE__
  #include <OpenGL/glu.h>
  #include <OpenGL/glext.h>
#else
  #include "GL/glext.h"
  #include <SDL/SDL_opengl.h>

  /* Shading language prototypes. */
  extern PFNGLACTIVETEXTUREARBPROC           glActiveTextureARB;

  extern PFNGLCREATEPROGRAMOBJECTARBPROC     glCreateProgramObjectARB;
  extern PFNGLDELETEOBJECTARBPROC            glDeleteObjectARB;
  extern PFNGLCREATESHADEROBJECTARBPROC      glCreateShaderObjectARB;
  extern PFNGLSHADERSOURCEARBPROC            glShaderSourceARB;
  extern PFNGLCOMPILESHADERARBPROC           glCompileShaderARB;
  extern PFNGLGETOBJECTPARAMETERIVARBPROC    glGetObjectParameterivARB;
  extern PFNGLATTACHOBJECTARBPROC            glAttachObjectARB;
  extern PFNGLGETINFOLOGARBPROC              glGetInfoLogARB;
  extern PFNGLLINKPROGRAMARBPROC             glLinkProgramARB;
  extern PFNGLUSEPROGRAMOBJECTARBPROC        glUseProgramObjectARB;
  extern PFNGLGETUNIFORMLOCATIONARBPROC      glGetUniformLocationARB;
  
  extern PFNGLUNIFORM4FARBPROC               glUniform4fARB;
  extern PFNGLUNIFORM3FARBPROC               glUniform3fARB;
  extern PFNGLUNIFORM2FARBPROC               glUniform2fARB;
  extern PFNGLUNIFORM1FARBPROC               glUniform1fARB;
  
  extern PFNGLUNIFORM1IARBPROC               glUniform1iARB;
  
  extern PFNGLUNIFORMMATRIX4FVARBPROC        glUniformMatrix4fvARB;
#endif



///////////////////////////// BLENDING /////////////////////////////////////////

#if defined(WIN32)
extern PFNGLBLENDEQUATIONPROC glBlendEquation;
#endif




/////////////////////////////////// BUFFERS ////////////////////////////////////
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB  0x88E4

extern PFNGLGENBUFFERSARBPROC    GNAglGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC    GNAglBindBufferARB;
extern PFNGLBUFFERDATAARBPROC    GNAglBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC GNAglDeleteBuffersARB;




/////////////////////////////////// DRAWING ////////////////////////////////////
#if defined(__WIN32__)
  extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
#endif









#endif