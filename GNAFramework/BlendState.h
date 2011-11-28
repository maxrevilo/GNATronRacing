#ifndef BLENDSTATE_H
#define	BLENDSTATE_H

#include "GL/gl.h"
#include <SDL/SDL_opengl.h>

namespace GNAFramework {

    class BlendState {
    public:

        enum Blend {
            Zero = GL_ZERO,
            One = GL_ONE,
            SourceColor = GL_SRC_COLOR,
            InverseSourceColor = GL_ONE_MINUS_SRC_COLOR,
            SourceAlpha = GL_SRC_ALPHA,
            InverseSourceAlpha = GL_ONE_MINUS_SRC_ALPHA,
            DestinationAlpha = GL_DST_ALPHA,
            InverseDestinationAlpha = GL_ONE_MINUS_DST_ALPHA,
            DestinationColor = GL_DST_COLOR,
            InverseDestinationColor = GL_ONE_MINUS_DST_COLOR,
            SourceAlphaSaturation = GL_SRC_ALPHA_SATURATE,
            //BlendFactor,
            //InverseBlendFactor
        };

        enum BlendFunction {
            Add = GL_FUNC_ADD,
            Max = GL_MAX,
            Min = GL_MIN,
            ReverseSubtract = GL_FUNC_REVERSE_SUBTRACT,
            Subtract = GL_FUNC_SUBTRACT
        };



        const static BlendState Additive;
        const static BlendState AlphaBlend;
        const static BlendState NonPremultiplied;
        const static BlendState Opaque;


        Blend SourceBlend;
        Blend DestinationBlend;
        BlendFunction ColorBlendFunction;


        BlendState();

        BlendState(Blend SourceBlend, Blend DestinationBlend, BlendFunction ColorBlendFunction);

        bool operator==(const BlendState &other) const;
        bool operator!=(const BlendState &other) const;

        //BlendState(const BlendState& orig);
        //virtual ~BlendState();
    private:

    };
}

#endif	/* BLENDSTATE_H */

