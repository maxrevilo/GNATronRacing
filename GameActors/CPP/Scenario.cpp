#include "../Scenario.h"
#include "../../GNAFramework/GNAExeptions.h"
#include "../../GNAFramework/Effect.h"
#include "../../Managers/DataManager.h"
#include <GL/gl.h>
 

void createMeshVertes(float *vertex, int n) {
    
}

void createMeshIndices(int *index, int n) {
    
}



Scenario::Scenario(Game* game, Camera *camera) :GameActor(game) {
    meshes = NULL;
    meshes_leght = 0;
    
    floor = Floor(game);
    
    
    VertexElement *ves = new VertexElement[2];
    ves[0] = VertexElement(0                , 0, VertexElement::Vector3, VertexElement::Position         );
    ves[1] = VertexElement(3*sizeof(GLfloat), 0, VertexElement::Vector2, VertexElement::TextureCoordinate);
    
    vd = VertexDeclaration(ves, 2);
    
#define SIZE 512
    
    int indices = 6*(SIZE-1)*(SIZE-1);
    int *index = new int[indices];
    ibo512 = IndexBuffer(game->graphicDevice, IndexBuffer::SixteenBits, indices, None);
    createMeshIndices(index, SIZE);
    ibo512.SetData(index , 0, indices);
    
    delete index;
    
    int vertices = SIZE*SIZE;
    float *vertex = new float[vertices * (3+2)];
    createMeshVertes(vertex, SIZE);
    vbo512 = VertexBuffer(game->graphicDevice, vd, vertices, None); //Revisar puede que sea 512*512*(3+2)
    vbo512.SetData(vertex , 0, vertices);
    
    delete vertex;
    
    
    this->camera = camera;
}

void Scenario::Initialize(TiXmlNode* node) {
    if(node == NULL)  
        throw new ArgumentException("scenario Tag coundn't be found. check the level's XML.");
    
    TiXmlElement *e = node->ToElement();
    
    //          WIDTH:          //
    if(e->QueryIntAttribute("width"   , &width)  != TIXML_SUCCESS)
        throw new ArgumentException("\"width\" attribute couldn't be found on Scenario tag. check the level's XML.");
    
    //          HEIGHT:         //
    if(e->QueryIntAttribute("height"   , &height)  != TIXML_SUCCESS)
        throw new ArgumentException("\"height\" attribute couldn't be found on Scenario tag. check the level's XML.");
    
    floor.effect->Begin();
    Vector2 uvScale = Vector2(width, height);
    floor.effect->getParameter("uvScale").SetValue<Vector2>(&uvScale);
    floor.effect->End();
    
    // <editor-fold defaultstate="collapsed" desc="Loading Meshes">
    if(meshes != NULL) delete []meshes;
    meshes_leght = 0;
    
    TiXmlNode *meshes_n = node->FirstChild("meshes");
    if(meshes_n != NULL) {
        TiXmlElement *meshes_e = meshes_n->ToElement();
        if(meshes_e->QueryIntAttribute("lenght"   , &meshes_leght)  != TIXML_SUCCESS)
                throw new ArgumentException("\"lenght\" attribute couldn't be found on Scenario->meshes tag. check the level's XML.");
        
        meshes = new Mesh[meshes_leght];
        int i = 0;
        for (TiXmlNode *o_node = meshes_n->FirstChild(); o_node != 0; o_node = o_node->NextSibling()) {
            if(i >= meshes_leght)
            throw new ArgumentException("There are more meshes than specified by \"lenght\" attribute.");
            
            if(o_node->ToElement() != NULL) {
                meshes[i++] = Mesh(o_node, game->Content);
            }
        }
        if(i < meshes_leght)
            throw new ArgumentException("There are less meshes than specified by \"lenght\" attribute.");
    }
    // </editor-fold>

}

void Scenario::Update(GameTime gameTime) { }

void Scenario::Draw(GameTime gameTime, DrawOptions option) {
    Matrix world = Matrix::CreateScale(width * DataManager::unit_size, height * DataManager::unit_size, 1.f);
    floor.Draw(camera, world, option);

    
    for(int i = 0; i < meshes_leght; i++) {
        meshes[i].Draw(camera, world, option);
    }
}

Scenario::~Scenario() {
    if(meshes)        delete []meshes;
    if(floor.texture) delete floor.texture;
    if(floor.effect)  delete floor.effect;
}


// <editor-fold defaultstate="collapsed" desc="Mesh">
Scenario::Mesh::Mesh() {
}

Scenario::Mesh::Mesh(TiXmlNode* node, ContentManager *cm) {
    TiXmlNode *n;

    n = node->FirstChild("position");
    if (n == NULL) throw new ArgumentException("position Tag not found under Mesh Tag. check the level's XML.");
    position = XML2VEC3(n);

    n = node->FirstChild("dimension");
    if (n == NULL) throw new ArgumentException("dimension Tag not found under Mesh Tag. check the level's XML.");
    dimension = XML2VEC3(n);

    const char *path;
    // <editor-fold defaultstate="collapsed" desc="Loading heightmap Texure2D:">
    try {
        n = node->FirstChild("heightmap");
        if (n == NULL) throw new ArgumentException("heightmap Tag not found under Mesh Tag. check the level's XML.");

        path = n->ToElement()->Attribute("path");
        if (path == NULL) throw new ArgumentException("\"path\" attribute couldn't be found on Scenario->meshes->heightmap tag. check the level's XML.");

        heightMap = cm->Load<Texture2D > (path);
    } catch (ContentLoadException *cle) {
        throw new ArgumentException("Error loading Mesh's heightmap.", cle);
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Loading colormap Texure2D:">
    try {
        n = node->FirstChild("colormap");
        if (n == NULL) throw new ArgumentException("colormap Tag not found under Mesh Tag. check the level's XML.");

        path = n->ToElement()->Attribute("path");
        if (path == NULL) throw new ArgumentException("\"path\" attribute couldn't be found on Scenario->meshes->colormap tag. check the level's XML.");

        colorMap = cm->Load<Texture2D > (path);
    } catch (ContentLoadException *cle) {
        throw new ArgumentException("Error loading Mesh's colormap.", cle);
    }
    // </editor-fold>


    // <editor-fold defaultstate="collapsed" desc="Creating NormalMap Texture2D">
    // </editor-fold>
}

void Scenario::Mesh::Draw(Camera* camera, Matrix world, DrawOptions option) {
    
}

Scenario::Mesh::~Mesh() {
    delete heightMap;
    delete colorMap;
    delete normalMap;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Floor">

Scenario::Floor::Floor(Game* game) {
    texture = game->Content->Load<Texture2D > ("Scenario/Floor.bmp");
    effect  = game->Content->Load<Effect> ("Shaders/scenario.prog");

    effect->Begin();
    effect->getParameter("baseMap").SetValue<Texture2D > (texture);
    float sp = 16.f;
    effect->getParameter("specPow").SetValue<float>(&sp);

    Ep_WVP    = effect->getParameter("rMatWorldViewProjection");
    Ep_W      = effect->getParameter("rMatWorld");
    Ep_CamPos = effect->getParameter("rCamPos");
    effect->End();
}

void Scenario::Floor::Draw(Camera* camera, Matrix world, DrawOptions option) {
    if (option & DrawColor) {
        effect->Begin();
        Vector3 v = camera->getPosition();
        Ep_CamPos.SetValue<Vector3 > (&v);

        Matrix wvp = world * camera->viewMatrix() * camera->projectionMatrix();
        Ep_WVP.SetValue<Matrix > (&wvp);


        Ep_W.SetValue<Matrix > (&world);

        glBegin(GL_QUADS);
        glVertex3f(0.f, 0.f, 0.f);
        glNormal3f(0.f, 0.f, 1.f);
        glTexCoord2f(0.f, 0.f);
        glVertex3f(1.f, 0.f, 0.f);
        glNormal3f(0.f, 0.f, 1.f);
        glTexCoord2f(0.f, 1.f);
        glVertex3f(1.f, 1.f, 0.f);
        glNormal3f(0.f, 0.f, 1.f);
        glTexCoord2f(1.f, 1.f);
        glVertex3f(0.f, 1.f, 0.f);
        glNormal3f(0.f, 0.f, 1.f);
        glTexCoord2f(1.f, 0.f);
        glEnd();

        effect->End();
    }
}

// </editor-fold>
