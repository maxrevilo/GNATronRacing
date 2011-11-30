#ifndef GRAPHICDEVICE_H
#define	GRAPHICDEVICE_H

#include "BlendState.h"
#include "RasterizerState.h"
#include "SDL/SDL.h"
#include "GL/gl.h"
#include "Color.h"
#include <stdint.h>
#include "GameWindow.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Geom.h"

namespace GNAFramework {
    
    struct ViewPort{
    public:
        int X;
        int Y;
        int width;
        int height;
        
        ViewPort() {
            X = Y = width = height = 0;
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
    };
}

#endif	/* GRAPHICDEVICE_H */

