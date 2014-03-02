#ifndef TEXTURE2D_H
#define	TEXTURE2D_H

#include "GraphicDevice.h"
#include "Texture.h"
#include "Color.h"
#include "SurfaceFormat.h"


namespace GNAFramework {

    enum TextureAddressMode {
        Wrap = 0,
        Clamp = 1,
        Mirror = 2
    };
    
    enum TextureFilteringMode {
        Nearest = 0,
        Linear = 1,
        Bilinear = 2,
        Trilinear = 3
    };

    class Texture2D : public Texture {
        friend class SpriteBatch;
    public:

        virtual int getLevelCount() const;
        virtual SurfaceFormat getFormat() const;

        int getWidth() const;
        int getHeight() const;
        RectangleF getBounds() const;
        
        void setSamplerAddressUV(TextureAddressMode addressMode);
        void setSamplerAddressU(TextureAddressMode addressMode);
        void setSamplerAddressV(TextureAddressMode addressMode);
        
        void MaxFiltering(TextureFilteringMode mode);
        void MinFiltering(TextureFilteringMode mode);
        
        void UseMipMap(bool mipMap);

        Texture2D(GraphicDevice *graphicsDevice, int width, int height);
        Texture2D(GraphicDevice *graphicsDevice, int width, int height, bool mipMap, SurfaceFormat format);

        static Texture2D *FromStream(GraphicDevice *graphicsDevice, const uint8_t *stream, int width, int height, SurfaceFormat format);

        template<class T> void getData(T *data) const;
        template<class T> void getData(T *data, int startIndex, int elementCount) const;

        template<class T> void setData(const T *data);
        template<class T> void setData(const T *data, int startIndex, int elementCount);

        /**
         * Pone un pixel de color "data" en la posicion (x,y).
         * @param data
         * @param x
         * @param y
         */
        void setPixel(Color data, int x, int y);

        GLuint Pointer() const {
            return pointer;
        };


        /**
         * Colorea una region de la textura de color "data" en el rectangulo definido
         * por "rect".
         * @param data
         * @param rect
         * @return
         */
        void setPixelsRegion(Color data, RectangleF rect);

        virtual ~Texture2D();


    private:
        int width;
        int height;
        GLuint pointer;
        GraphicDevice *graphicDevice;

        static Color RawToARGB(const uint8_t * raw, SurfaceFormat format);
        static void ARGBToRaw(Color color, SurfaceFormat format, uint8_t *raw);

        void init(GraphicDevice *graphicsDevice, int width, int height, bool mipMap, SurfaceFormat format);

    };
}

#endif	/* TEXTURE2D_H */

