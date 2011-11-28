#include "../GNAExeptions.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

namespace GNAFramework {

    // <editor-fold defaultstate="collapsed" desc="GNAException:">

    GNAException::GNAException() {
        message = NULL;
        innerException = NULL;
    }

    GNAException::GNAException(const char* message) {
        this->message = new char[strlen(message)];
        strcpy(this->message, message);
        this->innerException = NULL;
    }

    GNAException::GNAException(const char* message, GNAException* innerException) {
        this->message = new char[strlen(message)];
        strcpy(this->message, message);
        this->innerException = innerException;
    }

    GNAException *GNAException::InnerException() const {
        return innerException;
    }

    const GNAException *GNAException::GetBaseException() const {
        if (innerException != NULL) {
            const GNAException *exception = innerException;
            while (exception->InnerException() != NULL) {
                exception = exception->InnerException();
            }

            return exception;
        } else return this;
    }

    const char *GNAException::Message() const {
        return message;
    }

    const char *GNAException::getExceptionName() const {
        return "GNAException";
    }

    char *GNAException::toString() const {
        char buffer[1024];
        char *result;
        if (InnerException() != NULL) {
            sprintf(buffer, "%s {Message: \"%s\" InnerException: %s}", getExceptionName(), Message(), InnerException()->getExceptionName());
        } else {
            sprintf(buffer, "%s {Message: \"%s\"}", getExceptionName(), Message());
        }
        result = new char[strlen(buffer)];
        strcpy(result, buffer);

        return result;
    }

    char *GNAException::ExceptionTrace() const {
        char buffer[10240];
        char *result, *aux;
        const GNAException *e = InnerException();
        
        strcpy(buffer, aux = toString());
        free(aux);

        while (e != NULL) {
            strcat(buffer, "\n");
            strcat(buffer, aux = e->toString());
            free(aux);
            e = e->InnerException();
        }

        strcat(buffer, "\n");

        result = new char[strlen(buffer)];
        strcpy(result, buffer);

        return result;
    }

    GNAException::~GNAException(){
        if(InnerException() != NULL)
            delete InnerException();
    }

    // </editor-fold>

}

