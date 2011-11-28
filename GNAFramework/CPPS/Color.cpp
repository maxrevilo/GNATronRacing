
#include "../Color.h"
#include <stdlib.h>
#include <stdio.h>


namespace GNAFramework {

    // <editor-fold defaultstate="collapsed" desc="Color">

    const Color_GNA Color_GNA::White   (1.f, 1.f, 1.f, 1.f);
    const Color_GNA Color_GNA::Red     (1.f, 1.f, 0.f, 0.f);
    const Color_GNA Color_GNA::Green   (1.f, 0.f, 1.f, 0.f);
    const Color_GNA Color_GNA::Blue    (1.f, 0.f, 0.f, 1.f);
    const Color_GNA Color_GNA::Black   (1.f, 0.f, 0.f, 0.f);
    const Color_GNA Color_GNA::Grey    (1.f, .5f, .5f, .5f);
    const Color_GNA Color_GNA::DarkGrey(1.f, .25f, .25f, .25f);

    Color_GNA::Color_GNA() {
        A = 0;
        R = 0;
        G = 0;
        B = 0;
    }

    Color_GNA::Color_GNA(float A, float R, float G, float B) {
        setValue(A, R, G, B);
    }

    Color_GNA::Color_GNA(Vector4 vect) {
        setValue(vect);
    }

    Color_GNA::Color_GNA(uint8_t A, uint8_t R, uint8_t G, uint8_t B) {
        setValue(A, R, G, B);
    }

    void Color_GNA::setValue(uint8_t A, uint8_t R, uint8_t G, uint8_t B) {
        this->A = A;
        this->R = R;
        this->G = G;
        this->B = B;
    }

    void Color_GNA::setValue(float A, float R, float G, float B) {
        setValue((uint8_t) (A * 255),
                (uint8_t) (R * 255),
                (uint8_t) (G * 255),
                (uint8_t) (B * 255));
    }

    void Color_GNA::setValue(Vector4 vect) {
        setValue((float) vect.W, (float) vect.X, (float) vect.Y, (float) vect.Z);
    }

    Vector4 Color_GNA::toVector4() const{
        Vector4 res(R / (float) 256, G / (float) 256, B / (float) 256, A / (float) 256);
        return res;
    }

    bool Color_GNA::operator ==(const Color_GNA &other) const {
        return this->A == other.A &&
                this->R == other.R &&
                this->G == other.G &&
                this->B == other.B;
    }

    bool Color_GNA::operator !=(const Color_GNA &other) const {
        return this->A != other.A ||
                this->R != other.R ||
                this->G != other.G ||
                this->B != other.B;
    }// </editor-fold>

    char * Color_GNA::toString() const{
        char * result = (char *) malloc(sizeof(char) * (4 * 3 + 17));
        sprintf(result, "{a:%d, r:%d, g:%d, b:%d}", A, R, G, B);
        return result;
    }

}


