#ifndef SPHERE_MESH_H
#define	SPHERE_MESH_H

#include "../GNAFramework/Game.h"
#include "../GNAFramework/SpriteBatch.h"
#include "../GNAFramework/SpriteFont.h"
#include "../GNAFramework/VertexBuffer.h"
#include "../GNAFramework/IndexBuffer.h"
#include "../GNAFramework/GraphicDevice.h"
#include "../GNAFramework/MathHelper.h"
#include "../GNAFramework/GNAExeptions.h"

using namespace GNAFramework;

class Sphere_mesh {
public:
    
    Sphere_mesh(GraphicDevice *graphicDevice, int parallels, int meridians){
        if(parallels < 1 || meridians < 2) throw new ArgumentOutOfRangeException("Error: parallels < 1 || meridians < 2");
        
        this->parallels = parallels;
        this->meridians = meridians;
        
        this->graphicDevice = graphicDevice;
        
        VertexElement *ves = new VertexElement[3];
        ves[0] = VertexElement(0                , 0, VertexElement::Vector3, VertexElement::Position         );
        ves[1] = VertexElement(3*sizeof(GLfloat), 0, VertexElement::Vector3, VertexElement::Normal           );
        ves[2] = VertexElement(6*sizeof(GLfloat), 0, VertexElement::Vector2, VertexElement::TextureCoordinate);
        
        vd = VertexDeclaration(ves, 3);
        
        
        float vertices[ (3+3+2) * vertexNumber()];
        createVertex(vertices);
        
        short indices[indexNumber()];
        createIndices(indices);
        
        vbo = VertexBuffer(graphicDevice, vd, vertexNumber(), None);
        vbo.SetData( vertices , 0, vertexNumber());
        
        ibo = IndexBuffer(graphicDevice, IndexBuffer::ThirtyTwoBits, indexNumber(), None);
        ibo.SetData<short>(indices, 0, indexNumber());
        
    }
    
    void Draw(GraphicDevice::PrimitiveType pt){
        graphicDevice->setIndices(ibo);
        graphicDevice->setVertexBuffer(vbo);
        graphicDevice->DrawIndexedPrimitives(pt, 0, vertexNumber(), indexNumber());
    }
    
    void Draw(){ Draw(GraphicDevice::TriangleList); }
    
    int vertexNumber(){
        return 2 * parallels * meridians + 2;
    }
    
    int indexNumber(){
        return 12 * meridians * parallels;
    }
    
private:
    VertexDeclaration vd;
    VertexBuffer   vbo;
    IndexBuffer    ibo;
    GraphicDevice *graphicDevice;
    
    int parallels, meridians;
    
#define RAD 1.f
    
    void createVertex(float *vertices){
        int i = 0;
        const int stride = (3+3+2);
        
        //Polo Norte:
        vertices[i*stride + 0] = 0.f;//X
        vertices[i*stride + 1] = RAD;//Y
        vertices[i*stride + 2] = 0.f;//Z
        
        vertices[i*stride + 3] = 0.f;//nX
        vertices[i*stride + 4] = 1.f;//nY
        vertices[i*stride + 5] = 0.f;//nZ
        
        
        //getUV(vertices, i, stride);
        vertices[i*stride + 6] = 0.0f;//U
        vertices[i*stride + 7] = 0.0f;//V
        i++;
        
        const float pAng = Pi / (parallels + 1),
                    mAng = Pi / meridians,
                    mU   = 1.0f / (float)parallels,
                    mV   = 1.0f / 2.0f / (float)meridians;
        
        int p, m;
        for(p = 0; p < parallels; p++){
            for(m = 0; m < 2 * meridians; m++){
                vertices[i*stride + 3] = sin( mAng * m) * sin( pAng * (p + 1) );//nX
                vertices[i*stride + 4] = cos( pAng * (p + 1) );                 //nY
                vertices[i*stride + 5] = cos( mAng * m) * sin( pAng * (p + 1) );//nZ
                
                vertices[i*stride + 0] = RAD * vertices[i*stride + 3];//X
                vertices[i*stride + 1] = RAD * vertices[i*stride + 4];//Y
                vertices[i*stride + 2] = RAD * vertices[i*stride + 5];//Z

                //getUV(vertices, i, stride);
                vertices[i*stride + 6] = m * mU;//U
                vertices[i*stride + 7] = p * mV;//V
                i++;
            }
        }
        
        
        //Polo Sur:
        vertices[i*stride + 0] = 0.f;//X
        vertices[i*stride + 1] =-RAD;//Y
        vertices[i*stride + 2] = 0.f;//Z
        
        vertices[i*stride + 3] = 0.f;//nX
        vertices[i*stride + 4] =-1.f;//nY
        vertices[i*stride + 5] = 0.f;//nZ
        
        //getUV(vertices, i, stride);
        vertices[i*stride + 6] = 0.0f;//U
        vertices[i*stride + 7] = 1.0f;//V
        
    }
    
    void createIndices(short *indices){
        int i = 0, k, l;
        //Polo Norte:
        for(k = 0; k < 2 * meridians; k++){
            indices[i++] = getParallelIndex( 0, k+1);
            indices[i++] = getParallelIndex( 0, k  );
            indices[i++] = getParallelIndex(-1, 0  );
        }
        
        for(l = 0; l < parallels - 1; l++ ){
            for(k = 0; k < 2 * meridians; k++){
                indices[i++] = getParallelIndex( l , k  );
                indices[i++] = getParallelIndex(l+1, k+1);
                indices[i++] = getParallelIndex(l+1, k  );
                
                indices[i++] = getParallelIndex( l , k  );
                indices[i++] = getParallelIndex( l , k+1);
                indices[i++] = getParallelIndex(l+1, k+1);
            }
        }
        
        //Polo Sur:
        for(k = 0; k < 2 * meridians; k++){
            indices[i++] = getParallelIndex(parallels-1, k  );
            indices[i++] = getParallelIndex(parallels-1, k+1);
            indices[i++] = getParallelIndex(parallels  , 0  );
        }
        
    }
    
    void getUV(float *arr, int i, int stride) {
        float X = arr[i*stride + 0];
        float Y = arr[i*stride + 1];
        float Z = arr[i*stride + 2];
        
        float sq = sqrt(X*X + Y*Y + Z*Z);
        arr[i*stride + 6] = X/sq;
        arr[i*stride + 7] = Y/sq;
    }
    
    int getParallelIndex(int p, int m){
        int r;
        int mm = m % (2 * meridians);
        
        if     (p == -1) r = 0;
        else if(p == parallels) r = vertexNumber() - 1;
        else r =  p * 2 * meridians + mm + 1; 
        
        return r;
    }
};

#endif	/* SPHERE_MESH_H */

