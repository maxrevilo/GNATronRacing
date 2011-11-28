#include "stdio.h"

#include "GNAFramework/Game.h"
#include "GNAFramework/Keyboard.h"
#include "GNAFramework/Geom.h"
#include "GNAFramework/SpriteBatch.h"
#include "GNAFramework/BlendState.h"
#include "GNAFramework/MathHelper.h"
#include "GNAFramework/Mouse.h"
#include "GNAFramework/SpriteFont.h"

#include "InputManager.h"
#include "Logica/Match.h"
#include "DataProvider.h"

using namespace GNAFramework;


class TronRacing : public Game {
public:
    
    //Manejador de entrada:
    InputManager im;
    
    
    enum EstadoJuego {
        Lobby,
        Playing
    } game_state;
    
    Match *match;

    TronRacing(){
        Content->RootDirectory = (char *) "Content/";
    }

    virtual void Initialize() {
        printf("1");
        im.Initialize();
        printf("2");
        
        game_state = Lobby;
        
        DataProvider::loadGameDescription("GAME_CONF.xml");
        printf("3");
        
        
        Game::Initialize();
        printf("4");
        
        GraphicManager::Initialize(this);
        
        printf("5\n");
    }

    
    virtual void Update(GameTime gameTime) {
        im.Update();
        
        if (im.KeyPressed(Keyboard::Escape) 
                || (im.KeyPressed(Keyboard::F4) && im.KeyPressed(Keyboard::Left_Alt)) )
            Exit();
        
        switch(game_state) {
            case Lobby:
                printf("6 Creating Match\n");
                match = new Match(this);
                game_state = Playing;
                printf("7 Match Created\n");
                break;
                
            case Playing:
                printf("8 Updating Match\n");
                match->Update(gameTime);
                printf("9 Updating Created\n");
                break;
        }

        
        Game::Update(gameTime);
    }
    
    
    
    virtual void Draw(GameTime gameTime) {
        graphicDevice->Clear(Color_GNA::Grey);
        
        //if(Mouse::getMouseVisibility())
        //        Mouse::setMouseVisibility(true);
        
        switch(game_state) {
            case Lobby:
                
            break;
            case Playing:
                match->Draw(gameTime);
                break;
        }

        Game::Draw(gameTime);
    }

    virtual void Exit(){
        Game::Exit();
    }


};


int main(int argc, char** argv) {
    TronRacing game;
    printf("0");
    game.Start();
    
    return 0;
}