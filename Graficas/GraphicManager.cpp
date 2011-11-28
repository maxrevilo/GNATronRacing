#include <cmath>
#include "GraphicManager.h"
#include "../GNAFramework/CPPS/AdaptacionesDeFunciones.h"
#include "../GNAFramework/Color.h"
#include "../Logica/Plane.h"
#include "../Logica/Ray.h"

GraphicManager *GraphicManager::instance = NULL;

// <editor-fold defaultstate="collapsed" desc="Constructor">

GraphicManager::GraphicManager(Game *game) {
    this->game = game;
    spriteBatch = new SpriteBatch(game->graphicDevice);

    game->graphicDevice->setRasterizerState(RasterizerState::CullCounterClockwise);

    loads.ligth_load = false;
    loads.heavy_load = false;

    camera = Camera(DataManager::camera_ini_pos, DataManager::camera_ini_cen, Vector3(0.f, 1.f, 0.f), Frustum(45.0f, 8.0 / 6.0, 1.f, 5000.0f));
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="Loads">
void GraphicManager::ligthLoad() {
    printf("ligthLoad() 0...\n"); fflush(stdout);
    font.title = game->Content->Load<SpriteFont > ("Title.spritefont");
    font.title2 = game->Content->Load<SpriteFont > ("Title2.spritefont");
    font.text = game->Content->Load<SpriteFont > ("Text.spritefont");
    printf("ligthLoad() 1...\n"); fflush(stdout);
    loading_tex = game->Content->Load<Texture2D > ("Loading.bmp");
    printf("ligthLoad() 2...\n"); fflush(stdout);
    loads.ligth_load = true;
}

void GraphicManager::heavyLoad() {
    printf("heavyLoad() 0...\n"); fflush(stdout);
    cube   = new Cube_mesh(game->graphicDevice);
    printf("heavyLoad() 0.1...\n"); fflush(stdout);
    sphere = new Sphere_mesh(game->graphicDevice, 32, 16);
    printf("heavyLoad() 1...\n"); fflush(stdout);
    scenario.effect      = game->Content->Load<Effect>   ("Shaders/scenario.prog");
    scenario.floor_tex   = game->Content->Load<Texture2D>("Scenarios/Floor.bmp"  );
    scenario.cristal_tex = game->Content->Load<Texture2D>("Scenarios/cristal.bmp");
    scenario.ambient     = game->Content->Load<Texture2D>("Scenarios/ambient.bmp");
    printf("heavyLoad() 2...\n"); fflush(stdout);
    cube_effect = game->Content->Load<Effect>("Shaders/Cube.prog");

    obst_tex_length = 2;
    obst_tex = new _OBST_TEX[obst_tex_length];
    obst_tex[0].texture = game->Content->Load<Texture2D>("Obstacles/cube_diff.bmp");
    
    obst_tex[1].texture = game->Content->Load<Texture2D>("Obstacles/jupiter.bmp");
    printf("heavyLoad() 3...\n"); fflush(stdout);
    flecha_eff = game->Content->Load<Effect>   ("Shaders/flechas.prog");
    flecha_tex = game->Content->Load<Texture2D>("flechas.bmp");
    printf("heavyLoad() 4...\n"); fflush(stdout);
    player_effect = game->Content->Load<Effect>("Shaders/player.prog");
    printf("heavyLoad() 5...\n"); fflush(stdout);
    loads.heavy_load = true;
}
// </editor-fold>

// <editor-fold defaultstate="collapsed" desc="DRAWS">
void GraphicManager::DrawMatchLoading() {
    if (!loads.ligth_load) ligthLoad();

    game->graphicDevice->DeepBufferEnabled(false);
    spriteBatch->Begin(SpriteBatch::Immediate, BlendState::NonPremultiplied);
    spriteBatch->Draw(loading_tex, RectangleF(0.f, 0.f, 800.f, 600.f), Color_GNA::White);
    spriteBatch->DrawString(font.title, "TRON: Disc Battle", Vector2(20.f, 10.f), DataManager::loading_color);
    spriteBatch->DrawString(font.text, "Cargando...", DataManager::loading_pos, DataManager::loading_color);
    spriteBatch->End();
}


void GraphicManager::DrawMatchDefeat(){
    if(!loads.ligth_load) ligthLoad();
    
    game->graphicDevice->DeepBufferEnabled(false);
    spriteBatch->Begin(SpriteBatch::Immediate, BlendState::NonPremultiplied);
    spriteBatch->DrawString(font.title, "TRON: Disc Battle", Vector2(20.f, 10.f), DataManager::loading_color);
    spriteBatch->DrawString(font.text, "Has sido derrotado", Vector2(200.f, 300.f), DataManager::loading_color);
    spriteBatch->End();
}

void GraphicManager::DrawMatchVictory(){
    if(!loads.ligth_load) ligthLoad();
    
    game->graphicDevice->DeepBufferEnabled(false);
    spriteBatch->Begin(SpriteBatch::Immediate, BlendState::NonPremultiplied);
    spriteBatch->DrawString(font.title, "TRON: Disc Battle", Vector2(20.f, 10.f), DataManager::loading_color);
    spriteBatch->DrawString(font.text, "Has ganado", Vector2(200.f, 300.f), DataManager::loading_color);
    spriteBatch->End();
}

float uvm = 0.f;
void GraphicManager::DrawMatch(Match *match){
    printf("Drawing 0\n");
    if(!loads.ligth_load) ligthLoad();
    if(!loads.heavy_load) heavyLoad();
    
    printf("Drawing 1 Loaded\n");
    
    
    // <editor-fold defaultstate="collapsed" desc="Camera Setting">
    float cam_angXZ = match->playerStats.cam.angL;
    float cam_angY = match->playerStats.cam.angV;
    Vector3 cam_d(cos(cam_angXZ) * cos(cam_angY), sin(cam_angY), sin(cam_angXZ) * cos(cam_angY));
    camera.setPosition(match->playerStats.cam.pos);
    camera.setFocus(match->playerStats.cam.pos + cam_d);

    Level_d *level = match->level();
    Matrix viewProy = camera.viewMatrix() * camera.projectionMatrix();
    // </editor-fold>
    printf("Drawing 2\n");
    RasterizerState rs = game->graphicDevice->getRasterizerState();
    BlendState bs = game->graphicDevice->getBlendState();
    game->graphicDevice->DeepBufferEnabled(true);
    
    printf("Drawing 3\n");
    
    // <editor-fold defaultstate="collapsed" desc="Draw Back Scenario">
    {
        float height = level->height, width = level->width, deep = level->deep;
        
        Vector2 vec;
        Matrix wvp;
        
        game->graphicDevice->setRasterizerState(RasterizerState::CullNone);

        scenario.effect->Begin();
        
        // <editor-fold defaultstate="collapsed" desc="SkyMap">
        game->graphicDevice->setBlendState(BlendState::Opaque);
        wvp = Matrix::CreateScale(1000.f, 1000.f, 1000.f) * viewProy;
        scenario.effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);
        scenario.effect->getParameter("baseMap").SetValue<Texture2D > (scenario.ambient);
        vec = Vector2(1, 1);
        scenario.effect->getParameter("uvScale").SetValue<Vector2 > (&vec);
        
        sphere->Draw();
        // </editor-fold>
        
        // <editor-fold defaultstate="collapsed" desc="Floor">
        wvp = Matrix::CreateTranslation(1.f, 1.f, 1.f) *
                Matrix::CreateScale(width / 2.f, height / 2.f, deep / 2.f) *
                viewProy;
        vec = Vector2(width, deep);
        scenario.effect->getParameter("uvScale").SetValue<Vector2 > (&vec);
        scenario.effect->getParameter("baseMap").SetValue<Texture2D > (scenario.floor_tex);
        scenario.effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);
        
        cube->Draw(GraphicDevice::TriangleList, Cube_mesh::DOWN);
        
        // </editor-fold>
        
        scenario.effect->End();
    }
    // </editor-fold>
    printf("Drawing 4\n");
    // <editor-fold defaultstate="collapsed" desc="Draw Obstacles">
    Obstacle *obstacles = match->obstacles;
    game->graphicDevice->setRasterizerState(RasterizerState::CullCounterClockwise);
    cube_effect->Begin();
    for (int i = 0; i < level->obstacles.length; i++) {
        if(obstacles[i].alive()){
            Vector2 vec2;
            Matrix wvp =
                    Matrix::CreateScale(obstacles[i].size() / 2) *
                    Matrix::CreateTranslation(obstacles[i].position()) *
                    viewProy;


            cube_effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);

            switch (obstacles[i].type()) {
                case Obstacle_d::Sphere:
                    vec2 = Vector2(5.f, 1.f);
                    cube_effect->getParameter("uvScale").SetValue<Vector2 > (&vec2);
                    cube_effect->getParameter("baseMap").SetValue<Texture2D > (obst_tex[0].texture);
                    sphere->Draw(GraphicDevice::TriangleList);
                    break;
                case Obstacle_d::Cube:
                    vec2 = Vector2(1.f, 1.f);
                    cube_effect->getParameter("uvScale").SetValue<Vector2 > (&vec2);
                    cube_effect->getParameter("baseMap").SetValue<Texture2D > (obst_tex[0].texture);
                    cube->Draw(GraphicDevice::TriangleList);
                    break;
            }
        }
    }
    cube_effect->End();
    // </editor-fold>
    printf("Drawing 5\n");
    // <editor-fold defaultstate="collapsed" desc="Draw Players">
    Player *players = match->players;
    
    // <editor-fold defaultstate="collapsed" desc="Draw Path">
    game->graphicDevice->setRasterizerState(RasterizerState::CullNone);
    game->graphicDevice->setBlendState(BlendState::NonPremultiplied);
    game->graphicDevice->DeepBufferWritteEnabled(false);
    
    uvm += 0.03f;
    Vector2 vecS(1.f, 10.f);
    Vector2 vecA(0.f, uvm);
    flecha_eff->Begin();
    flecha_eff->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&viewProy);
    flecha_eff->getParameter("uvScale").SetValue<Vector2 > (&vecS);
    flecha_eff->getParameter("uvAdd").SetValue<Vector2 > (&vecA);
    flecha_eff->getParameter("baseMap").SetValue<Texture2D > (flecha_tex);
    for (int i = 0; i <= level->enemys.length; i++) {
        if(players[i].alive()) {
            Vector3 *path = players[i].descriptor->pathway.points;
            Vector4 color = Player::color(i).toVector4();
            flecha_eff->getParameter("Color").SetValue<Vector4 > (&color);
            int L = players[i].descriptor->pathway.length;
            if (L > 1) {
                glBegin(GL_QUADS);
                for (int j = 1; j < L; j++) {
                    DrawLine(path[j], path[j - 1]);
                }
                DrawLine(path[0], path[L - 1]);
                if (L == 2) DrawLine(path[L - 1], path[0]);
                glEnd();
            }
        }
    }
    flecha_eff->End();
    // </editor-fold>
    
    // <editor-fold defaultstate="collapsed" desc="Draw Vehicle">
    game->graphicDevice->setBlendState(BlendState::Opaque);
    game->graphicDevice->DeepBufferWritteEnabled(true);
    game->graphicDevice->setRasterizerState(RasterizerState::CullCounterClockwise);
    
    player_effect->Begin();
    for (int i = 0; i <= level->enemys.length; i++) {
        if(players[i].alive()) {
            Vector4 color;
            Vector3 pos  = players[i].position();
            Vector3 posa = players[i].position(0.1f) - pos + Vector3(0.f, 0.f, 0.00000000001f);
            
            Matrix wvp =
                    Matrix::CreateScale(players[i].size()) *
                    Matrix::CreateWorld(pos, Vector3::Normalize(posa), Vector3::Up) *
                    viewProy;
            
            player_effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);
            color = Player::color(i).toVector4();
            player_effect->getParameter("Color").SetValue<Vector4 > (&color);
            
            sphere->Draw();
        }
    }
    player_effect->End();
    // </editor-fold>

    // </editor-fold>
    printf("Drawing 6\n");
    // <editor-fold defaultstate="collapsed" desc="Draw Discs">
    game->graphicDevice->setRasterizerState(RasterizerState::CullNone);
    
    Disc *discs = match->discs;
    player_effect->Begin();
    for (int i = 0; i <= MAX_DISCS; i++) {
        if(discs[i].active()) {
            Vector4 disc_color = Player::color(discs[i].player_index).toVector4();
            Matrix wvp = Matrix::CreateTranslation(discs[i].position) * viewProy;
            player_effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);
            player_effect->getParameter("Color").SetValue<Vector4> (&disc_color);
            
            game->graphicDevice->setBlendState(BlendState::Opaque);
            glBegin(GL_TRIANGLE_FAN);
            //printf("Disc draw %d <%f, %f, %f>\n", i, discs[i].position.X, discs[i].position.Y, discs[i].position.Z);
            glVertex3f(0.f, 0.f, 0.f);
            float diff = TwoPi / 16.f;
            for(int a = 0; a <= 16; a++) {
                glVertex3f(cos(a*diff)*discs[i].size()/2.f, 0.f, sin(a*diff)*discs[i].size()/2.f);
            }
            glEnd();
            
            Plane pl(Vector3(), Vector3::Up);
            Vector3 p = pl.NearestPoint(Sphere(discs[i].position, 0.f));
            
            wvp = Matrix::CreateTranslation(p) * viewProy;
            player_effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);
            disc_color = disc_color * 0.6f;
            player_effect->getParameter("Color").SetValue<Vector4> (&disc_color);
            
            game->graphicDevice->setBlendState(BlendState::NonPremultiplied);
            glBegin(GL_TRIANGLE_FAN);
            //printf("Disc draw %d <%f, %f, %f>\n", i, discs[i].position.X, discs[i].position.Y, discs[i].position.Z);
            glVertex3f(0.f, 0.f, 0.f);
            for(int a = 0; a <= 16; a++) {
                glVertex3f(cos(a*diff)*discs[i].size()/2.f, 0.01f, sin(a*diff)*discs[i].size()/2.f);
            }
            glEnd();
        }
    }
    player_effect->End();
    // </editor-fold>
    printf("Drawing 7\n");
    // <editor-fold defaultstate="collapsed" desc="Draw Front Scenario">
    {
        float height = level->height, width = level->width, deep = level->deep;
        Vector2 vec;
        Matrix wvp;
        
        game->graphicDevice->setRasterizerState(RasterizerState::CullClockwise);
        game->graphicDevice->setBlendState(BlendState::NonPremultiplied);
        
        scenario.effect->Begin();
        
        wvp = Matrix::CreateTranslation(1.f, 1.f, 1.f) *
                Matrix::CreateScale(width / 2.f, height / 2.f, deep / 2.f) *
                viewProy;
        scenario.effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);
        
        // UP
        scenario.effect->getParameter("baseMap").SetValue<Texture2D > (scenario.cristal_tex);
        vec = Vector2(width / 8.f, deep / 8.f);
        scenario.effect->getParameter("uvScale").SetValue<Vector2 > (&vec);
        cube->Draw(GraphicDevice::TriangleList, Cube_mesh::UP);

        // FRONT-BACK
        vec = Vector2(width / 8.f, height / 8.f);
        scenario.effect->getParameter("uvScale").SetValue<Vector2 > (&vec);
        cube->Draw(GraphicDevice::TriangleList, Cube_mesh::FRONT);
        cube->Draw(GraphicDevice::TriangleList, Cube_mesh::BACK);

        // RIGHT-LEFT
        vec = Vector2(deep / 8.f, height / 8.f);
        scenario.effect->getParameter("uvScale").SetValue<Vector2 > (&vec);
        cube->Draw(GraphicDevice::TriangleList, Cube_mesh::RIGHT);
        cube->Draw(GraphicDevice::TriangleList, Cube_mesh::LEFT);

        scenario.effect->End();
    }// </editor-fold>
    printf("Drawing 8\n");
    if(Mouse::getMouseVisibility())
        Mouse::setMouseVisibility(false);
    // <editor-fold defaultstate="collapsed" desc="Mouse">
    {    
        float t, *tt = &t;
        Ray ray = match->playerStats.cam.ray;
        ray.Intersects(match->scenario[0], &tt);
        
        if(tt != NULL) {
            player_effect->Begin();
            Vector4 color = Player::color(0).toVector4() * 0.7f;
            Matrix wvp = Matrix::CreateTranslation(ray.point_at(t))
                    * viewProy;
            player_effect->getParameter("rMatWorldViewProjection").SetValue<Matrix > (&wvp);
            player_effect->getParameter("Color").SetValue<Vector4> (&color);

            glBegin(GL_QUADS);
                glVertex3f(-1.f,  .01f,  0.f);
                glVertex3f( 0.f,  .01f,  .5f);
                glVertex3f( 1.f,  .01f,  0.f);
                glVertex3f( 0.f,  .01f, -.5f);
            glEnd();
            player_effect->End();
        }
    }
    // </editor-fold>
    printf("Drawing 9\n");
            
    game->graphicDevice->setBlendState(bs);
    game->graphicDevice->setRasterizerState(rs);
    game->graphicDevice->DeepBufferEnabled(false);
    
    spriteBatch->Begin(SpriteBatch::Immediate, BlendState::NonPremultiplied);
    if(match->pause) {
        spriteBatch->DrawString(font.title, "TRON: Disc Battle", Vector2(20.f, 10.f), DataManager::loading_color);
        spriteBatch->DrawString(font.title, "Pausado", Vector2(200.f, 300.f), DataManager::loading_color);
        spriteBatch->DrawString(font.text, "Presione P para continuar", Vector2(200.f, 370.f), DataManager::loading_color);
    }
    spriteBatch->End();
    printf("Drawing 10\n");
}


// <editor-fold defaultstate="collapsed" desc="Helpers">

void GraphicManager::DrawLine(Vector3 v0, Vector3 v1) {
    float width = 0.3f;
    Vector3 dir = Vector3::Normalize(v0 - v1);
    Vector3 ort = Vector3(dir.Z, dir.Y, -dir.X) * width;
    
    glVertex3f(v0.X - ort.X, v0.Y + 0.1f, v0.Z - ort.Z);
    glTexCoord2f(0.f, 0.f);
    glVertex3f(v0.X + ort.X, v0.Y + 0.1f, v0.Z + ort.Z);
    glTexCoord2f(0.f, 1.f);
    glVertex3f(v1.X + ort.X, v1.Y + 0.1f, v1.Z + ort.Z);
    glTexCoord2f(1.f, 1.f);
    glVertex3f(v1.X - ort.X, v1.Y + 0.1f, v1.Z - ort.Z);
    glTexCoord2f(1.f, 0.f);
}

// </editor-fold>

// </editor-fold>

