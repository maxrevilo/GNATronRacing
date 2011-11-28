#ifndef GAME_H
#define	GAME_H


#include "TimeSpan.h"
#include "GraphicDevice.h"
#include "Mouse.h"
#include "ContentManager.h"
#include "GNAExeptions.h"

namespace GNAFramework {

    class ContentManager;
    class GraphicDevice;

    typedef struct {
        TimeSpan elapsedTime;
        TimeSpan totalTime;
        bool isRunningSlowly;
    } GameTime;

    /**
     * Clase base para un juego.
     */
    class Game {
    public:
        GraphicDevice *graphicDevice;

        /** Indica la cantidad de fotogramas por segundo a la que corre el juego los
         * valores seguros son entre 1 y 120, por defecto vale 60
         **/
        int fps;

        /** Es el manejador de contenido por defecto del Game, al cerrar el juego
         * se descarga automaticamente todo el contenido de memoria
         **/
        ContentManager *Content;


        Game();
        void Start();

        virtual void Initialize();
        virtual void Draw(GameTime gameTime);
        virtual void Update(GameTime gameTime);
        virtual void Exit();

        //Game(const Game& orig);
        virtual ~Game();

        bool IsActive() const {
            return active;
        }

    private:
        TimeSpan begin;
        TimeSpan actual;

        bool active;

        class EndGameException : public GNAException { };

        void GameLoop();
        void DrawCall();
        void catchGNAException(GNAException *e) const throw(GNAException);
    };
}
#endif	/* GAME_H */

