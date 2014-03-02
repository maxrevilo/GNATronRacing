#include "../Scenario.h"
#include "../../GNAFramework/GNAExeptions.h"
#include "../../GNAFramework/BlendState.h"
#include "../../GNAFramework/Effect.h"
#include "../../Managers/DataManager.h"
#include "../../Managers/DebugManager.h"
#include <string.h>

Scenario::Scenario(Game* game, Camera *camera) : GameActor(game) {
    meshes = NULL;
    meshes_length = 0;
    
    DebugManager::debugInfo("Loading Scenario Floor.");
    floor = Floor(game);

    Mesh::initializeBuffers(game->graphicDevice);
    
    this->camera = camera;
    
    
    skyMap = new Model3D(game->graphicDevice, "Content/Scenario/skymap.obj");
    Effect *effect = game->Content->Load<Effect > ("Shaders/skymap.prog");
    Texture2D *diffMap = game->Content->Load<Texture2D > ("Scenario/skymap.bmp");
    effect->getParameter("diffMap").SetValue<Texture2D > (diffMap);
    skyMapWVPMat = effect->getParameter("rMatWorldViewProjection");
    (*skyMap)[0]->effect = effect;


    model = new Model3D(game->graphicDevice, "Content/Scenario/scenario.obj");

    if (model->Meshes() - 1 != 6) {
        throw new GNAException("El modelo se cambio pero no se ajusto la cantidad e EffectParameter en Scenario.");
    }
    
    Texture2D *black = game->Content->Load<Texture2D> ("Scenario/Black.bmp");
    Vector2 uvScale = Vector2(1.f, 1.f);
    char path[256];
    for (int i = 0; i < model->Meshes() - 1; i++) {
        const char *name = (*model)[i]->Name;

        Effect *effect = game->Content->Load<Effect > ("Shaders/scenario.prog");

        sprintf(path, "Scenario/%s.bmp", name);
        Texture2D *diffMap = game->Content->Load<Texture2D > (path);
        
        modelWVPMat[i] = effect->getParameter("rMatWorldViewProjection");
        effect->getParameter("diffMap").SetValue<Texture2D > (diffMap);
        effect->getParameter("uvScale").SetValue<Vector2 > (&uvScale);
        
        Texture2D *lumMap;
        if (strcmp((*model)[i]->Name, "Luces") == 0) {
            lumMap = game->Content->Load<Texture2D>("Scenario/Luces.bmp");
        } else if (strcmp((*model)[i]->Name, "MarcaAzul") == 0) {
            lumMap = game->Content->Load<Texture2D>("Scenario/Lum_MarcaAzul.bmp");
        } else if (strcmp((*model)[i]->Name, "Marcador") == 0) {
            lumMap = game->Content->Load<Texture2D>("Scenario/Lum_Marcador.bmp");
        } else {
            lumMap = black;
        }
        effect->getParameter("lumMap").SetValue(lumMap);
        
        (*model)[i]->effect = effect;
    }
}

void Scenario::Initialize(TiXmlNode* node) {
    if (node == NULL)
        throw new ArgumentException("scenario Tag coundn't be found. check the level's XML.");

    TiXmlElement *e = node->ToElement();

    //          WIDTH:          //
    if (e->QueryIntAttribute("width", &width) != TIXML_SUCCESS)
        throw new ArgumentException("\"width\" attribute couldn't be found on scenario tag. check the level's XML.");

    //          HEIGHT:         //
    if (e->QueryIntAttribute("height", &height) != TIXML_SUCCESS)
        throw new ArgumentException("\"height\" attribute couldn't be found on scenario tag. check the level's XML.");


    DebugManager::debugInfo("Floor Effect uv Scale Set up.");
    floor.effect->Begin();
    Vector2 uvScale = Vector2(width, height);
    floor.effect->getParameter("uvScale").SetValue<Vector2 > (&uvScale);
    floor.effect->End();

    // <editor-fold defaultstate="collapsed" desc="Loading Meshes">
    DebugManager::debugInfo("Loading Meshes.");
    if (meshes != NULL) delete []meshes;
    meshes_length = 0;

    TiXmlNode *meshes_n = node->FirstChild("meshes");
    if (meshes_n != NULL) {
        TiXmlElement *meshes_e = meshes_n->ToElement();
        if (meshes_e->QueryIntAttribute("length", &meshes_length) != TIXML_SUCCESS)
            throw new ArgumentException("\"length\" attribute couldn't be found on Scenario->meshes tag. check the level's XML.");

        meshes = new Mesh[meshes_length];
        int i = 0;
        for (TiXmlNode *o_node = meshes_n->FirstChild(); o_node != 0; o_node = o_node->NextSibling()) {
            if (o_node != NULL && o_node->ToElement() != NULL) {
                if (i >= meshes_length)
                    throw new ArgumentException("There are more meshes than specified by \"length\" attribute.");

                DebugManager::debugInfo("Loading mesh %d", i);
                meshes[i++] = Mesh(o_node, game->Content);
            }
        }

        DebugManager::debugInfo("Meshes loaded %d", meshes_length);
        if (i < meshes_length)
            throw new ArgumentException("There are less meshes than specified by \"length\" attribute.");
    }
    // </editor-fold>

}

void Scenario::Update(GameTime gameTime) {
}

void Scenario::Draw(GameTime gameTime, DrawOptions option) {
    DebugManager::debugInfo("Drawing Scenario.");

    switch (option) {
        case DrawColor:
        {
            // <editor-fold defaultstate="collapsed" desc="Scenario Draw">
            Camera scCam = *camera;
            Frustum frus = scCam.getFrustum();
            float maxSize = (width > height ? width : height) * DataManager::unit_size;
            scCam.setFrustum(Frustum(frus.getFovY(), frus.getAspectRatio(), maxSize, 1000 * maxSize));

            game->graphicDevice->setBlendState(BlendState::Opaque);
            game->graphicDevice->setRasterizerState(RasterizerState::CullClockwise);

            Matrix reorient = Matrix::Zeros;
            reorient.M13(1.f);
            reorient.M21(1.f);
            reorient.M32(1.f);
            reorient.M44(1.f);

            Matrix WVP = reorient * Matrix::CreateScale(maxSize) * camera->viewMatrix() * scCam.projectionMatrix();

            skyMapWVPMat.SetValue(&WVP);
            skyMap->Draw();

            for (int i = 0; i < model->Meshes() - 1; i++) {
                modelWVPMat[i].SetValue(&WVP);
                if (strcmp((*model)[i]->Name, "Brillos") == 0) continue;

                (*model)[i]->Draw();
            }

            //game->graphicDevice->setBlendState(BlendState(BlendState::One, BlendState::One, BlendState::Add));
            //(*model)["Brillos"]->Draw();


            game->graphicDevice->setRasterizerState(RasterizerState::CullCounterClockwise);
            game->graphicDevice->Clear(GraphicDevice::DepthBuffer, Color::Blue, 1.0, 0);
            // </editor-fold>

            Matrix floor_world = Matrix::CreateScale(width * DataManager::unit_size, height * DataManager::unit_size, 1.f);

            DebugManager::debugInfo("Drawing Floor.");
            game->graphicDevice->setBlendState(BlendState::NonPremultiplied);
            floor.Draw(camera, floor_world, game->graphicDevice, option);

            game->graphicDevice->setBlendState(BlendState::Opaque);

            DebugManager::debugInfo("    Drawing meshes.");
            for (int i = 0; i < meshes_length; i++) {
                meshes[i].Draw(camera, game->graphicDevice, option);
            }
        }
            break;
        case DrawBright: 
        {
            Camera scCam = *camera;
            Frustum frus = scCam.getFrustum();
            float maxSize = (width > height ? width : height) * DataManager::unit_size;
            scCam.setFrustum(Frustum(frus.getFovY(), frus.getAspectRatio(), maxSize, 1000 * maxSize));

            game->graphicDevice->setBlendState(BlendState::Opaque);
            game->graphicDevice->setRasterizerState(RasterizerState::CullClockwise);

            Matrix reorient = Matrix::Zeros;
            reorient.M13(1.f);
            reorient.M21(1.f);
            reorient.M32(1.f);
            reorient.M44(1.f);

            Matrix WVP = reorient * Matrix::CreateScale(maxSize) * camera->viewMatrix() * scCam.projectionMatrix();
            
            for (int i = 0; i < model->Meshes() - 1; i++) {
                if (strcmp((*model)[i]->Name, "Luces") == 0) {
                    modelWVPMat[i].SetValue(&WVP);
                    (*model)[i]->Draw();
                    break;
                }
            }
        }
            break;
    }
}

Scenario::~Scenario() {
    //if (meshes) delete []meshes;
    //if (floor.texture) delete floor.texture;
    //if (floor.effect) delete floor.effect;
}


// <editor-fold defaultstate="collapsed" desc="Mesh">
#define SIZE 256

VertexDeclaration Scenario::Mesh::vd = VertexDeclaration();
IndexBuffer Scenario::Mesh::ibo512 = IndexBuffer();
VertexBuffer Scenario::Mesh::Svbo512 = VertexBuffer();

// <editor-fold defaultstate="collapsed" desc="Initialize Buffer Helpers">

void createMeshVertes(Vector2 *vertex, int n) {
    int index = 0;
    float fn = (float) n;
    for (float x = 0; x < n; x++) {
        for (float y = 0; y < n; y++) {
            vertex[index++] = Vector2(x / fn, y / fn);
        }
    }
    if (index != n * n) throw new GNAException("Error creating heightmap vertices.");
}

void createMeshIndices(int *index, int n) {
    int counter = 0;
    for (int y = 0; y < n - 1; y++) {
        for (int x = 0; x < n - 1; x++) {
            int lowerLeft = x + y*n;
            int lowerRight = (x + 1) + y*n;
            int topLeft = x + (y + 1) * n;
            int topRight = (x + 1) + (y + 1) * n;

            index[counter++] = lowerLeft;
            index[counter++] = lowerRight;
            index[counter++] = topLeft;

            index[counter++] = lowerRight;
            index[counter++] = topRight;
            index[counter++] = topLeft;
        }
    }

}

void createMeshHeightVertes(Vector3 *vertex, Texture2D *tex, int n, float dimZ) {
    int index = 0;
    float fn = (float) n, xn, yn;

    uint8_t *height = new uint8_t[4 * tex->getHeight() * tex->getWidth()];
    tex->getData<uint8_t > (height);

    for (float x = 0; x < n; x++) {
        for (float y = 0; y < n; y++) {
            xn = x / fn;
            yn = y / fn;
            int tp = (int) (xn * tex->getWidth()) + (int) (yn * tex->getHeight()) * tex->getWidth();
            vertex[index] = Vector3(xn, yn, ((float) height[1 + 4 * tp]) / 254.f * dimZ);
            index++;
        }
    }
    delete height;
    if (index != n * n) throw new GNAException("Error creating heightmap vertices.");
}
// </editor-fold>

void Scenario::Mesh::initializeBuffers(GraphicDevice *gd) {
    if (DataManager::useShaderHeightmap) {
        VertexElement *ves = new VertexElement[1];
        ves[0] = VertexElement(0, 0, VertexElement::Vector2, VertexElement::Position);

        vd = VertexDeclaration(ves, 1);

        DebugManager::debugInfo("    Instantiating Heigthmap_512 vertices.");
        int vertices = SIZE * SIZE;
        Vector2 *vertex = new Vector2[vertices];
        createMeshVertes(vertex, SIZE);
        DebugManager::debugInfo("    Creating Vertex buffer.");
        Svbo512 = VertexBuffer(gd, vd, vertices, None);
        DebugManager::debugInfo("    Setting Index Buffer Data.");
        Svbo512.SetData(vertex, 0, vertices);


        DebugManager::debugInfo("    Cleaning Heigthmap_512 Vertices array.");
        delete vertex;
    }

    DebugManager::debugInfo("    Instantiating Heigthmap_512 indices.");
    int indices = 6 * (SIZE - 1) * (SIZE - 1);
    int *index = new int[indices];
    DebugManager::debugInfo("    Creating Index buffer.");
    ibo512 = IndexBuffer(gd, IndexBuffer::ThirtyTwoBits, indices, None);
    createMeshIndices(index, SIZE);
    DebugManager::debugInfo("    Setting Index Buffer Data.");
    ibo512.SetData(index, 0, indices);

    DebugManager::debugInfo("    Cleaning Heigthmap_512 indices array.");
    delete index;
}

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



        DebugManager::debugInfo("    Loading heightmap texture.");
        heightMap = cm->Load<Texture2D > (path);
        heightMap->MaxFiltering(TextureFilteringMode::Nearest);

        if (!DataManager::useShaderHeightmap) {
            VertexElement *ves = new VertexElement[1];
            ves[0] = VertexElement(0, 0, VertexElement::Vector3, VertexElement::Position);

            vd = VertexDeclaration(ves, 1);

            DebugManager::debugInfo("    Instantiating Heigthmap_512 vertices.");
            int vertices = SIZE * SIZE;
            Vector3 *vertex = new Vector3[vertices];
            createMeshHeightVertes(vertex, heightMap, SIZE, dimension.Z);
            DebugManager::debugInfo("    Creating Vertex buffer.");
            vbo512 = VertexBuffer(NULL, vd, vertices, None);
            DebugManager::debugInfo("    Setting Index Buffer Data.");
            vbo512.SetData(vertex, 0, vertices);


            DebugManager::debugInfo("    Cleaning Heigthmap_512 Vertices array.");
            delete vertex;

            delete heightMap;
        }
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

        DebugManager::debugInfo("    Loading colorMap texture.");
        colorMap = cm->Load<Texture2D > (path);
    } catch (ContentLoadException *cle) {
        throw new ArgumentException("Error loading Mesh's colormap.", cle);
    }
    // </editor-fold>

    // <editor-fold defaultstate="collapsed" desc="Creating NormalMap Texture2D">
    normalMap = NULL;
    // </editor-fold>

    DebugManager::debugInfo("    Loading Heightmap effect.");
    if (DataManager::useShaderHeightmap) {
        effect = cm->Load<Effect > ("Shaders/heightmap.prog");
    } else {
        effect = cm->Load<Effect > ("Shaders/heightmapSimple.prog");
    }

    DebugManager::debugInfo("    Setting Heightmap parameters.");
    effect->Begin();
    effect->getParameter("colorMap").SetValue<Texture2D > (colorMap);
    if (DataManager::useShaderHeightmap) {
        effect->getParameter("heightMap").SetValue<Texture2D > (heightMap);
        float elevation = dimension.Z;
        effect->getParameter("elevation").SetValue<float> (&elevation);
    }

    effect->End();
}

void Scenario::Mesh::Draw(Camera* camera, GraphicDevice *gd, DrawOptions option) {
    effect->Begin();
    Matrix world = Matrix::CreateScale(dimension * DataManager::unit_size) *
            Matrix::CreateTranslation(position * DataManager::unit_size);
    Matrix wvp = world * camera->viewMatrix() * camera->projectionMatrix();

    effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);

    gd->setIndices(ibo512);
    if (DataManager::useShaderHeightmap) {
        gd->setVertexBuffer(Svbo512);
    } else {
        gd->setVertexBuffer(vbo512);
    }
    gd->DrawIndexedPrimitives(GraphicDevice::TriangleList, 0, SIZE*SIZE, 6 * (SIZE - 1)*(SIZE - 1));
    effect->End();
}

Scenario::Mesh::~Mesh() {
    /*if(effect)    delete effect;
    if(heightMap) delete heightMap;
    if(colorMap)  delete colorMap;
    if(normalMap) delete normalMap;*/
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Floor">

struct VertNormTexInd {
    Vector3 v;
    Vector3 n;
    Vector2 t;
};

Scenario::Floor::Floor(Game* game) {
    Texture2D *diffMap = game->Content->Load<Texture2D > ("Scenario/Floor.bmp");
    effect = game->Content->Load<Effect > ("Shaders/Floor.prog");


    effect->Begin();
    effect->getParameter("diffMap").SetValue<Texture2D > (diffMap);
    //float sp = 16.f;
    //effect->getParameter("specPow").SetValue<float>(&sp);

    Ep_WVP = effect->getParameter("rMatWorldViewProjection");
    //Ep_W = effect->getParameter("rMatWorld");
    //Ep_CamPos = effect->getParameter("rCamPos");
    effect->End();

    // <editor-fold defaultstate="collapsed" desc="Buffers Creation">
    VertexElement *ves = new VertexElement[3];
    ves[0] = VertexElement(0, 0, VertexElement::Vector3, VertexElement::Position);
    ves[1] = VertexElement(3 * sizeof (GLfloat), 0, VertexElement::Vector3, VertexElement::Normal);
    ves[2] = VertexElement(6 * sizeof (GLfloat), 0, VertexElement::Vector2, VertexElement::TextureCoordinate);

    vd = VertexDeclaration(ves, 3);


    DebugManager::debugInfo("Instantiating floor indices.");
    int indices = 4;
    int *index = new int[indices];
    for (int i = 0; i < indices; i++) index[i] = i;
    DebugManager::debugInfo("Creating Index buffer.");
    ibo = IndexBuffer(game->graphicDevice, IndexBuffer::ThirtyTwoBits, indices, None);
    DebugManager::debugInfo("Setting Index Buffer Data.");
    ibo.SetData(index, 0, indices);

    delete index;

    DebugManager::debugInfo("Instantiating Heigthmap_512 vertices.");
    int vertices = 4;
    VertNormTexInd vertex[] = {
        {
            Vector3(0.f, 1.f, 0.f),
            Vector3(0.f, 0.f, 1.f),
            Vector2(0.f, 1.f)
        },
        {
            Vector3(1.f, 1.f, 0.f),
            Vector3(0.f, 0.f, 1.f),
            Vector2(1.f, 1.f)
        },
        {
            Vector3(1.f, 0.f, 0.f),
            Vector3(0.f, 0.f, 1.f),
            Vector2(1.f, 0.f)
        },
        {
            Vector3(0.f, 0.f, 0.f),
            Vector3(0.f, 0.f, 1.f),
            Vector2(0.f, 0.f)
        }
    };
    DebugManager::debugInfo("Creating Vertex buffer.");
    vbo = VertexBuffer(game->graphicDevice, vd, vertices, None);
    DebugManager::debugInfo("Setting Index Buffer Data.");
    vbo.SetData(vertex, 0, vertices); // </editor-fold>



}

void Scenario::Floor::Draw(Camera* camera, Matrix world, GraphicDevice *gd, DrawOptions option) {
    if (option & DrawColor) {

        DebugManager::debugInfo("    Starting Effect.");
        effect->Begin();
        //Vector3 v = camera->getPosition();
        //Ep_CamPos.SetValue<Vector3 > (&v);

        Matrix wvp = world * camera->viewMatrix() * camera->projectionMatrix();
        Ep_WVP.SetValue<Matrix > (&wvp);


        ///Ep_W.SetValue<Matrix > (&world);

        DebugManager::debugInfo("    Seting indices and Vertex.");
        gd->setIndices(ibo);
        gd->setVertexBuffer(vbo);
        DebugManager::debugInfo("    Drawing primitives.");
        gd->DrawIndexedPrimitives(GraphicDevice::QuadList, 0, 4, 4);

        effect->End();
    }
}
// </editor-fold>
