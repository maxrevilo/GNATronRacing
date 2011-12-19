#include "../RenderTarget2D.h"
#include "AdaptacionesDeFunciones.h"

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
    glGenRenderbuffersEXT(1, &deepBuffer);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, deepBuffer);
    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
}

void RenderTarget2D::GenerateMipMaps() {
    glActiveTextureARB(GL_TEXTURE0_ARB);        
    glBindTexture(GL_TEXTURE_2D, Pointer());
    
    glGenerateMipmapEXT(GL_TEXTURE_2D);
}

RenderTarget2D::~RenderTarget2D() {
        
}

