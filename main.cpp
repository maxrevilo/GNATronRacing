#include "stdio.h"
#include <vector>

#include "GNAFramework/Game.h"
#include "GNAFramework/Keyboard.h"
#include "GNAFramework/Geom.h"
#include "GNAFramework/SpriteBatch.h"
#include "GNAFramework/BlendState.h"
#include "GNAFramework/MathHelper.h"
#include "GNAFramework/Mouse.h"
#include "GNAFramework/SpriteFont.h"

#include "tinyxml/tinyxml.h"

#include "Managers/InputManager.h"
#include "Managers/DataManager.h"
#include "Managers/DebugManager.h"

#include "GameActors/Scenario.h"
#include "GameActors/Enemy.h"
#include "GameActors/ExitPosition.h"

using namespace GNAFramework;

#define MAX_ENEMIES 16

class TronRacing : public Game {
public:

    enum GameState {
        Lobby,
        Loading,
        Playing,
        WaitingNextLVL,
        GameOver
    } game_state;

    InputManager im;

    int actual_level;
    bool paused;
    ExitPosition *exitPosition;
    
    int cameras;

    Camera camera;
    //CameraState camState;
    Scenario *scenario;
    LightCycle *player;

    int enemies_length;
    Enemy **enemies;

    Texture2D *loading_tex;
    SpriteBatch *spriteBatch;

    TronRacing() {
        Content->RootDirectory = (char *) "Content/";
    }

    virtual void Initialize() {
        DebugManager::debugInfo("Initialazing ImputManager.");
        im.Initialize();

        DebugManager::debugInfo("Loading GAME_CONF.xml.");
        DataManager::loadGameDescription("GAME_CONF.xml");


        actual_level = 0;
        float ar = graphicDevice->getViewPort().aspectRatio();
        camera = Camera(Vector3(-555, 430, 320) * DataManager::unit_size, Vector3(0, 0, 500.f) * DataManager::unit_size, Vector3(0, 0, 1), Frustum(45.0f, ar, 1.f, 10000.0f));
        cameras = 1;


        DebugManager::debugInfo("Instantiating Scenario");
        scenario = new Scenario(this, &camera);

        DebugManager::debugInfo("Instantiating Player");
        player = new LightCycle(this, &camera);

        enemies_length = 0;
        enemies = new Enemy*[MAX_ENEMIES];
        for (int i = 0; i < MAX_ENEMIES; i++) enemies[i] = NULL;
        
        exitPosition = new ExitPosition(this);
        
        game_state = Lobby;

        DebugManager::debugInfo("Initialazing GNAGame.");
        Game::Initialize();

        spriteBatch = new SpriteBatch(this->graphicDevice);
        loading_tex = Content->Load<Texture2D > ("Loading.bmp");

        DebugManager::debugInfo("Tron Race Initialized.");
    }

    virtual void Update(GameTime gameTime) {
        im.Update();

        if (im.KeyPressed(Keyboard::Escape)
                || (im.KeyPressed(Keyboard::F4) && im.KeyPressed(Keyboard::Left_Alt)))
            Exit();

        switch (game_state) {
            case Lobby:
                game_state = Loading;
                /* On Loading "Pause = true" indicates that we have to wait that
                 * the game draw the load screen. */
                paused = true;
                break;

            case Loading:
                // <editor-fold defaultstate="collapsed" desc="Loading">
                if (!paused) {


                    DebugManager::debugInfo("Loading Level.");

                    if (actual_level >= DataManager::levels_paths_lenght) {
                        throw new ArgumentOutOfRangeException("Trying to access a level out of range.");
                    }

                    char path[256];
                    sprintf(path, "Levels/%s", DataManager::levels_paths[actual_level]);
                    TiXmlDocument doc = TiXmlDocument(path);
                    if (doc.LoadFile()) {

                        TiXmlNode *base = doc.FirstChild("level");
                        if (base == NULL)
                            throw new ArgumentException("level root Tag coundn't be found. check the level's XML.");


                        // <editor-fold defaultstate="collapsed" desc="Loading Scenario">
                        DebugManager::debugInfo("Loading Scenario.");
                        scenario->Initialize(base->FirstChild("scenario"));
                        // </editor-fold>


                        // <editor-fold defaultstate="collapsed" desc="Loading Player">
                        DebugManager::debugInfo("Loading Player.");
                        player->Initialize(base->FirstChild("player"));
                        // </editor-fold>


                        // <editor-fold defaultstate="collapsed" desc="Loading Enemies">
                        TiXmlNode *nodeEnemys = base->FirstChild("enemys");
                        enemies_length = 0;

                        DebugManager::debugInfo("Loading Enemies.");

                        if (nodeEnemys != NULL) {
                            TiXmlElement *elemEnemys = nodeEnemys->ToElement();

                            for (TiXmlNode *o_node = elemEnemys->FirstChild(); o_node != 0; o_node = o_node->NextSibling()) {
                                if (o_node != NULL && o_node->ToElement() != NULL) {
                                    DebugManager::debugInfo("Loading Enemy %d.", enemies_length);
                                    if (enemies[enemies_length] == NULL) {
                                        DebugManager::debugInfo("Instantiating Enemy %d.", enemies_length);
                                        enemies[enemies_length] = new Enemy(this, &camera);
                                    }

                                    DebugManager::debugInfo("Instantializing Enemy %d.", enemies_length);
                                    enemies[enemies_length++]->Initialize(o_node);
                                    if (enemies_length >= MAX_ENEMIES)
                                        throw new ArgumentException("the number of enemies exceeds the limit. check the level's XML.");
                                }

                            }

                        }

                        // </editor-fold>
                        
                        
                        
                        exitPosition->Initialize(base->FirstChild("exit_position"));

                    } else {
                        char msg[128];
                        sprintf(msg, "Level XML on \"%s\" couldn't be loaded.", path);
                        throw new ContentLoadException(msg);
                    }


                    DebugManager::debugInfo("Level Loaded.");
                    game_state = Playing;
                    paused = true;
                }
                // </editor-fold>
                break;

            case Playing:
                ImputHanlder(gameTime);

                if (paused) {

                } else {
                    player->Update(gameTime);


                    for (int i = 0; i < enemies_length; i++) {
                        enemies[i]->Update(gameTime);
                    }
                }
                break;

            case WaitingNextLVL:
                break;

            case GameOver:
                break;
        }


        Game::Update(gameTime);
    }

    virtual void Draw(GameTime gameTime) {
        graphicDevice->Clear(Color::Grey);

        if (Mouse::getMouseVisibility()) Mouse::setMouseVisibility(false);


        switch (game_state) {
            case Lobby:

                break;

            case Loading:
            {
                graphicDevice->DeepBufferEnabled(false);
                ViewPort vp = graphicDevice->getViewPort();
                spriteBatch->Begin(SpriteBatch::Immediate, BlendState::Opaque);
                spriteBatch->Draw(loading_tex, RectangleF(0.f, 0.f, vp.width, vp.height), Color::White);
                spriteBatch->End();

                paused = false;
            }
                break;

            case Playing:
            {
                if (!paused) {
                    Vector3 p;
                    Vector3 f;
                    Vector3 u;
                    switch (cameras) {
                        case 1:
                            player->getFirstPersonView(p, f, u);
                            setCamera(p, f, u);
                            miniDraw(DrawColor, gameTime);
                            break;
                        case 2:
                            player->getThirdPersonView(p, f, u);
                            setCamera(p, f, u);
                            miniDraw(DrawColor, gameTime);
                            break;
                        case 3:
                            p = exitPosition->position + Vector3(0.f, 0.f, 2000.f);
                            f = player->Position();
                            u = Vector3::UnitZ;
                            setCamera(p, f, u);
                            miniDraw(DrawColor, gameTime);
                            break;
                        default:
                            
                            ViewPort new_vp, old_vp = graphicDevice->getViewPort();
                            new_vp = old_vp;
                            
                            
                            //First Person:
                            new_vp.Y = old_vp.height / 2;
                            new_vp.height = old_vp.height / 2;
                            graphicDevice->setViewPort(new_vp);
                            player->getFirstPersonView(p, f, u);
                            setCamera(p, f, u);
                            miniDraw(DrawColor, gameTime);
                            
                            //Third Person:
                            new_vp.Y = 0;
                            new_vp.width = old_vp.width / 2;
                            graphicDevice->setViewPort(new_vp);
                            player->getThirdPersonView(p, f, u);
                            setCamera(p, f, u);
                            miniDraw(DrawColor, gameTime);
                            
                            
                            
                            //Exit Positon:
                            new_vp.X = old_vp.width / 2;
                            graphicDevice->setViewPort(new_vp);
                            p = exitPosition->position + Vector3(0.f, 0.f, 2000.f);
                            f = player->Position();
                            u = Vector3::UnitZ;
                            setCamera(p, f, u);
                            setCamera(p, f, u);
                            miniDraw(DrawColor, gameTime);
                            
                            
                            graphicDevice->setViewPort(old_vp);
                            break;
                    }
                } else {
                    
                    miniDraw(DrawColor, gameTime);
                }

            }
                break;

            case WaitingNextLVL:
                break;

            case GameOver:
                break;
        }

        DebugManager::debugInfo("End Drawing.");
        Game::Draw(gameTime);
    }
    
    void setCamera(Vector3 p, Vector3 f, Vector3 u) {
        Frustum frustum;
        float ar = graphicDevice->getViewPort().aspectRatio();
        if (cameras == 0) {
            frustum = Frustum(76.0f, ar, 1.f, 10000.0f);
        } else {
            frustum = Frustum(50.0f, ar, 1.f, 10000.0f);
        }

        camera = Camera(p, f, u, frustum);
    }

    void miniDraw(DrawOptions, GameTime gameTime) {

        graphicDevice->DeepBufferEnabled(true);
        graphicDevice->DeepBufferWritteEnabled(true);

        graphicDevice->setRasterizerState(RasterizerState::CullCounterClockwise);
        graphicDevice->setBlendState(BlendState::Opaque);


        scenario->Draw(gameTime, DrawColor);


        player->Draw(gameTime, DrawColor);

        for (int i = 0; i < enemies_length; i++) {
            enemies[i]->Draw(gameTime, DrawColor);
        }

        graphicDevice->DeepBufferWritteEnabled(false);
        graphicDevice->setBlendState(BlendState(BlendState::One, BlendState::One, BlendState::Add));
        graphicDevice->setRasterizerState(RasterizerState::CullNone);

        player->DrawTrail(DrawColor);

        for (int i = 0; i < enemies_length; i++) {
            enemies[i]->DrawTrail(DrawColor);
        }

        graphicDevice->DeepBufferWritteEnabled(true);
        graphicDevice->setBlendState(BlendState::Opaque);
        graphicDevice->setRasterizerState(RasterizerState::CullCounterClockwise);
    }

    virtual void Exit() {
        DebugManager::debugInfo("Calling to Exit().");
        delete scenario;

        Game::Exit();
    }


    // <editor-fold defaultstate="collapsed" desc="Helpers">

    void ImputHanlder(GameTime gameTime) {
        if (im.KeyGetPressed(Keyboard::P)) paused = !paused;

        float time = gameTime.elapsedTime.toSecondsF();

        if (paused) {

        } else {
            if (im.KeyPressed(Keyboard::Right)) {
                player->Turn(-DataManager::turnRate * time);
            } else if (im.KeyPressed(Keyboard::Left)) {
                player->Turn(+DataManager::turnRate * time);
            }

            if (im.KeyPressed(Keyboard::Space) && player->Fuel() > 0.f) {
                player->Acelerate(true);
            } else {
                player->Acelerate(false);
            }

            if (im.KeyPressed(Keyboard::F1)) {
                cameras = 1;
            } else if (im.KeyPressed(Keyboard::F2)) {
                cameras = 2;
            } else if (im.KeyPressed(Keyboard::F3)) {
                cameras = 3;
            } else if (im.KeyPressed(Keyboard::F4)) {
                cameras = 4;
            } else if (im.KeyPressed(Keyboard::F5)) {
                cameras = 5;
            } else if (im.KeyPressed(Keyboard::F6)) {
                cameras = 6;
            }
        }
    }
    // </editor-fold>

};

int main(int argc, char** argv) {
    DebugManager::debugInfo("Instantiating TronRacing game.");
    TronRacing game;
    DebugManager::debugInfo("Starting Game.");
    game.Start();

    return 0;
}