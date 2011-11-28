#include "../SpriteFont.h"
#include "../SpriteBatch.h"
#include "../MathHelper.h"
#include <stdlib.h>

#include <SDL/SDL_ttf.h>



using namespace GNAFramework;

SpriteFont::SpriteFont() {
    font = NULL;
}

const char *SpriteFont::Characters() const{
    return characters;
}

Vector2 SpriteFont::MeasureString(const char* text) const{
    Vector2 result;
    int w, h;
    TTF_SizeText((TTF_Font *)font, text, &w, &h);
    result.setVal(w,h);
    return result;
}

SpriteFont::~SpriteFont() {
    if(font) TTF_CloseFont((TTF_Font *) font);
    if(style) free(style);
}

