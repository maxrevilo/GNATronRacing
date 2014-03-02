#include "../RenderTarget2D.h"
#include "AdaptacionesDeFunciones.h"
#include <limits.h>

using namespace GNAFramework;

RenderTarget2D::RenderTarget2D(
                GraphicDevice *graphicsDevice,
                int width,
                int height,
                bool mipMap,
                SurfaceFormat preferredFormat,
                DepthFormat preferredDepthFormat,
                int preferredMultiSampleCount,
                RenderTargetUsage usage
                )
: Texture2D(graphicsDevice, width, height, mipMap, preferredFormat)
{
    deepBuffer = INT_MAX;
}

unsigned int RenderTarget2D::depthBufferPointer() {
    if(deepBuffer == INT_MAX) {
        glGenRenderbuffersEXT(1, &deepBuffer);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, deepBuffer);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, getWidth(), getHeight());
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
    }
    return deepBuffer;
}

void RenderTarget2D::GenerateMipMaps() {
    glActiveTextureARB(GL_TEXTURE0_ARB);        
    glBindTexture(GL_TEXTURE_2D, Pointer());
    
    glGenerateMipmapEXT(GL_TEXTURE_2D);
}

RenderTarget2D::~RenderTarget2D() {
        
}

