#include <SDL/SDL_video.h>

#include "../GameWindow.h"
#include <string.h>

using namespace GNAFramework;

GameWindow::GameWindow(int width, int height, char *tittle, char *icon_Path) {

    SDL_Surface *icon;
    if(icon = SDL_LoadBMP(icon_Path)){
        SDL_WM_SetIcon(icon, NULL);
    } else {
        printf("SDL could not load the Icon image: %s\n", SDL_GetError());
    }
    
    screen = SDL_SetVideoMode(width, height, 0, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_FULLSCREEN);
    
    if(!screen){
        printf("Unable to initialize OpenGL: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,      8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      32);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,     32);

    SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,    8);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,  1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,  4);
    
    

    this->tittle = NULL;
    SetTittle(tittle);
}

char *GameWindow::GetTittle() const {
    return tittle;
}

void GameWindow::SetTittle(char* tittle) {
    if(this->tittle != NULL)
        delete this->tittle;

    this->tittle = new char[strlen(tittle) + 1];
    strcpy(this->tittle, tittle);

    SDL_WM_SetCaption(this->tittle, NULL);
}

int GameWindow::windowHeight(){
    return screen->h;
}

int GameWindow::windowWidth(){
    return screen->w;
}

GameWindow::~GameWindow() {
    SDL_FreeSurface(screen);
    delete tittle;
}

