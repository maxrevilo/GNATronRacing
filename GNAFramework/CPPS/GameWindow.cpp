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
    
    screen = SDL_SetVideoMode(width, height, 0, SDL_OPENGL | SDL_FULLSCREEN | SDL_DOUBLEBUF);
    if(!screen){
        printf("Unable to initialize OpenGL: %s\n", SDL_GetError());
        exit(1);
    }

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

