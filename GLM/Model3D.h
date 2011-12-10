#ifndef MODEL3D_H
#define	MODEL3D_H

#include "../GNAFramework/GraphicDevice.h"
#include "../GNAFramework/Effect.h"
#include <vector>
#include "../GNAFramework/GNAExeptions.h"

using namespace GNAFramework;

class Model3D {
public:
    class Mesh;

    Model3D(GraphicDevice *graphicDevice, const char *path, bool keepInfo = false);

    void Draw(GraphicDevice::PrimitiveType pt = GraphicDevice::TriangleList);

    int Meshes() const;
    
    Mesh *operator [](const char* name) const;
    
    Mesh *operator [](int Index) const;

    virtual ~Model3D();
private:
    GraphicDevice *graphicDevice;
    int numVertices;
    Vector3 *vertices;

    std::vector<Mesh *> meshes;

};

class Model3D::Mesh {
    friend class Model3D;
public:
    char    *Name;
    Effect  *effect;
    
    void Draw(GraphicDevice::PrimitiveType pt = GraphicDevice::TriangleList);

    virtual ~Mesh() {
    };
private:
    GraphicDevice *   graphicDevice;
    VertexDeclaration vd;
    VertexBuffer      vbo;
    int               vertices;
    IndexBuffer       ibo;
    int               indices;
    
    
    Mesh();
    Mesh(char const *name, GraphicDevice *graphicDevice);
};

#endif	/* MODEL3D_H */

