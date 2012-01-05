#ifndef KEYBOARD_H
#define	KEYBOARD_H

#define TOTAL_KEYS 323

#include "SDL/SDL.h"
#include "SDL/SDL_keysym.h"

namespace GNAFramework {

    class Keyboard;

    struct KeyboardState {
        friend class Keyboard;
    public:

        //list<Keyboard::Key, ...> GetPressedKeys();


        /**
         * Indica si alguna tecla esta siendo presionada en el estado actual del
         * teclado.
         */
        bool IsAnyKeyDown();

        /**
         * Indica el numero de teclas presionadas en el estado actual del teclado.
         */
        int NumberOfKeysDown();

        /**
         * Indica si la tecla "key" esta siendo presionada en el estado actual del
         * teclado.
         * @param key es de tipo Keyboard::Key
         */
        bool IsKeyDown(int key);
        /**
         * Indica si la tecla "key" no esta siendo presionada en el estado actual
         * del teclado.
         * @param key es de tipo Keyboard::Key
         */
        bool IsKeyUp(int key);

        bool operator==(const KeyboardState & other) const;
        bool operator!=(const KeyboardState & other) const;

    private:
        bool KeysPressed[TOTAL_KEYS];
        int NumberOfKeysPressed;
    };

    /**
     * Inerfaz para comunicarse con el teclado.
     * (usa Singlenton).
     */
    class Keyboard {
    public:
        static KeyboardState getState();

        typedef enum {
            Q = SDLK_q, W = SDLK_w, E = SDLK_e, R = SDLK_r, T= SDLK_t, Y = SDLK_y, U = SDLK_u, I = SDLK_i, O = SDLK_o, P = SDLK_p, A = SDLK_a, S = SDLK_s, D = SDLK_d, F = SDLK_f, G = SDLK_g, H = SDLK_h, J = SDLK_j, K = SDLK_k, L = SDLK_l, Z = SDLK_z, X = SDLK_x, C = SDLK_c, V = SDLK_v, B = SDLK_b, N = SDLK_n, M = SDLK_m,
            Rigth_Control = SDLK_RCTRL, Rigth_Alt = SDLK_RALT, Rigth_Shift = SDLK_RSHIFT,
            Left_Control = SDLK_LCTRL, Left_Alt = SDLK_LALT, Left_Shift = SDLK_LSHIFT,
            Up = SDLK_UP, Down = SDLK_DOWN, Right = SDLK_RIGHT, Left = SDLK_LEFT,
            Tab = SDLK_TAB, Escape = SDLK_ESCAPE, Enter = SDLK_RETURN, Space = SDLK_SPACE,
            F1 = SDLK_F1, F2 = SDLK_F2, F3 = SDLK_F3, F4 = SDLK_F4, F5 = SDLK_F5, F6 = SDLK_F6,
            F7 = SDLK_F7, F8 = SDLK_F8, F9 = SDLK_F9, F10 = SDLK_F10, F11 = SDLK_F11, F12 = SDLK_F12,
                    Unkown = SDLK_UNKNOWN
        } Key;

    private:
        static bool initialized;
        static bool keysStatus[TOTAL_KEYS];
        static int NumKeyPressed;
    };

}

#endif	/* KEYBOARD_H */

