#include "../IndexBuffer.h"

#include "AdaptacionesDeFunciones.h"


using namespace GNAFramework;

IndexBuffer::IndexBuffer () {
    graphicsDevice   = NULL;
    indexElementSize = (IndexElementSize)0;
    indexCount       = 0;
    usage            = (BufferUsage)0;
    pointer          = (GLuint) 0;
}

IndexBuffer::IndexBuffer (
         GraphicDevice*    graphicsDevice,
         IndexElementSize indexElementSize,
         int              indexCount,
         BufferUsage      usage
        ) {
    
    this->graphicsDevice   = graphicsDevice;
    this->indexElementSize = indexElementSize;
    this->indexCount       = indexCount;
    this->usage            = usage;
    GNAglGenBuffersARB(1, &this->pointer);
}


int IndexBuffer::elementSize(IndexElementSize ies){
    switch(ies){
        case(ThirtyTwoBits):
            return sizeof(GLuint);
            break;
        case(SixteenBits):
            return sizeof(GLushort);
            break;
        default: throw new ArgumentException("IndexElementSize no reconocido.");
    };
}


namespace GNAFramework {
    template <>
    void IndexBuffer::SetData<short>(const short* data, int startIndex, int elementCount){
        GNAglBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, pointer);
        //printf("glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, %d, data, GL_STATIC_DRAW);\n", elementCount*elementSize(indexElementSize));
        GNAglBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, elementCount*elementSize(indexElementSize), data, GL_STATIC_DRAW);
    }
    
    template <>
    void IndexBuffer::SetData<int>(const int* data, int startIndex, int elementCount){
        GNAglBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, pointer);
        
        GNAglBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, elementCount*elementSize(indexElementSize), data, GL_STATIC_DRAW);
    }
}

IndexBuffer::~IndexBuffer() {
    //GNAglDeleteBuffersARB(1, &pointer);
}

