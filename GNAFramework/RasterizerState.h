#ifndef RASTERIZERSTATE_H
#define	RASTERIZERSTATE_H

#include "GL/gl.h"

namespace GNAFramework {

    class RasterizerState {
    public:
        const static RasterizerState CullClockwise;
        const static RasterizerState CullCounterClockwise;
        const static RasterizerState CullNone;
        
        enum CullMode {
            CullClockwiseFace        = GL_BACK, 
            CullCounterClockwiseFace = GL_FRONT,
            None                     = GL_FRONT_AND_BACK
        };

        enum FillMode {
            Point     = GL_POINT,
            Solid     = GL_FILL,
            WireFrame = GL_LINE
        };
        
        RasterizerState();
        RasterizerState(CullMode cullMode, FillMode fillMode);

        CullMode cullMode;
        FillMode fillMode;
        
        bool operator==(const RasterizerState &other) const;
        bool operator!=(const RasterizerState &other) const;

        virtual ~RasterizerState();
    private:

    };
    
}

#endif	/* RASTERIZERSTATE_H */

