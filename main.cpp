#include "stdio.h"

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


using namespace GNAFramework;


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
    
    Camera camera;
    Scenario *scenario;

    TronRacing(){
        Content->RootDirectory = (char *) "Content/";
    }

    virtual void Initialize() {
        DebugManager::debugInfo("Initialazing ImputManager.");
        im.Initialize();
        
        DebugManager::debugInfo("Loading GAME_CONF.xml.");
        DataManager::loadGameDescription("GAME_CONF.xml");
        
        
        actual_level = 0;
        float ar = graphicDevice->getViewPort().aspectRatio();
        camera = Camera(Vector3(-1000,-1000,500), Vector3(1000, 2000, 0), Vector3(0,0,1), Frustum(45.0f, ar, 1.f, 10000.0f));
        
        DebugManager::debugInfo("Instantiating Scenario");
        scenario = new Scenario(this, &camera);
        
        
        DebugManager::debugInfo("Initialazing GNAGame.");
        Game::Initialize();
        
        DebugManager::debugInfo("Tron Race Initialized.");
    }

    
    virtual void Update(GameTime gameTime) {
        im.Update();
        
        if (im.KeyPressed(Keyboard::Escape) 
                || (im.KeyPressed(Keyboard::F4) && im.KeyPressed(Keyboard::Left_Alt)) )
            Exit();
        
        switch(game_state) {
            case Lobby:
                game_state = Loading;
                /* On Loading "Pause = true" indicates that we have to wait that
                 * the game draw the load screen. */
                paused = true;
                break;
                
            case Loading:
                // <editor-fold defaultstate="collapsed" desc="Loading">
                if(!paused) {
                    DebugManager::debugInfo("Loading Level.");

                    if (actual_level >= DataManager::levels_paths_lenght) {
                        throw new ArgumentOutOfRangeException("Trying to access a level out of range.");
                    }

                    paused = true;

                    // <editor-fold defaultstate="collapsed" desc="Loading Scenario">
                    char path[256];
                    sprintf(path, "Levels/%s", DataManager::levels_paths[actual_level]);
                    TiXmlDocument doc = TiXmlDocument(path);
                    if (doc.LoadFile()) {
                        TiXmlNode *base = doc.FirstChild("level");

                        scenario->Initialize(base->FirstChild("scenario"));
                    } else {
                        char msg[128];
                        sprintf(msg, "Level XML on \"%s\" couldn't be loaded.", path);
                        throw new ContentLoadException(msg);
                    }// </editor-fold>

                    DebugManager::debugInfo("Level Loaded.");
                    game_state = Playing;
                    paused = true;
                }
                // </editor-fold>
                break;
                
            case Playing:
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
        
        if(Mouse::getMouseVisibility()) Mouse::setMouseVisibility(false);
        
        
        switch(game_state) {
            case Lobby:
                break;
                
            case Loading:
                graphicDevice->DeepBufferEnabled(false);
                /*
                spriteBatch->Begin(SpriteBatch::Immediate, BlendState::NonPremultiplied);
                spriteBatch->Draw(loading_tex, RectangleF(0.f, 0.f, 800.f, 600.f), Color::White);
                spriteBatch->DrawString(font.title, "TRON: Racing", Vector2(20.f, 10.f), DataManager::loading_color);
                spriteBatch->DrawString(font.text, "Cargando...", DataManager::loading_pos, DataManager::loading_color);
                spriteBatch->End();
                */
                paused = false;
                break;
                
            case Playing:
                graphicDevice->DeepBufferEnabled(true);
                DebugManager::debugInfo("Drawing Scenario.");
                scenario->Draw(gameTime, DrawColor);
                
                break;
                
            case WaitingNextLVL:
                break;
                
            case GameOver:
                break;
        }
        
        DebugManager::debugInfo("End Drawing.");
        Game::Draw(gameTime);
    }

    virtual void Exit(){
        DebugManager::debugInfo("Calling to Exit().");
        delete scenario;
        
        Game::Exit();
    }
    
    
    // <editor-fold defaultstate="collapsed" desc="Helpers">
    // </editor-fold>

};


int main(int argc, char** argv) {
    DebugManager::debugInfo("Instantiating TronRacing game.");
    TronRacing game;
    DebugManager::debugInfo("Starting Game.");
    game.Start();
    
    return 0;
}