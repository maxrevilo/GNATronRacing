#include "../Texture2D.h"
#include "GL/gl.h"
#include "../Geom.h"
#include <stdint.h>
#include <stdlib.h>
#include <SDL/SDL_opengl.h>
#include "../GraphicDevice.h"
#include "../SpriteBatch.h"
#include "AdaptacionesDeFunciones.h"


namespace GNAFramework {
    // <editor-fold defaultstate="collapsed" desc="Texture2D:">

    void Texture2D::init(GraphicDevice *graphicsDevice, int width, int height, bool mipMap, SurfaceFormat format){
        this->height = height;
        this->width = width;
        this->LevelCount = 1;
        this->format = format;
        this->graphicDevice = graphicsDevice;
        int bpp =  ((format == RGBA) || (format == BGRA)) ? 4 : 3;

        // Have OpenGL generate a texture object handle for us
        glGenTextures(1, &pointer);

        // Bind the texture object
        glActiveTextureARB(GL_TEXTURE0_ARB);
                
        glBindTexture(GL_TEXTURE_2D, pointer);

        glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
        
        // Set the texture's stretching properties
        if(mipMap){
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        }
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
        
        
        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D(GL_TEXTURE_2D, 0, bpp, width, height, 0, format, GL_UNSIGNED_BYTE, NULL);
    }

    Texture2D::Texture2D(GraphicDevice *graphicsDevice, int width, int height) {
        init(graphicsDevice, width, height, true, RGBA);
    }

    Texture2D::Texture2D(GraphicDevice *graphicsDevice, int width, int height, bool mipMap, SurfaceFormat format) {
        init(graphicsDevice, width, height, mipMap, format);
    }
    
    void Texture2D::UseMipMap(bool mipMap) {
            glActiveTextureARB(GL_TEXTURE0_ARB);
            
            glBindTexture(GL_TEXTURE_2D, pointer);
            
            // Set the texture's stretching properties
            if(mipMap){
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            } else {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            }
        }
    
    Texture2D *Texture2D::FromStream(GraphicDevice *graphicsDevice, const uint8_t *stream, int width, int height, SurfaceFormat format) {
        Texture2D *result = new Texture2D(graphicsDevice, width, height, true, format);
        
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glBindTexture(GL_TEXTURE_2D, result->pointer);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, (const GLvoid *) stream);

        return result;
    }

    Color Texture2D::RawToARGB(const uint8_t * raw, SurfaceFormat format) {
        Color result;

        switch (format) {
            case RGB:
                result.setValue(255, raw[0], raw[1], raw[2]);
                break;
            case RGBA:
                result.setValue(raw[3], raw[0], raw[1], raw[2]);
                break;
            case BGR:
                result.setValue(255, raw[2], raw[1], raw[0]);
                break;
            case BGRA:
                result.setValue(raw[3], raw[2], raw[1], raw[0]);
                break;
        }

        return result;
    }

    void Texture2D::ARGBToRaw(Color color, SurfaceFormat format, uint8_t *raw) {
        switch (format) {
            case RGB:
                raw[0] = color.R;
                raw[1] = color.G;
                raw[2] = color.B;
                raw[3] = 255;
                break;
            case RGBA:
                raw[0] = color.R;
                raw[1] = color.G;
                raw[2] = color.B;
                raw[3] = color.A;
                break;
            case BGR:
                raw[0] = color.B;
                raw[1] = color.G;
                raw[2] = color.R;
                raw[3] = 255;
                break;
            case BGRA:
                raw[0] = color.B;
                raw[1] = color.G;
                raw[2] = color.R;
                raw[3] = color.A;
                break;
        }
    }

    template <>
    void Texture2D::setData<uint8_t>(const uint8_t *data) {
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glBindTexture(GL_TEXTURE_2D, pointer);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, format, GL_UNSIGNED_BYTE, (const GLvoid *) data);
    }

    template <>
    void Texture2D::setData<Color>(const Color *data) {
        int size = width * height;
        int i;
        uint8_t *raw = (uint8_t *) malloc(sizeof(uint8_t) * size * 4);

        for(i = 0; i < size; i++){
            ARGBToRaw(data[i], format, raw + 4*i);
        }

        setData<uint8_t>(raw);

        free(raw);
    }



    template <>
    void Texture2D::getData<uint8_t>(uint8_t *data, int startIndex, int elementCount) const{
        if (startIndex * elementCount < 0) {
            //perror("InalidArgument: los indices deben ser mayores a 0");
            exit(1);
        }

        int size = height * width * 4;

        if (elementCount + startIndex > size) {
            //perror("InalidArgument: fuera de los indices de la textura");
            exit(1);
        }
        
        uint8_t *raw  = new uint8_t[size];
        uint8_t *rawI = raw;
        
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glBindTexture(GL_TEXTURE_2D, this->pointer);
        glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, raw);

        for (rawI += startIndex; elementCount > 0; *data++ = *rawI++, elementCount--);

        delete [] raw;
    }

    template <>
    void Texture2D::getData<uint8_t>(uint8_t *data) const{
        getData<uint8_t > (data, 0, height * width * 4);
    }

    template <>
    void Texture2D::getData<Color>(Color *data, int startIndex, int elementCount) const{
        if (startIndex * elementCount < 0) {
            //perror("InalidArgument: los indices deben ser mayores a 0");
            exit(1);
        }

        int size = height * width;

        if (elementCount + startIndex > size) {
            //perror("InalidArgument: fuera de los indices de la textura");
            exit(1);
        }
        
        uint8_t *raw = new uint8_t[4 * size];
        uint8_t *rawI = raw;
        
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glBindTexture(GL_TEXTURE_2D, this->pointer);
        glGetTexImage(GL_TEXTURE_2D, 0, format, GL_UNSIGNED_BYTE, raw);

        for (rawI += 4 * startIndex; elementCount > 0; elementCount--) {
            *data++ = RawToARGB(rawI, format);
            rawI += 4;
        }

        rawI = raw + 4 * startIndex;

        delete [] raw;
    }

    template <>
    void Texture2D::getData<Color>(Color *data) const{
        getData<Color> (data, 0, height * width);
    }




    SurfaceFormat Texture2D::getFormat() const{
        return format;
    }

    int Texture2D::getLevelCount() const{
        return LevelCount;
    }

    int Texture2D::getWidth() const{
        return width;
    }

    int Texture2D::getHeight() const{
        return height;
    }

    RectangleF Texture2D::getBounds() const{
        RectangleF rect;
        rect.setValue(0.f, 0.f, width, height);
        return rect;
    }

    //Texture2D::setData(Color* data){}

    void Texture2D::setPixel(Color data, int x, int y) {
        Color *colPointer = &data;
        uint8_t *dataIn = (uint8_t *) colPointer;

        glBindTexture(GL_TEXTURE_2D, this->pointer);
        glTexSubImage2D(GL_TEXTURE_2D,
                0,
                x,
                y,
                1,
                1,
                (GLuint) format,
                GL_UNSIGNED_BYTE,
                dataIn);
    }

    void Texture2D::setPixelsRegion(Color data, RectangleF rect) {
        Color *colPointer = &data;
        uint8_t *dataIn = (uint8_t *) colPointer;

        glBindTexture(GL_TEXTURE_2D, this->pointer);
        glTexSubImage2D(GL_TEXTURE_2D,
                0,
                rect.X,
                rect.Y,
                rect.width,
                rect.height,
                (GLuint) format,
                GL_UNSIGNED_BYTE,
                dataIn);
    }

    //Texture::Textures(const Texture& orig) {}

    Texture2D::~Texture2D() {
        glDeleteTextures(1, &pointer);
    }

    // </editor-fold>

}