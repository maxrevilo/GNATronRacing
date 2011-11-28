#ifndef VERTEXBUFFER_H
#define	VERTEXBUFFER_H


#include <GL/gl.h>
#include "BufferEnums.h"
#include "VertexDeclaration.h"

namespace GNAFramework {
    
    class GraphicDevice;
    
    class VertexBuffer {
        friend class GraphicDevice;
    public:
        
        VertexBuffer ();
        
        VertexBuffer (
             GraphicDevice *   graphicsDevice,
             VertexDeclaration vertexDeclaration,
             int               vertexCount,
             BufferUsage       usage
        );
        
        
        void SetData(const void* data, int startVertex, int elementCount);
        
        virtual ~VertexBuffer();
    private:
        GraphicDevice *graphicsDevice;
        VertexDeclaration vertexDeclaration;
        int VertexCount;
        BufferUsage usage;
        GLuint pointer;
    };
}
#endif	/* VERTEXBUFFER_H */

