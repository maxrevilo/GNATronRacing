#ifndef SPRITEBATCH_H
#define	SPRITEBATCH_H

#include "SDL/SDL.h"
#include "GL/gl.h"
#include "Geom.h"
#include "BlendState.h"
#include "Game.h"
#include "Texture2D.h"
#include "Color.h"
#include <exception>
#include "GNAExeptions.h"
#include "SpriteFont.h"


namespace GNAFramework {


    class SpriteBatch {
    public:

        enum SortMode {
            BackToFront,
            Deferred,
            FrontToBac,
            Immediate,
            Texture
        };
        
        /**
         * Indica si se restaurará el estado del GraphicDevice despues de ser
         * llamada la funcion End(), es util para cuando se estan dibujando
         * objetos 3D al mismo tiempo que se usa el SpriteBatch, por defecto
         * es False.
         */
        bool SaveState;

        GraphicDevice *graphicDevice;

        SpriteBatch(GraphicDevice *graphicDevice);

        /**
         * Dibuja una textura en pantalla en la posicion y dimenciones indicadas por
         * dest.
         * @param texture Textura a ser dibujada en pantalla
         * @param dest Rectangulo en pantalla donde se dibujara la textura
         * @param color La textura es entintada por este color, para no alterar el
         * color se debe usar blanco Color::White.
         */
        void Draw(const Texture2D *texture, RectangleF dest, Color_GNA color) throw (InvalidOperationException *);

        /**
         * Dibuja una textura en pantalla en la posicion y dimenciones indicadas por
         * dest, ademas el rectangulo dibujado en pantalla es rotado segun el angulo
         * indicado por ang (en radianes) tomando como pivote las coordenadas de
         * center relativas a la esquina superior izquierda del rectangulo.
         * @param texture Textura a ser dibujada en pantalla
         * @param dest Rectangulo en pantalla donde se dibujara la textura
         * @param ang Angulo al que sera rotado el rectangulo en pantalla
         * @param center Pivote de rotacion para el rectangulo en pantalla.
         * @param color La textura es entintada por este color, para no alterar el
         * color se debe usar blanco Color::White.
         */
        void Draw(const Texture2D *texture, RectangleF dest, float rotation, Vector2 origin, Color_GNA color) throw (InvalidOperationException *);

        /**
         * Dibuja una sub seccion de la textura en pantalla en la posicion y
         * dimenciones indicadas por dest, la sub secccion es indicada por src y
         * esta en coordenadas de la textura, ademas el rectangulo dibujado en
         * pantalla es rotado segun el angulo indicado por ang (en radianes) tomando
         * como pivote las coordenadas de center relativas a la esquina superior
         * izquierda del rectangulo.
         * @param texture Textura a ser dibujada en pantalla
         * @param dest Rectangulo en pantalla donde se dibujara la textura
         * @param src Rectangulo que indica la subseccion de la textura que se dibujara.
         * @param ang Angulo al que sera rotado el rectangulo en pantalla
         * @param center Pivote de rotacion para el rectangulo en pantalla.
         * @param color La textura es entintada por este color, para no alterar el
         * color se debe usar blanco Color::White.
         */
        void Draw(const Texture2D *texture, RectangleF dest, RectangleF src, float rotation, Vector2 origin, Color_GNA color) throw (InvalidOperationException *);

        
        void DrawString(const SpriteFont *spriteFont, const char *text, Vector2 position, Color_GNA color) throw (InvalidOperationException *);
        
        void DrawString(const SpriteFont *spriteFont, const char *text, Vector2 position, float rotation, Vector2 origin, Color_GNA color) throw (InvalidOperationException *);


        void Begin() throw (InvalidOperationException *);
        void Begin(SortMode sortMode, BlendState blendState) throw (InvalidOperationException *);

        void End() throw (InvalidOperationException *);


        /**
         * Carga una imagen BMP de discoduro.
         * @param path
         * @return
         */
        //Texture2D *LoadTexture2D(char *path);

        //SpriteBatch(const SpriteBatch& orig);
        virtual ~SpriteBatch();
    private:
        bool begin;
        BlendState SavedBlendState;

        void set2DDrawState();

        enum ExceptionType{
            Begin_MSG,
            Draw_MSG,
            End_MSG
        };

        char * Exception_MSG(ExceptionType type) const;

    };

}
#endif	/* SPRITEBATCH_H */

