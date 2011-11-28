#ifndef GAMEWINDOW_H
#define	GAMEWINDOW_H

#include "SDL/SDL.h"

namespace GNAFramework {

    class GameWindow {
    public:
        /**
         * Se crea la ventana del juego.
         * @param width Ancho de la ventana.
         * @param height Alto de la ventana.
         * @param tittle El string es copiado para ser mostrado como el nombre de la
         * ventana, por lo tanto la variable debe ser liberada al ser usada.
         * @param icon_Path Indica la direccion en discoduro donde se encuentra el
         * archibo .bmp que contiene el icono.
         */
        GameWindow(int width, int height, char *tittle, char *icon_Path);


        /**
         * Se obtiene una copia del texto del titulo de la ventana.
         */
        char* GetTittle() const;

        /**
         * Se reemplaza el titulo anterior de la ventana por el indicado.
         */
        void SetTittle(char* tittle);

        int windowWidth();
        int windowHeight();

        virtual ~GameWindow();

    private:
        char * tittle;
        SDL_Surface *screen;
    };

}

#endif	/* GAMEWINDOW_H */

