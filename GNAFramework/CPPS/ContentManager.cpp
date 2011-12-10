#include "../ContentManager.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "SDL/SDL.h"
#include <SDL/SDL_ttf.h>
#include "../Textures.h"
#include "../SpriteFont.h"
#include "../SpriteBatch.h"
#include "../Effect.h"

#include <iostream>
#include <fstream>



namespace GNAFramework {
    
    ContentManager::ContentManager(char * RootDirectory) {
        this->RootDirectory = RootDirectory;

        texture2D.content = NULL;
        texture2D.next = NULL;

        spriteFont.content = NULL;
        spriteFont.next = NULL;
    }

    char *ContentManager::createFullPath(const char* path) const{
        char * fullPath;

        if (RootDirectory != NULL) {

            int length = strlen(path) + strlen(RootDirectory) + 1;
            fullPath = new char[length];

            strcpy(fullPath, RootDirectory);
            strcat(fullPath, path);

        } else {
            int length = strlen(path) + 1;
            fullPath = new char[length];

            strcpy(fullPath, path);
        }

        return fullPath;
    }

    template <>
    Texture2D *ContentManager::Load<Texture2D>(const char* path) 
        throw (ContentLoadException *, ArgumentNullException *, ObjectDisposedException *)
    {
        Texture2D *result;
        SDL_Surface *surface;
        SurfaceFormat format;
        GLint bpp;
        CntNode *node;

        if(path == NULL)
            throw new ArgumentNullException((char *)"El valor del parametro \"path\" en ContentManager::Load no puede ser nulo");

        char * fullPath = createFullPath(path);
        
        if ((surface = SDL_LoadBMP(fullPath))) {

            delete [] fullPath;

            // get the number of channels in the SDL surface
            bpp = surface->format->BytesPerPixel;
            if (bpp == 4) // contains an alpha channel
            {
                if (surface->format->Rmask == 0x000000ff)
                    format = RGBA;
                else
                    format = BGRA;
            } else if (bpp == 3) // no alpha channel
            {
                if (surface->format->Rmask == 0x000000ff)
                    format = RGB;

                else
                    format = BGR;
            } else {
                SDL_FreeSurface(surface);
                throw new ContentLoadException((char *)"La imagen no es TrueColor, probablemente esta dañada");
                // this error should not go unhandled
            }
            
            result = Texture2D::FromStream(NULL, (uint8_t *) surface->pixels, surface->w, surface->h, format);


            SDL_FreeSurface(surface);

            /*node = texture2D.next;
            texture2D.next = new CntNode();
            texture2D.next->content = (void *) result;
            texture2D.next->next = node;*/

            return result;
        } else {
            delete [] fullPath;
            ContentLoadException *ei = new ContentLoadException(SDL_GetError());
            throw new ContentLoadException((char *)"No se pudo cargar la Texture2D", ei);
        }
    }

    int NextPowerofTwo(int val){
        int result = 1;
        while(result < val){
            result *= 2;
        }
        return result;
    }

    template <>
    SpriteFont *ContentManager::Load<SpriteFont>(const char* path)
        throw (ContentLoadException *, ArgumentNullException *, ObjectDisposedException *)
    {
        
        FILE *file;
        CntNode *node;

        if(path == NULL)
            throw new ArgumentNullException((char *)"El valor del parametro \"path\" en ContentManager::Load no puede ser nulo");

        char * fullPath = createFullPath(path);
        
        if (file = fopen(fullPath, "r")) {

            free(fullPath);

            TTF_Font *font;
            char fontPath[256];
            SpriteFont *result = new SpriteFont();

            try {
                int a;
                a = fscanf(file, "FontName %s ", fontPath);
                a = fscanf(file, "Size %d ", &(result->size));

                a = fscanf(file, "Spacing %d ", &(result->spacing));
                a = fscanf(file, "Style %s ", result->style);

                a = fscanf(file, "Start %d ", &(result->start));
                a = fscanf(file, "End %d ", &(result->end));

            } catch(...) {
                fclose(file);
                throw new ContentLoadException((char *)"Formato de spritefont corrupto");
            }
            
            fclose(file);

            fullPath = createFullPath(fontPath);

            if(font = TTF_OpenFont(fullPath, result->size)){

                result->font = (void *) font;

                node = spriteFont.next;
                spriteFont.next = (CntNode *) malloc(sizeof (CntNode));
                spriteFont.next->content = (void *) result;
                spriteFont.next->next = node;

                return result;
                
            } else {
                free(fullPath);
                ContentLoadException *ei = new ContentLoadException(TTF_GetError());
                ContentLoadException *ei2 = new ContentLoadException((char *)"No se ha podido cargar la Fuente", ei);
                throw new ContentLoadException((char *)"No se pudo cargar el spriteFont", ei2);
            }
        } else {
            free(fullPath);
            throw new ContentLoadException((char *)"No se pudo cargar el spriteFont");
        }
    }
    
    using namespace std;
    template <>
    Effect *ContentManager::Load<Effect>(const char* path) 
        throw (ContentLoadException *, ArgumentNullException *, ObjectDisposedException *)
    {
        Effect *effect = NULL;
        if(path == NULL)
            throw new ArgumentNullException((char *)"El valor del parametro \"path\" en ContentManager::Load no puede ser nulo");
        
        char * fullPath = createFullPath(path);
        ifstream::pos_type size;
        char * memblock = NULL;
        ifstream file (fullPath, ios::in|ios::binary|ios::ate);
        
        if (file.is_open())
        {
            size = file.tellg();
            memblock = new char [size + (ifstream::pos_type) 1];
            file.seekg (0, ios::beg);
            file.read (memblock, size);
            file.close();
            memblock[size] = '\0';
            
            effect = new Effect(NULL, (const char *)memblock);

            delete[] memblock;
            free(fullPath);
            
            return effect;
        } else {
            char buf[128];
            char *msg;
            sprintf(buf,"No se pudo cargar el archivo de Efecto en \"%s\"", fullPath);
            msg = new char[strlen(buf)+1];
            strcpy(msg, buf);
            
            free(fullPath);
            throw new ContentLoadException(msg);
        }
    }

    ContentManager::~ContentManager() {
        /*CntNode *actual;
        CntNode *toFree;
        Texture2D *texture2Daux;

        actual = texture2D.next;

        while (actual != NULL) {
            texture2Daux = (Texture2D *) actual->content;
            delete texture2Daux;

            toFree = actual;
            actual = actual->next;
            free(toFree);
        }

        
        SpriteBatch *spriteBatchaux;
        actual = spriteFont.next;

        while (actual != NULL) {

            spriteBatchaux = (SpriteBatch *) actual->content;
            delete  spriteBatchaux;

            toFree = actual;
            actual = actual->next;
            free(toFree);
        }*/

    }
}
