#ifndef GRAPHICDEVICE_H
#define	GRAPHICDEVICE_H

#include "BlendState.h"
#include "RasterizerState.h"
#include "SDL/SDL.h"
#include "GL/gl.h"
#include <GL/glu.h>
#include "Color.h"
#include <stdint.h>
#include "GameWindow.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Geom.h"

namespace GNAFramework {
    
    class RenderTarget2D;
     
    struct ViewPort{
    public:
        int X;
        int Y;
        int width;
        int height;
        
        ViewPort() {
            X = Y = width = height = 0;
        }
        
        ViewPort(int X, int Y, int Width, int Height) {
            this->X = X; this->Y = Y;
            this->height = Height;
            this->width  = Width;
        }
        
        float aspectRatio() { return (float) width / (float) height; }
        
        //Vector3 Unproject(Vector3 source, Matrix projection, Matrix view, Matrix world);
    } ;
    
    class GraphicDevice {
    public:

        enum ClearOptions {
            DepthBuffer = GL_DEPTH_BUFFER_BIT,
            Stencil = GL_STENCIL_BUFFER_BIT,
            Target = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        };
        
        enum PrimitiveType {
            PointList     = GL_POINTS,
            TriangleList  = GL_TRIANGLES,
            TriangleStrip = GL_TRIANGLE_STRIP,
            LineList      = GL_LINES,
            LineStrip     = GL_LINE_STRIP,
            LineLoop      = GL_LINE_LOOP,
            QuadList      = GL_QUADS
        };
        static void checkError(const char* const label);
        
        IndexBuffer getIndices();
        void        setIndices(IndexBuffer indexBuffer);
        
        VertexBuffer getVertexBuffer();
        void setVertexBuffer(VertexBuffer vertexBuffer);
        
        ViewPort getViewPort();
        void setViewPort(ViewPort viewPort);

        BlendState getBlendState();
        void setBlendState(BlendState blendState);
        
        
        //Temporal
        void DeepBufferEnabled(bool enabled);
        void DeepBufferWritteEnabled(bool enabled);
        
        RasterizerState getRasterizerState();
        void setRasterizerState(RasterizerState rasterizerState);

        bool isFullScreen();
        
        void setFullScreen(bool fullScreen);
        
        /*
        enum AttachmentPoint {
            Color0   = GL_COLOR_ATTACHMENT0_EXT,
            Color1   = GL_COLOR_ATTACHMENT1_EXT,
            Color2   = GL_COLOR_ATTACHMENT2_EXT,
            Color3   = GL_COLOR_ATTACHMENT3_EXT,
            Color4   = GL_COLOR_ATTACHMENT4_EXT,
            Color5   = GL_COLOR_ATTACHMENT5_EXT,
            Color6   = GL_COLOR_ATTACHMENT6_EXT,
            Color7   = GL_COLOR_ATTACHMENT7_EXT,
            Color8   = GL_COLOR_ATTACHMENT8_EXT,
            Color9   = GL_COLOR_ATTACHMENT9_EXT,
            Deep0    = GL_DEPTH_ATTACHMENT_EXT,
            Stencil0 = GL_STENCIL_ATTACHMENT_EXT  
        };*/
        
        void SetRenderTarget(RenderTarget2D *renderTarget, bool useDepth = true);
        
        void SetRenderTargets(int number, RenderTarget2D **renderTargets);
        
        
        void DrawIndexedPrimitives (
                         PrimitiveType primitiveType,
                         //int baseVertex,
                         int minVertexIndex,
                         int numVertices,
                         //int startIndex,
                         int primitiveCount
        );


        void Clear(Color color);
        void Clear(ClearOptions Mask, Color color, GLclampd depth, int stencil);

        GameWindow *getGameWindow();

        GraphicDevice(int width, int heigth, bool fullScreen);

        //GraphicDevice(const GraphicDevice& orig);
        virtual ~GraphicDevice();
    private:
        ViewPort viewPort;
        BlendState blendState;
        RasterizerState rasterizerState;
        IndexBuffer indexBuffer;
        VertexBuffer vertexBuffer;
        bool fullScreen;
        GameWindow *gameWindow;
        Color *clearColor;
        
        unsigned int frameBuffer;
        int maxDrawbuffers;
        int usedBuffers;
        unsigned int defaultDepthRenderBuffer;
        bool fbActivated;
        unsigned int attachPoints[16];
    };
}

#endif	/* GRAPHICDEVICE_H */

