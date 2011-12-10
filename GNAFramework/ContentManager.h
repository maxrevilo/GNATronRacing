#ifndef CONTENTMANAGER_H
#define	CONTENTMANAGER_H


#include "GNAExeptions.h"

namespace GNAFramework {

    /**
     * Clase encargada de manejar el contenido del juego,
     * se encarga de cargar contenido de discoduro a memoria eficientemente,
     * y al ser destruida se encarga de eliminar todos los contenidos que haya
     * cargado por lo que todo contenido cargado con una instancia de esta clase
     * sera eliminado al ser eliminada la instancia.
     */
    class ContentManager {
    public:
        char * RootDirectory;
        bool loadInstance;
        

        ContentManager(char * RootDirectory);

        /**
         * Carga un recurso de tipo T del discoduro en la ruta "path" relativa a
         * la ruta "root".
         * @param path ruta del recurso relativa a la ruta "root".
         * @throw ContentLoadException El archivo no pudo ser cargado
         * correctamente, bien sea que esta corrupto o que no existe.
         * @throw ObjectDisposedException Load fue llamado despues de que
         * el ContentManager fue "Disposed".
         * @throw ArgumentNullException el parametro "path" es NULL.
         */
        template <class T>
        T *Load(const char * path) throw (ContentLoadException *, ArgumentNullException *, ObjectDisposedException *);

        //ContentManager(const ContentManager& orig);
        virtual ~ContentManager();
    private:

        typedef struct cntNode {
            void * content;
            struct cntNode* next;
        } CntNode;

        CntNode texture2D;
        CntNode spriteFont;

        char *createFullPath(const char* path) const;
    };
}

#endif	/* CONTENTMANAGER_H */