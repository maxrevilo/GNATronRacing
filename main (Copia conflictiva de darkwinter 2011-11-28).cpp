#include "stdio.h"

#include "GNAFramework/Game.h"
#include "GNAFramework/Keyboard.h"
#include "GNAFramework/Geom.h"
#include "GNAFramework/SpriteBatch.h"
#include "GNAFramework/BlendState.h"
#include "GNAFramework/MathHelper.h"
#include "GNAFramework/Mouse.h"
#include "GNAFramework/SpriteFont.h"

#include "Managers/InputManager.h"
#include "Managers/DataProvider.h"
#include "Managers/DebugManager.h"


using namespace GNAFramework;


class TronRacing : public Game {
public:
    
    InputManager im;
    
    
    enum EstadoJuego {
        Lobby,
        Loading,
        Playing,
        WaitingNextLVL,
        GameOver
    } game_state;

    TronRacing(){
        Content->RootDirectory = (char *) "Content/";
    }

    virtual void Initialize() {
        DebugManager::debugInfo("Initialazing ImputManager.");
        im.Initialize();
        
        DebugManager::debugInfo("Loading GAME_CONF.xml.");
        DataManager::loadGameDescription("GAME_CONF.xml");
        
        
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
                break;
                
            case Playing:
                break;
        }

        
        Game::Update(gameTime);
    }
    
    
    
    virtual void Draw(GameTime gameTime) {
        graphicDevice->Clear(Color_GNA::Grey);
        
        switch(game_state) {
            case Lobby:
                break;
            case Playing:
                break;
        }

        Game::Draw(gameTime);
    }

    virtual void Exit(){
        DebugManager::debugInfo("Calling to Exit().");
        Game::Exit();
    }


};


int main(int argc, char** argv) {
    DebugManager::debugInfo("Instantiating TronRacing game.");
    TronRacing game;
    DebugManager::debugInfo("Starting Game.");
    game.Start();
    
    return 0;
}