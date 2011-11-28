/* 
 * File:   VertexDeclaration.h
 * Author: max
 *
 * Created on 14 de octubre de 2011, 20:52
 */

#ifndef VERTEXDECLARATION_H
#define	VERTEXDECLARATION_H

#include "GNAExeptions.h"

namespace GNAFramework {

    class VertexElement {
    public:
        enum VertexElementFormat{
            Single,
            Vector2,
            Vector3,
            Vector4,
            GNAColor,
            Short2,
            Short4,
            Byte4
        };
        
        enum VertexElementUsage{
            Position,
            Normal,
            Tangent,
            Binormal,
            Color,
            PointSize,
            TextureCoordinate
        };
        
        VertexElement();
        
        VertexElement(
                int Offset,
                int UsageIndex,
                VertexElementFormat vertexElementFormat,
                VertexElementUsage vertexElementUsage
        );
        
        void setValue(
                int Offset,
                int UsageIndex,
                VertexElementFormat vertexElementFormat,
                VertexElementUsage vertexElementUsage
        );
        
        static int strideOf(VertexElementFormat vef);
        static int GL_Type(VertexElementFormat vef);
        static int GL_Type_Number(VertexElementFormat vef);
        
        int Offset;
        int UsageIndex;
        VertexElementFormat vertexElementFormat;
        VertexElementUsage vertexElementUsage;
    };

    class VertexDeclaration {
    public:
        
        VertexDeclaration();

        VertexDeclaration(VertexElement* vertexElements, int elementNumber);

        const VertexElement *GetVertexElements();
        
        int getElementNumber();
        
        int getVertexStride();

        virtual ~VertexDeclaration();
    private:
        int vertexStride;
        VertexElement* vertexElements;
        int elementNumber;
        
        int calculateVertexStride(VertexElement *vertexElements, int elementNumber);
    };

}

#endif	/* VERTEXDECLARATION_H */

