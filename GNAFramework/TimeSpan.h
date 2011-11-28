#ifndef TIMESPAN_H
#define	TIMESPAN_H

#include <time.h>

namespace GNAFramework {

    /**
     * Representa un lapso de tiempo almacenado en alta presicion.
     */
    class TimeSpan {
    public:
        TimeSpan();
        TimeSpan(clock_t clocks);
        TimeSpan(int seconds, int milliseconds);

        //Sobrecargas de Operadores.
        TimeSpan operator+(const TimeSpan &other) const;
        TimeSpan operator-(const TimeSpan &other) const;

        bool operator==(const TimeSpan &other) const;
        bool operator>(const TimeSpan &other) const;
        bool operator>=(const TimeSpan &other) const;
        bool operator<(const TimeSpan &other) const;
        bool operator<=(const TimeSpan &other) const;

        void setTo(clock_t clocks);
        void setTo(int seconds, int milliseconds);

        long long int toTicks() const;
        double toTicksD() const;

        float toSecondsF() const;
        float toMillisecondsF() const;

        double toSecondsD() const;
        double toMillisecondsD() const;

        //TimeSpan(const TimeSpan& orig);
        virtual ~TimeSpan();
    private:
        clock_t total_clocks;
    };

}

#endif	/* TIMESPAN_H */

