#include "../Model.h"

using namespace GNAFramework;

// <editor-fold defaultstate="collapsed" desc="Model">

Model::Model() {
    
}

void Model::Draw() const{
    for(int i = 0; i < Meshes.length; i++) {
        Meshes[i].Draw();
    }
}

void Model::CopyAbsoluteBoneTransformsTo(Matrix *destinationBoneTransforms) const{
    throw new NotImplementedException("CopyAbsoluteBoneTransformsTo");
}

Model::~Model(){
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Bone">
Bone::~Bone() {
    delete Name;
}

// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ModelMesh">
void ModelMesh::Draw() const{
    
}

ModelMesh::~ModelMesh() {
    delete Name;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="ModelMeshPart">
ModelMeshPart::~ModelMeshPart() {
    delete effect;
}
// </editor-fold>


// <editor-fold defaultstate="collapsed" desc="Collections">
template <class T>
const T &Collection<T>::operator [](int Index) const{
    return objects[Index];
}

template <class T>
Collection<T>::Collection(vector<T> objects){
    this->objects = objects;
}

ModelMeshCollection::ModelMeshCollection(vector<ModelMesh> objects) : Collection<ModelMesh>(objects) {
}

const ModelMesh &ModelMeshCollection::operator [](const char* name) const{
    for(int i = 0; i < objects.size(); i++) {
        if(strcmp(objects[i].Name, name)==0) return objects[i];
    }
    throw new ArgumentException("ModelMesh not found.");
}

const ModelMesh &ModelMeshCollection::operator [](int Index) const{
    return objects[Index];
}


ModelBoneCollection::ModelBoneCollection(vector<Bone> objects) : Collection<Bone>(objects) {
}

const Bone &ModelBoneCollection::operator [](const char* name) const{
    for(int i = 0; i < objects.size(); i++) {
        if(strcmp(objects[i].Name, name)==0) return objects[i];
    }
    throw new ArgumentException("Bone not found.");
}

const Bone &ModelBoneCollection::operator [](int Index) const{
    return objects[Index];
}
// </editor-fold>


