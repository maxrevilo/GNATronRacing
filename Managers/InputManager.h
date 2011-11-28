#ifndef INPUTMANAGER_H
#define	INPUTMANAGER_H

#include "../GNAFramework/Keyboard.h"
#include "../GNAFramework/Mouse.h"


using namespace GNAFramework;

class InputManager {
    public:
        InputManager();
        
        void Initialize();

        void Update();

        bool KeyPressed(int key);

        bool KeyGetPressed(int key);

        virtual ~InputManager();
    private:
        KeyboardState kbs;
        KeyboardState kbs_prev;

        MouseState    mss;
        MouseState    mss_prev;
};

#endif	/* INPUTMANAGER_H */

