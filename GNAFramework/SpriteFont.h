#ifndef SPRITEFONT_H
#define	SPRITEFONT_H

#include "Geom.h"

namespace GNAFramework {

    class SpriteFont {
        friend class ContentManager;
        friend class SpriteBatch;
    public:
        char *DefaultCharacter;
        int LineSpacing;
        float Spacing;

        const char *Characters() const;
        
        Vector2 MeasureString(const char *text) const;
        
    private:
        void *font;
        char *characters, style[32];
        int size, spacing, start, end;
        
        SpriteFont();
        virtual ~SpriteFont();
    };
}

#endif	/* SPRITEFONT_H */
