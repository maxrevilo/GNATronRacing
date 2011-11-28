#ifndef CUBE_MESH_H
#define	CUBE_MESH_H

#include "../GNAFramework/Game.h"
#include "../GNAFramework/GraphicDevice.h"
#include "../GNAFramework/SpriteBatch.h"
#include "../GNAFramework/SpriteFont.h"

#include "../GNAFramework/VertexBuffer.h"
#include "../GNAFramework/IndexBuffer.h"

using namespace GNAFramework;

class Cube_mesh {
public:
    enum FACE{
        FRONT = 0,
        BACK  = 1,
        UP    = 2,
        DOWN  = 3,
        RIGHT = 4,
        LEFT  = 5
    };
    
    Cube_mesh(GraphicDevice *graphicDevice){
        this->graphicDevice = graphicDevice;
        
        VertexElement *ves = new VertexElement[3];
        ves[0] = VertexElement(0                , 0, VertexElement::Vector3, VertexElement::Position         );
        ves[1] = VertexElement(3*sizeof(GLfloat), 0, VertexElement::Vector3, VertexElement::Normal           );
        ves[2] = VertexElement(6*sizeof(GLfloat), 0, VertexElement::Vector2, VertexElement::TextureCoordinate);
        
        vd = VertexDeclaration(ves, 3);

        int i;
        for(i = 0; i < 6; i++){
           faces[i] = VertexBuffer(graphicDevice, vd, 12, None);
           faces[i].SetData( createFace((FACE)i) , 0, 4);
        }

        const short ind[] = {0,1,2,0,2,3};
        ibo = IndexBuffer(graphicDevice, IndexBuffer::ThirtyTwoBits, 6, None);
        ibo.SetData<short>(ind, 0, 6);
    }
    
    void Draw(GraphicDevice::PrimitiveType pt){
        int i;
        graphicDevice->setIndices(ibo);
        for(i = 0; i < 6; i++){
            graphicDevice->setVertexBuffer(faces[i]);
            graphicDevice->DrawIndexedPrimitives(pt, 0, 8, 6);
        }
    }
    
    void Draw(GraphicDevice::PrimitiveType pt, FACE face){
        graphicDevice->setIndices(ibo);
        graphicDevice->setVertexBuffer(faces[(int)face]);
        graphicDevice->DrawIndexedPrimitives(pt, 0, 8, 6);
    }
    
    void Draw(){ Draw(GraphicDevice::TriangleList); }
    
    inline const VertexBuffer *getFace(FACE face){
        return &faces[(int) face];
    }
    
private:
    VertexDeclaration vd;
    VertexBuffer   faces[6];
    IndexBuffer    ibo;
    GraphicDevice *graphicDevice;
    
    const GLfloat *createFace(FACE face){
        switch(face){
            case FRONT:
                static const GLfloat f[32] = {
                -1.0f,  1.0f,  1.0f, //V
                 0.0f,  0.0f,  1.0f, //N
                 0.0f,  0.0f,        //C
                
                 1.0f,  1.0f,  1.0f, //V
                 0.0f,  0.0f,  1.0f, //N
                 1.0f,  0.0f,        //C
                
                 1.0f, -1.0f,  1.0f, //V
                 0.0f,  0.0f,  1.0f, //N
                 1.0f,  1.0f,        //C
                
                -1.0f, -1.0f,  1.0f, //V
                 0.0f,  0.0f,  1.0f, //N
                 0.0f,  1.0f,        //C
                };
                return f;
            case BACK:
                static const GLfloat b[32] = {
                
                 1.0f, -1.0f, -1.0f, //V
                 0.0f,  0.0f, -1.0f, //N
                 1.0f,  1.0f,        //C
                    
                 1.0f,  1.0f, -1.0f, //V
                 0.0f,  0.0f, -1.0f, //N
                 1.0f,  0.0f,        //C
                 
                -1.0f,  1.0f, -1.0f, //V
                 0.0f,  0.0f, -1.0f, //N
                 0.0f,  0.0f,        //C
                
                -1.0f, -1.0f, -1.0f, //V
                 0.0f,  0.0f, -1.0f, //N
                 0.0f,  1.0f,        //C
                };
                return b;
            case UP:
                static const GLfloat u[32] = {
                -1.0f,  1.0f, -1.0f, //V
                 0.0f,  1.0f,  0.0f, //N
                 0.0f,  0.0f,        //C
                
                 1.0f,  1.0f, -1.0f, //V
                 0.0f,  1.0f,  0.0f, //N
                 1.0f,  0.0f,        //C
                
                 1.0f,  1.0f,  1.0f, //V
                 0.0f,  1.0f,  0.0f, //N
                 1.0f,  1.0f,        //C
                
                -1.0f,  1.0f,  1.0f, //V
                 0.0f,  1.0f,  0.0f, //N
                 0.0f,  1.0f,        //C
                };
                return u;
            case DOWN:
                static const GLfloat d[32] = {
                 1.0f, -1.0f,  1.0f, //V
                 0.0f, -1.0f,  0.0f, //N
                 1.0f,  1.0f,        //C
                    
                 1.0f, -1.0f, -1.0f, //V
                 0.0f, -1.0f,  0.0f, //N
                 1.0f,  0.0f,        //C
                 
                 -1.0f, -1.0f, -1.0f, //V
                 0.0f, -1.0f,  0.0f, //N
                 0.0f,  0.0f,        //C
                
                -1.0f, -1.0f,  1.0f, //V
                 0.0f, -1.0f,  0.0f, //N
                 0.0f,  1.0f,        //C
                };
                return d;
            case RIGHT:
                static const GLfloat r[32] = {
                 1.0f,  1.0f,  1.0f, //V
                 1.0f,  0.0f,  0.0f, //N
                 0.0f,  0.0f,        //C
                
                 1.0f,  1.0f, -1.0f, //V
                 1.0f,  0.0f,  0.0f, //N
                 1.0f,  0.0f,        //C
                
                 1.0f, -1.0f, -1.0f, //V
                 1.0f,  0.0f,  0.0f, //N
                 1.0f,  1.0f,        //C
                
                 1.0f, -1.0f,  1.0f, //V
                 1.0f,  0.0f,  0.0f, //N
                 0.0f,  1.0f,        //C
                };
                return r;
            case LEFT:
                static const GLfloat l[32] = {
                -1.0f, -1.0f, -1.0f, //V
                -1.0f,  0.0f,  0.0f, //N
                 1.0f,  1.0f,        //C
                
                -1.0f,  1.0f, -1.0f, //V
                -1.0f,  0.0f,  0.0f, //N
                 1.0f,  0.0f,        //C
                                   
                -1.0f,  1.0f,  1.0f, //V
                -1.0f,  0.0f,  0.0f, //N
                 0.0f,  0.0f,        //C
                
                -1.0f, -1.0f,  1.0f, //V
                -1.0f,  0.0f,  0.0f, //N
                 0.0f,  1.0f,        //C
                };
                return l;
        }
    }
};

#endif	/* CUBE_MESH_H */

