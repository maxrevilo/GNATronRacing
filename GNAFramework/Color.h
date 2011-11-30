#ifndef COLOR_GNA_H
#define	COLOR_GNA_H

#include <stdint.h>
#include "Geom.h"

namespace GNAFramework {
    struct Color {
    public:
        uint8_t R;
        uint8_t G;
        uint8_t B;
        uint8_t A;

        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Black;
        static const Color Grey;
        static const Color DarkGrey;

        Color();

        /**
         * Crea un color a partir de 4 componentes decimales, donde 1 indica que el
         * canal completo esta coloreado y 0 indica que esta vacio.
         * @param A
         * @param R
         * @param G
         * @param B
         */
        Color(float A, float R, float G, float B);
        /**
         * Crea un color a partir de un Vector4, donde cada componente del vector
         * representan los canales A R G y B en ese orden, la componente indica
         * que tan lleno esta cada canal con 0 como minimo y 1 como ful.
         * @param vect
         */
        Color(Vector4 vect);

        Color(uint8_t A, uint8_t R, uint8_t G, uint8_t B);

        void setValue(uint8_t A, uint8_t R, uint8_t G, uint8_t B);

        void setValue(float A, float R, float G, float B);

        void setValue(Vector4 vect);

        Vector4 toVector4() const;

        bool operator ==(const Color & other) const;
        bool operator !=(const Color & other) const;

        char * toString() const;

    };
}

#endif	/* COLOR_GNA_H */

