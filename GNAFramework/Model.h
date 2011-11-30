#ifndef GNA_MODEL_H
#define	GNA_MODEL_H

#include <vector>
#include "Geom.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Effect.h"
#include "GNAExeptions.h"

using namespace std;
namespace GNAFramework {
    class Bone;
    class ModelMesh;
    
    template <class T>
    class Collection {
    public:
        int length;
        virtual const T& operator[] (int Index) const;
        virtual const T& operator[] (const char *name) const = 0;
        
        Collection(){};
        Collection(vector<T> objects);
        ~Collection() {};
    protected:
        vector<T> objects;
    };
    
    class ModelBoneCollection : public Collection<Bone> {
    public:
        ModelBoneCollection(){};
        ModelBoneCollection(vector<Bone> objects);
        
        const virtual Bone& operator[] (int Index) const;
        const virtual Bone& operator[] (const char *name) const;
        ~ModelBoneCollection() {};
    };
    
    class ModelMeshCollection : public Collection<ModelMesh> {
    public:
        ModelMeshCollection(){};
        ModelMeshCollection(vector<ModelMesh> objects);
        
        const virtual ModelMesh &operator [](int Index) const;
        const virtual ModelMesh &operator[] (const char *name) const;
        ~ModelMeshCollection() {};
    };
    
    class Bone {
    public:
        ModelBoneCollection Children;
        int Index;
        char *Name;
        
        Bone *Parent;
        Matrix Transform;
        
        Bone();
        ~Bone();
    };
    
    class ModelMeshPart {
    public:
        Effect       *effect;
        IndexBuffer  indexBuffer;
        VertexBuffer vertexBuffer;
        int NumVertices;
        int PrimitiveCount;
        int StartIndex;
        int VertexOffset;
        
        ModelMeshPart();
        ~ModelMeshPart();
    };
    
    class ModelMesh {
    public:
        BoundingSphere boundingSphere;
        vector<Effect *> Effects;
        vector<ModelMeshPart> MeshParts;
        
        char *Name;
        Bone *ParentBone;
        
        void *Tag;
        
        void Draw() const;
        
        ModelMesh();
        ~ModelMesh();
    };
    
    
    class Model {
    public:
        Model();
        
        ModelBoneCollection Bones;
        ModelMeshCollection Meshes;
        
        Bone *Root;
        
        void *Tag;
        
        void Draw() const;
        
        void CopyAbsoluteBoneTransformsTo(Matrix *destinationBoneTransforms) const;
        
        virtual ~Model();
    private:
    };

}
#endif	/* GNA_MODEL_H */

