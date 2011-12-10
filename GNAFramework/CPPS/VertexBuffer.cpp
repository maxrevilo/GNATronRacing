#include "../VertexBuffer.h"
#include "AdaptacionesDeFunciones.h"

#include <stdio.h>

using namespace GNAFramework;

VertexBuffer::VertexBuffer (){
    graphicsDevice    = NULL;
    VertexCount       = 0;
    usage             = (BufferUsage) 0;
    pointer           = (GLuint) 0;
}
        
VertexBuffer::VertexBuffer (
             GraphicDevice *   graphicsDevice,
             VertexDeclaration vertexDeclaration,
             int               vertexCount,
             BufferUsage       usage
        )
{
    this->graphicsDevice    = graphicsDevice;
    this->vertexDeclaration = vertexDeclaration;
    this->VertexCount       = vertexCount;
    this->usage             = usage;
    GNAglGenBuffersARB(1, &this->pointer);
}
        
        
void VertexBuffer::SetData(const void* data, int startVertex, int elementCount){
    GNAglBindBufferARB(GL_ARRAY_BUFFER, pointer);
    GNAglBufferDataARB( GL_ARRAY_BUFFER, 
                        elementCount*vertexDeclaration.getVertexStride(),
                        ((char *)data) + startVertex*vertexDeclaration.getVertexStride(), 
                        GL_STATIC_DRAW);
    GNAglBindBufferARB(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer(){
    //GNAglDeleteBuffersARB(1, &pointer);
}