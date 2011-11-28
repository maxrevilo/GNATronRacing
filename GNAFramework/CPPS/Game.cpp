#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include "../Game.h"
#include "../GNAExeptions.h"
#include <stdio.h>

using namespace GNAFramework;

void Game::catchGNAException(GNAException *e) const throw(GNAException){
    char *str;
    perror(str = e->ExceptionTrace());
    delete [] str;
    fflush(stderr);

    throw e;
}

Game::Game() {
    try {

        fps = 100;
        graphicDevice = NULL;

        Content = new ContentManager(NULL);
        active = false;

        

    } catch (GNAException *e) {

        catchGNAException(e);

    }
}

void Game::Start() {
    try {

        active = true;

        begin.setTo(clock());
        actual = begin;

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            ExecutionEngineException *ei = new ExecutionEngineException(SDL_GetError());
            throw new ExecutionEngineException((char *)"No se pudo inicializar SDL", ei);
        }

        if (TTF_Init() != 0)
        {
          ExecutionEngineException *ei = new ExecutionEngineException(TTF_GetError());
          throw new ExecutionEngineException((char *)"No se pudo inicializar SDL_ttf", ei);
        }

        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        graphicDevice = new GraphicDevice(800, 600, false);

        Mouse::setMouseVisibility(false);

        GameLoop();

    } catch (EndGameException *ege) {
        printf("Game ended\n");
        fflush(stdout);
        delete ege;
        // Terminar el juego.
        
    } catch (GNAException *e) {
        catchGNAException(e);
        
    } catch (GNAException e) {
        catchGNAException(&e);
    }
}

void Game::Initialize() {
}

void Game::Draw(GameTime gameTime) {
}

void Game::Update(GameTime gameTime) {
}

void Game::Exit() {

    active = false;

    if (Content) delete Content;
    if (graphicDevice) delete graphicDevice;

    SDL_Quit();
    TTF_Quit();

    throw new EndGameException();
}

//Game::Game(const Game& orig) {}

Game::~Game() {

}

//Private:

void Game::GameLoop() {
    GameTime gameTime;
    clock_t quantum = (int) ( CLOCKS_PER_SEC / ((double)fps) );
    clock_t last = clock();;
    clock_t now;
    
    Initialize();
    
    //printf("Quantum: %d clocks\n", quantum);
    
    gameTime.totalTime.setTo(clock());
    
    while (active) {
        //Actualizar tiempo:
        //printf("Past   %.40f\n", actual.toMillisecondsF());
        now = clock();
        //printf("Tiempo: %d clocks\n", now);
        while ((now - last) < quantum) {
            //printf("Wait %.40f\n", (aux-actual).toMillisecondsF());
            //printf("Espera: (%d-%d)=%d clocks\n", now, last, (now - last));
            now = clock();
        }
        //printf("Actual %.40f\n", aux.toMillisecondsF());
        gameTime.elapsedTime = TimeSpan(now - last);
        gameTime.totalTime   = TimeSpan(now);
        last = clock();
        //printf("Dif    %.40f\n", gameTime.elapsedTime.toMillisecondsF());

        //En caso de ser lanzada una exception de final de juego:


        //Update Call:
        Update(gameTime);

        //Draw Call:
        Draw(gameTime);

        DrawCall();
    }
}

void Game::DrawCall() {
    //Refrescar la pantalla:
    SDL_GL_SwapBuffers();
}