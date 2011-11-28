#include "../VertexDeclaration.h"
#include <stdlib.h>
#include <GL/gl.h>

using namespace GNAFramework;

int VertexDeclaration::calculateVertexStride(VertexElement *vertexElements, int elementNumber){
    int i, maxStride = 0;
    int offset, stride;
    
    for(i = 0; i < elementNumber; i++) {
        offset = vertexElements[i].Offset;
        stride = VertexElement::strideOf(vertexElements[i].vertexElementFormat);
        if(offset + stride > maxStride) maxStride = offset + stride;
    }
    
    return maxStride;
}

VertexDeclaration::VertexDeclaration(){
    this->vertexStride   = 0;
    this->vertexElements = NULL;
    this->elementNumber  = 0;
}

VertexDeclaration::VertexDeclaration(
                        VertexElement* vertexElements,
                        int elementNumber) 
{
    this->vertexStride   = calculateVertexStride(vertexElements, elementNumber);
    this->vertexElements = vertexElements;
    this->elementNumber  = elementNumber;
}

int VertexDeclaration::getVertexStride() {
    
    return vertexStride;
}

int VertexDeclaration::getElementNumber() {
    return elementNumber;
}

const VertexElement *VertexDeclaration::GetVertexElements(){
    return vertexElements;
}

VertexDeclaration::~VertexDeclaration() {
    //if(vertexElements)
    //    free(vertexElements);
}




// <editor-fold defaultstate="collapsed" desc="VertexElement">

VertexElement::VertexElement() {
    this->Offset              = 0;
    this->UsageIndex          = -1;
    this->vertexElementFormat = (VertexElementFormat) -1;
    this->vertexElementUsage  = (VertexElementUsage) -1;
}

void VertexElement::setValue(
                int Offset,
                int UsageIndex,
                VertexElementFormat vertexElementFormat,
                VertexElementUsage vertexElementUsage
        ){
    this->Offset              = Offset;
    this->UsageIndex          = UsageIndex;
    this->vertexElementFormat = vertexElementFormat;
    this->vertexElementUsage  = vertexElementUsage;
}


VertexElement::VertexElement(
                int Offset,
                int UsageIndex,
                VertexElementFormat vertexElementFormat,
                VertexElementUsage vertexElementUsage
        ){
    setValue(Offset, UsageIndex, vertexElementFormat, vertexElementUsage);
}


int VertexElement::strideOf(VertexElementFormat vef){
    switch(vef) {
        case Single:
            return sizeof(GLfloat);
            
        case Vector2:
            return 2*sizeof(GLfloat);
        case Vector3:
            return 3*sizeof(GLfloat);
        case Vector4:
            return 4*sizeof(GLfloat);
            
        case GNAColor:
            return strideOf(Byte4);
            
        case Short2:
            return 4*sizeof(GLshort);
        case Short4:
            return 4*sizeof(GLshort);
            
        case Byte4:
            return 4*sizeof(GLubyte);
            
        default: throw new ArgumentException("VertexElementFormat no Soportado");
    }
}


int VertexElement::GL_Type(VertexElementFormat vef){
    switch(vef) {
        case Single:
        case Vector2:
        case Vector3:
        case Vector4:
            return GL_FLOAT;
            
        case GNAColor:
        case Byte4:
            return GL_UNSIGNED_BYTE;
            
        case Short2:
        case Short4:
            return GL_SHORT;
            
        
            
        default: throw  new ArgumentException("VertexElementFormat no Soportado");
    }
}


int VertexElement::GL_Type_Number(VertexElementFormat vef){
    switch(vef) {
        case Single:
            return 1;
            
        case Vector2:
        case Short2:
            return 2;
            
        case Vector3:
            return 3;
            
        case Vector4:
        case GNAColor:
        case Byte4:
        case Short4:
            return 4;
            
        default: throw  new ArgumentException("VertexElementFormat no Soportado");
    }
}

// </editor-fold>

