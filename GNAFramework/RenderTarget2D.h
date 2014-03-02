#ifndef RENDERTARGET2D_H
#define	RENDERTARGET2D_H

#include "Texture2D.h"

namespace GNAFramework {

    class RenderTarget2D : public Texture2D {
    public:

        enum DepthFormat {
            None,
            Depth16,
            Depth24,
            Depth24Stencil8
        };

        enum RenderTargetUsage {
            DiscardContents,
            PreserveContents,
            PlatformContents
        };
        
        RenderTarget2D(
                GraphicDevice *graphicsDevice,
                int width,
                int height,
                bool mipMap,
                SurfaceFormat preferredFormat,
                DepthFormat preferredDepthFormat,
                int preferredMultiSampleCount,
                RenderTargetUsage usage
                );
        void GenerateMipMaps();

        unsigned int depthBufferPointer();
        
        virtual ~RenderTarget2D();
    private:
        unsigned int deepBuffer;
    };
}
#endif	/* RENDERTARGET2D_H */

