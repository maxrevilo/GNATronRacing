#ifndef SURFACEFORMAT_H
#define	SURFACEFORMAT_H

//#define NO_SDL_GLEXT
#include <SDL/SDL_opengl.h>

namespace GNAFramework {

    enum SurfaceFormat {
        BGRA = GL_BGRA,
        BGR = GL_BGR,
        RGBA = GL_RGBA,
        RGB = GL_RGB,
    };

}


#endif	/* SURFACEFORMAT_H */

