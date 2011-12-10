#include "Model3D.h"
#include "glm.h"
#include <stdio.h>
#include <map>
#include <limits>

struct VertNormTexInd {
    int v, n, t;
    VertNormTexInd() {v = -1; n = -1; t = -1;};
    VertNormTexInd(int v, int n, int t) {
        this->v = v; this->n = n; this->t = t;
    }
    bool operator<(const VertNormTexInd & other) const {
        if(v == other.v) {
            if(n == other.n) {
                return t < other.t;
            } else return n < other.n;
        } else return v < other.v;
    }
    bool operator() (const VertNormTexInd& lhs, const VertNormTexInd & rhs) const { return lhs < rhs; }
};


Vector3 f3toV3(float *v3) { return Vector3(v3[0], v3[1], v3[2]); }
Vector2 f2toV2(float *v2) { return Vector2(v2[0], 1.f - v2[1]); }
struct VertNormTex {
    Vector3 v;
    Vector3 n;
    Vector2 t;
    VertNormTex(){}
    VertNormTex(float * v, float * n, float * t) {
        this->v = f3toV3(v); this->n = f3toV3(n); this->t = f2toV2(t);
    }
};

typedef std::map<VertNormTexInd, short, VertNormTexInd> VertMap;

Model3D::Model3D(GraphicDevice *graphicDevice, const char *path, bool keepInfo) {
    this->graphicDevice = graphicDevice;

    //printf("Loading Model:\n");
    GLMmodel * glmModel = glmReadOBJ(path);

    //printf("Model: v:%d, n:%d, t:%d\n", glmModel->numvertices, glmModel->numnormals, glmModel->numtexcoords);
    
    
    //glmReverseWinding(glmModel);
    
    if (glmModel->numnormals == 0) {
        if (glmModel->facetnorms == 0) {
            glmFacetNormals(glmModel);
        }
        glmVertexNormals(glmModel, 90);
    }
    

    //printf("Model: v:%d, n:%d, t:%d\n", glmModel->numvertices, glmModel->numnormals, glmModel->numtexcoords);
    

    if (keepInfo) {
        numVertices = glmModel->numvertices;
        //Copiar el arreglo de vertices
    }
    
    VertexElement *ves = new VertexElement[3];
    ves[0] = VertexElement(0                , 0, VertexElement::Vector3, VertexElement::Position         );
    ves[1] = VertexElement(3*sizeof(GLfloat), 0, VertexElement::Vector3, VertexElement::Normal           );
    ves[2] = VertexElement(6*sizeof(GLfloat), 0, VertexElement::Vector2, VertexElement::TextureCoordinate);
    
    VertexDeclaration vd = VertexDeclaration(ves, 3);

    if (glmModel->groups) {
        //printf("Loading Meshes:\n");

        for (GLMgroup *group = glmModel->groups; group; group = group->next) {
            const char * name = strrchr(group->name, ' ');
            Mesh *mesh = new Mesh(name? name+1 : group->name, graphicDevice);
            
            meshes.push_back(mesh);

            VertMap packages;
            VertMap::iterator it;
            
            int index = 0;
            short *indices = new short[group->numtriangles * 3];


            for (int t = 0; t < group->numtriangles; t++) {
                GLMtriangle *tri = &glmModel->triangles[group->triangles[t]];
                
                for (int i = 0; i < 3; i++) {
                    VertNormTexInd v(
                            tri->vindices[i],
                            tri->nindices[i],
                            tri->tindices[i]
                            );
                    
                    it = packages.find(v);
                    if (it != packages.end()) {
                        indices[3*t + i] = it->second;
                    } else {
                        packages[v] = index;
                        indices[3*t + i] = index;
                        index++;
                    }
                }
            }
            
            
            VertNormTex *rvb = new VertNormTex[packages.size()];
            
            for ( it=packages.begin() ; it != packages.end(); it++ ) {
                rvb[it->second] = VertNormTex(
                        &glmModel->vertices[3*it->first.v],
                        &glmModel->normals[3*it->first.n],
                        &glmModel->texcoords[2*it->first.t]
                        );
                
                //printf("%d/%d\n", it->first.v, it->first.t);
                
                //VertNormTex vnt = rvb[it->second];
                //printf("%d -> %f %f \n", it->second, vnt.t.X, vnt.t.Y);
                
            }
            
            /*
            for(int i = 0; i < 3*group->numtriangles; i++) {
                if(i % 3 == 0) printf("\nf ");
                printf("%f %f %f / %f %f   ||   ", rvb[indices[i]].v.X, rvb[indices[i]].v.Y, rvb[indices[i]].v.Z, rvb[indices[i]].t.X, rvb[indices[i]].t.Y);
            }*/
            
            //Vertex declaration:
            mesh->vd = vd;
            
            mesh->vertices = packages.size();
            mesh->indices  = group->numtriangles * 3;
            
            mesh->vbo = VertexBuffer(graphicDevice, vd, mesh->vertices, None);
            mesh->vbo.SetData(rvb , 0, mesh->vertices);
            
            
            mesh->ibo = IndexBuffer(graphicDevice, IndexBuffer::SixteenBits, mesh->indices, None);
            mesh->ibo.SetData<short>(indices, 0, mesh->indices);
            
            delete [] indices;
            delete [] rvb;
            
            printf("Name '%s' v: %d,  i: %d\n", mesh->Name, mesh->vertices, mesh->indices);
        }
    }
}

int Model3D::Meshes() const {
    return meshes.size();
}

void Model3D::Draw(GraphicDevice::PrimitiveType pt) {
    for(int i = 0; i < meshes.size(); i++) {
        meshes[i]->Draw(pt);
    }
}


Model3D::Mesh *Model3D::operator [](const char* name) const {
    for (int i = 0; i < meshes.size(); i++) {
        if (strcmp(meshes[i]->Name, name) == 0) return meshes[i];
    }
    throw new ArgumentException("Mesh not found.");
}

Model3D::Mesh *Model3D::operator [](int Index) const {
    return meshes[Index];
}

Model3D::~Model3D() {
}

Model3D::Mesh::Mesh() {
    effect = NULL;
    Name = NULL;
    vertices = 0;
    indices = 0;
    this->graphicDevice = NULL;
}

Model3D::Mesh::Mesh(char const *name, GraphicDevice *graphicDevice) {
    effect = NULL;
    Name = new char[strlen(name) + 1];
    strcpy(Name, name);
    vertices = 0;
    indices = 0;
    this->graphicDevice = graphicDevice;
}

void Model3D::Mesh::Draw(GraphicDevice::PrimitiveType pt) {
    if(graphicDevice == NULL)
        throw new InvalidOperationException("This mesh can't be draw because is not initialized.");
    
    if(effect != NULL) effect->Begin();
    
    graphicDevice->setIndices(ibo);
    graphicDevice->setVertexBuffer(vbo);
    graphicDevice->DrawIndexedPrimitives(pt, 0, vertices, indices);
    
    if(effect != NULL) effect->End();
}