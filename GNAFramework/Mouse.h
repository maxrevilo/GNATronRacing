#ifndef MOUSE_H
#define	MOUSE_H

#include "SDL/SDL.h"
#include "SDL/SDL_mouse.h"
#include "Geom.h"

#define TOTAL_MOUSEBUTTONS 5


namespace GNAFramework {

    struct MouseState {
        friend class Mouse;
    public:
        /**
         * Indica si el boton indicado esta siendo presionado en el estado actual
         * del mouse.
         * @param button es de tipo Mouse::MouseButton
         */
        bool ButtonPressed(int button);

        bool LeftButton();
        bool MiddleButton();
        bool RightButton();
        bool XButton1();
        bool XButton2();

        /**
         * Retorna la direccion en la que se mueve la rueda del mouse.
         * @return Retorna un valor positivo si se mueve hacia arriba, retorna un
         * valor negativo si se mueve hacia abajo y 0 si no se ha movido, el valor
         * representa la cantidad de movimiento de la rueda desde que se consulto
         * el ultimo MouseState de Mouse.
         */
        int ScrollWheelValue();
        int X();
        int Y();

        bool operator==(const MouseState & other) const;
        bool operator!=(const MouseState & other) const;

    private:
        int x, y, scrrolWheel;
        bool mouseButtonsStatus[TOTAL_MOUSEBUTTONS];
    };

    /**
     * Inerfaz para comunicarse con el Mouse.
     * (usa Singlenton).
     */
    class Mouse {
    public:
        static MouseState getState();

        static void setMouseVisibility(bool toggle);
        static bool getMouseVisibility();

        static void setPosition(Uint16 X, Uint16 Y);
        static void setPosition(int X, int Y);
        static void setPosition(Vector2 pos);

        static WMcursor WindowHandle();

        typedef enum {
            LeftButton = 0,
            MiddleButton = 1,
            RightButton = 2,
            XButton1 = 3,
            XButton2 = 4,
        } MouseButton;
    };
}
#endif	/* MOUSE_H */

