#include "../SpriteBatch.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>
#include <stdint.h>
#include "../Geom.h"
#include "../SurfaceFormat.h"
#include <stdio.h>

using namespace GNAFramework;

SpriteBatch::SpriteBatch(GraphicDevice *graphicDevice) {
    this->graphicDevice = graphicDevice;
    begin = false;
    SaveState = false;
}

void SpriteBatch::set2DDrawState(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    ViewPort vp = graphicDevice->getViewPort();

    glOrtho(0.0f, vp.width, vp.height, 0.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpriteBatch::Draw(const Texture2D *texture, RectangleF rect, Color_GNA color) throw(InvalidOperationException *){

    Vector4 fColor = color.toVector4();

    if(!begin) throw new InvalidOperationException(Exception_MSG(Draw_MSG));
    
    // Bind the texture to which subsequent calls refer to
    glBindTexture(GL_TEXTURE_2D, texture->pointer);

    glBegin(GL_QUADS);
        //Bottom-left vertex (corner)
        glColor4f(fColor.X,fColor.Y,fColor.Z, fColor.W);
        glTexCoord2i(0, 0);
        glVertex3f(rect.X, rect.Y, 0.0f);

        //Bottom-right vertex (corner)
        glTexCoord2i(1, 0);
        glVertex3f(rect.X + rect.width, rect.Y, 0.f);

        //Top-right vertex (corner)
        glTexCoord2i(1, 1);
        glVertex3f(rect.X + rect.width, rect.Y + rect.height, 0.f);

        //Top-left vertex (corner)
        glTexCoord2i(0, 1);
        glVertex3f(rect.X, rect.Y + rect.height, 0.f);
    glEnd();
}

void SpriteBatch::Draw(const Texture2D *texture, RectangleF rect,
                                                    float ang, Vector2 center, Color_GNA color) throw(InvalidOperationException *){
    if(!begin) new InvalidOperationException(Exception_MSG(Draw_MSG));
    

    Vector2 in;
    Vector4 fColor = color.toVector4();

    center.X += rect.X;
    center.Y += rect.Y;


    // Bind the texture to which subsequent calls refer to
    glBindTexture(GL_TEXTURE_2D, texture->pointer);

    glBegin(GL_QUADS);
        //Bottom-left vertex (corner)
        glColor4f(fColor.X,fColor.Y,fColor.Z, fColor.W);
        in.setVal(rect.X, rect.Y);
        in.rotate(center, ang);

        glTexCoord2i(0, 0);
        glVertex3f(in.X, in.Y, 0.0f);

        //Bottom-right vertex (corner)
        in.setVal(rect.X + rect.width, rect.Y);
        in.rotate(center, ang);

        glTexCoord2i(1, 0);
        glVertex3f(in.X, in.Y, 0.0f);

        //Top-right vertex (corner)
        in.setVal(rect.X + rect.width, rect.Y + rect.height);
        in.rotate(center, ang);

        glTexCoord2i(1, 1);
        glVertex3f(in.X, in.Y, 0.0f);

        //Top-left vertex (corner)
        in.setVal(rect.X, rect.Y + rect.height);
        in.rotate(center, ang);

        glTexCoord2i(0, 1);
        glVertex3f(in.X, in.Y, 0.0f);
        
    glEnd();
}

void SpriteBatch::Draw(const Texture2D *texture, RectangleF dest, RectangleF src,
                                                    float ang, Vector2 center, Color_GNA color) throw(InvalidOperationException *){
    if(!begin) throw new InvalidOperationException(Exception_MSG(Draw_MSG));

    src.X /= texture->getWidth();
    src.Y /= texture->getHeight();
    src.width /= texture->getWidth();
    src.height /= texture->getHeight();

    printf("Src: (X:%f, Y:%f, W:%f, H:%f)\n", src.X, src.Y, src.width, src.height);


    Vector2 in;
    Vector4 fColor = color.toVector4();

    center.X += dest.X;
    center.Y += dest.Y;


    // Bind the texture to which subsequent calls refer to
    glBindTexture(GL_TEXTURE_2D, texture->pointer);

    glBegin(GL_QUADS);
        glColor4f(fColor.X,fColor.Y,fColor.Z, fColor.W);
        
        //Bottom-left vertex (corner)
        in.setVal(dest.X, dest.Y);
        in.rotate(center, ang);

        glTexCoord2f(src.X, src.Y);
        glVertex3f(in.X, in.Y, 0.0f);

        //Bottom-right vertex (corner)
        in.setVal(dest.X + dest.width, dest.Y);
        in.rotate(center, ang);

        glTexCoord2f(src.X + src.width, src.Y);
        glVertex3f(in.X, in.Y, 0.0f);

        //Top-right vertex (corner)
        in.setVal(dest.X + dest.width, dest.Y + dest.height);
        in.rotate(center, ang);

        glTexCoord2f(src.X + src.width, src.Y + src.height);
        glVertex3f(in.X, in.Y, 0.0f);

        //Top-left vertex (corner)
        in.setVal(dest.X, dest.Y + dest.height);
        in.rotate(center, ang);

        glTexCoord2f(src.X, src.Y + src.height);
        glVertex3f(in.X, in.Y, 0.0f);

    glEnd();
}

void SpriteBatch::DrawString(const SpriteFont *spriteFont, const char *text, Vector2 position,
                                    float rotation, Vector2 origin, Color_GNA color)
throw (InvalidOperationException *)
{
    SDL_Color sdl_color = {255,255,255};
    SDL_Surface *resulting_text = TTF_RenderText_Blended((TTF_Font *) spriteFont->font, text, sdl_color);
    Texture2D *texture = Texture2D::FromStream(NULL, (uint8_t *) resulting_text->pixels, resulting_text->w, resulting_text->h, RGBA);


    RectangleF r(position.X, position.Y, (float) resulting_text->w, (float) resulting_text->h);

    Draw(texture, r, rotation, origin, color);

    SDL_FreeSurface(resulting_text);
    delete texture;
}

void SpriteBatch::DrawString(const SpriteFont *spriteFont, const char *text, Vector2 position, Color_GNA color)
throw (InvalidOperationException *)
{
    SDL_Color sdl_color = {255,255,255};
    SDL_Surface *resulting_text = TTF_RenderText_Blended((TTF_Font *) spriteFont->font, text, sdl_color);
    Texture2D *texture = Texture2D::FromStream(NULL, (uint8_t *) resulting_text->pixels, resulting_text->w, resulting_text->h, RGBA);
    
    RectangleF r(position.X, position.Y, (float) resulting_text->w, (float) resulting_text->h);

    Draw(texture, r, color);
    
    SDL_FreeSurface(resulting_text);
    delete texture;
}


void SpriteBatch::Begin() throw(InvalidOperationException *){
    if(begin) throw new InvalidOperationException(Exception_MSG(Begin_MSG));
    
    set2DDrawState();
    
    begin = true;
}

void SpriteBatch::Begin(SortMode sortMode, BlendState blendState) throw(InvalidOperationException *){
    if(begin) throw new InvalidOperationException(Exception_MSG(Begin_MSG));

    switch(sortMode){
        case Immediate:
            break;
        default:
            perror("SpriteBatch: El unico SortMode soportado es Inmediate.");
            exit(1);
            break;
    }

    set2DDrawState();

    SavedBlendState = graphicDevice->getBlendState();
    graphicDevice->setBlendState(blendState);
    
    begin = true;
}

void SpriteBatch::End() throw(InvalidOperationException *){
    if(!begin) throw new InvalidOperationException(Exception_MSG(End_MSG));

    begin = false;

    if(SaveState){
        graphicDevice->setBlendState(SavedBlendState);
    }
}

// <editor-fold defaultstate="collapsed" desc="Deprecated:">
/*Texture2D *SpriteBatch::LoadTexture2D(char *path) {
    Texture2D *result;
    GLuint texture; // This is a handle to our texture object
    SDL_Surface *surface; // This surface will tell us the details of the image
    SurfaceFormat texture_format;
    GLint nOfColors;

    if ((surface = SDL_LoadBMP(path))) {

        // Check that the image's width is a power of 2
        if ((surface->w & (surface->w - 1)) != 0) {
            printf("warning: image.bmp's width is not a power of 2\n");
        }

        // Also check if the height is a power of 2
        if ((surface->h & (surface->h - 1)) != 0) {
            printf("warning: image.bmp's height is not a power of 2\n");
        }

        // get the number of channels in the SDL surface
        nOfColors = surface->format->BytesPerPixel;
        if (nOfColors == 4) // contains an alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = RGBA;
            else
                texture_format = BGRA;
        } else if (nOfColors == 3) // no alpha channel
        {
            if (surface->format->Rmask == 0x000000ff)
                texture_format = RGB;
            else
                texture_format = BGR;
        } else {
            printf("warning: the image is not truecolor..  this will probably break\n");
            // this error should not go unhandled
        }

        // Have OpenGL generate a texture object handle for us
        glGenTextures(1, &texture);

        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set the texture's stretching properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D(GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
                texture_format, GL_UNSIGNED_BYTE, surface->pixels);
    } else {
        printf("SDL could not load image.bmp: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    // Free the SDL_Surface only if it was successfully created
    if (surface) {
        SDL_FreeSurface(surface);
    }

    result = new Texture2D(texture, surface->w, surface->h, 0, texture_format);

    return result;

}*/// </editor-fold>

char* SpriteBatch::Exception_MSG(ExceptionType type) const{
    switch(type){
        case Draw_MSG:
            return (char *)"No se puede llamar a la funcion DrawTexture sin llamar Begin.";
            break;
        case Begin_MSG:
            return (char *)"No se puede llamar a la funcion Begin dos veces seguidas antes de llamar a End.";
            break;
        case End_MSG:
            return (char *)"No se puede llamar a la funcion End antes de haber llamado la funcion Begin o llamar End dos veces seguidas.";
            break;
    }
}

SpriteBatch::~SpriteBatch() { }
