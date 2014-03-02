#include "../SpriteBatch.h"
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_video.h>
#include <stdint.h>
#include "../Geom.h"
#include "../SurfaceFormat.h"
#include <stdio.h>

using namespace GNAFramework;

Effect         *SpriteBatch::effect = NULL;
EffectParameter SpriteBatch::color;
EffectParameter SpriteBatch::src;

SpriteBatch::SpriteBatch(GraphicDevice *graphicDevice) {
    this->graphicDevice = graphicDevice;
    begin = false;
    SaveState = false;
    
    if(effect == NULL) {
        effect = new Effect(graphicDevice, "uniform sampler2D src; uniform vec4 color; varying vec2 tc;\n#ifdef VERTEX\nvoid main(void){gl_Position=gl_Vertex;tc=gl_MultiTexCoord0.xy;}\n#endif\n#ifdef FRAGMENT\nvoid main(void){gl_FragColor=texture2D(src,tc)*color;}\n#endif\n");
        color  = effect->getParameter("color");
        src    = effect->getParameter("src");
    }
}

void SpriteBatch::set2DDrawState(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    ViewPort vp = graphicDevice->getViewPort();

    glOrtho(0.0f, vp.width, vp.height, 0.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void SpriteBatch::Draw(const Texture2D *texture, RectangleF rect, Color color) throw(InvalidOperationException *){
    if(!begin) throw new InvalidOperationException(Exception_MSG(Draw_MSG));
    
    Vector4 fColor = color.toVector4();
    glBindTexture(GL_TEXTURE_2D, texture->pointer);
    
    glBegin(GL_QUADS);
        //Bottom-left vertex (corner)
        glColor4f(fColor.X,fColor.Y,fColor.Z, fColor.W);
        glTexCoord2i(0, 0);
        glVertex3f(rect.X, rect.Y, 0.0f);

        //Bottom-right vertex (corner)
        glTexCoord2i(1, 0);
        glVertex3f(rect.X + rect.width, rect.Y, 0.0f);

        //Top-right vertex (corner)
        glTexCoord2i(1, 1);
        glVertex3f(rect.X + rect.width, rect.Y + rect.height, 0.0f);

        //Top-left vertex (corner)
        glTexCoord2i(0, 1);
        glVertex3f(rect.X, rect.Y + rect.height, 0.0f);
    glEnd();
}




void SpriteBatch::DrawFullScreen(const Texture2D *texture, Color color) throw(InvalidOperationException *){
    if(!begin) throw new InvalidOperationException(Exception_MSG(Draw_MSG));
    
    src.SetValue(texture);
    effect->Begin();
    this->color.SetValue(&color);
    
    glBegin(GL_QUADS);
        glTexCoord2i(0, 1);
        glVertex2f(-1, -1);
        
        glTexCoord2i(1, 1);
        glVertex2f(1, -1);
        
        glTexCoord2i(1, 0);
        glVertex2f(1, 1);
        
        glTexCoord2i(0, 0);
        glVertex2f(-1, 1);
    glEnd();
    
    effect->End();
}

void SpriteBatch::DrawFullScreen(Effect *effect) throw(InvalidOperationException *){
    if(!begin) throw new InvalidOperationException(Exception_MSG(Draw_MSG));
    
    effect->Begin();
    
    glBegin(GL_QUADS);
        glTexCoord2i(0, 1);
        glVertex2i(-1, -1);
        
        glTexCoord2i(1, 1);
        glVertex2i(1, -1);
        
        glTexCoord2i(1, 0);
        glVertex2i(1, 1);
        
        glTexCoord2i(0, 0);
        glVertex2i(-1, 1);
    glEnd();
    
    effect->End();
}

void SpriteBatch::DrawFullScreenYInv(const Texture2D *texture, Color color) throw(InvalidOperationException *){
    if(!begin) throw new InvalidOperationException(Exception_MSG(Draw_MSG));
    
    src.SetValue(texture);
    effect->Begin();
    this->color.SetValue(&color);
    
    glBegin(GL_QUADS);
        glTexCoord2i(0, 0);
        glVertex2f(-1, -1);
        
        glTexCoord2i(1, 0);
        glVertex2f(1, -1);
        
        glTexCoord2i(1, 1);
        glVertex2f(1, 1);
        
        glTexCoord2i(0, 1);
        glVertex2f(-1, 1);
    glEnd();
    
    effect->End();
}





void SpriteBatch::Draw(const Texture2D *texture, RectangleF rect,
                                                    float ang, Vector2 center, Color color) throw(InvalidOperationException *){
    if(!begin) new InvalidOperationException(Exception_MSG(Draw_MSG));
    

    Vector2 in;
    Vector4 fColor = color.toVector4();

    center.X += rect.X;
    center.Y += rect.Y;
    
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
                                                    float ang, Vector2 center, Color color) throw(InvalidOperationException *){
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
                                    float rotation, Vector2 origin, Color color)
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

void SpriteBatch::DrawString(const SpriteFont *spriteFont, const char *text, Vector2 position, Color color)
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
