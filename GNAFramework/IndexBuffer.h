#ifndef INDEXBUFFER_H
#define	INDEXBUFFER_H


#include <GL/gl.h>
#include "BufferEnums.h"
#include "GNAExeptions.h"

namespace GNAFramework {
    
    class GraphicDevice;
    
    class IndexBuffer {
        friend class GraphicDevice;
    public:
        
        enum IndexElementSize {
            SixteenBits,
            ThirtyTwoBits
        };
        
        IndexBuffer ();
        
        IndexBuffer (
             GraphicDevice *  graphicsDevice,
             IndexElementSize indexElementSize,
             int              indexCount,
             BufferUsage      usage
        );
        
        template <class T>
        void SetData(const T* data, int startIndex, int elementCount);
        
        virtual ~IndexBuffer();
    private:
        GraphicDevice *graphicsDevice;
        IndexElementSize indexElementSize;
        int indexCount;
        BufferUsage usage;
        GLuint pointer;
        
        int elementSize(IndexElementSize ies);
    };
}
#endif	/* INDEXBUFFER_H */

