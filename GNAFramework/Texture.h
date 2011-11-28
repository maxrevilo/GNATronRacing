#ifndef TEXTURE_H
#define	TEXTURE_H

#include "SurfaceFormat.h"

namespace GNAFramework {
    class Texture {
    public:
        virtual int getLevelCount() const = 0;
        virtual SurfaceFormat getFormat() const = 0;
        virtual ~Texture() {}
    private:
    protected:
        SurfaceFormat format;
        int LevelCount;
    };
}

#endif	/* TEXTURE_H */

