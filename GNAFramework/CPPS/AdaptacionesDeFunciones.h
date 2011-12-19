#ifndef ADAPDEFUN_H
#define	ADAPDEFUN_H

#include <GL/gl.h>
#include <SDL/SDL.h>



extern bool ShadersSuport;
extern bool BuffersSuport;


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

/////////////////////////////////// BUFFERS ////////////////////////////////////
#define GL_ARRAY_BUFFER_ARB 0x8892
#define GL_STATIC_DRAW_ARB  0x88E4

extern PFNGLGENBUFFERSARBPROC    glGenBuffersARB;
extern PFNGLBINDBUFFERARBPROC    glBindBufferARB;
extern PFNGLBUFFERDATAARBPROC    glBufferDataARB;
extern PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB;

#if defined(__WIN32__)
// Framebuffer object
extern PFNGLGENFRAMEBUFFERSEXTPROC                     glGenFramebuffersEXT;             // FBO name generation procedure
extern PFNGLDELETEFRAMEBUFFERSEXTPROC                  glDeleteFramebuffersEXT;             // FBO deletion procedure
extern PFNGLBINDFRAMEBUFFEREXTPROC                     glBindFramebufferEXT;             // FBO bind procedure
extern PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC              glCheckFramebufferStatusEXT;             // FBO completeness test procedure
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT; // return various FBO parameters
extern PFNGLGENERATEMIPMAPEXTPROC                      glGenerateMipmapEXT;             // FBO automatic mipmap generation procedure
extern PFNGLFRAMEBUFFERTEXTURE2DEXTPROC                glFramebufferTexture2DEXT;             // FBO texdture attachement procedure
extern PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC             glFramebufferRenderbufferEXT;             // FBO renderbuffer attachement procedure
// Renderbuffer object
extern PFNGLGENRENDERBUFFERSEXTPROC                    glGenRenderbuffersEXT;             // renderbuffer generation procedure
extern PFNGLDELETERENDERBUFFERSEXTPROC                 glDeleteRenderbuffersEXT;                 // renderbuffer deletion procedure
extern PFNGLBINDRENDERBUFFEREXTPROC                    glBindRenderbufferEXT;                    // renderbuffer bind procedure
extern PFNGLRENDERBUFFERSTORAGEEXTPROC                 glRenderbufferStorageEXT;                 // renderbuffer memory allocation procedure
extern PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC          glGetRenderbufferParameterivEXT;          // return various renderbuffer parameters
extern PFNGLISRENDERBUFFEREXTPROC                      glIsRenderbufferEXT;                      // determine renderbuffer object type
#endif




/////////////////////////////////// DRAWING ////////////////////////////////////
#if defined(__WIN32__)
  extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
  extern PFNGLBLENDEQUATIONPROC     glBlendEquation;
#endif









#endif