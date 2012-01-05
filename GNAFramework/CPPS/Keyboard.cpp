#include "../Keyboard.h"
 
using namespace GNAFramework;

// <editor-fold defaultstate="collapsed" desc="Keyboard:">
bool Keyboard::initialized = false;
int Keyboard::NumKeyPressed = 0;
bool Keyboard::keysStatus[TOTAL_KEYS];

KeyboardState Keyboard::getState() {

    SDL_Event keyevent;
    KeyboardState result;
    int i;

    if (!initialized) {
        initialized = true;
        for (i = 0; i < TOTAL_KEYS; i++)
            keysStatus[i] = false;
    }

    while (SDL_PollEvent(&keyevent))
    {
        switch (keyevent.type) {
            case SDL_KEYDOWN:
                keysStatus[keyevent.key.keysym.sym] = true;
                NumKeyPressed++;
                break;
            case SDL_KEYUP:
                keysStatus[keyevent.key.keysym.sym] = false;
                NumKeyPressed--;
                break;
            default:
                break;
        }
    }

    if(NumKeyPressed<0){
        perror("Keyboard Manager Anomality, NumKeyPressed is lower than 0.");
        NumKeyPressed = 0;
    }

    for (i = 0; i < TOTAL_KEYS; i++) {
        result.KeysPressed[i] = keysStatus[i];
    }

    result.NumberOfKeysPressed = NumKeyPressed;

    return result;
}


// </editor-fold>



// <editor-fold defaultstate="collapsed" desc="KeyboardState:">


bool KeyboardState::IsKeyDown(int key) {
    return KeysPressed[key];
}


bool KeyboardState::IsKeyUp(int key) {
    return !KeysPressed[key];
}

bool KeyboardState::IsAnyKeyDown(){
    return NumberOfKeysPressed > 0;
}

int KeyboardState::NumberOfKeysDown(){
    return NumberOfKeysPressed;
}

bool KeyboardState::operator ==(const KeyboardState& other) const {
    int i;
    for (i = 0; i < TOTAL_KEYS; i++) {
        if (other.KeysPressed[i] != KeysPressed[i]) {
            return false;
        }
    }
    return true;
}

bool KeyboardState::operator !=(const KeyboardState& other) const {
    int i;
    for (i = 0; i < TOTAL_KEYS; i++) {
        if (other.KeysPressed[i] != KeysPressed[i]) {
            return true;
        }
    }
    return false;
}

// </editor-fold>
