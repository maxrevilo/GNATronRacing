
#include "../Color.h"
#include <stdlib.h>
#include <stdio.h>


namespace GNAFramework {

    // <editor-fold defaultstate="collapsed" desc="Color">

    const Color Color::White   (1.f, 1.f, 1.f, 1.f);
    const Color Color::Red     (1.f, 1.f, 0.f, 0.f);
    const Color Color::Green   (1.f, 0.f, 1.f, 0.f);
    const Color Color::Blue    (1.f, 0.f, 0.f, 1.f);
    const Color Color::Black   (1.f, 0.f, 0.f, 0.f);
    const Color Color::Grey    (1.f, .5f, .5f, .5f);
    const Color Color::DarkGrey(1.f, .25f, .25f, .25f);

    Color::Color() {
        A = 0;
        R = 0;
        G = 0;
        B = 0;
    }

    Color::Color(float A, float R, float G, float B) {
        setValue(A, R, G, B);
    }

    Color::Color(Vector4 vect) {
        setValue(vect);
    }

    Color::Color(uint8_t A, uint8_t R, uint8_t G, uint8_t B) {
        setValue(A, R, G, B);
    }

    void Color::setValue(uint8_t A, uint8_t R, uint8_t G, uint8_t B) {
        this->A = A;
        this->R = R;
        this->G = G;
        this->B = B;
    }

    void Color::setValue(float A, float R, float G, float B) {
        setValue((uint8_t) (A * 255),
                (uint8_t) (R * 255),
                (uint8_t) (G * 255),
                (uint8_t) (B * 255));
    }

    void Color::setValue(Vector4 vect) {
        setValue((float) vect.W, (float) vect.X, (float) vect.Y, (float) vect.Z);
    }

    Vector4 Color::toVector4() const{
        Vector4 res(R / (float) 256, G / (float) 256, B / (float) 256, A / (float) 256);
        return res;
    }

    bool Color::operator ==(const Color &other) const {
        return this->A == other.A &&
                this->R == other.R &&
                this->G == other.G &&
                this->B == other.B;
    }

    bool Color::operator !=(const Color &other) const {
        return this->A != other.A ||
                this->R != other.R ||
                this->G != other.G ||
                this->B != other.B;
    }// </editor-fold>

    char * Color::toString() const{
        char * result = (char *) malloc(sizeof(char) * (4 * 3 + 17));
        sprintf(result, "{a:%d, r:%d, g:%d, b:%d}", A, R, G, B);
        return result;
    }

}


