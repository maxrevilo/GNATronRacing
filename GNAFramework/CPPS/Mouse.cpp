#include "../Mouse.h"
#include "../Geom.h"

using namespace GNAFramework;


// <editor-fold defaultstate="collapsed" desc="Mouse:">

MouseState Mouse::getState() {

    Uint8 ButtonMask;
    MouseState result;

    ButtonMask = SDL_GetMouseState(&(result.x), &(result.y));

    result.mouseButtonsStatus[Mouse::LeftButton] = (SDL_BUTTON_LMASK & ButtonMask) != 0;
    result.mouseButtonsStatus[Mouse::MiddleButton] = (SDL_BUTTON_MMASK & ButtonMask) != 0;
    result.mouseButtonsStatus[Mouse::RightButton] = (SDL_BUTTON_RMASK & ButtonMask) != 0;
    result.mouseButtonsStatus[Mouse::XButton1] = (SDL_BUTTON_X1MASK & ButtonMask) != 0;
    result.mouseButtonsStatus[Mouse::XButton2] = (SDL_BUTTON_X2MASK & ButtonMask) != 0;

    if ((ButtonMask & SDL_BUTTON(SDL_BUTTON_WHEELUP)) != 0) {
        result.scrrolWheel = 1;
    } else if ((ButtonMask & SDL_BUTTON(SDL_BUTTON_WHEELDOWN)) != 0) {
        result.scrrolWheel = -1;
    } else {
        result.scrrolWheel = 0;
    }

    return result;
}

void Mouse::setMouseVisibility(bool toggle) {
    SDL_ShowCursor((int) toggle);
}

bool Mouse::getMouseVisibility() {
    return (bool) SDL_ShowCursor(-1);
}

void Mouse::setPosition(Uint16 X, Uint16 Y) {
    SDL_WarpMouse(X, Y);
}

void Mouse::setPosition(int X, int Y) {
    setPosition((Uint16) X, (Uint16) Y);
}

void Mouse::setPosition(Vector2 vec) {
    setPosition((Uint16) vec.X, (Uint16) vec.Y);
}

// </editor-fold>



// <editor-fold defaultstate="collapsed" desc="MouseState:">

bool MouseState::ButtonPressed(int button) {
    return mouseButtonsStatus[button];
}

bool MouseState::LeftButton() {
    return ButtonPressed(Mouse::LeftButton);
}

bool MouseState::MiddleButton() {
    return ButtonPressed(Mouse::MiddleButton);
}

bool MouseState::RightButton() {
    return ButtonPressed(Mouse::RightButton);
}

bool MouseState::XButton1() {
    return ButtonPressed(Mouse::XButton1);
}

bool MouseState::XButton2() {
    return ButtonPressed(Mouse::XButton2);
}

int MouseState::ScrollWheelValue(){
    perror("ScrollWheelValue no soportado");
    exit(1);
}

int MouseState::X(){
    return x;
}

int MouseState::Y(){
    return y;
}

bool MouseState::operator ==(const MouseState& other) const {
    int i;
    bool val = (scrrolWheel == other.scrrolWheel) &&
                    (x == other.x) && (x == other.x);

    if(!val) return false;

    for (i = 0; i < TOTAL_MOUSEBUTTONS; i++) {
        if (other.mouseButtonsStatus[i] != mouseButtonsStatus[i]) {
            return false;
        }
    }
    return true;
}

bool MouseState::operator !=(const MouseState& other) const {
    int i;
    bool val = (scrrolWheel == other.scrrolWheel) &&
                    (x == other.x) && (y == other.y);

    if(!val) return true;

    for (i = 0; i < TOTAL_MOUSEBUTTONS; i++) {
        if (other.mouseButtonsStatus[i] != mouseButtonsStatus[i]) {
            return true;
        }
    }
    return false;
}

// </editor-fold>
